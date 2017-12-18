#include "bmpimage.h"
#include <QMap>
#include <QVector>

BmpImage::BmpImage()
{

}

void BmpImage::showColorTable()
{
    unsigned int sizeOfColorsTable = this->colorTable.size();
    qDebug() <<"SIZE OF COLORS TABLE" << sizeOfColorsTable;
    for (int i = 0; i < sizeOfColorsTable; i++)
    {
        qDebug()<<"#"<<i<<": BLUE: "<<colorTable.at(i).rgbBlue;
        qDebug()<<"#"<<i<<": GREEN: "<<colorTable.at(i).rgbGreen;
        qDebug()<<"#"<<i<<": RED: "<<colorTable.at(i).rgbRed;
        if (this->type == 1)
            qDebug()<<"#"<<i<<": RESERVED: "<<colorTable.at(i).rgbReserved;
    }
}
unsigned char BmpImage::getPixelImg(int x, int y)
{
    try
    {
        return this->result.at(x*this->width + y);
    }
    catch (int a)
    {

    }
}

unsigned char BmpImage::getPixel(int x, int y)
{
//    if (!((y*this->width+x < this->width*this->height) && (x<this->width) && (this->height)))
//    {
//        return 0;
//    }
    try
    {
        return (this->colorTable.at(this->imgNumberToTable.at(x*this->width + y))).rgbBlue;
    }
    catch (int a)
    {

    }
}
void BmpImage::setPixel(int x, int y, unsigned char value)
{
    try
    {
        this->img[x*this->width + y] = value;
    }
    catch (int a)
    {

    }
}

void BmpImage::showImg()
{
    for (int y = 0; y < this->height; ++y)
    {
        for (int x = 0; x < this->width; ++x)
        {
            qDebug()<<(int)this->getPixel(x, y);
        }
    }
}

void BmpImage::createQimage(QImage &img)
{
    for (int y = 0; y < this->height; ++y)
    {
        for (int x = 0; x < this->width; ++x)
        {
            int grey = (int)this->getPixel(y, x);
            img.setPixelColor(x, this->height - y - 1, QColor(grey,grey,grey));
        }
    }
    img.save("/Users/v144/Downloads/Image\ Processing/images4dz/GRAY_WIN_MY.bmp");
}

void BmpImage::createQimageFromImg(QImage &img, bool flag)
{
    qDebug()<< result.size();
    qDebug()<< this->width * this->height;
    qDebug()<< this->img.size();
    for (int y = 0; y < this->height; ++y)
    {
        for (int x = 0; x < this->width; ++x)
        {
            int grey = 0;
            if (flag)
                try
                {
                    grey = this->img.at(y*this->width + x);
                } catch (int a)
                {

                }

            else
                grey = (int)this->getPixelImg(y, x);
            img.setPixelColor(x, this->height - y - 1, QColor(grey,grey,grey));
        }
    }
    qDebug()<<"end";
    img.save("/Users/v144/Downloads/Image\ Processing/images4dz/GRAY_WIN_MY.bmp");
}
void BmpImage::setImg()
{
    for (int y = 0; y < this->height; ++y)
    {
        for (int x = 0; x < this->width; ++x)
        {
            int grey = (int)this->getPixel(y, x);
            this->img.append(grey);
        }
    }
}

void BmpImage::Convolution(const QVector<QVector<int>> pkernel)
{
    int iKernelSize = pkernel.at(0).size();
    int iHalf = iKernelSize / 2;
    unsigned char* pSrc = &this->img[0];
    unsigned char* pRes = new unsigned char[this->width*this->height];
    int iHeight = this->height;
    int iWidth = this->width;
    memcpy( pRes, pSrc, sizeof(*pRes) * iWidth * iHeight );

    for ( int y = iHalf; y < iHeight - iHalf; ++y )
    { // свертка
        for( int x = iHalf; x < iWidth - iHalf; ++x )
        {
            const unsigned char* ps = &pSrc[(y - iHalf) * iWidth + x - iHalf];
            int iSum = 0;
            for ( int v = 0; v < iKernelSize; ++v )
            {
                for ( int u = 0; u < iKernelSize; ++u )
                    iSum += ps[u] * pkernel.at(v).at(u);
                    ps += iWidth;
            }
            if ( iSum > 255 ) iSum = 255; // Контроль выхода из диапазона 8 bpp else
            if ( iSum < 0 ) iSum = 0;
            pRes[y * iWidth + x] = (unsigned char)iSum;
        }
    }
    for (int y = 0; y < this->height; ++y)
    {
        for (int x = 0; x < this->width; ++x)
        {
            unsigned char value = *pRes;
            this->result.push_back(value);
            pRes++;
        }
    }
}

