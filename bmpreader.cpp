#include "bmpreader.h"
#include "main.h"
#include <math.h>
#include <QString>

template <typename Type>
void readRawData(Type &buffer, BMPReader* bmpreader)
{
    bmpreader->AmountOfReadBits+=bmpreader->dS.readRawData((char*)&buffer, sizeof(buffer));
    bmpreader->dS.readRawData()
}

template <typename Type>
void readRawData(Type &buffer, BMPReader* bmpreader, int size)
{
    bmpreader->AmountOfReadBits+=bmpreader->dS.readRawData((char*)&buffer, size);
}

BMPReader::BMPReader(const QString &fileName)
{
    this->filename = fileName;
    this->file.setFileName(fileName);
    if (!this->file.open(QIODevice::ReadOnly))
    {
        qDebug() <<  "Error opening file '" <<  fileName;
        exit(1);
    }
    dS.setDevice(&this->file);
}
void BMPReader::checkBMP(const unsigned short &type)
{
    if (type != 0x4D42) {
        qDebug()  <<  "Error: '"  <<  this->filename  <<  "' is not BMP file.";
        exit(1);
    }
}

void BMPReader::parseBMPFILE()
{
    // read first header
    BITMAPFILEHEADER fileHeader;
    readRawData(fileHeader.bfType, this);
    readRawData(fileHeader.bfSize, this);
    readRawData(fileHeader.bfReserved1, this);
    readRawData(fileHeader.bfReserved2, this);
    readRawData(fileHeader.bfOffBits, this);
    readRawData(fileHeader.biSize, this);
//    this->dS.readRawData((char*)&fileHeader.bfType, sizeof(fileHeader.bfType));
//    this->dS.readRawData((char*)&fileHeader.bfSize, sizeof(fileHeader.bfSize));
//    this->dS.readRawData((char*)&fileHeader.bfReserved1, sizeof(fileHeader.bfReserved1));
//    this->dS.readRawData((char*)&fileHeader.bfReserved2, sizeof(fileHeader.bfReserved2));
//    this->dS.readRawData((char*)&fileHeader.bfOffBits, sizeof(fileHeader.bfOffBits));
//    this->dS.readRawData((char*)&fileHeader.biSize, sizeof(fileHeader.biSize));


    // check is it bmp
    checkBMP(fileHeader.bfType);

    this->bmpFilesize = fileHeader.bfSize;
    this->offBits = fileHeader.bfOffBits;
    this->type = fileHeader.biSize;


    if (fileHeader.biSize == OS) {
        BITMAPINFOHEADER_OS fileInfoHeader;

        readRawData(fileInfoHeader.biWidth, this);
        readRawData(fileInfoHeader.biHeight, this);
        readRawData(fileInfoHeader.biPlanes, this);
        readRawData(fileInfoHeader.biBitCount, this);

//        this->dS.readRawData((char*)&fileInfoHeader.biWidth, sizeof(fileInfoHeader.biWidth));
//        this->dS.readRawData((char*)&fileInfoHeader.biHeight, sizeof(fileInfoHeader.biHeight));
//        this->dS.readRawData((char*)&fileInfoHeader.biPlanes, sizeof(fileInfoHeader.biPlanes));
//        this->dS.readRawData((char*)&fileInfoHeader.biBitCount, sizeof(fileInfoHeader.biBitCount));

        this->width = fileInfoHeader.biWidth;
        this->height = fileInfoHeader.biHeight;
        this->bitsPerPixel = fileInfoHeader.biBitCount;
        return;
    } else if (fileHeader.biSize == WINDOW)
        {
            BITMAPINFOHEADER_WINDOW fileInfoHeader;
            readRawData(fileInfoHeader.biWidth, this);
            readRawData(fileInfoHeader.biHeight, this);
            readRawData(fileInfoHeader.biPlanes, this);
            readRawData(fileInfoHeader.biBitCount, this);
//            this->dS.readRawData((char*)&fileInfoHeader.biWidth, sizeof(fileInfoHeader.biWidth));
//            this->dS.readRawData((char*)&fileInfoHeader.biHeight, sizeof(fileInfoHeader.biHeight));
//            this->dS.readRawData((char*)&fileInfoHeader.biPlanes, sizeof(fileInfoHeader.biPlanes));
//            this->dS.readRawData((char*)&fileInfoHeader.biBitCount, sizeof(fileInfoHeader.biBitCount));

//            this->dS.readRawData((char*)&fileInfoHeader.biCompression, sizeof(fileInfoHeader.biCompression));
//            this->dS.readRawData((char*)&fileInfoHeader.biSizeImage, sizeof(fileInfoHeader.biSizeImage));
//            this->dS.readRawData((char*)&fileInfoHeader.biXPelsPerMeter, sizeof(fileInfoHeader.biXPelsPerMeter));
//            this->dS.readRawData((char*)&fileInfoHeader.biYPelsPerMeter, sizeof(fileInfoHeader.biYPelsPerMeter));
//            this->dS.readRawData((char*)&fileInfoHeader.biClrUsed, sizeof(fileInfoHeader.biClrUsed));
//            this->dS.readRawData((char*)&fileInfoHeader.biClrImportant, sizeof(fileInfoHeader.biClrImportant));
            readRawData(fileInfoHeader.biCompression, this);
            readRawData(fileInfoHeader.biSizeImage, this);
            readRawData(fileInfoHeader.biXPelsPerMeter, this);
            readRawData(fileInfoHeader.biYPelsPerMeter, this);
            readRawData(fileInfoHeader.biClrUsed, this);
            readRawData(fileInfoHeader.biClrImportant, this);

            this->width = fileInfoHeader.biWidth;
            this->height = fileInfoHeader.biHeight;
            this->bitsPerPixel = fileInfoHeader.biBitCount;

            checkAllowCompression(fileInfoHeader.biCompression);

            this->compression = fileInfoHeader.biCompression;
            this->sizeImage = fileInfoHeader.biSizeImage;
            this->biXPelsPerMeter = fileInfoHeader.biXPelsPerMeter;
            this->biYPelsPerMeter = fileInfoHeader.biYPelsPerMeter;
            this->biClrUsed = fileInfoHeader.biClrUsed;
            this->biClrImportant = fileInfoHeader.biClrImportant;
            return;
        }
    qDebug()  <<  "incorrect size of srtruct"  <<  fileHeader.biSize;
    exit(1);
}

