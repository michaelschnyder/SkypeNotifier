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

            var allFiles = new List<string>();

            if (Directory.Exists(path))
            {
                allFiles.AddRange(Directory.GetFiles(path, "*.gif"));
            }
            else if (File.Exists(path))
            {
                allFiles.Add(path);
            }

            if (allFiles.Count == 0)
            {
                Console.WriteLine($"No file found at '{Path.GetFullPath(path)}'");
            }

            foreach (var file in allFiles)
            {
                ConvertFile(file);
            }
        }

        private static void ConvertFile(string gifFile)
        {
            var fileNameWithoutExtension = Path.GetFileNameWithoutExtension(gifFile);

            var outfile = $"{fileNameWithoutExtension}.h";
            var sourcePath = Path.GetDirectoryName(gifFile);
            var outFilePath = Path.Combine(sourcePath, outfile);

            var variableName = fileNameWithoutExtension.Replace("-", "");

            using var writer = new StreamWriter(outFilePath);
            using var img = Image.Load(gifFile);

            writer.WriteLine("#include <pgmspace.h>\n" +
                             $"// File '{fileNameWithoutExtension}.gif', Resolution: {img.Width}x{img.Height}px, Frames: {img.Frames.Count}\n" +
                             $"const PROGMEM uint16_t {variableName} [] = {{");

            Console.WriteLine($"Found {img.Frames.Count} in provided file.");
            int frameId = 0;

            foreach (var imgFrame in img.Frames)
            {
                writer.WriteLine();
                writer.WriteLine("// Frame " + frameId);

                if (imgFrame is ImageFrame<Rgba32> frame)
                {
                    for (int y = 0; y < frame.Height; y++)
                    {
                        for (int x = 0; x < frame.Width; x++)
                        {
                            var pixel = frame[x, y];
                            var rgb16 = ToRgb16(pixel.R, pixel.G, pixel.B);

                            writer.Write($"0x{rgb16:x4}, ");
                        }

                        writer.WriteLine();
                    }
                }

                frameId++;
                Console.WriteLine($"Completed frame {frameId}");
            }

            writer.WriteLine("};");
            writer.Close();
        }
    }
}
