#ifndef BMPIMAGE_H
#define BMPIMAGE_H
#include <QVector>
#include <main.h>
#include <QImage>

class BmpImage
{
public:
    void showColorTable();
    void showImg();
    void setImg();
    BmpImage();
    unsigned int width;
    unsigned int height;
    unsigned char getPixel(int x, int y);
    void setPixel(int x, int y, unsigned char value);
    void createQimage(QImage &img);
    void createQimageFromImg(QImage &img, bool flag);
    unsigned char getPixelImg(int x, int y);
    void Convolution(const QVector<QVector<int>> pkernel);
    void Kuvahary(int sizeOfKernel);

    // fields
    QVector<RGBQUAD> colorTable;
    QVector<unsigned int> imgNumberToTable;
    QVector<unsigned char> img;
    QVector<unsigned char> result;
    int type;

};

#endif // BMPIMAGE_H