void BMPReader::showInfo()
{

    if (!(this->type == WINDOW || this->type == OS))
    {
        qDebug() << "incorrect type";
        exit(1);
    }
    qDebug() << "for filename: '" << this->filename << "' info:";
    qDebug() << "bmpFilesize: " << this->bmpFilesize;
    qDebug() << "offBits: " << this->offBits;
    qDebug() << "width: " << this->width;
    qDebug() << "height: " << this->height;
    qDebug() << "bitsPerPixel: " << this->bitsPerPixel;
    qDebug() << "Bits readed" << this->AmountOfReadBits;
    if (this->type == WINDOW)
    {
        qDebug() << "compression: " << this->compression;
        qDebug() << "biSizeImage: " << this->sizeImage;
        qDebug() << "biXPelsPerMeter: " << this->biXPelsPerMeter;
        qDebug() << "biYPelsPerMeter: " << this->biYPelsPerMeter;
        qDebug() << "biClrUsed: " << this->biClrUsed;
        qDebug() << "biClrImportant: " << this->biClrImportant;
    }
}

void BMPReader::checkAllowCompression(const unsigned int &compression)
{
    if (compression > 1)
    {
        qDebug()<<"unsupported compression algorithm";
        exit(1);
    }
}

int BMPReader::checkRGBmask()
{
    // TODO: find biCrlUsed !=0 and implement for that algorithm
    if (this->bitsPerPixel <= 8)
        return 1;
    else
    {
        if (this->biClrUsed != 0)
            return 2;
    }
    return 0;
}

void BMPReader::readTableOfImage(BmpImage &img)
{
    unsigned int amountOfColors = pow(2, this->bitsPerPixel);
    for (int i = 0; i < amountOfColors; i++)
    {
        RGBQUAD colors;
        readRawData(colors.rgbBlue, this);
        readRawData(colors.rgbGreen, this);
        readRawData(colors.rgbRed, this);
//        this->dS.readRawData((char*)&colors.rgbBlue, sizeof(colors.rgbBlue));
//        this->dS.readRawData((char*)&colors.rgbGreen, sizeof(colors.rgbGreen));
//        this->dS.readRawData((char*)&colors.rgbRed, sizeof(colors.rgbRed));
        if (img.type == 1)
            readRawData(colors.rgbReserved, this);
//            this->dS.readRawData((char*)&colors.rgbReserved, sizeof(colors.rgbReserved));
        img.colorTable.append(colors);
    }
}
void BMPReader::readDataOfImage(BmpImage &img)
{
//    if (this->AmountOfReadBits != this->offBits)
//    {
//        qDebug() << "ALERT";
//        exit(1);
//    }
    img.width = this->width;
    img.height = this->height;
    unsigned int padding = 4 - (this->width * this->bitsPerPixel >> 3) & 3;
    if (padding == 4)
        padding = 0;
    qDebug()<<"PADDING"<<padding;
    for (int i = 0; i < this->height; i++)
    {
        for (int j = 0; j < this->width; j++)
        {
            unsigned char number;
            readRawData(number, this, this->bitsPerPixel/8);
            img.imgNumberToTable.push_back(number);
        }
        this->AmountOfReadBits += this->dS.skipRawData(padding);
    }
    // ??
    for (int j = 0; j < 1; j++)
    {
        unsigned short int number;
        readRawData(number, this);
        qDebug() << "EXPLICIT" << number;
    }

    qDebug() <<"AT END"<< this->dS.atEnd();
}
void BMPReader::readCompressionDataOfImage(BmpImage &image)
{
    image.width = this->width;
    image.height = this->height;
    unsigned int padding = 2 - (this->width * sizeof(unsigned short int)/8) % 2;
    if (padding == 2)
        padding = 0;
    qDebug()<<"PADDING"<<padding;
    qDebug()<<this->AmountOfReadBits;
    unsigned int currentWidth = 0;
    unsigned int currentHeight = 0;
    int i = 0;

    while(true)
    {
        if (i%2 == 1)
        {
            this->AmountOfReadBits += this->dS.skipRawData(sizeof(unsigned char));
            i++;
        }
        unsigned char firstbyte;
        readRawData(firstbyte, this);
        i++;
        unsigned char secondbyte;
        readRawData(secondbyte, this);
        i++;

        if (firstbyte == 0x00)
        {
            if (secondbyte == 0x00)
            {
                continue;
            }
            if (secondbyte == 0x02)
            {
                unsigned char GorizontShift;
                readRawData(GorizontShift, this);
                i++;
                unsigned char VerticalShift;
                readRawData(VerticalShift, this);
                i++;
                continue;
            }
            if (secondbyte == 0x01)
                break;
            for (int j = 0; j < secondbyte; j++)
            {
                unsigned char value;
                readRawData(value, this);
                i++;
                image.imgNumberToTable.push_back(value);
            }

        } else
        {
            for (int j = 0; j < firstbyte; j++)
            {
                int grey = (int)secondbyte;
                image.imgNumberToTable.push_back(grey);
            }
        }

    }
    qDebug()<<this->AmountOfReadBits;
}



