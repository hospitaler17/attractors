#ifndef ALG_H
#define ALG_H

#include <QObject>
#include <QGenericMatrix>
#include <QRandomGenerator>

#include "common.h"

class Alg : public QObject
{
    Q_OBJECT
public:
    explicit Alg(QObject *parent = nullptr);

public slots:
    void DSIF(uint size, uint sizeWindow, uint level);
    void RSIF(uint size, uint sizeWindow, uint level);

    void setVector(QVector<double>);
    void setInterval(double newX1, double newX2, double newY1, double newY2);
    void setVectorProbability(QVector<double>);

signals:
    void DSIFcomplete();
    void RSIFcomplete();
    void pixelFound(uint, uint);

private:
    double x1; double x2;
    double y1; double y2;

    int localProbability;

    QVector<double> vec;
    QVector<double> vecProbability;

    int getProbabilityNumber(int rundomNumber); // число number =  0 - 100
};

#endif // ALG_H
