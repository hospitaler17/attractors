#ifndef FMATTARCTORVIEWER_H
#define FMATTARCTORVIEWER_H

#include <QWidget>

namespace Ui {
class fmAttarctorViewer;
}

class fmAttarctorViewer : public QWidget
{
    Q_OBJECT

public:
    explicit fmAttarctorViewer(QWidget *parent = nullptr);
    ~fmAttarctorViewer();

private:
    Ui::fmAttarctorViewer *ui;
};

#endif // FMATTARCTORVIEWER_H
