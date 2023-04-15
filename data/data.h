#ifndef DATA_H
#define DATA_H

#include <QObject>
#include "settings.h"
#include "models.h"

#define DEFAULT_SETTINGS_FILENAME "settings.ini"
class Settings;

class Data : public QObject
{
    Q_OBJECT
public:
    explicit Data(QObject *parent = nullptr);
    ~Data();
    Settings * getSettings() {return settings;}
    Models   * getModels()   {return models;}
private:
    Settings * settings;
    Models   * models;
signals:

};

#endif // DATA_H