double getMean(QVector<double> input)
{
    double result;
    for (int i = 0; i < input.size(); i++)
    {
        result+=input.at(i);
    }
    return result/input.size();
}

double getVar(QVector<double> input, double mean)
{
    QVector<double> tmp;
    for (int i = 0; i < input.size(); i++)
    {
        tmp.push_back(((int)input.at(i)-mean)*((int)input.at(i)-mean));
    }
    return getMean(tmp);
}

void getVarAndMean(int width, int sizeOfMask, int valueToShift, unsigned char* curPixel, double &mean, double &var)
{
    unsigned char * tmp= curPixel;
    QVector<double> input;
    curPixel-=valueToShift;
    for (int i = 0; i < sizeOfMask; i++)
        for (int j = 0; j < sizeOfMask; j++)
        {
            input.push_back((double)*(curPixel+width*i+j));
        }
    qDebug()<<"input"<<input;
    mean = getMean(input);
    var = getVar(input, mean);
    curPixel = tmp;
}

void BmpImage::Kuvahary(int sizeOfKernel)
{
    this->img.clear();
    int sizeOfMask = (sizeOfKernel+1)/2;
    int iKernelSize = sizeOfKernel;
    int iHalf = iKernelSize / 2;
    unsigned char* pSrc = &this->result[0];
    unsigned char* pRes = new unsigned char[this->width*this->height];
    int iHeight = this->height;
    int iWidth = this->width;
    memcpy( pRes, pSrc, sizeof(*pRes) * iWidth * iHeight );
    unsigned char* ps = &pSrc[(sizeOfMask-1)+iWidth*(sizeOfMask-1)];

    for ( int y = iHalf; y < iHeight - iHalf; ++y )
    {
        for( int x = iHalf; x < iWidth - iHalf; ++x )
        {
//            for (int i = y - iHalf; i < iKernelSize + y - iHalf; i++)
//            {
//                for (int j = x - iHalf; j < iKernelSize + x - iHalf; j++)
//                    qDebug()<<result[i*this->width + j];
//                qDebug()<<" ";
//            }
            double mean1 = 0;
            double var1 = 0;
            double mean2 = 0;
            double var2 = 0;
            double mean3 = 0;
            double var3 = 0;
            double mean4 = 0;
            double var4 = 0;
            getVarAndMean(iWidth, sizeOfMask, (sizeOfMask-1)+iWidth*(sizeOfMask-1), ps, mean1, var1);
            getVarAndMean(iWidth,sizeOfMask, iWidth*(sizeOfMask-1), ps, mean2, var2);
            getVarAndMean(iWidth,sizeOfMask, sizeOfMask-1, ps, mean3, var3);
            getVarAndMean(iWidth,sizeOfMask, 0, ps, mean4, var4);
            ps++;
            QMap<QString, double> varMeanMap;
            varMeanMap.insert(QString::number(var1), mean1);
            varMeanMap.insert(QString::number(var2), mean2);
            varMeanMap.insert(QString::number(var3), mean3);
            varMeanMap.insert(QString::number(var4), mean4);
            qDebug() << varMeanMap;
            QVector<double> vars;
            vars.push_back(var1);
            vars.push_back(var2);
            vars.push_back(var3);
            vars.push_back(var4);
            qSort(vars.begin(),vars.end());
            qDebug() << vars;
            qDebug()<< QString::number(vars.first());
            int valuek = varMeanMap.take(QString::number(vars.first()));
            if ( valuek > 255 ) valuek = 255; // Контроль выхода из диапазона 8 bpp else
            if ( valuek < 0 ) valuek = 0;
            qDebug() << "VALUE" << valuek;
//            QVector<double> a;
//            a.push_back(0);
//            a.push_back(1);
//            a.push_back(2);
//            qDebug() <<"TEST MEAN" << getMean(a);
//            qDebug() <<"TEST VAR" << getVar(a, getMean(a));
//            qDebug() << "VALUE" << valuek;
//            exit(1);
            pRes[y * iWidth + x] = (unsigned char)valuek;
//            exit(1);
        }
        ps+=iHalf*2;
    }
    for (int y = 0; y < this->height; ++y)
    {
        for (int x = 0; x < this->width; ++x)
        {
            unsigned char value = *pRes;
            this->img.push_back(value);
            pRes++;
        }
    }
}
