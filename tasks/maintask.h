#ifndef MAINTASK_H
#define MAINTASK_H

#include <QObject>
#include <QDebug>
#include <QThread>
#include <QFile>

#include "data.h"
#include "alg.h"
#include "printtask.h"

class MainTask : public QObject
{
    Q_OBJECT
public:
    explicit MainTask(QObject *parent = nullptr);
    ~MainTask();

    void setData(Data * d) {data = d;}


private:
    Alg         * alg;
    PrintTask   * printer;
    Data        * data;
    QThread     * thread;

    uint sizeCoefs;                     // Размерность вектора аффинных коэффицинтов
    uint currentLevel;

    QVector<double> vecCoefs;           // Вектор аффинных коэффициентов
    QVector<double> vecProbability;     // Вектор вероятностных коэффициентов


    int  getVectorFromFile(QString path, uint &size, QVector<double> &vector);

    void setPrintColors();


public slots:
    bool processLine(QByteArray line, QVector<double> &vector);
    int  sumVector(QVector<double> vec);

    int slotInitVectorCoefs(QString pathToCoef);    // Загрузка вектора коэфициэнтов из файлов
    int slotInitVectorProbability(QString pathToProbability, bool isMonocristal);

    // main func
    void startSingleAlg(ALGS algType, uint sizeWindow, uint level);
    void startMultipleAlg(ALGS algType, uint sizeWindow, uint startLevel, uint endLevel);

    void slotOnAlgComplete();
signals:
    void setVector(QVector<double>);
    void setVectorProb(QVector<double>);
    void setInterval(qreal,qreal,qreal,qreal);
    void signalStartRSIF(uint,uint,uint);
    void signalStartDSIF(uint,uint,uint);
    void printOnStatusBar(QString);
    void signalGettingImage(QImage,uint);
};

#endif // MAINTASK_H
