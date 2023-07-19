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

void FmAttarctorViewer::resizeEvent(QResizeEvent *re)
{
    Q_UNUSED(re);
    _tabSize = ui->tabWidget->currentWidget()->size();

    QWidget::resizeEvent(re);
}

void FmAttarctorViewer::on_pb_close_clicked()
{
    close();
}

QString FmAttarctorViewer::makeCurrentPictureDescription()
{
//    QWidget * w = ui->tabWidget->currentWidget();
    QString text;
    QString filename = ui->tabWidget->tabText(ui->tabWidget->currentIndex());
    //TODO: find in model current index by name

    return text;
}

void FmAttarctorViewer::initPicture(QModelIndex ind)
{
    QAbstractItemModel * model = _data->getModels()->getMultipleModel();

    QSizePolicy sp;
    sp.setHorizontalPolicy(QSizePolicy::Expanding);
    sp.setVerticalPolicy(QSizePolicy::Expanding);

//    QLabel * picture = new QLabel();
//    picture->setSizePolicy(sp);
    QPixmap pixmap = model->data(model->index(ind.row(), MMC_PICTURE), Qt::DecorationRole).value<QPixmap>();
    pixmap.scaled(_tabSize, Qt::KeepAspectRatio);
//    picture->setPixmap(pixmap);
//    picture->setAlignment(Qt::AlignCenter);
//    QGraphicsPixmapItem graphicPixmapItem(pixmap);
    QGraphicsScene * graphicsScene = new QGraphicsScene();
    graphicsScene->addPixmap(pixmap);

    QGraphicsView * view = new QGraphicsView(graphicsScene);
    view->setSizePolicy(sp);
    view->setInteractive(true);
    view->setDragMode(QGraphicsView::ScrollHandDrag);
    view->setResizeAnchor(QGraphicsView::AnchorViewCenter);
    ui->tabWidget->addTab(view/*picture*/,
                          model->data(model->index(ind.row(), MMC_NAME)).toString());
}

void FmAttarctorViewer::initPicture(QModelIndexList inds)
{
    for(int i = 0; i < inds.count(); ++i)
    {
        initPicture(inds.at(i));
    }
}
