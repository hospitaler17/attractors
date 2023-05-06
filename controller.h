#ifndef CONTROLLER_H
#define CONTROLLER_H

#include <QObject>

#include "data.h"

// forms
#include "fmmainmenu.h"
#include "fmprogressbar.h"
#include "fmattarctorviewer.h"

// tasks
#include "maintask.h"

class Controller : public QObject
{
    Q_OBJECT
public:
    explicit Controller(QObject *parent = nullptr);
    ~Controller();

    void start();

private:
    Data * data;

    fmMainMenu          *fmMM;
    fmProgressBar       *fmPB;
    fmAttarctorViewer   *fmAV;

    MainTask            *mainTask;

    void connectFormToTask();

signals:

};

#endif // CONTROLLER_H
