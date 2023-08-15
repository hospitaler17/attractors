#include "fmmainmenu.h"
#include "ui_fmmainmenu.h"


fmMainMenu::fmMainMenu(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::fmMainMenu)
{
    ui->setupUi(this);

    // image
    connect(ui->sb_imageColorRed, SIGNAL(textChanged(QString)), this, SLOT(onColorChange()));
    connect(ui->sb_imageColorGreen, SIGNAL(textChanged(QString)), this, SLOT(onColorChange()));
    connect(ui->sb_imageColorBlue, SIGNAL(textChanged(QString)), this, SLOT(onColorChange()));
    onColorChange();

    // background
    connect(ui->sb_backgroundColorRed, SIGNAL(textChanged(QString)), this, SLOT(onColorBackgroundChange()));
    connect(ui->sb_backgroundColorGreen, SIGNAL(textChanged(QString)), this, SLOT(onColorBackgroundChange()));
    connect(ui->sb_backgroundColorBlue, SIGNAL(textChanged(QString)), this, SLOT(onColorBackgroundChange()));
    onColorBackgroundChange();

    ui->rb_DSIF->click();

    on_cb_multipleBuilding_stateChanged(0);

    ui->pb_saveToFile->setEnabled(false);

    //TODO: сделать автоматическое смещение окна (мирового) по изображению
    ui->cb_autoFindWindow->hide();


}

fmMainMenu::~fmMainMenu()
{
    delete ui;
}

void fmMainMenu::setData(Data *data)
{
    d = data;

    initColors();
    initMasks();
    initLastWorksSettings();

    ui->tableView->setModel(d->getModels()->getMultipleModel());

    ui->tableView->resizeRowsToContents();
    ui->tableView->resizeColumnsToContents();

    ui->tableView->horizontalHeader()->setStretchLastSection(true);
    ui->tableView->setAutoScroll(true);
    ui->tableView->setSortingEnabled(true);

    ui->tableView->setSelectionMode(QAbstractItemView::SingleSelection);
    ui->tableView->setSelectionBehavior(QAbstractItemView::SelectRows);
    ui->tableView->setEditTriggers(QAbstractItemView::NoEditTriggers);

    connect(ui->tableView, SIGNAL(doubleClicked(QModelIndex)),
            this, SIGNAL(signalOpenPicture(QModelIndex)));

    ui->tabWidget->setCurrentIndex(0);

    connect(d->getModels()->getMultipleModel(), SIGNAL(itemChanged(QStandardItem*)),
            this, SLOT(slotOnItemChanged(QStandardItem*)));
}

