#include "fmprogressbar.h"
#include "ui_fmprogressbar.h"

fmProgressBar::fmProgressBar(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::fmProgressBar)
{
    ui->setupUi(this);

    setAttribute(Qt::WA_DeleteOnClose, true);


}

fmProgressBar::~fmProgressBar()
{
    killTimer(timerID_label);
    delete ui;

}

void fmProgressBar::timerEvent(QTimerEvent *event)
{
    static quint8 counter = 0;
    int ID = event->timerId();
    if(timerID_label == ID)
    {
        if      (counter == 0)
        {
            ui->label->setText(tr("Ожидайте"));
        }
        else if (counter == 1)
        {
            ui->label->setText(tr("Ожидайте."));
        }
        else if (counter == 2)
        {
            ui->label->setText(tr("Ожидайте.."));
        }
        else if (counter == 3)
        {
            ui->label->setText(tr("Ожидайте..."));
        }

        if(counter >= 3)
        {
            counter = 0;
        }
        else
        {
            counter++;
        }
    }
}

void fmProgressBar::setMaximum(int max)
{
    Qt::WindowFlags flags;
    flags = Qt::WindowMinimizeButtonHint;
    this->setWindowFlags(flags);
    timerID_label = startTimer(1000);

    ui->progressBar->setMaximum(max);
}

void fmProgressBar::setValue(int value)
{
    ui->progressBar->setValue(value);
}


