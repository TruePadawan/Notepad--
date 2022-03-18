#include "maincontroller.h"

MainController::MainController(QObject *parent)
    : QObject{parent}
{

}

TabWidget *MainController::newTabWidget(QWidget *parent)
{
    tabWidget = new TabWidget(parent);
    return tabWidget;
}

TabWidget *MainController::newTabWidget(QFile &file, QWidget *parent)
{
    tabWidget = new TabWidget(file, parent);
    return tabWidget;
}

void MainController::save()
{
    tabWidget->save();
}

void MainController::saveAs()
{
    tabWidget->saveAs();
}

void MainController::setCurrentTabWidget(TabWidget *widget)
{
    tabWidget->setFileNameAndPath(widget->getFileName(),widget->getFilePath());
    tabWidget->setFileType(widget->getFileType());
    tabWidget->setIsTextEditDataSaved(widget->getIsTextEditDataSaved());
    tabWidget->setText(widget->toPlainText());
}
