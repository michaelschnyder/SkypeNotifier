using System;
using System.Collections.Generic;
using System.IO;

namespace Gif2CppConverter
{
    class BinaryEncoder : OutputEncoder
    {
        private readonly Compression _compression;

        public BinaryEncoder(Compression compression)
        {
            _compression = compression;
        }

        public override void Write(List<List<int>> frameBytes, ImageMetadata imageMetadata, string outPath, string? fileNameWithoutExtension, int[] source)
        {
            var ext = _compression == Compression.None ? "bin" : "rle";

            var outFilePath = Path.Combine(outPath, $"{fileNameWithoutExtension}.{ext}");

            using var writer = new BinaryWriter(new FileStream(outFilePath, FileMode.Create, FileAccess.Write));

                for (var index = 0; index < source.Length; index++)
                {
                    var value = (ushort)source[index];
                    var bytes = BitConverter.GetBytes(value);

                    if (_compression == Compression.RunLength)
                    {
                        // TODO
                        var currentValue = source[index];

                        short offset = 1;
                        while (index + offset < source.Length && source[index + offset] == currentValue && offset < (16 * 16) -1)
                        {
                            offset++;
                        }

                        index = index + offset - 1;

                        var offsetBytes = BitConverter.GetBytes(offset);
                        writer.Write(offsetBytes[1]);
                        writer.Write(offsetBytes[0]);
                    }

                    writer.Write(bytes[1]);
                    writer.Write(bytes[0]);
            }

            writer.Close();

        }
    }
}