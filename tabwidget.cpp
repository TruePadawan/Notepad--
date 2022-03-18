#include "tabwidget.h"
#include <QFileInfo>
#include <QFileDialog>

TabWidget::TabWidget(QWidget *parent): QTextEdit{parent}
{
    setFileNameAndPath("Untitled");
    configureTextEdit();
}

TabWidget::TabWidget(QFile &file, QWidget *parent) :QTextEdit{parent}
{
    configureTextEdit();

    loadTextFromFile(file);
}

void TabWidget::loadTextFromFile(QFile &file)
{
    if (!file.exists())
        return;
    if (!file.open(QIODevice::ReadOnly))
    {
        qWarning() << file.errorString();
        return;
    }

    QTextStream readTextFromFile(&file);
    this->setText(readTextFromFile.readAll());

    QFileInfo info{file};

    setFileNameAndPath(info.fileName(),info.filePath());
    setFileType(info.suffix());

    file.close();
}

void TabWidget::save()
{
    QFile file{filePath};
    if (!file.exists())
    {
        saveAs();
    }
    else
    {
        if (!file.open(QFile::WriteOnly))
        {
            qWarning() << "Error while saving : " << file.errorString();
            return;
        }

        QTextStream writeTextToFile{&file};
        writeTextToFile << this->toPlainText();

        setIsTextEditDataSaved(true);

        file.close();
    }
}

void TabWidget::saveAs()
{
    QString savename = QFileDialog::getSaveFileName(nullptr,tr("Save File As"),QString(),tr("Text File (*.txt);;C++ Files (*.cpp *.h)"));
    if (!savename.isNull())
    {
        qDebug() << savename;

        QFile file{savename};
        if (!file.open(QIODevice::WriteOnly))
        {
            qWarning() << file.errorString();
            return;
        }

        QTextStream writeTextToFile{&file};
        writeTextToFile << this->toPlainText();
        setIsTextEditDataSaved(true);

        QFileInfo info{savename};
        setFileNameAndPath(info.fileName(),info.filePath());
        setFileType(info.suffix());

        file.close();
    }
}

void TabWidget::setIsTextEditDataSaved(bool state)
{
    isTextEditDataSaved = state;
    emit textEditDataSaved(state);
}

void TabWidget::configureTextEdit()
{
    this->setFont(QFont("Fira Code Medium"));
    this->setFontPointSize(10);
    this->setStyleSheet("background-color: rgb(42, 42, 42); color: white;");
}

void TabWidget::setFileType(const QString &fileExtension)
{
    if (fileExtension == "h" || fileExtension == "cpp")
    {
        fileType = "cpp";
    }
    else if (fileExtension == "txt")
    {
        fileType = "txt";
    }

    emit fileTypeChanged(fileType);
}

void TabWidget::setFileNameAndPath(const QString &name, const QString &path)
{
    fileName = name;
    filePath = path;
}

QString TabWidget::getFileName()
{
    return fileName;
}

QString TabWidget::getFilePath()
{
    return filePath;
}

QString TabWidget::getFileType()
{
    return fileType;
}

bool TabWidget::getIsTextEditDataSaved()
{
    return isTextEditDataSaved;
}
