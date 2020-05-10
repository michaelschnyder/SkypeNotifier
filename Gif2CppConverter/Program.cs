using System;
using System.Collections.Generic;
using System.IO;
using SixLabors.ImageSharp;
using SixLabors.ImageSharp.PixelFormats;

namespace Gif2CppConverter
{
    internal enum Compression
    {
        None,
        RunLength,
    }

    internal enum OutputMode
    {
        Binary,
        Cpp
    }

    class Program
    {
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
                    outPath = path;
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
            using var img = Image.Load(gifFile);
            var imageMetadata = new ImageMetadata(img.Width, img.Height, img.Frames.Count);

            var frameBytes = GifToRgb16Reader.GetFrameBytes(img);

            var fileNameWithoutExtension = Path.GetFileNameWithoutExtension(gifFile);
            if (mode == OutputMode.Cpp)
            {
                var encoder = new HeaderFileEncoder();
                encoder.Write(compression, outPath, frameBytes, fileNameWithoutExtension, imageMetadata);
            }
            else
            {
                var encoder = new BinaryEncoder();
                encoder.WriteBinary(compression, outPath, frameBytes, fileNameWithoutExtension, imageMetadata);
            }

            Console.WriteLine($"Writing output done");
        }
    }

    class GifToRgb16Reader
    {
        private static int ToRgb16(int red, int green, int blue)
        {
            return ((red & 0b11111000) << 8) | ((green & 0b11111100) << 3) | (blue >> 3);
        }

        public static List<List<int>> GetFrameBytes(Image img)
        {
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
            return frameBytes;
        }
    }
}
