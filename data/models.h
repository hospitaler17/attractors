#ifndef MODELS_H
#define MODELS_H

#include <QObject>
#include <QAbstractItemModel>
#include <QStandardItemModel>
#include <QStandardItem>
#include <QDateTime>

#define MMB_SIZE 4
enum MODEL_MULTIPLE_COLUMNS
{
    MMC_NUMBER      = 0,
    MMC_PICTURE     = 3,
    MMC_NAME        = 1,
    MMC_LEVEL       = 2

};

class Models : public QObject
{
    Q_OBJECT
public:
    explicit Models(QObject *parent = nullptr);

    void clearMultipleModel();
    QStandardItemModel * getMultipleModel(){return modelMultipleBuild;}

public slots:
    void addImage(QImage img, uint level);

private:
    QStandardItemModel * modelMultipleBuild;

    void initModel();
signals:

};

#endif // MODELS_H
