#include "alg.h"

Alg::Alg(const uint &key, QObject *parent)
    : QObject{parent}
{
    setKey(key);

    x1 = -3; x2 = 4;
    y1 = -3; y2 = 3;
}

void Alg::DSIF(const uint &size, const uint &sizeWindow, const uint &level)
{
    /// ВХОД
    // C        - аффинорный коэфициенты
    // size     - число аффинорных отображений
    // m        - размер кв окна (sizeWindow)
    // E_0      - матрица m на m начальных значений
    // level    - число итераций
    /// ВЫХОД
    // Т - бинарная матрица аттрактора размера m на m

    _size = size;
    _level = level;
    _sizeWindow = sizeWindow;

    uint i, j, k, l, buffX, buffY = 1;

    qreal ** matrixS = (qreal**) malloc (sizeof (qreal*) * sizeWindow);
    for (i = 0; i < sizeWindow; i++)
    {
        matrixS[i] = (qreal*) malloc (sizeof (qreal) * sizeWindow);
        for(j = 0; j < sizeWindow; ++j)
        {
            matrixS[i][j] = 0.0;
        }
    }

    qreal ** matrixT = (qreal**) malloc (sizeof (qreal*) * sizeWindow);
    for (i = 0; i < sizeWindow; i++)
    {
        matrixT[i] = (qreal*) malloc (sizeof (qreal) * sizeWindow);
        for(j = 0; j < sizeWindow; ++j)
        {
            if(i == j)
            {
                matrixT[i][j] = 1.0;
            }
            else
            {
                matrixT[i][j] = 0.0;
            }
        }
    }

    // Афинные коэффициенты
    qreal ** matrix = (qreal**) malloc (sizeof (qreal*) * size);
    {
        uint counter = 0;
        for (i = 0; i < size; ++i)
        {
            matrix[i] = (qreal*) malloc (sizeof (qreal) * SIZE_COEF);
            for(j = 0; j < SIZE_COEF; ++j)
            {
                matrix[i][j] = vec.at(counter);
                ++counter;
                qDebug() << matrix[i][j];
            }
        }
    }

    convertCoefs(size, sizeWindow, matrix);

    /// Алгоритм ДСИФ
    for(k = 0; k < level; ++k)
    {
        for(i = 0; i < sizeWindow; ++i)
        {
            for(j = 0; j < sizeWindow; ++j)
            {
                if(matrixT[i][j] == 1.0)
                {
                    for(l = 0; l < size; ++l)
                    {
                        buffX = (matrix[l][0] * i + matrix[l][1] * j + matrix[l][4]) + 1;
                        if(buffX >= 1 && buffX < sizeWindow)
                        {
                            buffY = (matrix[l][2] * i + matrix[l][3] * j + matrix[l][5]) + 1;
                            if(buffY >= 1 && buffY < sizeWindow)
                            {
                                matrixS[buffX][buffY] = 1;
                            }
                        }
                    }
                }
            }
        }
        // перенесем T = S; занулим S
        for(i = 0; i < sizeWindow; ++i)
        {
            for(j = 0; j < sizeWindow; ++j)
            {
                matrixT[i][j] = matrixS[i][j];
                matrixS[i][j] = 0.0;
            }
        }
    }

    for(i = 0; i < sizeWindow; ++i)
    {
        for(j = 0; j < sizeWindow; ++j)
        {
            if(matrixT[i][j] == 1.0)
            {
                emit pixelFound(i, j);
            }
        }
    }
    /// Конец алгоритма
    qDebug() << "DSIF complete!";
    emit DSIFcomplete(_key);


    for (i = 0; i < size; i++)
        free(matrix[i]);
    free(matrix);
    for (i = 0; i < sizeWindow; i++)
        free(matrixS[i]);
    free(matrixS);
    for (i = 0; i < sizeWindow; i++)
        free(matrixT[i]);
    free(matrixT);
    return ;
}

