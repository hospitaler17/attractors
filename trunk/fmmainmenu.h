#ifndef FMMAINMENU_H
#define FMMAINMENU_H

#include <QMainWindow>
#include <QFileDialog>
#include <QMessageBox>
#include <QThread>
#include <QColorDialog>


#include "builder.h"
#include "alg.h"
#include "common.h"
#include "fmprogressbar.h"

QT_BEGIN_NAMESPACE
namespace Ui { class fmMainMenu; }
QT_END_NAMESPACE

enum TABS
{
    SINGLE_BUILDINGS = 0,
    MULTIPLE_BUILDINGS = 1,
    SETTINGS = 2
};

enum ALGS
{
    RSIF,
    DSIF
};

class fmMainMenu : public QMainWindow
{
    Q_OBJECT

public:
    fmMainMenu(QWidget *parent = nullptr);
    ~fmMainMenu();



private:
    Ui::fmMainMenu *ui;
    Builder * builder;
    Alg * alg;
    QThread * thread;

    QHash<int, QString> colors;

    bool processLine(QByteArray line, QVector<double> & vector);
    int  getVectorFromFile(QString path, uint & size, QVector<double> & vector);
    int  sumVector(QVector<double> vec);
    void algChanged(ALGS alg);
    void setPrintColorFromForm();
    int setIntervalFromForm();


public slots:
    void printPixel(uint x, uint y);

    void onAlgComplete();

private slots:
    void on_pushButton_clicked();                   // кнопка "старт"
    void on_pb_saveToFile_clicked();                // кнопка "сохранить в файл"
    void on_tb_getPath_clicked();
    void on_tabWidget_currentChanged(int index);
    void on_pb_startMultipleAlg_clicked();
    void on_tb_getPath_multiple_clicked();
    void on_tb_getPath_probability_clicked();
    void on_rb_RSIF_clicked();
    void on_rb_DSIF_clicked();

    void onColorChange();
    void on_toolButton_clicked();

signals:
    void alg_DSIF(uint, uint, uint);
    void alg_RSIF(uint, uint, uint);
    void alg_setVector(QVector<double>);
    void alg_setVectorProbability(QVector<double>);
    void alg_setInterval(double,double,double,double);

};
#endif // FMMAINMENU_H
