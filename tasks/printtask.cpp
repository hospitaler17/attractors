#include "printtask.h"

PrintTask::PrintTask(QObject *parent)
    : QObject{parent}
{
    image = new QImage();
}

PrintTask::~PrintTask()
{
    if(image)
        delete image;
}

void PrintTask::newImage(uint sizeX, uint sizeY)
{
    borderX = sizeX;
    borderY = sizeY;
    image = new QImage(sizeX, sizeY, QImage::Format_RGB32);

    if( !m_isTransparent )
    {
        image->fill(QColor(backgroundColor));
    }
}

void PrintTask::setPixelColor(uint newColor)
{
    color = newColor;
}

void PrintTask::setBackgroundColor(uint color, bool isTransparent)
{
    m_isTransparent = isTransparent;
    backgroundColor = color;
}

bool PrintTask::setPixel(qint64 x, qint64 y)
{
//    qDebug() << "new x = " << x << "; new y = " << y;
    if((x <= borderX && x > 0) && (y <= borderY && y > 0))
    {
        image->setPixel(x, y, color);
        return true;
    }
    return false;
}

QImage PrintTask::getImage()
{
    return *image;
}

bool PrintTask::writeImage(QString name)
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
    qDebug() << "saving filename == " << filename;
    return image->save(filename, "PNG");
}

void PrintTask::setPath(QString filepath)
{
    path = filepath;
}

QString PrintTask::getPath()
{
    return path;
}

