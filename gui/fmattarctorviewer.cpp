#include "fmattarctorviewer.h"
#include "ui_fmattarctorviewer.h"

FmAttarctorViewer::FmAttarctorViewer(QModelIndex ind, Data *d, QWidget *parent) :
    QWidget(parent),
    ui(new Ui::FmAttarctorViewer)
{
    ui->setupUi(this);

    _data = d;

    initPicture(ind);
}

FmAttarctorViewer::FmAttarctorViewer(QModelIndexList inds, Data *d, QWidget *parent) :
    QWidget(parent),
    ui(new Ui::FmAttarctorViewer)
{
    ui->setupUi(this);

    _data = d;

    initPicture(inds);
}

FmAttarctorViewer::~FmAttarctorViewer()
{
    delete ui;
}



void FmAttarctorViewer::on_pb_close_clicked()
{
    close();
}

void FmAttarctorViewer::initPicture(QModelIndex ind)
{
    QAbstractItemModel * model = _data->getModels()->getMultipleModel();

    QSizePolicy sp;
    sp.setHorizontalPolicy(QSizePolicy::Expanding);
    sp.setVerticalPolicy(QSizePolicy::Expanding);

    QLabel * picture = new QLabel();
    picture->setSizePolicy(sp);
    picture->setPixmap(model->data(model->index(ind.row(), MMC_PICTURE), Qt::DecorationRole).value<QPixmap>());

    ui->tabWidget->addTab(picture,
                          model->data(model->index(ind.row(), MMC_NAME)).toString());
}

void FmAttarctorViewer::initPicture(QModelIndexList inds)
{
    for(int i = 0; i < inds.count(); ++i)
    {
        initPicture(inds.at(i));
    }
}

