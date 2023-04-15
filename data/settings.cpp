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


QSettings::Status Settings::openSettings(QString filename)
{
    qsettings = new QSettings(filename, QSettings::IniFormat);
    return qsettings->status();
}

void Settings::setRedColorPixel(quint8 red)
{
    qsettings->setValue("pixelColor/red", red);
}

quint8 Settings::getRedColorPixel()
{
    return qsettings->value("pixelColor/red", 0).toInt();
}

void Settings::setGreenColorPixel(quint8 green)
{
    qsettings->setValue("pixelColor/green", green);
}

quint8 Settings::getGreenColorPixel()
{
    return qsettings->value("pixelColor/green", 0).toInt();
}

void Settings::setBlueColorPixel(quint8 blue)
{
    qsettings->setValue("pixelColor/blue", blue);
}

quint8 Settings::getBlueColorPixel()
{
    return qsettings->value("pixelColor/blue", 0).toInt();
}

void Settings::setRedColorBackground(quint8 red)
{
    qsettings->setValue("background/red", red);
}

quint8 Settings::getRedColorBackground()
{
    return qsettings->value("background/red", 0).toInt();
}

void Settings::setGreenColorBackground(quint8 green)
{
    qsettings->setValue("background/green", green);
}

quint8 Settings::getGreenColorBackground()
{
    return qsettings->value("background/green", 0).toInt();
}

void Settings::setBlueColorBackground(quint8 blue)
{
    qsettings->setValue("background/blue", blue);
}

quint8 Settings::getBlueColorBackground()
{
    return qsettings->value("background/blue", 0).toInt();
}

void Settings::setLastMethod(ALGS value)
{
    qsettings->setValue("lastWorkSettings/lastMethod", value);
}

ALGS Settings::getLastMethod()
{
    return (ALGS) qsettings->value("lastWorkSettings/lastMethod", ALGS::DSIF).toInt();
}

void Settings::setLastCoefPath(QString value)
{
    qsettings->setValue("lastWorkSettings/lastCoefPath", value);
}

QString Settings::getLastCoefPath()
{
    return qsettings->value("lastWorkSettings/lastCoefPath", "").toString();
}

void Settings::setLastProbPath(QString value)
{
    qsettings->setValue("lastWorkSettings/lastProbPath", value);
}

QString Settings::getLastProbPath()
{
    return qsettings->value("lastWorkSettings/lastProbPath", "").toString();
}

void Settings::setLastIsMonocristal(bool value)
{
    qsettings->setValue("lastWorkSettings/lastIsMonocristal", value);
}

bool Settings::getLastIsMonocristal()
{
    return qsettings->value("lastWorkSettings/lastIsMonocristal", false).toBool();
}

void Settings::setLastIsMultipleBuilds(bool value)
{
    qsettings->setValue("lastWorkSettings/lastIsMultipleBuilds", value);
}

bool Settings::getLastIsMultipleBuilds()
{
    return qsettings->value("lastWorkSettings/lastIsMultipleBuilds", false).toBool();
}

void Settings::setLastInterval(uint value)
{
    qsettings->setValue("lastWorkSettings/lastInterval", value);
}

uint Settings::getLastInterval()
{
    return qsettings->value("lastWorkSettings/lastInterval", false).toUInt();
}

void Settings::setLastIntervalMaximum(uint value)
{
    qsettings->setValue("lastWorkSettings/lastIntervalMaximum", value);
}

uint Settings::getLastIntervalMaximum()
{
    return qsettings->value("lastWorkSettings/lastIntervalMaximum", false).toUInt();
}

void Settings::setLastSizeWindowX(uint value)
{
    qsettings->setValue("lastWorkSettings/lastSizeWindowX", value);
}

uint Settings::getLastSizeWindowX()
{
    return qsettings->value("lastWorkSettings/lastSizeWindowX", 500).toUInt();
}

void Settings::setLastX1(qreal value)
{
    qsettings->setValue("lastWorkSettings/lastX1", value);
}

qreal Settings::getLastX1()
{
    return qsettings->value("lastWorkSettings/lastX1", 0.0).toDouble();
}

void Settings::setLastX2(qreal value)
{
    qsettings->setValue("lastWorkSettings/lastX2", value);
}

qreal Settings::getLastX2()
{
    return qsettings->value("lastWorkSettings/lastX2", 1.0).toDouble();
}

void Settings::setLastY1(qreal value)
{
    qsettings->setValue("lastWorkSettings/lastY1", value);
}

qreal Settings::getLastY1()
{
    return qsettings->value("lastWorkSettings/lastY1", 0.0).toDouble();
}

void Settings::setLastY2(qreal value)
{
    qsettings->setValue("lastWorkSettings/lastY2", value);
}

qreal Settings::getLastY2()
{
    return qsettings->value("lastWorkSettings/lastY2", 1.0).toDouble();

}


