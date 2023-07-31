#ifndef PRINTTASK_H
#define PRINTTASK_H

#include <QObject>
#include <QtGlobal>
#include <QImage>
#include <QDateTime>
#include <QDebug>

class PrintTask : public QObject
{
    Q_OBJECT
public:
    explicit PrintTask(QObject *parent = nullptr);
    ~PrintTask();


    /// Работа с изображением
    void setPixelColor(uint color);                             /*!< Установка цвета последующих пикселей */
    void setBackgroundColor(uint color, bool isTransparent);    /*!< Установка цвета фона */
    QImage getImage();                                          /*!< Получить текущие изображение */

    // Установка пути создания картинок (default = images/)
    void    setPath(QString filepath);
    QString getPath();

public slots:
    void newImage(uint sizeX, uint sizeY);  /*!< Создание изображения */
    bool setPixel(qint64 x, qint64 y);      /*!< Нарисовать пиксель по координатам */
    bool writeImage(QString name = "");     /*!< Запись текущего изображения в файл */


private:
    QString path;               /*!< Путь каталога */
    QImage * image;             /*!< Изображение */
    uint color;                 /*!< Цвет пикселя */
    uint backgroundColor;       /*!< Цвет фона */
    uint borderX;               /*!< Размер изображения по X */
    uint borderY;               /*!< Размер изображения по Y */
    bool m_isTransparent;       /*!< Прозрачноть фона */
};

#endif // PRINTTASK_H
