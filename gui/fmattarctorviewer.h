#ifndef FmAttarctorViewer_H
#define FmAttarctorViewer_H

#include <QWidget>
#include <QModelIndex>
#include <QAbstractItemModel>
#include <QImage>

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

private slots:

    void on_pb_close_clicked();

private:
    Ui::FmAttarctorViewer *ui;

    void initPicture(QModelIndex ind);
    void initPicture(QModelIndexList inds);
};

#endif // FmAttarctorViewer_H
