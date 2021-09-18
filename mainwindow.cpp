#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QMessageBox>
#include <QFileDialog>
#include <QDebug>
#include <QTextStream>
#include <QFile>
#include <QFileInfo>
#include <QChar>
#include <QVBoxLayout>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    ui->tabWidget->removeTab(1);
    ui->tabWidget->setMovable(true);
    ui->tabWidget->setTabText(0,"Untitled");

    // ADD A FILE INSTANCE FOR THE DEFAULT TAB
    FileInstance defaultTab{currentFileName,currentFilePath};
    fileInstances.append(defaultTab);

    currentTextEdit = ui->textEdit;

    connect(currentTextEdit, &QTextEdit::undoAvailable, [=] (bool val){
        ui->actionUndo->setEnabled(val);
    });

    connect(currentTextEdit, &QTextEdit::redoAvailable, [=] (bool val){
        ui->actionRedo->setEnabled(val);
    });

    connect(currentTextEdit, &QTextEdit::copyAvailable, [=] (bool val){
        ui->actionCopy->setEnabled(val);
    });
}

void MainWindow::modifyWindowTitle()
{
    if (!currentFilePath.isEmpty())
    {
        QString title{"Notepad-- ["};
        title.append(currentFilePath).append("]");
        setWindowTitle(title);
        ui->tabWidget->setTabText(ui->tabWidget->currentIndex(),currentFileName);
    }
}

int MainWindow::newTab()
{
    QWidget *page = new QWidget;
    QTextEdit *textarea = new QTextEdit(page);

    QVBoxLayout *layout = new QVBoxLayout;
    layout->addWidget(textarea);
    page->setLayout(layout);

    return ui->tabWidget->addTab(page,"Untitled");
}

void MainWindow::SaveAs()
{
    QString savename = QFileDialog::getSaveFileName(this,tr("Save File As"),QString(),tr("Text File (*.txt)"));
    QFile file{savename};
    if (!file.open(QIODevice::WriteOnly))
    {
        qWarning() << file.errorString();
        return;
    }

    QTextStream stream{&file};
    stream << currentTextEdit->toPlainText();
    file.close();

    QFileInfo fileinfo{savename};
    currentFileName = fileinfo.fileName();
    currentFilePath = fileinfo.filePath();
    modifyWindowTitle();
    setSaveState(true);
    return;
}

void MainWindow::Save()
{
    QFile file{currentFilePath};
    if (file.exists())
    {
        if (!file.open(QIODevice::WriteOnly))
        {
            qWarning() << file.errorString();
            return;
        }
        QTextStream stream{&file};
        stream << currentTextEdit->toPlainText();
        file.close();
        //setWindowTitle(windowTitle().sliced(1));
        setWindowTitle(windowTitle().remove('*'));
        setSaveState(true);
    }else
    {
        SaveAs();
    }
}

void MainWindow::setSaveState(bool state)
{
    if (state == false)
    {
        _savestate = false;

        ui->actionSave_File->setEnabled(true);
        ui->actionSave_As->setEnabled(true);

        if (windowTitle().at(0) != '*')
        {
            setWindowTitle(windowTitle().prepend("*"));
        }
    }else
    {
        _savestate = true;

        ui->actionSave_File->setEnabled(false);
        ui->actionSave_As->setEnabled(false);
        ui->actionSave->setEnabled(false);
        ui->actionSave_As_2->setEnabled(false);

    }
}

MainWindow::~MainWindow()
{
    delete ui;
    delete currentTextEdit;
}


void MainWindow::on_actionNew_File_triggered()
{
    if (!currentTextEdit->toPlainText().isEmpty() && _savestate == false)
    {
        auto decision = QMessageBox::question(this,"Confirmation","Are you sure you want to discard the current file before saving?",QMessageBox::Yes,QMessageBox::No);
        if (decision != QMessageBox::Yes)
        {
            return;
        }

    }
    auto val = newTab();
    qDebug() << val;
}


