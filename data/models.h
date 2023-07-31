#ifndef MODELS_H
#define MODELS_H

#include <QObject>
#include <QAbstractItemModel>
#include <QStandardItemModel>
#include <QStandardItem>
#include <QDateTime>
#include <QModelIndex>
#include <QPixmap>
#include <QImage>
#include <QBrush>
#include <QDir>

#include "common.h"

class Models : public QObject
{
    Q_OBJECT
public:
    explicit Models(QObject *parent = nullptr);

    void clearMultipleModel();
    QStandardItemModel * getMultipleModel(){return modelMultipleBuild;}

    QPixmap getImageByName(const QString& name) const;

public slots:
    void addImage(QImage img, uint level, uint size, uint window, QRectF worldWindow);
    int saveImageInFile(QModelIndex ind);

private:
    QStandardItemModel * modelMultipleBuild;

    void initModel();
    QHash<QString, QPixmap> _images;

    QColor _backgroundColor = QColor(150,150,150);


signals:
    void newImageAdded();

};

#endif // MODELS_H
