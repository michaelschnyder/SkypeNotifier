using System.Collections.Generic;

namespace Gif2CppConverter
{
    internal abstract class OutputEncoder
    {
        public abstract void Write(string outPath, List<List<int>> frameBytes, string? fileNameWithoutExtension, ImageMetadata imageMetadata);
    }
}