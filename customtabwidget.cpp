#include "customtabwidget.h"
#include <QFile>
#include <QDebug>

CustomTabWidget::CustomTabWidget(QWidget *parent)
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

CustomTabWidget::CustomTabWidget(const QString &filename, const QString &filepath, QWidget *parent)
    :QTextEdit(parent){
    this->setStyleSheet("background-color: rgb(42, 42, 42); color: white;");
    this->setFont(QFont("Fira Code Medium"));
    //this->setFontWeight(QFont::Bold);
    this->setFontPointSize(10);
    fileName = filename;
    filePath = filepath;
    saveState = true;

    QFileInfo info{filePath};
    if (info.suffix() == "cpp" || info.suffix() == "h")
        rule.setDocument(this->document());

    if (!filename.isEmpty())
    {
        openFile(filePath);
    }
}

void CustomTabWidget::openFile(QString filepath)
{
    QFile file(filepath);
        if (!file.exists())
            return;
        if (!file.open(QIODevice::ReadOnly))
        {
            qWarning() << file.errorString();
            return;
        }

        QTextStream stream(&file);
        QString content = stream.readAll();
        this->setText(content);

        QFileInfo info{filepath};
        fileName = info.fileName();

        file.close();
}

void CustomTabWidget::save()
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

void CustomTabWidget::saveAs()
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

bool CustomTabWidget::saved()
{
    return saveState;
}

void CustomTabWidget::setSaveState(bool state)
{
    saveState = state;
}

QString CustomTabWidget::getFileName() const
{
    return fileName;
}

QString CustomTabWidget::getFilePath() const
{
    return filePath;
}
