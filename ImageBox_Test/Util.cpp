//---------------------------------------------------------------------------
#include <Math.h>
#include <stdio.h>
#include <tchar.h>
#pragma hdrstop

#include "Util.h"

#pragma package(smart_init)

String FormatString(const TCHAR* format, ...) {
    va_list args;
    va_start (args, format);

    String text;
    text.vprintf(format, args);

    va_end (args);

    return text;
}

double GetTimeMs() {
    static LARGE_INTEGER freq;
    static BOOL r = QueryPerformanceFrequency(&freq);
    LARGE_INTEGER cnt;
    QueryPerformanceCounter(&cnt);
    return cnt.QuadPart * 1000.0 / freq.QuadPart;
}

void FreeBuffer(BYTE** buf) {
    if (*buf != NULL) {
        delete[] *buf;
        *buf = NULL;
    }
}

BYTE* AllocBuffer(size_t size) {
    BYTE* buf = new BYTE[size];
    memset(buf, 0, size);
    return buf;
}

void FloatBufToByte(void* floatBuf, int bw, int bh, int bytepp, void* byteBuf) {
    for (int y = 0; y < bh; y++) {
        BYTE* psrc = (BYTE*)floatBuf + (bw * y) * bytepp;
        BYTE* pdst = (BYTE*)byteBuf + bw * y;
        for (int x = 0; x < bw; x++, pdst++, psrc += bytepp) {
            if (bytepp == 4)
                *pdst = (BYTE)Clamp(*(float*)psrc, 0.0f, 255.0f);
            else if (bytepp == 8)
                *pdst = (BYTE)Clamp(*(double*)psrc, 0.0, 255.0);
        }
    }
}

int TColorToBGRA(TColor color) {
    int r = color & 0xff;
    int g = (color >> 8) & 0xff;
    int b = (color >> 16) & 0xff;
    return (b | g << 8 | r << 16 | 0xff << 24);
}

HPALETTE CreateGrayPalette() {
    BYTE* buf = AllocBuffer(sizeof(LOGPALETTE) + sizeof(PALETTEENTRY) * 255);
    LOGPALETTE* ppal = (LOGPALETTE*)buf;
    ppal->palVersion = 0x300;
    ppal->palNumEntries = 256;
    for (int i = 0; i < 256; i++) {
        ppal->palPalEntry[i].peRed = i;
        ppal->palPalEntry[i].peGreen = i;
        ppal->palPalEntry[i].peBlue = i;
        ppal->palPalEntry[i].peFlags = PC_RESERVED;
    }
    HPALETTE hPal = CreatePalette(ppal);
    FreeBuffer(&buf);
    return hPal;
}

void BitmapToImageBuffer(Graphics::TBitmap* bmp, BYTE** buf, int* bw, int* bh, int* bytepp)
{
    if (bmp->PixelFormat == pf1bit) {
        BitmapToImageBuffer1Bit(bmp, buf, bw, bh, bytepp);
        return;
    }
    *bw = bmp->Width;
    *bh = bmp->Height;
    if (bmp->PixelFormat == pf8bit)
        *bytepp = 1;
    else if (bmp->PixelFormat == pf16bit)
        *bytepp = 2;
    else if (bmp->PixelFormat == pf24bit)
        *bytepp = 3;
    else
        *bytepp = 4;

    *buf = AllocBuffer(*bw * *bh * *bytepp);

    int scanstep = *bw * *bytepp;
    for (int y = 0; y < *bh; y++) {
        BYTE* sptr = (BYTE*)bmp->ScanLine[y];
        BYTE* dptr = *buf + scanstep * y;
        memcpy(dptr, sptr, scanstep);
    }
}

void BitmapToImageBuffer1Bit(Graphics::TBitmap* bmp, BYTE** buf, int* bw, int* bh, int* bytepp)
{
    *bw = bmp->Width;
    *bh = bmp->Height;
    *bytepp = 1;

    *buf = AllocBuffer(*bw * *bh * *bytepp);

    int scanstep = *bw * *bytepp;
    int w8 = *bw / 8;
    for (int y = 0; y < *bh; y++) {
        BYTE* sptr = (BYTE*)bmp->ScanLine[y];
        BYTE* dptr = *buf + scanstep * y;
        for (int x8 = 0; x8 < w8; x8++, sptr++) {
            byte sp = *sptr;
            for (int x = 0; x < 8; x++, dptr++) {
                if (((sp << x) & 0x80) != 0)
                    *dptr = 255;
                else
                    *dptr = 0;
            }
        }
    }
}

