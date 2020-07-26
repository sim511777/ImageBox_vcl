#ifndef UtilH
#define UtilH

#include <vcl.h>

template<class T>
T Clamp(T value, T min, T max)
{
    if (value < min) return min;
    if (value > max) return max;
    return value;
}

template<class T>
T Min(T a, T b)
{
    if (a < b) return a;
    return b;
}

template<class T>
T Max(T a, T b)
{
    if (a > b) return a;
    return b;
}

String FormatString(const TCHAR* format, ...);
double GetTimeMs();
void FreeBuffer(BYTE** buf);
BYTE* AllocBuffer(size_t size);
void FloatBufToByte(void* floatBuf, int bw, int bh, int bytepp, void* byteBuf);
int TColorToBGRA(TColor color);
HPALETTE CreateGrayPalette();
void BitmapToImageBuffer(Graphics::TBitmap* bmp, BYTE** buf, int* bw, int* bh, int* bytepp);
void BitmapToImageBuffer1Bit(Graphics::TBitmap* bmp, BYTE** buf, int* bw, int* bh, int* bytepp);
Graphics::TBitmap* ImageBufferToBitmap(BYTE* buf, int bw, int bh, int bytepp);
Graphics::TBitmap* HraToBmp24(BYTE* buf, int bw, int bh, int bytepp);
BOOL LoadHraFile(TCHAR* filePath, BYTE** imgBuf, int* bw, int* bh, int* bytepp);
BOOL SaveHraFile(TCHAR* filePath, BYTE* imgBuf, int bw, int bh, int bytepp);

#endif

