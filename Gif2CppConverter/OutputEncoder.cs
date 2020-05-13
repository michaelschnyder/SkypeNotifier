using System.Collections.Generic;

namespace Gif2CppConverter
{
    internal abstract class OutputEncoder
    {
        public abstract void Write(List<List<int>> frameBytes, ImageMetadata imageMetadata, string outPath, string? fileNameWithoutExtension, int[] source);
    }
}