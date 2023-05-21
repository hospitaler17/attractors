#ifndef COMMON_H
#define COMMON_H

#include <QObject>

#define SIZE_COEF 6

enum ERRORS
{
    GOOD = 0,                       // Все хорошо
    FILE_NOT_READED,                // Не удалось прочитать файл
    EMPTY_STRING_PATH_TO_COEF,      // Пустая строка с путем до коэфициентов
    CANNOT_OPEN_FILE,               // Не удалось открыть файл
    SIZE_N_EQ_ZERO,                 // Кол-во строк с коэфициентами равна нулю
    INTERVAL_WINDOW_IS_EMPTY,       // Нет одного из интервалов окна
    INTERVAL_WINDOW_IS_UNCORRECT,   // Некорректный интервал окна

    NO_SELECTED_ITEMS               // Не выделены обьекты для отображения
};

enum ALGS
{
    RSIF,
    DSIF
};

#define MMB_SIZE 7
enum MODEL_MULTIPLE_COLUMNS
{
    MMC_NUMBER      = 0,
    MMC_PICTURE     = 6,
    MMC_NAME        = 1,
    MMC_LEVEL       = 2,
    MMC_SIZE        = 4,
    MMC_WORLD_WINDOW = 5,
    MMC_WINDOW      = 3
};


#endif // COMMON_H
