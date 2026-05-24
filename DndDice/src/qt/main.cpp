#include "MainWindow.h"
#include <QApplication>
#include <QLocale>
#include <QFile>
#include <QPalette>
#include <clocale>

int main(int argc, char* argv[]) {
    setlocale(LC_ALL, "Russian");
    QLocale::setDefault(QLocale(QLocale::Russian, QLocale::Russia));

    QApplication app(argc, argv);
    app.setApplicationName("DndDice");
    app.setOrganizationName("DndDice");

    QPalette pal = app.palette();
    pal.setColor(QPalette::WindowText, Qt::black);
    pal.setColor(QPalette::Text, Qt::black);
    pal.setColor(QPalette::ButtonText, Qt::black);
    app.setPalette(pal);

    QFile style(":/style.qss");
    if (style.open(QIODevice::ReadOnly))
        app.setStyleSheet(QString::fromUtf8(style.readAll()));

    MainWindow w;
    w.show();
    return app.exec();
}
