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

#include "common.h"

class Models : public QObject
{
    Q_OBJECT
public:
    explicit Models(QObject *parent = nullptr);

    void clearMultipleModel();
    QStandardItemModel * getMultipleModel(){return modelMultipleBuild;}

public slots:
    void addImage(QImage img, uint level, uint size, uint window, QRectF worldWindow);

private:
    QStandardItemModel * modelMultipleBuild;

    void initModel();
    QList<QPixmap> _images;

signals:
    void newImageAdded();
signals:

};

#endif // MODELS_H
