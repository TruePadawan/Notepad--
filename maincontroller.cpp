#include "maincontroller.h"

MainController::MainController(QObject *parent)
    : QObject{parent}
{
    qDebug() << "Creating Controller";
    widget = nullptr;
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
    bool wasSaveSuccessfull = widget->save();
    emit widgetTextSaved(wasSaveSuccessfull, widget->getFileName());
}

void MainController::saveAs()
{
    bool wasSaveSuccessfull = widget->saveAs();
    emit widgetTextSavedAs(wasSaveSuccessfull, widget->getFileName());
}

void MainController::setCurrentWidget(CustomTextEdit *_widget)
{
    if (this->widget != _widget)
    {
        this->widget = _widget;
    }

    auto fileNameAndSyntax = getNameAndPreferredSyntax(_widget);
    emit widgetChanged(fileNameAndSyntax);
}

void MainController::updatePreferredSyntaxForTab(const QString syntaxType)
{
    widget->setPreferredSyntax(syntaxType);
}

void MainController::setWidgetToNull()
{
    this->widget = nullptr;

    emit widgetChanged(getNameAndPreferredSyntax());
}

QHash<QString, QString> MainController::getNameAndPreferredSyntax(CustomTextEdit *_widget)
{
    if (_widget == nullptr)
    {
        QHash <QString,QString> defaultValue;
        defaultValue["fileName"] = "Notepad--";
        return defaultValue;
    }

    QString nameOfFileInCurrentTab = _widget->getFileName();

    QString newWindowTitle{QString("Notepad-- (%1)").arg(nameOfFileInCurrentTab)};

    QString prefferedSyntaxInCurrentTab = _widget->getPreferredSyntax();

    QHash <QString,QString> fileNameAndSyntax;
    fileNameAndSyntax["fileName"] = newWindowTitle;
    fileNameAndSyntax["syntax"] = prefferedSyntaxInCurrentTab;

    return fileNameAndSyntax;
}

MainController::~MainController()
{
    qDebug() << "Destroying controller";
}
