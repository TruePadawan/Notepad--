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
    ui->tabWidget->setMovable(true);
    ui->tabWidget->setTabText(0,"Untitled");

    // ADD A FILE INSTANCE FOR THE DEFAULT TAB
//    FileInstance defaultTab{currentFileName,currentFilePath};
//    fileInstances.append(defaultTab);

    currentTextEdit = ui->textEdit;

    // ADD A DEFAULT FILE INSTANCE
//    bool state{true};
//    saveStates.append(state);
    FileInstance defaultFile{};
    fileInstances.append(defaultFile);
    saveMechanism();
}

void MainWindow::modifyWindowTitle()
{
    if (!currentFilePath.isEmpty())
    {
        QString title{"Notepad-- ["};
        title.append(currentFilePath).append("]");
        setWindowTitle(title);
        ui->tabWidget->setTabText(ui->tabWidget->currentIndex(),currentFileName);
    }else
    {
        setWindowTitle("Notepad--");
    }
}

int MainWindow::newTab()
{
    // CREATE A NEW SAVE STATE AND TEXTEDIT PAGE FOR THE NEW TAB
//    bool newState{true};
//    saveStates.append(newState);
    FileInstance newFile{};
    fileInstances.append(newFile);

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

//    FileInstance newlySaved{currentFileName,currentFilePath};
//    fileInstances.insert(ui->tabWidget->currentIndex(),newlySaved);
    fileInstances[currentTabIndex].setName_and_Path(currentFileName,currentFilePath);
    fileInstances[currentTabIndex].setSaveState(true);

    //setSaveState(true, ui->tabWidget->currentIndex());
    checkSaveState();
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
        //setSaveState(true, ui->tabWidget->currentIndex());
        fileInstances[currentTabIndex].setSaveState(true);
        checkSaveState();
    }else
    {
        SaveAs();
    }
}

void MainWindow::setSaveState(bool state, int index)
{
    saveStates[index] = state;
    _savestate = state;

    checkSaveState();
}

void MainWindow::checkSaveState()
{
    if (fileInstances[currentTabIndex].getSaveState() == false)
    {
        if (windowTitle().at(0) != '*')
        {
            setWindowTitle(windowTitle().prepend("*"));
        }
        ui->actionSave_File->setEnabled(true);
        ui->actionSave_As->setEnabled(true);
        ui->actionSave->setEnabled(true);
        ui->actionSave_As_2->setEnabled(true);
    }else
    {
        ui->actionSave_File->setEnabled(false);
        ui->actionSave_As->setEnabled(false);
        ui->actionSave->setEnabled(false);
        ui->actionSave_As_2->setEnabled(false);
    }
}

bool MainWindow::allSaved()
{
    foreach (FileInstance tmp, fileInstances) {
        if (!tmp.getSaveState())
            return false;
    }
    return true;
}

void MainWindow::saveMechanism()
{
    connect(currentTextEdit, &QTextEdit::undoAvailable, [=] (bool val){
        ui->actionUndo->setEnabled(val);
    });

    connect(currentTextEdit, &QTextEdit::redoAvailable, [=] (bool val){
        ui->actionRedo->setEnabled(val);
    });

    connect(currentTextEdit, &QTextEdit::copyAvailable, [=] (bool val){
        ui->actionCopy->setEnabled(val);
    });

    connect(currentTextEdit, &QTextEdit::textChanged, this, &MainWindow::on_textEdit_textChanged);
}

MainWindow::~MainWindow()
{
    delete ui;
    delete currentTextEdit;
}


void MainWindow::on_actionNew_File_triggered()
{
//    if (!currentTextEdit->toPlainText().isEmpty() && _savestate == false)
//    {
//        auto decision = QMessageBox::question(this,"Confirmation","Are you sure you want to discard the current file before saving?",QMessageBox::Yes,QMessageBox::No);
//        if (decision != QMessageBox::Yes)
//        {
//            return;
//        }

//    }
    auto tab = newTab();
//    FileInstance newlyCreated{};
//    fileInstances.insert(ui->tabWidget->currentIndex()+1,newlyCreated);

    ui->tabWidget->setCurrentIndex(tab);

}


void MainWindow::on_actionExit_triggered()
{
    if (!allSaved())
    {
        auto decision = QMessageBox::question(this,"Exit","Some files have not been saved, Are you sure you want to exit?",QMessageBox::Yes,QMessageBox::No);
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
        FileInstance newFile{currentFileName,currentFilePath};

        if (!fileInstances.contains(newFile))
        {
            fileInstances.append(newFile);
            currentTextEdit = ui->tabWidget->widget(index)->findChild<QTextEdit *>();
            qDebug() << "Pass";
            QTextStream stream{&file};
            while (!stream.atEnd())
            {
                currentTextEdit->append(stream.readLine());
                //ui->textEdit->append(stream.readLine());
            }
            qDebug() << "Pass";
            // APPEND THE FILENAME TO THE TITLE OF THE WINDOW
            modifyWindowTitle();
        }
        else
        {
            auto tabIndex = fileInstances.indexOf(newFile);
            ui->tabWidget->setCurrentIndex(tabIndex);
        }

        file.close();
    }
}


void MainWindow::on_textEdit_textChanged()
{
    //setSaveState(false, ui->tabWidget->currentIndex());
    fileInstances[currentTabIndex].setSaveState(false);
    if (windowTitle().at(0) != '*')
    {
        setWindowTitle(windowTitle().prepend("*"));
    }

    if (windowTitle() == "*Notepad--" && currentTextEdit->toPlainText().isEmpty())
    {
        //setWindowTitle(windowTitle().sliced(1));
        setWindowTitle(windowTitle().remove('*'));
        //setSaveState(true, ui->tabWidget->currentIndex());
        fileInstances[currentTabIndex].setSaveState(true);
    }
    checkSaveState();
}


void MainWindow::on_actionSave_File_triggered()
{
    if (fileInstances[currentTabIndex].getSaveState() == false)
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
    if (!fileInstances[currentTabIndex].getSaveState())
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
    currentTabIndex = index;
    //_savestate = fileInstances[index].getSaveState();
    currentFileName = fileInstances[index].getFileName();
    currentFilePath = fileInstances[index].getFilePath();
    qDebug() << "Filename : " << currentFileName;
    qDebug() << "Filepath : " << currentFilePath;
    qDebug() << "Index : " << index << "Length : " << fileInstances.length();
    modifyWindowTitle();
    checkSaveState();
    //currentTextEdit = qobject_cast<QTextEdit *>(ui->tabWidget->widget(index));
    currentTextEdit = ui->tabWidget->widget(index)->findChild<QTextEdit *>();
    if (currentTextEdit != nullptr)
    {
        qDebug() << "Not a nullptr";
        saveMechanism();
    }
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
