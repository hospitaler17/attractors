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

    modelMultipleBuild->setHeaderData(MMC_SIZE, Qt::Horizontal,     tr("Кол. уров."));

    modelMultipleBuild->setHeaderData(MMC_WINDOW, Qt::Horizontal,   tr("Размер изобр."));

    modelMultipleBuild->setHeaderData(MMC_WORLD_WINDOW, Qt::Horizontal, tr("Мировое окно"));


}

void Models::clearMultipleModel()
{
    modelMultipleBuild->clear();
}

void Models::addImage(QImage img, uint level, uint size, uint window, QRectF worldWindow)
{
    int currentRow = modelMultipleBuild->rowCount();
    for(int i = 0; i < MMB_SIZE; ++i)
    {
        if(i == MMC_NUMBER)
        {
            QStandardItem * newRowCount = new QStandardItem();
            newRowCount->setCheckable(true);
            newRowCount->setCheckState(Qt::Unchecked);
            newRowCount->setBackground(QBrush(QColor(200,200,200)));
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
        else if (i == MMC_SIZE)
        {
            QStandardItem * newRowSize = new QStandardItem(QString::number(size));
            modelMultipleBuild->setItem(currentRow, MMC_SIZE, newRowSize);
        }
        else if (i == MMC_WINDOW)
        {
            QStandardItem * newRowWindow = new QStandardItem(QString::number(window));
            modelMultipleBuild->setItem(currentRow, MMC_WINDOW, newRowWindow);
        }
        else if (i == MMC_WORLD_WINDOW)
        {
            QString text = QString::number(worldWindow.topLeft().rx(), 'f') + tr(";") +
                    QString::number(worldWindow.topLeft().ry(), 'f') + tr(";") +
                    QString::number(worldWindow.bottomRight().rx(), 'f') + tr(";") +
                    QString::number(worldWindow.bottomRight().ry(), 'f');
            QStandardItem * newRowWorldWindow = new QStandardItem(text);
            modelMultipleBuild->setItem(currentRow, MMC_WORLD_WINDOW, newRowWorldWindow);
        }
    }
    emit newImageAdded();
}

