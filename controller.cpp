#include "controller.h"
#include "maintask.h"

Controller::Controller(QObject *parent)
    : QObject{parent}
{
    data = new Data();

    fmMM = new fmMainMenu();
    fmMM->setData(data);

    fmPB = nullptr;

    fmAV = nullptr;

    mainTask = new MainTask();
    mainTask->setData(data);
}

Controller::~Controller()
{
    delete data;
    delete fmMM;
    if(fmPB) delete fmPB;
    if(fmAV) delete fmAV;
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

}
