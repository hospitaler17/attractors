#ifndef ALG_H
#define ALG_H

#include <QObject>
#include <QGenericMatrix>
#include <QRandomGenerator>
#include <QRectF>

#include "common.h"

class Alg : public QObject
{
    Q_OBJECT
public:
    explicit Alg(uint key, QObject *parent = nullptr);

    void setKey(uint newKey);

    uint key() const;

    uint level() const;

    uint size() const;

    uint sizeWindow() const;

    QRectF interval() const;

public slots:
    void DSIF(uint size, uint sizeWindow, uint level);
    void RSIF(uint size, uint sizeWindow, uint level);

    void setVector(QVector<double>);
    void setInterval(QRectF);
    void setVectorProbability(QVector<double>);

signals:
    void DSIFcomplete(uint);
    void RSIFcomplete(uint);
    void pixelFound(qint64,qint64);

private:
    uint _sizeWindow;
    uint _size;
    uint _level;
    uint _key;
    qreal x1; qreal x2;
    qreal y1; qreal y2;

    int localProbability;

    QVector<double> vec;
    QVector<double> vecProbability;

    void convertCoefs(uint &size, uint &sizeWindow, double ** matrix);
    int getProbabilityNumber(int rundomNumber); // число number =  0 - 100
};

#endif // ALG_H
