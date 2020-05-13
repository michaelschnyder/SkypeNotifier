using System.Collections.Generic;
using System.IO;

namespace Gif2CppConverter
{
    class HeaderFileEncoder : OutputEncoder
    {
        private readonly Compression _compression;

        public HeaderFileEncoder(Compression compression)
        {
            _compression = compression;
        }

        public override void Write(List<List<int>> frameBytes, ImageMetadata imageMetadata, string outPath, string? fileNameWithoutExtension, int[] source)
        {
            var outFilePath = Path.Combine(outPath, $"{fileNameWithoutExtension}.h");

            using var writer = new StreamWriter(outFilePath);
            var variable = fileNameWithoutExtension.Replace("-", "");

            writer.WriteLine("#include <pgmspace.h>\n" +
                             $"// Original file: {fileNameWithoutExtension}.gif \n" +
                             $"// Compression:   {_compression}\n" +
                             $"// Resolution:    {imageMetadata.ImgWidth}x{imageMetadata.ImgHeight}px, Frames: {imageMetadata.FramesCount}\n" +
                             $"const PROGMEM uint16_t {variable} [] = {{");

            if (_compression == Compression.None)
            {
                var frameId = 0;

                for (var i = 0; i < source.Length; i++)
                {
                    if (i % (imageMetadata.ImgWidth * imageMetadata.ImgHeight) == 0)
                    {
                        writer.WriteLine();
                        writer.WriteLine("// Frame " + ++frameId);
                    }

                    writer.Write($"0x{source[i]:x4}, ");

                    if ((i + 1 ) % imageMetadata.ImgWidth == 0)
                    {
                        writer.WriteLine();
                    }
                }
            }
            else
            {
                var bytesWritten = 0;
                for (var index = 0; index < source.Length; index++)
                {
                    var currentValue = source[index];

                    var offset = 1;
                    while (index + offset < source.Length && source[index + offset] == currentValue && offset < (16 * 16) - 1)
                    {
                        offset++;
                    }

                    index = index + offset - 1;

                    writer.Write($"0x{offset:x2}, ");
                    writer.Write($"0x{currentValue:x4}, ");

                    bytesWritten += 2;

                    if (bytesWritten % 32 == 0)
                    {
                        writer.WriteLine();
                    }
                }
            }

            writer.WriteLine("};");
            writer.Close();
        }
    }
}