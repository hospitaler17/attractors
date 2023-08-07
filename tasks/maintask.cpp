#include "maintask.h"

MainTask::MainTask(QObject *parent)
    : QObject{parent}
{

}

MainTask::~MainTask()
{
    for(int key = hashAlg.count() - 1; key >= 0; --key)
    {
        removeTasksAt((uint) key);
    }
}

const QRectF &MainTask::interval() const
{
    return _interval;
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

    vector.clear();

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

void MainTask::setPrintColors(PrintTask * pt)
{
    // Цвет рисунка
    quint32 red = data->getSettings()->getRedColorPixel();
    quint32 green = data->getSettings()->getGreenColorPixel();
    quint32 blue = data->getSettings()->getBlueColorPixel();

    pt->setPixelColor((red << 16) + (green << 8) + blue); // #RRGGBB формат

    // Цвет фона
    red = data->getSettings()->getRedColorBackground();
    green = data->getSettings()->getGreenColorBackground();
    blue = data->getSettings()->getBlueColorBackground();

    pt->setBackgroundColor((red << 16) + (green << 8) + blue,   // #RRGGBB формат
            data->getSettings()->getBackgroudTransparent());    // прозрачность
}

void MainTask::moveTasksToThreads(uint key, Alg *alg, PrintTask *printTask)
{
    QThread * thread = new QThread();

    alg->moveToThread(thread);
    printTask->moveToThread(thread);

    thread->start();

    hashThread.insert(key, thread);
}

void MainTask::removeTasksAt(uint key)
{
    hashThread.value(key)->exit();
    hashThread.value(key)->wait();
    hashThread.value(key)->deleteLater();

    delete hashAlg.value(key);
    delete hashPaintTask.value(key);

    hashAlg.remove(key);
    hashPaintTask.remove(key);
    hashThread.remove(key);
}


int MainTask::slotInitVectorCoefs(QString pathToCoef)
{
    return getVectorFromFile(pathToCoef, sizeCoefs, vecCoefs);
}

int MainTask::slotInitVectorProbability(QString pathToProbability, bool isMonocristal)
{
    //FIXME: из-за того, что sizeVecProb = 1 в else происходит бардак
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
    else /*if(isMonocristal)*/
    {
        QVector<qreal> vecDet; // temperary
        qreal sum_s2j = 0.0;
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
        return GOOD;
    }
    return -1;
}

void MainTask::slotOnAlgComplete(uint key)
{
//    emit signalGettingImage(printer->getImage(), currentLevel);
    data->getModels()->addImage(hashPaintTask.value(key)->getImage(),
                                hashAlg.value(key)->level(),
                                hashAlg.value(key)->size(),
                                hashAlg.value(key)->sizeWindow(),
                                hashAlg.value(key)->interval());

    removeTasksAt(key);
}

void MainTask::slotStartTask(ALGS algType, uint sizeWindow, uint startLevel, uint endLevel)
{

    for(uint counter = startLevel; counter <= endLevel; ++counter)
    {
        uint key = counter;
        while(hashAlg.contains(key))
            key++;

        /// Инициализация Alg
        Alg * alg = new Alg(key);

        // sign in thread to algs
        connect(this, SIGNAL(signalStartDSIF(uint,uint,uint)),
                alg, SLOT(DSIF(uint,uint,uint)));

        connect(this, SIGNAL(signalStartRSIF(uint,uint,uint)),
                alg, SLOT(RSIF(uint,uint,uint)));

        // complete signals from alg thread
        connect(alg, SIGNAL(DSIFcomplete(uint)),
                this, SLOT(slotOnAlgComplete(uint)));

        connect(alg, SIGNAL(RSIFcomplete(uint)),
                this, SLOT(slotOnAlgComplete(uint)));

        // setters for algs
        alg->setVector(vecCoefs);
        if(!vecProbability.isEmpty())   alg->setVectorProbability(vecProbability);
        alg->setInterval(_interval);

        // Установим нач значения
        emit setVector(vecCoefs);

        /// Инициализация PrintTask
        PrintTask * pt = new PrintTask();

        // Создадим изображение для отрисовки
        setPrintColors(pt);
        pt->newImage(sizeWindow, sizeWindow);

        connect(alg, SIGNAL(pixelFound(qint64,qint64)),
                pt, SLOT(setPixel(qint64,qint64)));


        moveTasksToThreads(key, alg, pt);

        hashAlg.insert(key, alg);
        hashPaintTask.insert(key, pt);

        // Запускаем выбранный алгоритм
        if (algType == ALGS::DSIF)
        {
            emit signalStartDSIF(sizeCoefs, sizeWindow, counter);
        }
        else  if (algType == ALGS::RSIF)
        {
            if( !vecProbability.isEmpty() )
                emit setVectorProb(vecProbability);
            emit signalStartRSIF(sizeCoefs, sizeWindow, counter);
        }
        disconnect(this, SIGNAL(signalStartDSIF(uint,uint,uint)), alg, SLOT(DSIF(uint,uint,uint)));
        disconnect(this, SIGNAL(signalStartRSIF(uint,uint,uint)), alg, SLOT(RSIF(uint,uint,uint)));
    }
}
