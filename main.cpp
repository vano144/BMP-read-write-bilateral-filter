#include <QCoreApplication>
#include <iostream>
#include <fstream>
#include <QDebug>
#include <QDataStream>
#include "main.h"
#include "bmpreader.h"
#include "bmpimage.h"
#include <QImage>

int main(int argc, char *argv[])
{

    QCoreApplication a(argc, argv);
    QString filename = "/Users/v144/Downloads/Image\ Processing/images4dz/GRAY_RLE.bmp";
    BMPReader bmpreader(filename);
    bmpreader.parseBMPFILE();
    bmpreader.showInfo();
    int typeOfMask = bmpreader.checkRGBmask();
    qDebug()<<"TYPE of RGBMASK"<<bmpreader.checkRGBmask();
    if (typeOfMask == 0)
    {
        qDebug()<<"Without table case";
        qDebug()<<"Not Implemented";

    } else if (typeOfMask == 1)
    {
        qDebug()<<"With table case";
        BmpImage bmpImage;
        if (bmpreader.type == WINDOW)
        {
            bmpImage.type = 1;
            qDebug()<<"WINDOW";
        }
        if (bmpreader.type == OS)
        {
            bmpImage.type = 0;
            qDebug()<<"OS";
        }
        if ( ! bmpreader.compression)
        {
            bmpreader.readTableOfImage(bmpImage);
            bmpreader.readDataOfImage(bmpImage);
            bmpImage.showImg();
            QImage image(bmpImage.width, bmpImage.height, QImage::Format_Grayscale8);
            bmpImage.createQimage(image);
        }
        else
        {
            bmpreader.readTableOfImage(bmpImage);
            bmpreader.readCompressionDataOfImage(bmpImage);
//            bmpImage.showImg();
            bmpImage.setImg();
            const QVector<QVector<int>> kernel( { { 0,0,0,0,0 },{ 0,0,0,0,0 }, { 0,0,1,0,0 }, { 0,0,0,0,0 },{ 0,0,0,0,0 } } );
            bmpImage.Convolution(kernel);

//            bmpImage.createQimageFromImg(image);
            bmpImage.Kuvahary(kernel[0].size());
//            bmpImage.createQimageFromImg(image, false);
            QImage image(bmpImage.width, bmpImage.height, QImage::Format_Grayscale8);
            bmpImage.createQimageFromImg(image, true);
//            bmpImage.createQimage(image);
        }
    } else
        {
            qDebug()<<"Not Implemented";
        }
    qDebug() << "Bits readed" << bmpreader.AmountOfReadBits;

//    QString filename1 = "/Users/v144/Downloads/Image\ Processing/images4dz/GRAY_OS.bmp";
//    BMPReader bmpreader1(filename1);
//    bmpreader1.parseBMPFILE();
////    bmpreader1.showInfo();
//    qDebug()<<"TYPE of RGBMASK"<<bmpreader1.checkRGBmask();

//    QString filename2 = "/Users/v144/Downloads/Image\ Processing/images4dz/GRAY_WIN.bmp";
//    BMPReader bmpreader2(filename2);
//    bmpreader2.parseBMPFILE();
////    bmpreader2.showInfo();
//    qDebug()<<"TYPE of RGBMASK"<<bmpreader2.checkRGBmask();

//    QString filename3 = "/Users/v144/Downloads/Image\ Processing/images4dz/BIN_Os.bmp";
//    BMPReader bmpreader3(filename3);
//    bmpreader3.parseBMPFILE();
////    bmpreader3.showInfo();
//    qDebug()<<"TYPE of RGBMASK"<<bmpreader3.checkRGBmask();

//    QString filename4 = "/Users/v144/Downloads/Image\ Processing/images4dz/BIN_Win.bmp";
//    BMPReader bmpreader4(filename4);
//    bmpreader4.parseBMPFILE();
////    bmpreader4.showInfo();
//    qDebug()<<"TYPE of RGBMASK"<<bmpreader4.checkRGBmask();

//    QString filename5 = "/Users/v144/Downloads/Image\ Processing/images4dz/RGB_Os.bmp";
//    BMPReader bmpreader5(filename5);
//    bmpreader5.parseBMPFILE();
////    bmpreader5.showInfo();
//    qDebug()<<"TYPE of RGBMASK"<<bmpreader5.checkRGBmask();

//    QString filename6 = "/Users/v144/Downloads/Image\ Processing/images4dz/RGB_Win.bmp";
//    BMPReader bmpreader6(filename6);
//    bmpreader6.parseBMPFILE();
////    bmpreader6.showInfo();
//    qDebug()<<"TYPE of RGBMASK"<<bmpreader6.checkRGBmask();
    return 0;


    //    // получаем информацию о битности
    //    int colorsCount = fileInfoHeader.biBitCount >> 3;
    //    if (colorsCount < 3) {
    //        colorsCount = 3;
    //    }

    //    int bitsOnColor = fileInfoHeader.biBitCount / colorsCount;
    //    int maskValue = (1 << bitsOnColor) - 1;


//    // rgb info
//    RGBQUAD **rgbInfo = new RGBQUAD*[fileInfoHeader.biHeight];

//    for (unsigned int i = 0; i < fileInfoHeader.biHeight; i++) {
//        rgbInfo[i] = new RGBQUAD[fileInfoHeader.biWidth];
//    }

//    // определение размера отступа в конце каждой строки
//    int linePadding = ((fileInfoHeader.biWidth * (fileInfoHeader.biBitCount / 8)) % 4) & 3;

//    // чтение
//    unsigned int bufer;

//    for (unsigned int i = 0; i < fileInfoHeader.biHeight; i++) {
//        for (unsigned int j = 0; j < fileInfoHeader.biWidth; j++) {
//            read(fileStream, bufer, fileInfoHeader.biBitCount / 8);

//            rgbInfo[i][j].rgbRed = bitextract(bufer, fileInfoHeader.biRedMask);
//            rgbInfo[i][j].rgbGreen = bitextract(bufer, fileInfoHeader.biGreenMask);
//            rgbInfo[i][j].rgbBlue = bitextract(bufer, fileInfoHeader.biBlueMask);
//            rgbInfo[i][j].rgbReserved = bitextract(bufer, fileInfoHeader.biAlphaMask);
//        }
//        fileStream.seekg(linePadding, std::ios_base::cur);
//    }

//    // вывод
//    for (unsigned int i = 0; i < fileInfoHeader.biHeight; i++) {
//        for (unsigned int j = 0; j < fileInfoHeader.biWidth; j++) {
//            std::cout << std::hex
//                      << +rgbInfo[i][j].rgbRed << " "
//                      << +rgbInfo[i][j].rgbGreen << " "
//                      << +rgbInfo[i][j].rgbBlue << " "
//                      << +rgbInfo[i][j].rgbReserved
//                      << std::endl;
//        }
//        std::cout << std::endl;
//    }

     return a.exec();
}

unsigned char bitextract(const unsigned int byte, const unsigned int mask) {
    if (mask == 0) {
        return 0;
    }

    // определение количества нулевых бит справа от маски
    int
        maskBufer = mask,
        maskPadding = 0;

    while (!(maskBufer & 1)) {
        maskBufer >>= 1;
        maskPadding++;
    }

    // применение маски и смещение
    return (byte & mask) >> maskPadding;
}
