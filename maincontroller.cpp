#include "maincontroller.h"

MainController::MainController(QObject *parent)
    : QObject{parent}
{
    qDebug() << "Creating Controller";

    widget = nullptr;
    widgetFont = QFont{"Fira Code Medium"};
}

CustomTextEdit *MainController::newWidget(QWidget *parent)
{
    widget = new CustomTextEdit(parent);
    widget->setFont(widgetFont);

    return widget;
}

CustomTextEdit *MainController::newWidget(QString &filePath, QWidget *parent)
{
    QFile file{filePath};

    widget = new CustomTextEdit(file, parent);
    widget->setFont(widgetFont);

    return widget;
}

QString MainController::getText()
{
    return widget->toPlainText();
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

void MainController::undo()
{
    widget->undo();
}

void MainController::redo()
{
    widget->redo();
}

void MainController::copy()
{
    widget->copy();
}

void MainController::cut()
{
    widget->cut();
}

void MainController::paste()
{
    widget->paste();
}

void MainController::selectAll()
{
    widget->selectAll();
}

void MainController::setCurrentWidget(CustomTextEdit *_widget)
{
    if (this->widget != _widget)
    {
        this->widget = _widget;

//        if (widget->font() != widgetFont)
//        {
//            widget->setFont(widgetFont);
//        }
    }

    auto fileNameAndSyntax = getNameAndPreferredSyntax(_widget);
    emit widgetChanged(fileNameAndSyntax);
}

void MainController::updatePreferredSyntaxForTab(const QString syntaxType)
{
    widget->setPreferredSyntax(syntaxType);
}

void MainController::setFont(QFont &newFont)
{
    if (widgetFont != newFont)
    {
        widgetFont = newFont;
    }
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
