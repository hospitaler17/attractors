#ifndef MAINTASK_H
#define MAINTASK_H

#include <QObject>
#include <QDebug>
#include <QThread>
#include <QFile>
#include <QPair>
#include <QList>
#include <QRectF>

#include "data.h"
#include "alg.h"
#include "printtask.h"

class MainTask : public QObject
{
    Q_OBJECT
public:
    explicit MainTask(QObject *parent = nullptr);
    ~MainTask();

    const QRectF &interval() const;

    void setData(Data * d) {data = d;}

private:
    Data        * data;

    QHash<uint, Alg*>       hashAlg;
    QHash<uint, PrintTask*> hashPaintTask;
    QHash<uint, QThread*>   hashThread;

    QRectF _interval;                   // окно

    uint sizeCoefs;                     // Размерность вектора аффинных коэффицинтов
    uint currentLevel;

    QVector<double> vecCoefs;           // Вектор аффинных коэффициентов
    QVector<double> vecProbability;     // Вектор вероятностных коэффициентов

    int  getVectorFromFile(QString path, uint &size, QVector<double> &vector);

    void setPrintColors(PrintTask *pt);

    void moveTasksToThreads(uint key, Alg* alg, PrintTask* printTask);
    void removeTasksAt(uint key);


public slots:
    bool processLine(QByteArray line, QVector<double> &vector);
    int  sumVector(QVector<double> vec);

    int slotInitVectorCoefs(QString pathToCoef);    // Загрузка вектора коэфициэнтов из файлов
    int slotInitVectorProbability(QString pathToProbability, bool isMonocristal);
    void setInterval(QRectF interval) { _interval = interval; }

    // main func

    void slotOnAlgComplete(uint key);
    void slotStartTask(ALGS algType, uint sizeWindow, uint startLevel, uint endLevel);

signals:
    void setVector(QVector<double>);
    void setVectorProb(QVector<double>);
    void signalSetInterval(QRectF);
    void signalStartRSIF(uint,uint,uint);
    void signalStartDSIF(uint,uint,uint);
    void printOnStatusBar(QString);
    void signalGettingImage(QImage,uint);

    void signalTasksStatusComplete(uint, uint);
};

#endif // MAINTASK_H