void Alg::RSIF(const uint &size, const uint &sizeWindow, const uint &level)
{
    /// ВХОД
    // matrix   - аффинорный коэфициенты
    // size     - число аффинорных отображений
    // level    - число итераций
    /// ВЫХОД
    // Т - бинарная матрица аттрактора размера m на m

    _size = size;
    _level = level;
    _sizeWindow = sizeWindow;

    quint32 i = 0, j = 0, k = 0, x = 0, y = 0;
    quint32 x0 = 0, y0 = 0;

    // Аффинные коэффициенты
    qreal ** matrix = (qreal**) malloc (sizeof (qreal*) * size);
    {
        uint counter = 0;
        for (i = 0; i < size; ++i)
        {
            matrix[i] = (qreal*) malloc (sizeof (qreal) * SIZE_COEF);
            for(j = 0; j < SIZE_COEF; ++j)
            {
                matrix[i][j] = vec.at(counter);
                ++counter;
            }
        }
    }

    convertCoefs(size, sizeWindow, matrix);

    /// Алгоритм РСИФ
    QRandomGenerator *rg = QRandomGenerator::global();
//    for (i = 0; i < 100; i++)
//    {
//        if( !vecProbability.isEmpty() )
//        {
//            k = getProbabilityNumber(rg->bounded(0, 100));
//        }
//        else
//        {
//            k = rg->bounded(0, (int) size);
//        }
//        x = matrix[k][0] * x0 + matrix[k][1] * y0 + matrix[k][4];
//        y = matrix[k][2] * x0 + matrix[k][3] * y0 + matrix[k][5];
//        x0 = x;
//        y0 = y;
//    }

    for(i = 0; i < level * 1000; i++)
    {
        if( !vecProbability.isEmpty() )
        {
            k = getProbabilityNumber(rg->bounded(0, 100));
        }
        else
        {
            k = rg->bounded(0, (int) size);
        }

        x = matrix[k][0] * x0 + matrix[k][1] * y0 + matrix[k][4];
        y = matrix[k][2] * x0 + matrix[k][3] * y0 + matrix[k][5];

        if(x < sizeWindow && y < sizeWindow)
            emit pixelFound(x, y);

        x0 = x;
        y0 = y;
    }
    /// Конец алгоритма

    qDebug() << "RSIF complite!";
    emit RSIFcomplete(_key);

    for (i = 0; i < size; i++)
        free(matrix[i]);
    free(matrix);

    vecProbability.clear();
}

void Alg::setVector(const QVector<qreal> &vector)
{
    vec = vector;
}

void Alg::setInterval(const QRectF &rect)
{
    x1 = rect.topLeft().rx(); x2 = rect.bottomRight().rx();
    y1 = rect.topLeft().ry(); y2 = rect.bottomRight().ry();
}

void Alg::setVectorProbability(const QVector<qreal> &vector)
{
    vecProbability = vector;
}

uint Alg::sizeWindow() const
{
    return _sizeWindow;
}

QRectF Alg::interval() const
{
    QRectF rect(QPointF(x1, y1), QPointF(x2, y2));
    return rect;
}

uint Alg::size() const
{
    return _size;
}

uint Alg::level() const
{
    return _level;
}

uint Alg::key() const
{
    return _key;
}

void Alg::setKey(const uint &newKey)
{
    _key = newKey;
}

void Alg::convertCoefs(const uint &size, const uint &sizeWindow, qreal **matrix)
{
    qreal M11, M22, w1, w2;
    qreal i1 = 0; qreal i2 = sizeWindow;
    qreal j1 = sizeWindow; qreal j2 = 0;

    for(uint i = 0; i < size; ++i)
    {
        M11 = (i2 - i1)/(x2 - x1);
        M22 = (j2 - j1)/(y2 - y1);
        w1 = i1 - M11 * x1;
        w2 = j1 - M22 * y1;

        matrix[i][1] = (M11 / M22) * matrix[i][1]; // b
        matrix[i][2] = (M22 / M11) * matrix[i][2]; // c

        matrix[i][4] = (1 - matrix[i][0]) * w1 - matrix[i][1] * w2 + M11 * matrix[i][4]; // e
        matrix[i][5] = -1 * matrix[i][2] * w1 + (1 - matrix[i][3]) * w2 + M22 * matrix[i][5]; // f
    }
}

int Alg::getProbabilityNumber(int rundomNumber)
{
    if(counter >= vecProbability.size()) // ушли за границу
    {
        counter = 0;
        probability = 0;
        localProbability = vecProbability.size()-1;
        return localProbability;
    }
    probability += (int) (vecProbability.at(counter) * 100);
    if(rundomNumber < probability) // найдено
    {
        localProbability = counter;
        counter = 0;
        probability = 0;
        return localProbability;
    }
    else
    {
        counter++;
        getProbabilityNumber(rundomNumber);
    }
    // досброс
    counter = 0;
    probability = 0;
    return localProbability;
}

