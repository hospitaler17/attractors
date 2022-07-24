#ifndef COMMON_H
#define COMMON_H

#define SIZE_COEF 6

enum ERRORS
{
    GOOD = 0,                       // Все хорошо
    FILE_NOT_READED,                // Не удалось прочитать файл
    EMPTY_STRING_PATH_TO_COEF,      // Пустая строка с путем до коэфициентов
    CANNOT_OPEN_FILE,               // Не удалось открыть файл
    SIZE_N_EQ_ZERO,                 // Кол-во строк с коэфициентами равна нулю
    INTERVAL_WINDOW_IS_EMPTY        // Нет одного из интервалов окна
};




#endif // COMMON_H
