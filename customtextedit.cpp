#include "customtextedit.h"
#include <QFileInfo>
#include <QFileDialog>

CustomTextEdit::CustomTextEdit(QWidget *parent): QTextEdit{parent}
{
    setFileNameAndPath("Untitled");
    configureTextEdit();
}

CustomTextEdit::CustomTextEdit(QFile &file, QWidget *parent) :QTextEdit{parent}
{
    configureTextEdit();

    loadTextFromFile(file);
}

void CustomTextEdit::configureTextEdit()
{
    this->setFont(QFont("Fira Code Medium"));
    this->setFontPointSize(10);
    this->setStyleSheet("background-color: rgb(42, 42, 42); color: white;");
}

void CustomTextEdit::loadTextFromFile(QFile &file)
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

void CustomTextEdit::setFileNameAndPath(const QString &name, const QString &path)
{
    fileName = name;
    filePath = path;
}

void CustomTextEdit::save()
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

void CustomTextEdit::saveAs()
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

void CustomTextEdit::setIsTextEditDataSaved(bool state)
{
    isTextEditDataSaved = state;
    emit textEditDataSaved(state);
}



void CustomTextEdit::setFileType(const QString &fileExtension)
{
    if (fileExtension == "h" || fileExtension == "cpp")
    {
        fileType = "cpp";
        setPreferredSyntax("C++");
    }
    else if (fileExtension == "txt")
    {
        fileType = "txt";
        setPreferredSyntax("Plain Text");
    }

    emit fileTypeChanged(fileType);
}

void CustomTextEdit::setPreferredSyntax(const QString &syntax)
{
    preferredSyntax = syntax;
}

QString CustomTextEdit::getFileName()
{
    return fileName;
}

QString CustomTextEdit::getFilePath()
{
    return filePath;
}

QString CustomTextEdit::getFileType()
{
    return fileType;
}

QString CustomTextEdit::getPreferredSyntax()
{
    return preferredSyntax;
}

bool CustomTextEdit::getIsTextEditDataSaved()
{
    return isTextEditDataSaved;
}

CustomTextEdit::~CustomTextEdit()
{
    qDebug() << "Deleting custom_textedit";
}
