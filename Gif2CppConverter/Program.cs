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

            var gifFile = args[0];
            var outfile = args[1];
            var file = new FileInfo(gifFile);

            using var writer = new StreamWriter(outfile);
            using var img = Image.Load(gifFile);

            writer.WriteLine("#include <pgmspace.h>\n" +
                             $"// '{gifFile}', {img.Width}x{img.Height}px\n" +
                             $"const PROGMEM uint16_t {file.Name.Substring(0, file.Name.Length - file.Extension.Length)} [] = {{");

            Console.WriteLine($"Found {img.Frames.Count} in provided file.");
            int frameId = 0;

            foreach (var imgFrame in img.Frames)
            {
                writer.WriteLine();
                writer.WriteLine("// Frame " + frameId);

                if (imgFrame is ImageFrame<Rgba32> frame)
                {
                    for (int x = 0; x < frame.Width; x++)
                    {
                        for (int y = 0; y < frame.Height; y++)
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
