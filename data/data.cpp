#include "data.h"

Data::Data(QObject *parent)
    : QObject{parent}
{
    settings = new Settings();
    settings->openSettings(DEFAULT_SETTINGS_FILENAME);

    models   = new Models();
}

Data::~Data()
{
    if(settings)
        delete settings;
    if(models)
        delete models;
}
