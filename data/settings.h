#ifndef SETTINGS_H
#define SETTINGS_H

#include <QObject>
#include <QSettings>

#include "common.h"

#define DEFAULT_FILENAME "settings.ini"

class Settings : public QObject
{
    Q_OBJECT
public:
    explicit Settings(QObject *parent = nullptr);
    ~Settings();

    QSettings::Status openSettings(QString filename);

    // Pixel Color
    void    setRedColorPixel(quint8 red);
    quint8  getRedColorPixel();

    void    setGreenColorPixel(quint8 green);
    quint8  getGreenColorPixel();

    void    setBlueColorPixel(quint8 blue);
    quint8  getBlueColorPixel();

    // Background Color
    void    setRedColorBackground(quint8 red);
    quint8  getRedColorBackground();

    void    setGreenColorBackground(quint8 green);
    quint8  getGreenColorBackground();

    void    setBlueColorBackground(quint8 blue);
    quint8  getBlueColorBackground();

    // Last work settings
    void    setLastMethod(ALGS value);
    ALGS    getLastMethod();

    void    setLastCoefPath(QString value);
    QString getLastCoefPath();

    void    setLastProbPath(QString value);
    QString getLastProbPath();

    void    setLastIsMonocristal(bool value);
    bool    getLastIsMonocristal();

    void    setLastIsMultipleBuilds(bool value);
    bool    getLastIsMultipleBuilds();

    void    setLastInterval(uint value);
    uint    getLastInterval();

    void    setLastIntervalMaximum(uint value);
    uint    getLastIntervalMaximum();

    void    setLastSizeWindowX(uint value);
    uint    getLastSizeWindowX();

    void    setLastX1(qreal value);
    qreal   getLastX1();

    void    setLastX2(qreal value);
    qreal   getLastX2();

    void    setLastY1(qreal value);
    qreal   getLastY1();

    void    setLastY2(qreal value);
    qreal   getLastY2();









private:
    QSettings * qsettings;
signals:
};

#endif // SETTINGS_H
