#include "models.h"

Models::Models(QObject *parent)
    : QObject{parent}
{
    initModel();
}

void Models::initModel()
{
    modelMultipleBuild = new QStandardItemModel();
    modelMultipleBuild->insertColumns(0, MMB_SIZE);

    modelMultipleBuild->setHeaderData(MMC_NUMBER, Qt::Horizontal,   tr("#"));

    modelMultipleBuild->setHeaderData(MMC_PICTURE, Qt::Horizontal,  tr("Изображение"));

    modelMultipleBuild->setHeaderData(MMC_NAME, Qt::Horizontal,     tr("Название"));

    modelMultipleBuild->setHeaderData(MMC_LEVEL, Qt::Horizontal,    tr("Номер итерации"));

}

void Models::clearMultipleModel()
{
    modelMultipleBuild->clear();
}

void Models::addImage(QImage img, uint level)
{
    int currentRow = modelMultipleBuild->rowCount();
    for(int i = 0; i < MMB_SIZE; ++i)
    {
        if(i == MMC_NUMBER)
        {
            QStandardItem * newRowCount = new QStandardItem();
            newRowCount->setCheckable(true);
            newRowCount->setCheckState(Qt::Unchecked);
            newRowCount->setBackground(QBrush(QColor(Qt::white)));
            modelMultipleBuild->setItem(currentRow, MMC_NUMBER, newRowCount);
        }
        else if (i == MMC_PICTURE)
        {
            QVariant varImg = QVariant(img);
            modelMultipleBuild->setData(modelMultipleBuild->index(currentRow, MMC_PICTURE),
                                        varImg, Qt::DecorationRole);
        }
        else if (i == MMC_NAME)
        {
            QString name = QDateTime::currentDateTime().toString("yyyy-MM-dd_hh-mm-ss(zzz)") + QString(".PNG");
            QStandardItem * newRowName = new QStandardItem(name);
            modelMultipleBuild->setItem(currentRow, MMC_NAME, newRowName);

        }
        else if (i == MMC_LEVEL)
        {
            QStandardItem * newRowLevel = new QStandardItem(QString::number(level));
            modelMultipleBuild->setItem(currentRow, MMC_LEVEL, newRowLevel);
        }
    }

}

