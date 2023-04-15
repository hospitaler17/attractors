#ifndef FMMAINMENU_H
#define FMMAINMENU_H

#include <QMainWindow>
#include <QFileDialog>
#include <QMessageBox>
#include <QThread>
#include <QColorDialog>
#include <QLineEdit>


#include "data.h"

#include "printtask.h"
#include "alg.h"
#include "common.h"
#include "fmprogressbar.h"

QT_BEGIN_NAMESPACE
namespace Ui { class fmMainMenu; }
QT_END_NAMESPACE

enum BUILDS_TYPE
{
    SINGLE_BUILDS = 0,   // Единичное построение
    MULTIPLE_BUILDS = 1, // Множественное построение
    SETTINGS = 2            // Настройки
};

class fmMainMenu : public QMainWindow
{
    Q_OBJECT

public:
    fmMainMenu(QWidget *parent = nullptr);
    ~fmMainMenu();

    void setData(Data * data);

private:
    Ui::fmMainMenu *ui;

    Data * d;
    QHash<int, QString> colors;
    ALGS currentAlg;

    bool processLine(QByteArray line, QVector<double> & vector);
    int  getVectorFromFile(QString path, uint & size, QVector<double> & vector);
    int  sumVector(QVector<double> vec);
    void algChanged(ALGS alg);
    int  setIntervalFromForm();
    void blockButtonsOnStart(bool value);

    void setMask(QLineEdit *obj, QString mask);


public slots:
    void slotOnSingleAlgComplete(QImage image);

    int  taskErrorHandler(int);

    void slotOnChangeBuildsType(int index);
private slots:
    /// single page
    void on_tb_getPath_clicked();                   // кнопка "Путь до файла с афинных коэф"
    void on_tb_getPath_probability_clicked();       // кнопка "Путь до файла с вероятностных коэф"

    void on_pb_saveToFile_clicked();                // кнопка "Сохранить в файл"
    void on_pb_startSingleAlg_clicked();            // кнопка "Старт алгоритма единичного построения"

    void initLastWorksSettings();
    void initMasks();

    void saveLastWorksSettings();

    /// settings page
    void initColors();
    void onColorChange();
    void onColorBackgroundChange();

    void on_tb_imageColor_clicked();
    void on_tb_backgroundColor_clicked();

    ///  common slots
    void on_rb_RSIF_clicked();
    void on_rb_DSIF_clicked();

    void on_cb_multipleBuilding_stateChanged(int arg1);

signals:
    /// single
    void startSingleAlg(ALGS,uint,uint,uint);

    /// multiple
    void startMultipleAlg(ALGS,uint,uint,uint);
    void savePicturesToFile(QVector<int>);

    /// common signals
    void signalSetInterval(qreal,qreal,qreal,qreal);
    int  signalInitVectorCoef(QString);
    int  signalInitVectorProb(QString, bool);
};
#endif // FMMAINMENU_H
