#include "fmmainmenu.h"
#include "controller.h"

#include <QApplication>
#include <QLocale>
#include <QTranslator>
#include <QStyleFactory>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    QTranslator translator;
    const QStringList uiLanguages = QLocale::system().uiLanguages();
    for (const QString &locale : uiLanguages) {
        const QString baseName = "Attractors_" + QLocale(locale).name();
        if (translator.load(":/i18n/" + baseName)) {
            a.installTranslator(&translator);
            break;
        }
    }

    QApplication::setStyle(QStyleFactory::create("Fusion"));
    QPalette p;
    p = qApp->palette();
    p.setColor(QPalette::Window, QColor(53,53,53));//общий цвет фона
    p.setColor(QPalette::Button, QColor(53,53,53));//общий цвет фона кнопок
    p.setColor(QPalette::Highlight, QColor(/*0,210,20*/Qt::blue));//Цвет для индикации выделенной или текущей записи
    p.setColor(QPalette::HighlightedText, QColor(255,255,255));//Цвет текста, отличающий его от Highlight. По умолчанию, цвет текста равен Qt::white.
    p.setColor(QPalette::ButtonText, QColor(255,255,255));//Цвет переднего плана, используемый с цветом Button
    p.setColor(QPalette::WindowText, QColor(255,255,255));//Общий цвет переднего плана
    p.setColor(QPalette::Base, QColor(28,28,28));//цвета фона в виджетах для ввода текста
    p.setColor(QPalette::AlternateBase, QColor(28,28,28));//Используется в качестве альтернативного цвета фона в представлениях с чередующимися цветами строк
    p.setColor(QPalette::Text, QColor(255,255,255));//Общий цвет переднего плана
    p.setColor(QPalette::BrightText, QColor(255,2,255));//Цвет текста, очень сильно выделяющегося на Foreground, и сильно с ним контрастирующий, например Dark.


    qApp->setPalette(p);

    Controller * controller = new Controller();
    controller->start();

    return a.exec();
}
