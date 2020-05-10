namespace Gif2CppConverter
{
    internal class ImageMetadata
    {
        public ImageMetadata(int imgWidth, int imgHeight, int framesCount)
        {
            ImgWidth = imgWidth;
            ImgHeight = imgHeight;
            FramesCount = framesCount;
        }

        public int ImgWidth { get; private set; }
        public int ImgHeight { get; private set; }
        public int FramesCount { get; private set; }
    }
}