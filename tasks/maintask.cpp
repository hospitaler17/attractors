#include "maintask.h"

MainTask::MainTask(QObject *parent)
    : QObject{parent}
{
    // objs
    alg     = new Alg();
    thread  = new QThread();
    printer = new PrintTask();

    // thread
    alg->moveToThread(thread);
    thread->start();

    // sign in thread to algs
    connect(this, SIGNAL(signalStartDSIF(uint,uint,uint)), alg, SLOT(DSIF(uint,uint,uint)));
    connect(this, SIGNAL(signalStartRSIF(uint,uint,uint)), alg, SLOT(RSIF(uint,uint,uint)));

    // setters for algs
    connect(this, SIGNAL(setVector(QVector<double>)), alg, SLOT(setVector(QVector<double>)));
    connect(this, SIGNAL(setVectorProb(QVector<double>)), alg, SLOT(setVectorProbability(QVector<double>)));
    connect(this, SIGNAL(setInterval(qreal,qreal,qreal,qreal)), alg, SLOT(setInterval(qreal,qreal,qreal,qreal)));

    // complete signals from alg thread
    connect(alg, SIGNAL(DSIFcomplete()), this, SLOT(slotOnAlgComplete()));
    connect(alg, SIGNAL(RSIFcomplete()), this, SLOT(slotOnAlgComplete()));

    // signal print pixel
    connect(alg, SIGNAL(pixelFound(qint64,qint64)), printer, SLOT(setPixel(qint64,qint64)));
}

MainTask::~MainTask()
{
    thread->deleteLater();
    delete thread;
    delete alg;
}

bool MainTask::processLine(QByteArray line, QVector<double> &vector)
{
    QStringList strList = QString(line).split(" ");
    QString buff;
    bool ok = true;
    if(strList.count() == 0)
    {
        return !ok;
    }

    for(int i = 0; i < strList.count(); ++i)
    {
        buff = strList.at(i).trimmed();
        if(!buff.isEmpty())
        {
            vector.append(buff.toDouble(&ok));
            if(!ok){
                break;
            }
        }
    }

    return ok;
}

int MainTask::sumVector(QVector<double> vec)
{
    if(vec.size() == 0)
    {
        qDebug() << "vectorProbability is empty";
        return 1;
    }
    double dsum = 0.0;
    for(int i = 0; i < vec.size(); ++i)
    {
        vec += vec.at(i);
    }
    return (int) dsum;
}

int MainTask::getVectorFromFile(QString path, uint &size, QVector<double> &vector)
{
    if(path.isEmpty())
    {
        return EMPTY_STRING_PATH_TO_COEF;
    }

    QFile file(path);
    if (!file.open(QIODevice::ReadOnly))
    {
        return CANNOT_OPEN_FILE;
    }

    QByteArray line;
    while (!file.atEnd())
    {
        line = file.readLine();
        if(!processLine(line, vector))
            break;
    }
    file.close();

    uint counter = vector.count();
    size = (counter - 1) / SIZE_COEF + 1;
    if(size < 1)
    {
        return SIZE_N_EQ_ZERO;
    }

    return GOOD;
}

void MainTask::setPrintColors()
{
    // Цвет рисунка
    quint32 red = data->getSettings()->getRedColorPixel();
    quint32 green = data->getSettings()->getGreenColorPixel();
    quint32 blue = data->getSettings()->getBlueColorPixel();

    printer->setPixelColor((red << 16) + (green << 8) + blue); // #RRGGBB формат

    // Цвет фона
    red = data->getSettings()->getRedColorBackground();
    green = data->getSettings()->getGreenColorBackground();
    blue = data->getSettings()->getBlueColorBackground();

    printer->setBackgroundColor((red << 16) + (green << 8) + blue); // #RRGGBB формат
}


int MainTask::slotInitVectorCoefs(QString pathToCoef)
{
    return getVectorFromFile(pathToCoef, sizeCoefs, vecCoefs);
}

int MainTask::slotInitVectorProbability(QString pathToProbability, bool isMonocristal)
{
    uint sizeVecProb = 1;
    if(!pathToProbability.isEmpty() && !isMonocristal) // только если РСИФ
    {
        int res = getVectorFromFile(pathToProbability, sizeVecProb, vecProbability);
        if(res != GOOD) // Вышли из алгоритма если нет файла коэфициентов
        {
            return res;
        }

        if(sumVector(vecProbability) != 1)
        {
            return -1;
        }
    }
    else if(isMonocristal)
    {
        QVector<double> vecDet; // temperary
        double sum_s2j = 0.0;
        uint i;
        for(i = 0; i < sizeVecProb; i++)
        {
            vecDet.append(abs((vecCoefs.at(0+SIZE_COEF*i) * vecCoefs.at(3+SIZE_COEF*i)) - (vecCoefs.at(1+SIZE_COEF*i) * vecCoefs.at(2+SIZE_COEF*i))));

            sum_s2j += vecDet.at(i);
        }
        for(i = 0; i < sizeVecProb; i++)
        {
            vecProbability.append(vecDet.at(i)/sum_s2j);
        }
    }
    //TODO: переделать return!
    return -1;
}


/// ***********************
/// *** Single building ***


void MainTask::startSingleAlg(ALGS algType, uint sizeWindow, uint level) // старт
{

    // Установим нач значения
    emit setVector(vecCoefs);

    // Создадим изображение для отрисовки
    setPrintColors();
    printer->newImage(sizeWindow, sizeWindow);

    // Запускаем выбранный алгоритм
    if (algType == ALGS::DSIF)
    {
        emit signalStartDSIF(sizeCoefs, sizeWindow, level);
    }
    else  if (algType == ALGS::RSIF)
    {
        if( !vecProbability.isEmpty() )
            emit setVectorProb(vecProbability);
        emit signalStartRSIF(sizeCoefs, sizeWindow, level);
    }
//    ui->label->setFixedSize(sizeWindow, sizeWindow);
//    emit printOnStatusBar(tr("Выполняется расчет..."));

    //Почистим ненужное
    vecCoefs.clear();
    vecProbability.clear();
    sizeCoefs = 0;

}

/// *************************
/// *** Multiple building ***


void MainTask::startMultipleAlg(ALGS algType, uint sizeWindow, uint startLevel, uint endLevel)
{
    for(uint counter = startLevel; counter <= endLevel; ++counter)
    {
        currentLevel = counter;
        startSingleAlg(algType, sizeWindow, counter);
    }
}

void MainTask::slotOnAlgComplete()
{
//    emit signalGettingImage(printer->getImage(), currentLevel);
    data->getModels()->addImage(printer->getImage(), currentLevel);
}