void MainWindow::on_actionExit_triggered()
{
    if (_savestate == false)
    {
        auto decision = QMessageBox::question(this,"Exit","Are you sure you want to exit before saving?",QMessageBox::Yes,QMessageBox::No);
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

        int index = newTab();
        ui->tabWidget->setCurrentIndex(index);

        // STORE THE NAME AND PATH OF THE CURRENT FILE IN A VARIABLE
        QFileInfo info{filepath};
        currentFileName = info.fileName();
        currentFilePath = filepath;
        fileInstances[ui->tabWidget->currentIndex()].setName_and_Path(currentFileName,currentFilePath);

        currentTextEdit = qobject_cast<QTextEdit *>(ui->tabWidget->currentWidget());

        QTextStream stream{&file};
        while (!stream.atEnd())
        {
            currentTextEdit->append(stream.readLine());
            //ui->textEdit->append(stream.readLine());
        }
        qDebug() << "Pass";
        // APPEND THE FILENAME TO THE TITLE OF THE WINDOW
        modifyWindowTitle();
        _savestate = true;
        file.close();
    }
}


void MainWindow::on_textEdit_textChanged()
{
    setSaveState(false);
    if (windowTitle().at(0) != '*')
    {
        setWindowTitle(windowTitle().prepend("*"));
    }

    if (windowTitle() == "*Notepad--" && currentTextEdit->toPlainText().isEmpty())
    {
        //setWindowTitle(windowTitle().sliced(1));
        setWindowTitle(windowTitle().remove('*'));
        setSaveState(true);
    }
}


void MainWindow::on_actionSave_File_triggered()
{
    if (_savestate == false)
    {
        if (windowTitle() == "*Notepad--")
        {
            SaveAs();
        }else{
            Save();
        }
    }
}


void MainWindow::on_actionSave_As_triggered()
{
    SaveAs();
}


void MainWindow::on_actionClose_File_triggered()
{
    if (_savestate == false)
    {
        auto decision = QMessageBox::question(this,"Confirmation","Are you sure you want to close the current file before saving?",QMessageBox::Yes,QMessageBox::No);
        if (decision == QMessageBox::Yes)
        {
            setWindowTitle("Notepad--");
            currentTextEdit->clear();
            //ui->textEdit->clear();
        }
    }else
    {
        setWindowTitle("Notepad--");
        currentTextEdit->clear();
        //ui->textEdit->clear();
    }
    currentFileName = "";
    currentFilePath = "";
    ui->tabWidget->setTabText(ui->tabWidget->currentIndex(),"Untitled");
}

void MainWindow::on_actionAbout_Qt_triggered()
{
    QMessageBox::aboutQt(this);
}


void MainWindow::on_actionNew_File_2_triggered()
{
    on_actionNew_File_triggered();
}


void MainWindow::on_tabWidget_currentChanged(int index)
{
    currentTextEdit = qobject_cast<QTextEdit *>(ui->tabWidget->widget(index));
}


void MainWindow::on_actionOpen_File_2_triggered()
{
    on_actionOpen_File_triggered();
}


void MainWindow::on_actionSave_triggered()
{
    on_actionSave_File_triggered();
}


void MainWindow::on_actionSave_As_2_triggered()
{
    on_actionSave_As_triggered();
}


void MainWindow::on_actionClose_File_2_triggered()
{
    on_actionClose_File_triggered();
}


void MainWindow::on_actionExit_2_triggered()
{
    on_actionExit_triggered();
}


void MainWindow::on_actionUndo_triggered()
{
    currentTextEdit->undo();
}


void MainWindow::on_actionRedo_triggered()
{
    currentTextEdit->redo();
}


void MainWindow::on_actionCut_triggered()
{
    currentTextEdit->cut();
}


void MainWindow::on_actionCopy_triggered()
{
    currentTextEdit->copy();
}


void MainWindow::on_actionPaste_triggered()
{
    currentTextEdit->paste();
}

