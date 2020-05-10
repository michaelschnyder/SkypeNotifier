using System.Collections.Generic;
using System.IO;

namespace Gif2CppConverter
{
    class CppWriter
    {
        public void Write(Compression compression, string outPath,
            List<List<int>> frameBytes, string fileNameWithoutExtension, ImageMetadata imageMetadata)
        {
            var outFilePath = Path.Combine(outPath, $"{fileNameWithoutExtension}.h");

            using var writer = new StreamWriter(outFilePath);
            var variable = fileNameWithoutExtension.Replace("-", "");

            writer.WriteLine("#include <pgmspace.h>\n" +
                             $"// File '{fileNameWithoutExtension}.gif', Resolution: {imageMetadata.ImgWidth}x{imageMetadata.ImgHeight}px, Frames: {imageMetadata.FramesCount}\n" +
                             $"const PROGMEM uint16_t {variable} [] = {{");

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

                        if ((index + 1) % imageMetadata.ImgWidth == 0)
                        {
                            writer.WriteLine();
                        }
                    }

                    if (compression == Compression.RunLength)
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
            writer.Close();
        }
    }
}