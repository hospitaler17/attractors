#ifndef FMPROGRESSBAR_H
#define FMPROGRESSBAR_H

#include <QWidget>

namespace Ui {
class fmProgressBar;
}

class fmProgressBar : public QWidget
{
    Q_OBJECT

public:
    explicit fmProgressBar(QWidget *parent = nullptr);
    ~fmProgressBar();
    void timerEvent(QTimerEvent *event);

public slots:
    void setMaximum(int max);
    void setValue(int value);

private:
    Ui::fmProgressBar *ui;
    int timerID_label;
    int counter;
};

#endif // FMPROGRESSBAR_H
