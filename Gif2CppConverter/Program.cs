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
                ConvertFile(file, outPath, mode);
            }
        }

        private static void ConvertFile(string gifFile, string outPath, OutputMode mode)
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

            using var writer = new StreamWriter(outFilePath);
            using var img = Image.Load(gifFile);

            if (mode == OutputMode.Cpp)
            {
                writer.WriteLine("#include <pgmspace.h>\n" +
                                 $"// File '{fileNameWithoutExtension}.gif', Resolution: {img.Width}x{img.Height}px, Frames: {img.Frames.Count}\n" +
                                 $"const PROGMEM uint16_t {variableName} [] = {{");

            }

            Console.WriteLine($"Found {img.Frames.Count} in provided file.");
            int frameId = 0;

            foreach (var imgFrame in img.Frames)
            {
                if (mode == OutputMode.Cpp)
                {
                    writer.WriteLine();
                    writer.WriteLine("// Frame " + frameId);
                }

                if (imgFrame is ImageFrame<Rgba32> frame)
                {
                    for (int y = 0; y < frame.Height; y++)
                    {
                        for (int x = 0; x < frame.Width; x++)
                        {
                            var pixel = frame[x, y];
                            var rgb16 = ToRgb16(pixel.R, pixel.G, pixel.B);

                            if (mode == OutputMode.Cpp)
                            {
                                writer.Write($"0x{rgb16:x4}, ");
                            }
                            else
                            {
                                writer.Write(rgb16);
                            }
                        }

                        if (mode == OutputMode.Cpp)
                        {
                            writer.WriteLine();
                        }
                    }
                }

                frameId++;
                Console.WriteLine($"Completed frame {frameId}");
            }

            if (mode == OutputMode.Cpp)
            {
                writer.WriteLine("};");
            }

            writer.Close();
        }
    }

    internal enum OutputMode
    {
        Binary,
        Cpp
    }
}
