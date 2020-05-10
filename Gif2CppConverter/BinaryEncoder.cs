using System;
using System.Collections.Generic;
using System.IO;

namespace Gif2CppConverter
{
    class BinaryEncoder
    {
        public void WriteBinary(Compression compression, string outPath, List<List<int>> frameBytes, string? fileNameWithoutExtension, ImageMetadata imageMetadata)
        {
            var outFilePath = Path.Combine(outPath, $"{fileNameWithoutExtension}.bin");

            using var writer = new BinaryWriter(new FileStream(outFilePath, FileMode.Create, FileAccess.Write));

            foreach (var currentFrame in frameBytes)
            {
                for (var index = 0; index < currentFrame.Count; index++)
                {
                    var value = (ushort)currentFrame[index];
                    var bytes = BitConverter.GetBytes(value);

                    if (compression == Compression.RunLength)
                    {
                        // TODO
                        var currentValue = currentFrame[index];

                        short offset = 1;
                        while (index + offset < currentFrame.Count && currentFrame[index + offset] == currentValue && offset < 16 * 16)
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
            }

            writer.Close();

        }
    }
}