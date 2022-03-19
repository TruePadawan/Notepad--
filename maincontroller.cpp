#include "maincontroller.h"

MainController::MainController(QObject *parent)
    : QObject{parent}
{
    qDebug() << "Main Controller created!";
}

CustomTextEdit *MainController::newWidget(QWidget *parent)
{
    widget = new CustomTextEdit(parent);
    return widget;
}

CustomTextEdit *MainController::newWidget(QString &filePath, QWidget *parent)
{
    QFile file{filePath};

    widget = new CustomTextEdit(file, parent);
    return widget;
}

void MainController::save()
{
    widget->save();
}

void MainController::saveAs()
{
    widget->saveAs();
}

void MainController::setCurrentWidget(CustomTextEdit *widget)
{
    widget->setFileNameAndPath(widget->getFileName(),widget->getFilePath());
    widget->setFileType(widget->getFileType());
    widget->setIsTextEditDataSaved(widget->getIsTextEditDataSaved());
    widget->setText(widget->toPlainText());
}

MainController::~MainController()
{
    qDebug() << "Destroying controller";
}