void fmMainMenu::algChanged(ALGS alg)
{
    currentAlg = alg;
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

int fmMainMenu::setIntervalFromForm()
{
    if(        !ui->le_x1->text().isEmpty()
               || !ui->le_x2->text().isEmpty()
               || !ui->le_y1->text().isEmpty()
               || !ui->le_y2->text().isEmpty())
    {
        QRectF interval(ui->le_x1->text().toDouble(),
                        ui->le_y1->text().toDouble(),
                        ui->le_x2->text().toDouble(),
                        ui->le_y2->text().toDouble());

        emit signalSetInterval(interval);
        return GOOD;
    }
    else
    {
        return INTERVAL_WINDOW_IS_EMPTY;
    }
}

void fmMainMenu::blockButtonsOnStart(bool value)
{
    ui->pb_saveToFile->setEnabled(value);
}

void fmMainMenu::setMask(QLineEdit *obj, QString mask)
{
    QRegularExpression regExp(mask);
    QValidator *validSizeWindow = new QRegularExpressionValidator(regExp);
    obj->setValidator(validSizeWindow);

}

QModelIndexList fmMainMenu::getCheckedFromTable()
{
    QStandardItemModel * model = d->getModels()->getMultipleModel();
    QModelIndexList inds;
    for (int i = 0; i <  model->rowCount(); ++i)
    {
        QModelIndex ind = model->index(i, MMC_NUMBER);
        if(model->itemFromIndex(ind)->checkState()==Qt::Checked)
            inds.append(ind);
    }
    return inds;
}

void fmMainMenu::slotOnSingleAlgComplete(QImage image)
{

    //TODO: записать в модель
    Q_UNUSED(image);

    QString logString = tr("Расчет выполнен!");
    logString.push_front(QDateTime::currentDateTime().toString("hh:mm:ss.z - "));
    ui->statusbar->showMessage(logString, 10000);

    // Разблокируем кнопки
    blockButtonsOnStart(false);

}

int fmMainMenu::taskErrorHandler(int error)
{
    QString errorText = "";
    switch (error) {
    case GOOD:
        return error;
    case FILE_NOT_READED:
        errorText = tr("Не удалось прочитать файл");
        break;
    case EMPTY_STRING_PATH_TO_COEF:
        errorText = tr("Пустая строка пути до коэфициентов");
        break;
    case CANNOT_OPEN_FILE:
        errorText = tr("Не удалось открыть файл");
        break;
    case SIZE_N_EQ_ZERO:
        errorText = tr("Кол-во строк с коэфициентами равна нулю или не верно считалась");
        break;
    case INTERVAL_WINDOW_IS_EMPTY:
        errorText = tr("Не указан один или несколько значений интервалов окна");
        break;
    case INTERVAL_WINDOW_IS_UNCORRECT:
        errorText = tr("Некорректный интервал окна");
        break;
    case NO_SELECTED_ITEMS:
        errorText = tr("Не выделены обьекты для отображения");
        break;
    case CANNOT_CREATE_FILE:
        errorText = tr("Не удалось создать файл с изображением");
        break;
    default:
        errorText = tr("Неизвестная ошибка (0_0). Что-то пошло совсем не так ...");
    }

    blockButtonsOnStart(false);

    QMessageBox::critical(this, tr("Ошибка"), errorText);
    return error;

}

void fmMainMenu::on_pb_saveToFile_clicked()
{
    QModelIndexList list = getCheckedFromTable();
    QString resText;
    for(uint i = 0; i < list.count(); ++i)
    {
         int res = taskErrorHandler(d->getModels()->saveImageInFile(list.at(i)));
         res==GOOD?resText=tr("Сохранение прошло успешно"):tr("Проблемы при сохранении");
         ui->statusbar->showMessage(resText, 10000);
    }
}

void fmMainMenu::on_tb_getPath_clicked()
{
    QString filePath = QFileDialog::getOpenFileName();
    if(!filePath.isEmpty())
    {
        ui->le_pathToCoef->setText(filePath);
    }
}

void fmMainMenu::slotOnChangeBuildsType(int index)
{
    if(index == SINGLE_BUILDS)
    {
        ui->pb_startSingleAlg->setText(tr("Выполнить построение"));
        ui->label_line->hide();
        ui->le_level_maximum_multiple->hide();
    }
    else if(index == MULTIPLE_BUILDS)
    {
        ui->pb_startSingleAlg->setText(tr("Выполнить построения"));
        ui->label_line->show();
        ui->le_level_maximum_multiple->show();
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
    quint32 red = ui->sb_imageColorRed->text().toInt();
    quint32 green = ui->sb_imageColorGreen->text().toInt();
    quint32 blue = ui->sb_imageColorBlue->text().toInt();

    QColor color (red, green, blue);
    QVariant variant = color;
    QString colcode = variant.toString();
    ui->label_color->setStyleSheet("QLabel { background-color :"+colcode+" ; }");
}

void fmMainMenu::onColorBackgroundChange()
{
    quint32 red = ui->sb_backgroundColorRed->text().toInt();
    quint32 green = ui->sb_backgroundColorGreen->text().toInt();
    quint32 blue = ui->sb_backgroundColorBlue->text().toInt();

    QColor color (red, green, blue);
    QVariant variant = color;
    QString colcode = variant.toString();
    ui->label_colorBackground->setStyleSheet("QLabel { background-color :"+colcode+" ; }");
}

void fmMainMenu::initColors()
{
    ui->sb_backgroundColorRed->setValue(d->getSettings()->getRedColorBackground());
    ui->sb_backgroundColorGreen->setValue(d->getSettings()->getGreenColorBackground());
    ui->sb_backgroundColorBlue->setValue(d->getSettings()->getBlueColorBackground());

    ui->sb_imageColorRed->setValue(d->getSettings()->getRedColorPixel());
    ui->sb_imageColorGreen->setValue(d->getSettings()->getGreenColorPixel());
    ui->sb_imageColorBlue->setValue(d->getSettings()->getBlueColorPixel());

    ui->cb_without_background->setChecked(d->getSettings()->getBackgroudTransparent());
}

// tool box button
void fmMainMenu::on_tb_imageColor_clicked()
{
    QColorDialog *dialog = new QColorDialog(this);
    QColor color =  dialog->getColor();
    delete dialog;
    if( !color.isValid() )
        return;
    QVariant variant = color;
    QString colcode = variant.toString();
    ui->label_color->setStyleSheet("QLabel { background-color :"+colcode+" ; }");

    ui->sb_imageColorRed->setValue(color.red());
    ui->sb_imageColorGreen->setValue(color.green());
    ui->sb_imageColorBlue->setValue(color.blue());

    d->getSettings()->setRedColorPixel(color.red());
    d->getSettings()->setGreenColorPixel(color.green());
    d->getSettings()->setBlueColorPixel(color.blue());
}

void fmMainMenu::on_tb_backgroundColor_clicked()
{
    QColorDialog *dialog = new QColorDialog(this);
    QColor color = dialog->getColor();
    delete dialog;
    if( !color.isValid() )
        return;
    QVariant variant = color;
    QString colcode = variant.toString();

    ui->label_colorBackground->setStyleSheet("QLabel { background-color :"+colcode+" ; }");

    ui->sb_backgroundColorRed->setValue(color.red());
    ui->sb_backgroundColorGreen->setValue(color.green());
    ui->sb_backgroundColorBlue->setValue(color.blue());

    d->getSettings()->setRedColorBackground(color.red());
    d->getSettings()->setGreenColorBackground(color.green());
    d->getSettings()->setBlueColorBackground(color.blue());
}

void fmMainMenu::on_pb_startSingleAlg_clicked()
{
    saveLastWorksSettings();

    int result = GOOD;

    emit signalMakeNewWorkThread();

    // Установим интервалы мирового окна
    result = taskErrorHandler(setIntervalFromForm());
    if(result != GOOD)
        return;

    // Заблокируем кнопки на начало расчетов
    blockButtonsOnStart(true);

    // Инициализация векторов
    result = taskErrorHandler(emit signalInitVectorCoef(ui->le_pathToCoef->text()));
    if(result != GOOD) return;
    result = taskErrorHandler(emit signalInitVectorProb(ui->le_pathToCoef_probability->text(), ui->cb_monocristal->isChecked()));
    if(result != GOOD) return;

    // Старт расчета
    if( !ui->cb_multipleBuilding->isChecked() )
    {
        if(result == GOOD)
            emit startSingleAlg(currentAlg,
                                ui->le_sizeWindow->text().toUInt(),
                                ui->le_level->text().toUInt(),
                                ui->le_level->text().toUInt());
    }
    else
    {
        if(result == GOOD)
            emit startMultipleAlg(currentAlg,
                                  ui->le_sizeWindow->text().toUInt(),
                                  ui->le_level->text().toUInt(),
                                  ui->le_level_maximum_multiple->text().toUInt());

    }
}

void fmMainMenu::initLastWorksSettings()
{
    ui->le_sizeWindow->setText(QString::number(d->getSettings()->getLastSizeWindowX()));

    ui->le_level->setText(QString::number(d->getSettings()->getLastInterval()));
    ui->le_level_maximum_multiple->setText(QString::number(d->getSettings()->getLastIntervalMaximum()));

    ui->le_x1->setText(QString::number(d->getSettings()->getLastX1()));
    ui->le_x2->setText(QString::number(d->getSettings()->getLastX2()));
    ui->le_y1->setText(QString::number(d->getSettings()->getLastY1()));
    ui->le_y2->setText(QString::number(d->getSettings()->getLastY2()));

    ui->le_pathToCoef->setText(d->getSettings()->getLastCoefPath());
    ui->le_pathToCoef_probability->setText(d->getSettings()->getLastProbPath());
}

void fmMainMenu::initMasks()
{
    setMask(ui->le_sizeWindow,              "[0-9]{3,}");
    setMask(ui->le_level,                   "[1-9][0-9]{1,}");
    setMask(ui->le_level_maximum_multiple,  "[1-9][0-9]{1,}");
    setMask(ui->le_x1,                      "[-]?[0-9]{1,}[.][0-9]{1,}");
    setMask(ui->le_x2,                      "[-]?[0-9]{1,}[.][0-9]{1,}");
    setMask(ui->le_y1,                      "[-]?[0-9]{1,}[.][0-9]{1,}");
    setMask(ui->le_y2,                      "[-]?[0-9]{1,}[.][0-9]{1,}");
}

void fmMainMenu::saveLastWorksSettings()
{
    ui->rb_DSIF->isChecked()?d->getSettings()->setLastMethod(ALGS::DSIF):d->getSettings()->setLastMethod(ALGS::RSIF);
    ui->cb_monocristal->isChecked()?d->getSettings()->setLastIsMonocristal(true):d->getSettings()->setLastIsMonocristal(false);

    d->getSettings()->setLastSizeWindowX(ui->le_sizeWindow->text().toInt());

    d->getSettings()->setLastInterval(ui->le_level->text().toUInt());
    d->getSettings()->setLastIntervalMaximum(ui->le_level_maximum_multiple->text().toUInt());

    d->getSettings()->setLastX1(ui->le_x1->text().toDouble());
    d->getSettings()->setLastX2(ui->le_x2->text().toDouble());
    d->getSettings()->setLastY1(ui->le_y1->text().toDouble());
    d->getSettings()->setLastY2(ui->le_y2->text().toDouble());

    d->getSettings()->setLastCoefPath(ui->le_pathToCoef->text());
    d->getSettings()->setLastProbPath(ui->le_pathToCoef_probability->text());
}

void fmMainMenu::on_cb_multipleBuilding_stateChanged(int arg1)
{
    Q_UNUSED(arg1);

    if(!ui->cb_multipleBuilding->isChecked())
    {
        slotOnChangeBuildsType(BUILDS_TYPE::SINGLE_BUILDS);
    }
    else
    {
        slotOnChangeBuildsType(BUILDS_TYPE::MULTIPLE_BUILDS);
    }
}

void fmMainMenu::on_pb_remove_all_clicked()
{
    d->getModels()->getMultipleModel()->removeRows(0,
                                                   d->getModels()->getMultipleModel()->rowCount());
}

void fmMainMenu::on_pb_remove_current_clicked()
{
    QStandardItemModel * model = d->getModels()->getMultipleModel();
    QModelIndexList inds = getCheckedFromTable();

    for(int i = inds.count()-1; i >= 0; --i)
    {
        model->removeRow(inds.at(i).row());
    }
}

void fmMainMenu::on_pb_view_selected_clicked()
{
    QModelIndexList list = getCheckedFromTable();
    if(list.isEmpty())
    {
        taskErrorHandler(NO_SELECTED_ITEMS);
        return;
    }
    emit signalOpenPictures(list);
}

void fmMainMenu::on_cb_without_background_stateChanged(int arg1)
{
    d->getSettings()->setBackgroudTransparent(arg1==0?false:true);
}

void fmMainMenu::slotOnItemChanged(QStandardItem *item)
{
    Q_UNUSED(item);

    ui->tableView->horizontalHeader()->setSectionResizeMode(QHeaderView::ResizeToContents);
//    ui->tableView->verticalHeader()->setSectionResizeMode(QHeaderView::ResizeToContents);
}

