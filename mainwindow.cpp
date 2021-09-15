#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QMessageBox>
#include <QFileDialog>
#include <QDebug>
#include <QTextStream>
#include <QFile>
#include <QFileInfo>
#include <QChar>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
}

void MainWindow::modifyWindowTitle()
{
    if (!currentFileName.isEmpty())
    {
        QString title{"Notepad--["};
        title.append(currentFileName).append("]");
        setWindowTitle(title);
    }
}

bool MainWindow::SaveAs()
{
    QString savename = QFileDialog::getSaveFileName(this,tr("Save File As"),QString(),tr("Text File (*.txt)"));
    QFile file{savename};
    if (!file.open(QIODevice::WriteOnly))
    {
        qWarning() << file.errorString();
        return false;
    }

    QTextStream stream{&file};
    stream << ui->textEdit->toPlainText();
    file.close();

    QFileInfo fileinfo{savename};
    currentFileName = fileinfo.fileName();
    currentFilePath = fileinfo.filePath();
    modifyWindowTitle();
    return true;
}

MainWindow::~MainWindow()
{
    delete ui;
}


void MainWindow::on_actionNew_File_triggered()
{
    if (!ui->textEdit->toPlainText().isEmpty())
    {
        auto decision = QMessageBox::question(this,"Confirmation","Are you sure you want to discard the current file before saving?",QMessageBox::Yes,QMessageBox::No);
        if (decision == QMessageBox::Yes)
        {
            ui->textEdit->setText("");
        }
    }
}


void MainWindow::on_actionExit_triggered()
{
    if (!ui->textEdit->toPlainText().isEmpty())
    {
        auto decision = QMessageBox::question(this,"Exit","Are you sure you want to exit?",QMessageBox::Yes,QMessageBox::No);
        if (decision == QMessageBox::Yes)
        {
            close();
        }
    }else{
        close();
    }
}


void MainWindow::on_actionOpen_File_triggered()
{
    QString filepath = QFileDialog::getOpenFileName(this,"Open File",QString(),tr("Text File (*.txt *.ini)"));
    if (!filepath.isNull())
    {
        QFile file{filepath};
        if (!file.open(QIODevice::ReadOnly)){
            qWarning() << file.errorString();
            return;
        }
        // STORE THE NAME AND PATH OF THE CURRENT FILE IN A VARIABLE
        QFileInfo info{filepath};
        currentFileName = info.fileName();
        currentFilePath = filepath;

        QTextStream stream{&file};
        ui->textEdit->clear();
        while (!stream.atEnd())
        {
            ui->textEdit->append(stream.readLine());
        }
        // APPEND THE FILENAME TO THE TITLE OF THE WINDOW
        modifyWindowTitle();
        file.close();
    }
}


void MainWindow::on_textEdit_textChanged()
{
    if (!ui->actionSave_File->isEnabled())
    {
        ui->actionSave_File->setEnabled(true);
        ui->actionSave_As->setEnabled(true);
    }

    if (windowTitle().at(0) != '*')
    {
        setWindowTitle(windowTitle().prepend("*"));
    }

    if (windowTitle() == "*Notepad--" && ui->textEdit->toPlainText().isEmpty())
    {
        setWindowTitle(windowTitle().sliced(1));
    }
}


void MainWindow::on_actionSave_File_triggered()
{
    if (windowTitle().at(0) == '*')
    {
        if (windowTitle() == "*Notepad--")
        {
            SaveAs();
        }

        QFile file{currentFilePath};
        if (file.exists())
        {
            if (!file.open(QIODevice::WriteOnly))
            {
                qWarning() << file.errorString();
                return;
            }
            QTextStream stream{&file};
            stream << ui->textEdit->toPlainText();
            file.close();
            setWindowTitle(windowTitle().sliced(1));
        }else
        {
            SaveAs();
        }
    }
}


void MainWindow::on_actionSave_As_triggered()
{
    SaveAs();
}


void MainWindow::on_actionClose_File_triggered()
{
    if (windowTitle().at(0) == '*')
    {
        auto decision = QMessageBox::question(this,"Confirmation","Are you sure you want to close the current file before saving?",QMessageBox::Yes,QMessageBox::No);
        if (decision == QMessageBox::Yes)
        {
            setWindowTitle("Notepad--");
            ui->textEdit->clear();
        }
    }else
    {
        setWindowTitle("Notepad--");
        ui->textEdit->clear();
    }
}

