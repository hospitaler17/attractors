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

    QSettings::Status openSettings(const QString &filename);

    // Pixel Color
    void    setRedColorPixel(const quint8 &red);
    quint8  getRedColorPixel() const;

    void    setGreenColorPixel(const quint8 &green);
    quint8  getGreenColorPixel() const;

    void    setBlueColorPixel(const quint8 &blue);
    quint8  getBlueColorPixel() const;

    // Background Color
    void    setRedColorBackground(const quint8 &red);
    quint8  getRedColorBackground() const;

    void    setGreenColorBackground(const quint8 &green);
    quint8  getGreenColorBackground() const;

    void    setBlueColorBackground(const quint8 &blue);
    quint8  getBlueColorBackground() const;

    // Last work settings
    void    setLastMethod(const ALGS &value);
    ALGS    getLastMethod() const;

    void    setLastCoefPath(const QString &value);
    QString getLastCoefPath() const;

    void    setLastProbPath(const QString &value);
    QString getLastProbPath() const;

    void    setLastIsMonocristal(const bool &value);
    bool    getLastIsMonocristal() const;

    void    setLastIsMultipleBuilds(const bool &value);
    bool    getLastIsMultipleBuilds() const;

    void    setLastInterval(const uint &value);
    uint    getLastInterval() const;

    void    setLastIntervalMaximum(const uint &value);
    uint    getLastIntervalMaximum() const;

    void    setLastSizeWindowX(const uint &value);
    uint    getLastSizeWindowX() const;

    void    setLastX1(const qreal &value);
    qreal   getLastX1() const;

    void    setLastX2(const qreal &value);
    qreal   getLastX2() const;

    void    setLastY1(const qreal &value);
    qreal   getLastY1() const;

    void    setLastY2(const qreal &value) const;
    qreal   getLastY2() const;

    void    setBackgroudTransparent(const bool &value);
    bool    getBackgroudTransparent() const ;






private:
    QSettings * qsettings;
signals:
};

#endif // SETTINGS_H