Graphics::TBitmap* ImageBufferToBitmap(BYTE* buf, int bw, int bh, int bytepp)
{
    if (bytepp == 2) {
        return HraToBmp24(buf, bw, bh, bytepp);
    }
    TPixelFormat pixelFormat;
    if (bytepp == 1)
        pixelFormat = pf8bit;
    else if (bytepp == 3)
        pixelFormat = pf24bit;
    else if (bytepp == 4)
        pixelFormat = pf32bit;
    else return NULL;

    Graphics::TBitmap* bmp = new Graphics::TBitmap();
    bmp->Width = bw;
    bmp->Height = bh;
    bmp->PixelFormat = pixelFormat;
    if (bmp->PixelFormat == pf8bit) {
        HPALETTE hPal = CreateGrayPalette();
        DeleteObject(bmp->Palette);
        bmp->Palette = hPal;
    }
    int copySize = bw * bytepp;
    int stride = (copySize + 3) / 4 * 4;
    int paddingSize = stride - copySize;
    BYTE paddingBuf[] = { 0, 0, 0, 0 };
    for (int y = 0; y < bh; y++) {
        BYTE* sptr = buf + copySize * y;
        BYTE* dptr = (BYTE*)bmp->ScanLine[y];
        memcpy(dptr, sptr, copySize);
        if (paddingSize > 0)
            memcpy(dptr + copySize, paddingBuf, paddingSize);
    }

    return bmp;
}

Graphics::TBitmap* HraToBmp24(BYTE* buf, int bw, int bh, int bytepp)
{
    Graphics::TBitmap* bmp = new Graphics::TBitmap();
    bmp->Width = bw;
    bmp->Height = bh;
    bmp->PixelFormat = pf24bit;

    int copySize = bw * 3;
    int stride = (copySize + 3) / 4 * 4;
    int paddingSize = stride - copySize;
    BYTE paddingBuf[] = { 0, 0, 0, 0 };
    for (int y = 0; y < bh; y++) {
        BYTE* sptr = buf + bw * bytepp * y;
        BYTE* dptr = (BYTE*)bmp->ScanLine[y];
        for (int x = 0; x < bw; x++, sptr += bytepp, dptr += 3) {
            BYTE gray = sptr[0];
            dptr[0] = gray;
            dptr[1] = gray;
            dptr[2] = gray;
        }
        if (paddingSize > 0)
            memcpy(dptr, paddingBuf, paddingSize);
    }

    return bmp;
}

// hra Lolad
BOOL LoadHraFile(TCHAR* filePath, BYTE** imgBuf, int* bw, int* bh, int* bytepp) {
    FILE* fp = _tfopen(filePath, TEXT("rb"));
    if(fp == NULL)   // 파일오픈 체크
        return FALSE;

    fseek(fp, 252, SEEK_SET);
    fread(bytepp, sizeof(int), 1, fp);
    fread(bw, sizeof(int), 1, fp);
    fread(bh, sizeof(int), 1, fp);

    int bufSize = *bw * *bh * *bytepp;
    *imgBuf = AllocBuffer(bufSize);
    fread(*imgBuf, 1, bufSize, fp);

    fclose(fp);
    return TRUE;
}

// hra save
BOOL SaveHraFile(TCHAR* filePath, BYTE* imgBuf, int bw, int bh, int bytepp) {
    FILE* fp = _tfopen(filePath, TEXT("wb"));
    if(fp == NULL)   // 파일오픈 체크
        return FALSE;

    BYTE b = 0;
    for (int i = 0; i < 252; i++)
        fwrite(&b, sizeof(BYTE), 1, fp);
    fwrite(&bytepp, sizeof(int), 1, fp);
    fwrite(&bw, sizeof(int), 1, fp);
    fwrite(&bh, sizeof(int), 1, fp);

    int bufSize = bw * bh * bytepp;
    fwrite(imgBuf, sizeof(BYTE), bufSize, fp);

    fclose(fp);
    return TRUE;
}


