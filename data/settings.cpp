#include "settings.h"

Settings::Settings(QObject *parent)
    : QObject{parent}
{
    qsettings = nullptr;
}

Settings::~Settings()
{
    if(qsettings)
        delete qsettings;
}


QSettings::Status Settings::openSettings(const QString &filename)
{
    qsettings = new QSettings(filename, QSettings::IniFormat);
    return qsettings->status();
}

void Settings::setRedColorPixel(const quint8 &red)
{
    qsettings->setValue("pixelColor/red", red);
}

quint8 Settings::getRedColorPixel() const
{
    return qsettings->value("pixelColor/red", 0xff).toInt();
}

void Settings::setGreenColorPixel(const quint8 &green)
{
    qsettings->setValue("pixelColor/green", green);
}

quint8 Settings::getGreenColorPixel() const
{
    return qsettings->value("pixelColor/green", 0xff).toInt();
}

void Settings::setBlueColorPixel(const quint8 &blue)
{
    qsettings->setValue("pixelColor/blue", blue);
}

quint8 Settings::getBlueColorPixel() const
{
    return qsettings->value("pixelColor/blue", 0xff).toInt();
}

void Settings::setRedColorBackground(const quint8 &red)
{
    qsettings->setValue("background/red", red);
}

quint8 Settings::getRedColorBackground() const
{
    return qsettings->value("background/red", 0).toInt();
}

void Settings::setGreenColorBackground(const quint8 &green)
{
    qsettings->setValue("background/green", green);
}

quint8 Settings::getGreenColorBackground() const
{
    return qsettings->value("background/green", 0).toInt();
}

void Settings::setBlueColorBackground(const quint8 &blue)
{
    qsettings->setValue("background/blue", blue);
}

quint8 Settings::getBlueColorBackground() const
{
    return qsettings->value("background/blue", 0).toInt();
}

void Settings::setLastMethod(const ALGS &value)
{
    qsettings->setValue("lastWorkSettings/lastMethod", value);
}

ALGS Settings::getLastMethod() const
{
    return (ALGS) qsettings->value("lastWorkSettings/lastMethod", ALGS::DSIF).toInt();
}

void Settings::setLastCoefPath(const QString &value)
{
    qsettings->setValue("lastWorkSettings/lastCoefPath", value);
}

QString Settings::getLastCoefPath() const
{
    return qsettings->value("lastWorkSettings/lastCoefPath", "").toString();
}

void Settings::setLastProbPath(const QString &value)
{
    qsettings->setValue("lastWorkSettings/lastProbPath", value);
}

QString Settings::getLastProbPath() const
{
    return qsettings->value("lastWorkSettings/lastProbPath", "").toString();
}

void Settings::setLastIsMonocristal(const bool &value)
{
    qsettings->setValue("lastWorkSettings/lastIsMonocristal", value);
}

bool Settings::getLastIsMonocristal() const
{
    return qsettings->value("lastWorkSettings/lastIsMonocristal", false).toBool();
}

void Settings::setLastIsMultipleBuilds(const bool &value)
{
    qsettings->setValue("lastWorkSettings/lastIsMultipleBuilds", value);
}

bool Settings::getLastIsMultipleBuilds() const
{
    return qsettings->value("lastWorkSettings/lastIsMultipleBuilds", false).toBool();
}

void Settings::setLastInterval(const uint &value)
{
    qsettings->setValue("lastWorkSettings/lastInterval", value);
}

uint Settings::getLastInterval() const
{
    return qsettings->value("lastWorkSettings/lastInterval", 5).toUInt();
}

void Settings::setLastIntervalMaximum(const uint &value)
{
    qsettings->setValue("lastWorkSettings/lastIntervalMaximum", value);
}

uint Settings::getLastIntervalMaximum() const
{
    return qsettings->value("lastWorkSettings/lastIntervalMaximum", 10).toUInt();
}

void Settings::setLastSizeWindowX(const uint &value)
{
    qsettings->setValue("lastWorkSettings/lastSizeWindowX", value);
}

uint Settings::getLastSizeWindowX() const
{
    return qsettings->value("lastWorkSettings/lastSizeWindowX", 500).toUInt();
}

void Settings::setLastX1(const qreal &value)
{
    qsettings->setValue("lastWorkSettings/lastX1", value);
}

qreal Settings::getLastX1() const
{
    return qsettings->value("lastWorkSettings/lastX1", 0.0).toDouble();
}

void Settings::setLastX2(const qreal &value)
{
    qsettings->setValue("lastWorkSettings/lastX2", value);
}

qreal Settings::getLastX2() const
{
    return qsettings->value("lastWorkSettings/lastX2", 1.0).toDouble();
}

void Settings::setLastY1(const qreal &value)
{
    qsettings->setValue("lastWorkSettings/lastY1", value);
}

qreal Settings::getLastY1() const
{
    return qsettings->value("lastWorkSettings/lastY1", 0.0).toDouble();
}

void Settings::setLastY2(const qreal &value) const
{
    qsettings->setValue("lastWorkSettings/lastY2", value);
}

qreal Settings::getLastY2() const
{
    return qsettings->value("lastWorkSettings/lastY2", 1.0).toDouble();
}

void Settings::setBackgroudTransparent(const bool &value)
{
    qsettings->setValue("lastWorkSettings/TransparentBG", value);
}

bool Settings::getBackgroudTransparent() const
{
    return qsettings->value("lastWorkSettings/TransparentBG", true).toBool();
}


