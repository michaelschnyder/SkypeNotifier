using System;
using System.Collections.Generic;
using SixLabors.ImageSharp;
using SixLabors.ImageSharp.PixelFormats;

namespace Gif2CppConverter
{
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