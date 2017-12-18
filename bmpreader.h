#ifndef BMPREADER_H
#define BMPREADER_H

#include <QString>
#include <QDataStream>
#include <QFile>
#define WINDOW 40
#define OS 12
#include <QDebug>
#include <bmpimage.h>

class BMPReader
{
public:
    BMPReader(const QString &fileName);
    void parseBMPFILE();
    void showInfo();
    void checkBMP(const unsigned short &type);
    void checkAllowCompression(const unsigned int &compression);
    int checkRGBmask();
    void readTableOfImage(BmpImage &img);
    void readDataOfImage(BmpImage &img);
    void readCompressionDataOfImage(BmpImage &image);

    // fields
    unsigned int AmountOfReadBits = 0;
    QString filename;
    QDataStream dS;
    QFile file;
    unsigned short int type; // Os or Window
    unsigned int bmpFilesize;
    unsigned int offBits;
    unsigned int width;
    unsigned int height;
    unsigned short int bitsPerPixel;
    // only for Window
    bool compression=false;
    unsigned int sizeImage = 0;
    unsigned int biXPelsPerMeter = 0;
    unsigned int biYPelsPerMeter = 0;
    unsigned int biClrUsed = 0;
    unsigned int biClrImportant = 0;
};

#endif // BMPREADER_H
