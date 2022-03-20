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

void MainController::setCurrentWidget(CustomTextEdit *_widget)
{
    if (this->widget != _widget)
    {
        this->widget = _widget;
    }

    auto fileNameAndType = getNameAndTypeOfFile(_widget);
    emit widgetChanged(fileNameAndType);
}

QHash<QString, QString> MainController::getNameAndTypeOfFile(CustomTextEdit *_widget)
{
    QString nameOfFileInCurrentTab = _widget->getFileName();

    QString newWindowTitle{QString("Notepad-- (%1)").arg(nameOfFileInCurrentTab)};

    QString typeOfFileInCurrentTab = _widget->getFileType() == "txt" ? "Plain Text" : "C++";

    QHash <QString,QString> fileNameAndType;
    fileNameAndType["fileName"] = newWindowTitle;
    fileNameAndType["fileType"] = typeOfFileInCurrentTab;

    return fileNameAndType;
}

MainController::~MainController()
{
    qDebug() << "Destroying controller";
}
