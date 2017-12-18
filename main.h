#ifndef MAIN_H_INCLUDED
#define MAIN_H_INCLUDED
#include <QDebug>

// bitmap file header
typedef struct {
    unsigned short bfType;
    unsigned int   bfSize;
    unsigned short bfReserved1;
    unsigned short bfReserved2;
    unsigned int   bfOffBits;
    unsigned int   biSize;
} BITMAPFILEHEADER;

// bitmap OS info header
typedef struct {
    unsigned short   biWidth;
    unsigned short   biHeight;
    unsigned short biPlanes;
    unsigned short biBitCount;
} BITMAPINFOHEADER_OS;

// bitmap WINDOW info header
typedef struct {
    unsigned int   biWidth;
    unsigned int   biHeight;
    unsigned short biPlanes;
    unsigned short biBitCount;
    unsigned int   biCompression;
    unsigned int   biSizeImage;
    unsigned int   biXPelsPerMeter;
    unsigned int   biYPelsPerMeter;
    unsigned int   biClrUsed;
    unsigned int   biClrImportant;
} BITMAPINFOHEADER_WINDOW;

// rgb quad
typedef struct {
    unsigned char  rgbBlue;
    unsigned char  rgbGreen;
    unsigned char  rgbRed;
    unsigned char  rgbReserved;
} RGBQUAD;



// bit extract
unsigned char bitextract(const unsigned int byte, const unsigned int mask);

#endif // MAIN_H
