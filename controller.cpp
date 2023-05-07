#include "controller.h"

Controller::Controller(QObject *parent)
    : QObject{parent}
{
    data = new Data();

    fmMM = new fmMainMenu();
    fmMM->setData(data);

    mainTask = new MainTask();
    mainTask->setData(data);
}

Controller::~Controller()
{
    delete data;
    delete fmMM;
    delete mainTask;
}

void Controller::start()
{
    connectFormToTask();
    fmMM->showMaximized();
}

void Controller::connectFormToTask()
{
    // Vector'n'borders
    connect(fmMM,       SIGNAL(signalInitVectorCoef(QString)),
            mainTask,   SLOT(slotInitVectorCoefs(QString)));

    connect(fmMM,       SIGNAL(signalInitVectorProb(QString,bool)),
            mainTask,   SLOT(slotInitVectorProbability(QString,bool)));

    connect(fmMM,       SIGNAL(signalSetInterval(qreal,qreal,qreal,qreal)),
            mainTask,   SIGNAL(setInterval(qreal,qreal,qreal,qreal)));

    connect(fmMM,       SIGNAL(startSingleAlg(ALGS,uint,uint,uint)),
            mainTask,   SLOT(startMultipleAlg(ALGS,uint,uint,uint)));

    connect(fmMM,       SIGNAL(startMultipleAlg(ALGS,uint,uint,uint)),
            mainTask,   SLOT(startMultipleAlg(ALGS,uint,uint,uint)));

    connect(fmMM,       SIGNAL(signalOpenPicture(QModelIndex)),
            this,       SLOT(showPicture(QModelIndex)));
    connect(fmMM,       SIGNAL(signalOpenPictures(QModelIndexList)),
            this,       SLOT(showPictures(QModelIndexList)));


}

void Controller::showPicture(QModelIndex ind )
{
    FmAttarctorViewer *fmAV = new FmAttarctorViewer(ind, data);
    fmAV->showMaximized();
}

void Controller::showPictures(QModelIndexList inds )
{
    FmAttarctorViewer *fmAV = new FmAttarctorViewer(inds, data);
    fmAV->showMaximized();
}
