#include "fmmainmenu.h"
#include "ui_fmmainmenu.h"

fmMainMenu::fmMainMenu(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::fmMainMenu)
{
    ui->setupUi(this);

    builder = new Builder();
    alg = new Alg();

    thread = new QThread();
    alg->moveToThread(thread);
    thread->start();


    connect(this, SIGNAL(alg_DSIF(uint,uint,uint)), alg, SLOT(DSIF(uint,uint,uint)));
    connect(this, SIGNAL(alg_RSIF(uint,uint,uint)), alg, SLOT(RSIF(uint,uint,uint)));
    connect(this, SIGNAL(alg_setVector(QVector<double>)), alg, SLOT(setVector(QVector<double>)));
    connect(this, SIGNAL(alg_setVectorProbability(QVector<double>)), alg, SLOT(setVectorProbability(QVector<double>)));
    connect(this, SIGNAL(alg_setInterval(double,double,double,double)), alg, SLOT(setInterval(double,double,double,double)));

    connect(alg, SIGNAL(DSIFcomplete()), this, SLOT(onAlgComplete()));
    connect(alg, SIGNAL(RSIFcomplete()), this, SLOT(onAlgComplete()));

    connect(alg, SIGNAL(pixelFound(uint,uint)), this, SLOT(printPixel(uint,uint)));


    connect(ui->spinBox_color_blue, SIGNAL(textChanged(QString)), this, SLOT(onColorChange()));
    connect(ui->spinBox_color_red, SIGNAL(textChanged(QString)), this, SLOT(onColorChange()));
    connect(ui->spinBox_color_green, SIGNAL(textChanged(QString)), this, SLOT(onColorChange()));


    algChanged(DSIF);
    onColorChange();

    ui->pb_saveToFile->setEnabled(false);


    //на данный момент не работет множественное построение =(
    ui->tabWidget->setTabEnabled(TABS::MULTIPLE_BUILDINGS, false); //NOTE: прикрыли вкладку множественное построение



}

fmMainMenu::~fmMainMenu()
{
    thread->deleteLater();
    delete alg;
    delete builder;

    delete ui;
}


bool fmMainMenu::processLine(QByteArray line, QVector<double> &vector)
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

