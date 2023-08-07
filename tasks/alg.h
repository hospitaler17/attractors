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
    explicit Alg(const uint &key, QObject *parent = nullptr);

    void setKey(const uint &newKey);

    uint key() const;

    uint level() const;

    uint size() const;

    uint sizeWindow() const;

    QRectF interval() const;

public slots:
    void DSIF(const uint &size, const uint &sizeWindow, const uint &level);
    void RSIF(const uint &size, const uint &sizeWindow, const uint &level);

    void setVector(const QVector<qreal> &vector);
    void setInterval(const QRectF &);
    void setVectorProbability(const QVector<qreal> &vector);

signals:
    void DSIFcomplete(uint);        //!< Построение ДСИФ завершено
    void RSIFcomplete(uint);        //!< Построение РСИФ завершено
    void pixelFound(qint64,qint64); //!< Найдена точка аттрактора (пиксель)

private:
    uint _sizeWindow;       //!< Размер квадратного окна
    uint _size;             //!< Число аффинных отображений
    uint _level;            //!< Число итераций
    uint _key;              //!< Ключ-номер задачи
    qreal x1;               //!<
    qreal x2;               //!<
    qreal y1;               //!<
    qreal y2;               //!<

    int localProbability;

    int counter = 0;
    int probability = 0;

    QVector<qreal> vec;
    QVector<qreal> vecProbability;

    void convertCoefs(const uint &size, const uint &sizeWindow, qreal ** matrix);
    int getProbabilityNumber(int rundomNumber); // число number =  0 - 100
};

#endif // ALG_H
