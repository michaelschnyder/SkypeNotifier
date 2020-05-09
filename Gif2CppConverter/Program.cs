using System;
using System.Collections.Generic;
using System.IO;
using System.Net;
using SixLabors.ImageSharp;
using SixLabors.ImageSharp.PixelFormats;

namespace Gif2CppConverter
{
    class Program
    {
        static int ToRgb16(int red, int green, int blue)
        {
            return ((red & 0b11111000) << 8) | ((green & 0b11111100) << 3) | (blue >> 3);
        }

        static void Main(string[] args)
        {
            var path = args[0];
            var outPath = args.Length > 1 ? args[1] : null;
            var mode = args.Length > 2 && args[2] == "bin" ? OutputMode.Binary : OutputMode.Cpp;
            var compression = args.Length > 3 ? Enum.Parse<Compression>(args[3]) : Compression.None;

            var allFiles = new List<string>();

            if (Directory.Exists(path))
            {
                if (outPath == null)
                {
                    outPath =path;
                }

                allFiles.AddRange(Directory.GetFiles(path, "*.gif"));
            }
            else if (File.Exists(path))
            {
                if (outPath == null)
                {
                    outPath = Path.GetDirectoryName(path);
                }

                allFiles.Add(path);
            }

            if (allFiles.Count == 0)
            {
                Console.WriteLine($"No file found at '{Path.GetFullPath(path)}'");
            }

            foreach (var file in allFiles)
            {
                ConvertFile(file, outPath, mode, compression);
            }
        }

        private static void ConvertFile(string gifFile, string outPath, OutputMode mode, Compression compression)
        {
            var fileNameWithoutExtension = Path.GetFileNameWithoutExtension(gifFile);

            string outfile;

            if (mode == OutputMode.Cpp)
            {
                outfile = $"{fileNameWithoutExtension}.h";
            }
            else
            {
                outfile = $"{fileNameWithoutExtension}.bin";
            }

            var outFilePath = Path.Combine(outPath, outfile);

            var variableName = fileNameWithoutExtension.Replace("-", "");

            using var img = Image.Load(gifFile);

            Console.WriteLine($"Found {img.Frames.Count} in provided file.");
            int frameId = 0;
            List<List<int>> frameBytes = new List<List<int>>();

            foreach (var imgFrame in img.Frames)
            {
                if (imgFrame is ImageFrame<Rgba32> frame)
                {
                    var currentFrame = new List<int>();
                    frameBytes.Add(currentFrame);

                    for (int y = 0; y < frame.Height; y++)
                    {
                        for (int x = 0; x < frame.Width; x++)
                        {
                            var pixel = frame[x, y];
                            var rgb16 = ToRgb16(pixel.R, pixel.G, pixel.B);

                            currentFrame.Add(rgb16);
                        }
                    }
                }

                frameId++;
                Console.WriteLine($"Read frame {frameId}");
            }

            Console.WriteLine($"Writing output");



            if (mode == OutputMode.Cpp)
            {
                using var writer = new StreamWriter(outFilePath);
                WriteCpp(writer, frameBytes, fileNameWithoutExtension, img, variableName, compression);
                writer.Close();
            }
            else
            {
                using var writer = new BinaryWriter(new FileStream(outFilePath, FileMode.OpenOrCreate, FileAccess.Write));
                
                WriteBinary(writer, frameBytes, compression);
                writer.Close();
            }

            Console.WriteLine($"Writing output done");

        }

        private static void WriteBinary(BinaryWriter writer, List<List<int>> frameBytes, Compression compression)
        {
            foreach (var currentFrame in frameBytes)
            {
                for (var index = 0; index < currentFrame.Count; index++)
                {
                    var value = (ushort)currentFrame[index];
                    if (compression == Compression.None)
                    {
                        var bytes = BitConverter.GetBytes(value);
                        
                        writer.Write(bytes[1]);
                        writer.Write(bytes[0]);

                        // writer.Write(value);
                    }

                    if (compression == Compression.Forward)
                    {
                        // TODO
                        var currentValue = currentFrame[index];

                        var offset = 1;
                        while (index + offset < currentFrame.Count && currentFrame[index + offset] == currentValue && offset < 16 * 16)
                        {
                            offset++;
                        }

                        index = index + offset - 1;

                        writer.Write(offset);
                        writer.Write(currentValue);
                    }
                }
            }
        }

        private static void WriteCpp(StreamWriter writer, List<List<int>> frameBytes, string fileNameWithoutExtension,
            Image img, string variableName, Compression compression)
        {
            writer.WriteLine("#include <pgmspace.h>\n" +
                             $"// File '{fileNameWithoutExtension}.gif', Resolution: {img.Width}x{img.Height}px, Frames: {img.Frames.Count}\n" +
                             $"const PROGMEM uint16_t {variableName} [] = {{");

            for (int frameId = 0; frameId < frameBytes.Count; frameId++)
            {
                writer.WriteLine();
                writer.WriteLine("// Frame " + frameId);
                var currentFrame = frameBytes[frameId];

                for (var index = 0; index < currentFrame.Count; index++)
                {
                    var value = currentFrame[index];

                    if (compression == Compression.None)
                    {
                        writer.Write($"0x{value:x4}, ");

                        if ((index + 1) % img.Width == 0)
                        {
                            writer.WriteLine();
                        }
                    }

                    if (compression == Compression.Forward)
                    {
                        var currentValue = currentFrame[index];

                        var offset = 1;
                        while (index + offset < currentFrame.Count && currentFrame[index + offset] == currentValue && offset < 16 * 16)
                        {
                            offset++;
                        }

                        index = index + offset - 1;

                        writer.Write($"0x{offset:x}, ");
                        writer.Write($"0x{value:x4}, ");
                    }


                }
            }

            writer.WriteLine("};");

        }
    }

    internal enum Compression
    {
        None,
        Forward,
    }

    internal enum OutputMode
    {
        Binary,
        Cpp
    }
}
