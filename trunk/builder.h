#ifndef BUILDER_H
#define BUILDER_H

#include <QtGlobal>
#include <QImage>
#include <QDateTime>
#include <QDebug>

class Builder
{
public:
    Builder();

    /// Работа с изображением
    void newImage(uint sizeX, uint sizeY);  // Создание изображения
    void setPixelColor(uint color);         // Установка цвета последующих пикселей
    bool setPixel(uint x, uint y);          // Нарисовать пиксель по координатам
    QImage getImage();                      // Получить текущие изображение
    bool writeImage(QString name = "");     // Запись текущего изображения в файл


    // Установка пути создания картинок (default = images/)
    void    setPath(QString filepath);
    QString getPath();

private:
    QString path;       // Путь каталога
    QImage * image;     // Изображение
    uint color;         // Цвет пикселя
    uint borderX;       // Размер изображения по X
    uint borderY;       // Размер изображения по Y
};

#endif // BUILDER_H
