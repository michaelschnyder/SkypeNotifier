using System;
using System.Collections.Generic;
using System.IO;
using System.Linq;
using SixLabors.ImageSharp;

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

            var encoder = FindEncoder(mode, compression);

            foreach (var file in allFiles)
            {
                Convert(file, outPath, encoder);
            }
        }

        private static OutputEncoder FindEncoder(OutputMode mode, Compression compression)
        {
            OutputEncoder encoder;
            if (mode == OutputMode.Cpp)
            {
                encoder = new HeaderFileEncoder(compression);
            }
            else
            {
                encoder = new BinaryEncoder(compression);
            }

            return encoder;
        }

        private static void Convert(string gifFile, string outPath, OutputEncoder encoder)
        {
            using var img = Image.Load(gifFile);
            var imageMetadata = new ImageMetadata(img.Width, img.Height, img.Frames.Count);

            var frameBytes = GifToRgb16Reader.GetFrameBytes(img);

            var fileNameWithoutExtension = Path.GetFileNameWithoutExtension(gifFile);

            encoder.Write(frameBytes, imageMetadata, outPath, fileNameWithoutExtension, frameBytes.SelectMany(f => f).ToArray());

            Console.WriteLine($"Writing output done");
        }
    }
}