int fmMainMenu::getVectorFromFile(QString path, uint &size, QVector<double> &vector)
{
    if(path.isEmpty())
    {
        QMessageBox::warning(this, tr("Внимание"), tr("Укажите файл с аффинорными коэфициентами"));
        return EMPTY_STRING_PATH_TO_COEF;
    }

    QFile file(path);
    if (!file.open(QIODevice::ReadOnly))
    {
        QMessageBox::critical(this, tr("Ошибка"), tr("Не удалось открыть файл"));
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

void fmMainMenu::algChanged(ALGS alg)
{
    if(alg == RSIF)
    {
        ui->cb_monocristal->setVisible(true);
        ui->le_pathToCoef_probability->setVisible(true);
        ui->tb_getPath_probability->setVisible(true);
        ui->label_coef_probability->setVisible(true);
    }
    else if (alg == DSIF)
    {
        ui->cb_monocristal->setVisible(false);
        ui->le_pathToCoef_probability->setVisible(false);
        ui->tb_getPath_probability->setVisible(false);
        ui->label_coef_probability->setVisible(false);
    }
}

int fmMainMenu::sumVector(QVector<double> vec)
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

void fmMainMenu::setPrintColorFromForm()
{
    quint32 red = ui->spinBox_color_red->text().toInt();
    quint32 green = ui->spinBox_color_green->text().toInt();
    quint32 blue = ui->spinBox_color_blue->text().toInt();

    builder->setPixelColor((red << 16) + (green << 8) + blue); // #RRGGBB формат
}

int fmMainMenu::setIntervalFromForm()
{
    if(        !ui->le_x1->text().isEmpty()
            || !ui->le_x2->text().isEmpty()
            || !ui->le_y1->text().isEmpty()
            || !ui->le_y2->text().isEmpty())
    {
        double x1 = ui->le_x1->text().toDouble();
        double x2 = ui->le_x2->text().toDouble();
        double y1 = ui->le_y1->text().toDouble();
        double y2 = ui->le_y2->text().toDouble();
        emit alg_setInterval(x1, x2, y1, y2);
        return GOOD;
    }
    else
    {
        QMessageBox::critical(this, tr("Ошибка"), tr("Отсутвует значение мирового окна"));
        return INTERVAL_WINDOW_IS_EMPTY;
    }

}

void fmMainMenu::printPixel(uint x, uint y)
{
    builder->setPixel(x, y);
}

void fmMainMenu::onAlgComplete()
{
    ui->statusbar->showMessage(tr("Расчет выполнен!"), 10000);
    // Отрисуем результат
    QImage image = builder->getImage();
    ui->label->setPixmap(QPixmap::fromImage(image));

    // Разблокируем кнопки
    ui->pushButton->setEnabled(true);
    ui->pb_saveToFile->setEnabled(true);
}

void fmMainMenu::on_pushButton_clicked() // кнопка старт
{
    uint sizeWindow = ui->le_sizeWindow->text().toUInt();
    uint level = ui->le_level->text().toUInt() + 1;
    uint size = 1;
    QVector<double> vec, vecProbability;

    if(getVectorFromFile(ui->le_pathToCoef->text(), size, vec) != GOOD) // Вышли из алгоритма если нет файла коэфициентов
    {

        ui->statusbar->showMessage(tr("Не удалось считать вектор аффинных коэффициенов"), 10000);
        return;
    }

    // Вектор вероятности берем из файла или создаем по формулам
    if(!ui->le_pathToCoef_probability->text().isEmpty() && ui->rb_RSIF->isChecked() && !ui->cb_monocristal->isChecked()) // только если РСИФ
    {
        if(getVectorFromFile(ui->le_pathToCoef_probability->text(), size, vecProbability) != GOOD) // Вышли из алгоритма если нет файла коэфициентов
        {
            ui->statusbar->showMessage(tr("Не удалось считать вектор вероятностных коэффициенов"), 10000);
            return;
        }
        else
        {
            if(sumVector(vecProbability) != 1)
            {
                QMessageBox::critical(this, tr("Ошибка"), tr("Сумма вероятностных коэффициентов должна быть равна одному"));
                return;
            }
        }
    }
    else if(ui->rb_RSIF->isChecked() && ui->cb_monocristal->isChecked())
    {
        QVector<double> vecDet; // temperary
        double sum_s2j = 0.0;
        uint i;
        for(i = 0; i < size; i++)
        {
            vecDet.append(abs((vec.at(0+SIZE_COEF*i) * vec.at(3+SIZE_COEF*i)) - (vec.at(1+SIZE_COEF*i) * vec.at(2+SIZE_COEF*i))));

            sum_s2j += vecDet.at(i);
        }
        for(i = 0; i < size; i++)
        {
            vecProbability.append(vecDet.at(i)/sum_s2j);
        }
    }

    // Установим нач значения
    emit alg_setVector(vec);

    // Установим интервалы мирового окна
    if(setIntervalFromForm() != GOOD)
    {
        return;
    }

    // Создадим изображение для отрисовки
    setPrintColorFromForm();
    builder->newImage(sizeWindow, sizeWindow);

    // Заблокируем кнопки
    ui->pushButton->setEnabled(false);
    ui->pb_saveToFile->setEnabled(false);

    // Запускаем выбранный алгоритм
    if(ui->rb_DSIF->isChecked())
    {
        emit alg_DSIF(size, sizeWindow, level);
    }
    else
    {
        if( !vecProbability.isEmpty() )
            emit alg_setVectorProbability(vecProbability);
        emit alg_RSIF(size, sizeWindow, level);
    }
    ui->label->setFixedSize(sizeWindow, sizeWindow);
    ui->statusbar->showMessage(tr("Выполняется расчет..."));
}

void fmMainMenu::on_pb_startMultipleAlg_clicked() //NOTE: к сожалению из-за кучи дедлайнов эта штука временно не работает
{
    uint sizeWindow = ui->le_sizeWindow->text().toUInt();
//    uint level = ui->le_level->text().toUInt();
    uint size = 1;
    QVector<double> vec, vecProbability;

    if(getVectorFromFile(ui->le_pathToCoef_multiple->text(), size, vec) != GOOD) // Вышли из алгоритма если нет файла коэфициентов
    {
        ui->statusbar->showMessage(tr("Не удалось считать коэффициены"), 10000);
        return;
    }

    // Вектор вероятности берем из файла или создаем по формулам
    if(!ui->le_pathToCoef_probability_multiple->text().isEmpty() && ui->rb_RSIF_multiple->isChecked() && !ui->cb_monocristal->isChecked()) // только если РСИФ
    {
        if(getVectorFromFile(ui->le_pathToCoef_probability_multiple->text(), size, vecProbability) != GOOD) // Вышли из алгоритма если нет файла коэфициентов
        {
            ui->statusbar->showMessage(tr("Не удалось считать вектор вероятностных коэффициенов"), 10000);
            return;
        }
        else
        {
            if(sumVector(vecProbability) != 1)
            {
                QMessageBox::critical(this, tr("Ошибка"), tr("Сумма вероятностных коэффициентов должна быть равна одному"));
                return;
            }
        }
    }
    else if(ui->rb_RSIF_multiple->isChecked() && ui->cb_monocristal->isChecked())
    {
        //TODO: добавить алгоритм построения вектора вероятностей
//        for(uint i = 0; i < size; i ++)
//        {

//        }
    }

    // Установим нач значения
    emit alg_setVector(vec);

    // Создадим изображение для отрисовки
    setPrintColorFromForm();

    if(setIntervalFromForm() != GOOD) //TODO: переделать для множественного!
    {
        return;
    }

    // ProgressBar
    fmProgressBar * progressBar = new fmProgressBar();
    progressBar->showFullScreen();

    // Запускаем выбранный алгоритм
    int algType, min, max;
    min = ui->le_level_minimum_multiple->text().toInt();
    max = ui->le_level_maximum_multiple->text().toInt();
    if(ui->rb_DSIF_multiple->isChecked())
    {
        algType = DSIF;
    }
    else if(ui->rb_RSIF_multiple->isChecked())
    {
        algType = RSIF;
    }
    progressBar->setMaximum(max-min);
    bool ok = true;
    for(int i = min; i < max; i++)
    {
        builder->newImage(sizeWindow, sizeWindow);
        if(algType == DSIF)
        {
            emit alg_DSIF(size, sizeWindow, i);
        }
        else if (algType == RSIF)
        {
            if( !vecProbability.isEmpty() )
                emit alg_setVectorProbability(vecProbability);
            emit alg_RSIF(size, sizeWindow, i);
        }
        ok = builder->writeImage();
        progressBar->setValue(i-min);
        if(!ok)
        {
            QMessageBox::critical(this, tr("Ошибка"), tr("Созданные изображения не сохраняются.\nПроцесс прерван!"));
            break;
        }
    }

    progressBar->close();
    progressBar->deleteLater();
}


void fmMainMenu::on_pb_saveToFile_clicked()
{
    bool ok = builder->writeImage();
    QString text;
    if(ok)
    {
        text = tr("Файл успешно сохранен");
    }
    else
    {
        text = tr("Файл не получилось сохранить");
    }
    qDebug() << text;
    ui->statusbar->showMessage(text, 5000);
}


void fmMainMenu::on_tb_getPath_clicked()
{
    QString filePath = QFileDialog::getOpenFileName();
    if(!filePath.isEmpty())
    {
        ui->le_pathToCoef->setText(filePath);
    }
}


void fmMainMenu::on_tabWidget_currentChanged(int index)
{
    if(index == SINGLE_BUILDINGS)
    {
        // алгоритм
        if      (ui->rb_DSIF_multiple->isChecked())
        {
            ui->rb_DSIF->setChecked(true);
        }
        else if (ui->rb_RSIF_multiple->isChecked())
        {
            ui->rb_RSIF->setChecked(true);
        }

        // путь
        ui->le_pathToCoef->setText(ui->le_pathToCoef_multiple->text());
        ui->le_pathToCoef_probability->setText(ui->le_pathToCoef_probability_multiple->text());
        ui->le_sizeWindow->setText(ui->le_sizeWindow_multiple->text());
        ui->le_level->setText(ui->le_level_maximum_multiple->text());
    }
    else if(index == MULTIPLE_BUILDINGS)
    {
        // алгоритм
        if      (ui->rb_DSIF->isChecked())
        {
            ui->rb_DSIF_multiple->setChecked(true);
        }
        else if (ui->rb_RSIF->isChecked())
        {
            ui->rb_RSIF_multiple->setChecked(true);
        }

        // путь
        ui->le_pathToCoef_multiple->setText(ui->le_pathToCoef->text());
        ui->le_pathToCoef_probability_multiple->setText(ui->le_pathToCoef_probability->text());
        ui->le_sizeWindow_multiple->setText(ui->le_sizeWindow->text());
        ui->le_level_maximum_multiple->setText(ui->le_level->text());
    }
}


void fmMainMenu::on_tb_getPath_multiple_clicked()
{
    QString filePath = QFileDialog::getOpenFileName();
    if(!filePath.isEmpty())
    {
        ui->le_pathToCoef_multiple->setText(filePath);
    }
}


void fmMainMenu::on_tb_getPath_probability_clicked()
{
    QString filePath = QFileDialog::getOpenFileName();
    if(!filePath.isEmpty())
    {
        ui->le_pathToCoef_probability->setText(filePath);
    }
}


void fmMainMenu::on_rb_RSIF_clicked()
{
    algChanged(RSIF);
}


void fmMainMenu::on_rb_DSIF_clicked()
{
    algChanged(DSIF);
}

void fmMainMenu::onColorChange()
{
    quint32 red = ui->spinBox_color_red->text().toInt();
    quint32 green = ui->spinBox_color_green->text().toInt();
    quint32 blue = ui->spinBox_color_blue->text().toInt();

//    quint32 color = ((red << 16) + (green << 8) + blue); // #RRGGBB формат

//    QColorDialog *dialog = new QColorDialog(this);
    QColor color (red, green, blue);
    QVariant variant = color;
    QString colcode = variant.toString();
    ui->label_color->setStyleSheet("QLabel { background-color :"+colcode+" ; }");
}


void fmMainMenu::on_toolButton_clicked()
{
    QColorDialog *dialog = new QColorDialog(this);
    QColor color=  dialog->getColor();
    QVariant variant= color;
    QString colcode = variant.toString();
    ui->label->setStyleSheet("QLabel { background-color :"+colcode+" ; }");

    ui->spinBox_color_red->setValue(color.red());
    ui->spinBox_color_green->setValue(color.green());
    ui->spinBox_color_blue->setValue(color.blue());
}

