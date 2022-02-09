#include "customtextedit.h"
#include <QFile>
#include <QDebug>

CustomTextEdit::CustomTextEdit(QWidget *parent)
    :QTextEdit(parent){
    fileName = "Untitled";
    filePath = QString();
    saveState = true;
    //rule.setDocument(this->document());
    this->setFont(QFont("Fira Code Medium"));
    //this->setFontWeight(QFont::Bold);
    this->setFontPointSize(10);
    this->setStyleSheet("background-color: rgb(42, 42, 42); color: white;");
}

CustomTextEdit::CustomTextEdit(const QString &filename, const QString &filepath, QWidget *parent)
    :QTextEdit(parent){
    this->setStyleSheet("background-color: rgb(42, 42, 42); color: white;");
    this->setFont(QFont("Fira Code Medium"));
    this->setFontPointSize(10);

    fileName = QString();
    filePath = filepath;
    saveState = true;

    QFileInfo fileInfo{filePath};

    /* Add syntax highlighting if the file is a C++ file */
    if (fileInfo.suffix() == "cpp" || fileInfo.suffix() == "h")
        highlighterObject.setDocument(this->document());

    if (!filename.isEmpty())
    {
        openFile(filePath);
    }
}

void CustomTextEdit::openFile(QString filepath)
{
    QFile file(filepath);
    if (!file.exists())
        return;
    if (!file.open(QIODevice::ReadOnly))
    {
        qWarning() << file.errorString();
        return;
    }

    QTextStream streamFileData(&file);
    QString fileTextContents = streamFileData.readAll();
    this->setText(fileTextContents);

    QFileInfo info{filepath};
    fileName = info.fileName();

    file.close();
}

void CustomTextEdit::save()
{
    QFile file{filePath};
    if (!file.exists())
    {
        saveAs();
    }else
    {
        if (!file.open(QFile::WriteOnly))
        {
            qWarning() << "Save-Error : " << file.errorString();
            return;
        }
        QTextStream stream{&file};
        stream << this->toPlainText();
    }
    saveState = true;
    file.close();
}

void CustomTextEdit::saveAs()
{
    QString savename = QFileDialog::getSaveFileName(this,tr("Save File As"),QString(),tr("Text File (*.txt);;C++ Files (*.cpp *.h)"));
    if (!savename.isNull())
    {
        QFile file{savename};
        if (!file.open(QIODevice::WriteOnly))
        {
            qWarning() << file.errorString();
            return;
        }

        QTextStream stream{&file};
        stream << this->toPlainText();

        QFileInfo info{savename};
        fileName = info.fileName();
        filePath = savename;

        file.close();
        saveState = true;
    }
}

bool CustomTextEdit::saved()
{
    return saveState;
}

void CustomTextEdit::setSaveState(bool state)
{
    saveState = state;
}

QString CustomTextEdit::getFileName() const
{
    return fileName;
}

QString CustomTextEdit::getFilePath() const
{
    return filePath;
}
