#include "mainwindow.h"

#include <QApplication>
#include <QFile>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;

    QFile sheet("://resources/notepad.qss");
    sheet.open(QFile::ReadOnly);
    QString style = sheet.readAll();
    a.setStyleSheet(style);

    w.showMaximized();
    return a.exec();
}
