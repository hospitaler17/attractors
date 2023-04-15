#include "fmattarctorviewer.h"
#include "ui_fmattarctorviewer.h"

fmAttarctorViewer::fmAttarctorViewer(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::fmAttarctorViewer)
{
    ui->setupUi(this);
}

fmAttarctorViewer::~fmAttarctorViewer()
{
    delete ui;
}
