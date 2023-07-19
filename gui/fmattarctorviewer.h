#ifndef FmAttarctorViewer_H
#define FmAttarctorViewer_H

#include <QWidget>
#include <QModelIndex>
#include <QAbstractItemModel>
#include <QImage>
#include <QGraphicsScene>
#include <QGraphicsView>
#include <QGraphicsPixmapItem>

#include "data/data.h"

namespace Ui {
class FmAttarctorViewer;
}

class FmAttarctorViewer : public QWidget
{
    Q_OBJECT

    Data * _data;

public:
    explicit FmAttarctorViewer(QModelIndex ind, Data *d, QWidget *parent = nullptr);
    explicit FmAttarctorViewer(QModelIndexList inds, Data *d, QWidget *parent = nullptr);

    ~FmAttarctorViewer();

protected:
     void resizeEvent(QResizeEvent *re);

private slots:

    void on_pb_close_clicked();
    QString makeCurrentPictureDescription();

private:
    Ui::FmAttarctorViewer *ui;

    void initPicture(QModelIndex ind);
    void initPicture(QModelIndexList inds);

    QSize _tabSize;
};

#endif // FmAttarctorViewer_H
