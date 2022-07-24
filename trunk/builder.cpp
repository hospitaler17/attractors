#include "builder.h"

Builder::Builder()
{
    path = "images";
    borderX = 0;
    borderY = 0;
    color = 0xff;
}

void Builder::newImage(uint sizeX, uint sizeY)
{
    borderX = sizeX;
    borderY = sizeY;
    qDebug() << "new: borderX = " << borderX << "; borderY = " << borderY;
    image = new QImage(sizeX, sizeY, QImage::Format_RGB32);
    image->fill(Qt::white);
}

void Builder::setPixelColor(uint newColor)
{
    color = newColor;
}

bool Builder::setPixel(uint x, uint y)
{
    qDebug() << "new x = " << x << "; new y = " << y;
    if((x <= borderX && x > 0) && (y <= borderY && y > 0))
    {
        image->setPixel(x, y, color);
        return true;
    }
    qDebug() << "print point false";
    return false;
}

QImage Builder::getImage()
{
    return *image;
}

bool Builder::writeImage(QString name)
{
    QString filename = "";
    if(name == "")
    {
        QDateTime dt = QDateTime::currentDateTime();
        filename = path + '/' + dt.toString("dd_MM_yyyy_hh:mm:ss:zzz.PNG");
    }
    else
    {
        filename = name;
    }
    qDebug() << "save filename == " << filename;
    return image->save(filename, "PNG");
}

void Builder::setPath(QString filepath)
{
    path = filepath;
}

QString Builder::getPath()
{
    return path;
}

