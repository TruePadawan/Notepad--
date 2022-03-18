#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QMessageBox>
#include <QFileDialog>
#include <QFontDialog>
#include <QDebug>
#include <QTextStream>
#include <QFile>
#include <QFont>
#include <QFileInfo>
#include <QVBoxLayout>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    actionNew_File_triggered();
    ui->statusBar->addWidget(&edit);
    ui->statusBar->addPermanentWidget(&langBox);

    langBox.setStyleSheet("background-color: black; color: white;");
    langBox.addItem("C++");
    langBox.addItem("Qt/C++");
    langBox.addItem("Plain Text");
    pasteBin = new QPasteBin(ui->menuBar);
    configureFolderView();
}

void MainWindow::modifyWindowTitle()
{
    if (!currentFilePath.isEmpty())
    {
        QString title{"[*]Notepad-- ["};
        title.append(currentFilePath).append("]");
        setWindowTitle(title);
        ui->tabWidget->setTabText(ui->tabWidget->currentIndex(),currentFileName);
    }else
    {
        setWindowTitle("[*]Notepad--");
    }
}


void MainWindow::checkSaveState()
{
    auto tab = qobject_cast<CustomTextEdit *>(ui->tabWidget->currentWidget());
    if (!tab->saved())
    {
        setWindowModified(true);
        ui->actionSave_File->setEnabled(true);
        ui->actionSave_As->setEnabled(true);
        ui->actionSave->setEnabled(true);
        ui->actionSave_As_2->setEnabled(true);
    }else
    {
        setWindowModified(false);
        ui->actionSave_File->setEnabled(false);
        ui->actionSave_As->setEnabled(false);
        ui->actionSave->setEnabled(false);
        ui->actionSave_As_2->setEnabled(false);
    }
}

bool MainWindow::allSaved()
{
    for (int i = 0; i < ui->tabWidget->count(); i++)
    {
        auto tab = qobject_cast<CustomTextEdit *>(ui->tabWidget->widget(i));
        if (!tab->saved())
        {
            return false;
        }
    }
    return true;
}

void MainWindow::saveMechanism(CustomTextEdit *cust)
{
    connect(cust, &CustomTextEdit::undoAvailable, [=] (bool val){
        ui->actionUndo->setEnabled(val);
    });

    connect(cust, &CustomTextEdit::redoAvailable, [=] (bool val){
        ui->actionRedo->setEnabled(val);
    });

    connect(cust, &CustomTextEdit::copyAvailable, [=] (bool val){
        ui->actionCopy->setEnabled(val);
    });

    connect(cust, &CustomTextEdit::textChanged, this, &MainWindow::textEdit_Modified);
}

void MainWindow::printFiles()
{
    for (int i = 0; i < ui->tabWidget->count(); i++)
    {
        auto tab = qobject_cast<CustomTextEdit *>(ui->tabWidget->widget(i));
        qDebug() << "Filename: " << tab->getFileName();
        qDebug() << "Filepath: " << tab->getFilePath();
    }
}

void MainWindow::configureFolderView()
{
    model = new QFileSystemModel(this);
    model->setFilter(QDir::NoDotAndDotDot | QDir::Dirs | QDir::Files);
    model->setRootPath(QDir::currentPath());
    ui->treeView->setModel(model);
    ui->treeView->setRootIndex(model->setRootPath(QDir::currentPath()));
    ui->treeView->header()->hide();
    for (int i = 1; i <= 3; i++)
    {
        ui->treeView->setColumnHidden(i,true);
    }
    QItemSelectionModel *selectionModel = ui->treeView->selectionModel();
    connect(selectionModel,&QItemSelectionModel::selectionChanged,this,&MainWindow::fileClicked);
}

void MainWindow::fileClicked()
{
    QModelIndex currentIndex = ui->treeView->selectionModel()->currentIndex();
    if (!model->isDir(currentIndex))
    {
        if ( model->fileInfo(currentIndex).suffix() == "txt" || model->fileInfo(currentIndex).suffix() == "cpp" || model->fileInfo(currentIndex).suffix() == "h")
        {
            if (!checkFileCollision(model->filePath(currentIndex)))
            {
                auto * tab = new CustomTextEdit(model->filePath(currentIndex),this);
                int index = ui->tabWidget->addTab(tab,tab->getFileName());
                ui->tabWidget->setCurrentIndex(index);
            }
       }
    }
}

bool MainWindow::checkFileCollision(QString file)
{
    for (int i = 0; i < ui->tabWidget->count(); i++)
    {
        auto tab = qobject_cast<CustomTextEdit *>(ui->tabWidget->widget(i));
        if (tab->getFilePath() == file)
        {
            return true;
        }
    }
    return false;
}

void MainWindow::closeEvent(QCloseEvent *ev)
{
    if (!allSaved())
    {
        auto choice = QMessageBox::question(this,"Exit","Are you sure you want to discard unsaved files?",QMessageBox::Yes,QMessageBox::No);
        if (choice != QMessageBox::Yes)
        {
            ev->ignore();
        }
    }
}

MainWindow::~MainWindow()
{
    delete ui;
    delete model;
    delete pasteBin;
    qDebug() << "Ending Program";
}


void MainWindow::actionNew_File_triggered()
{
    auto * tab = new CustomTextEdit(this);
    ui->tabWidget->addTab(tab,"Untitled");
    saveMechanism(tab);

}


void MainWindow::actionExit_triggered()
{
    if (!allSaved())
    {
        auto choice = QMessageBox::question(this,"Exit","Are you sure you want to discard unsaved files?",QMessageBox::Yes,QMessageBox::No);
        if (choice == QMessageBox::Yes)
        {
            qApp->quit();
        }
    }else
        qApp->quit();

}


void MainWindow::actionOpen_File_triggered()
{
    QString file = QFileDialog::getOpenFileName(this);

    if (file.isNull())
        return;

    if (checkFileCollision(file))
        return;

    QFileInfo info{file};
    auto * tab = new CustomTextEdit(file,this);
    saveMechanism(tab);
    auto index = ui->tabWidget->addTab(tab, info.fileName());
    ui->tabWidget->setCurrentIndex(index);
}


void MainWindow::textEdit_Modified()
{
    auto tab = qobject_cast<CustomTextEdit *>(ui->tabWidget->currentWidget());
    if (tab != nullptr)
    {
        tab->setSaveState(false);
        //setWindowModified(true);
    }
    qDebug() << "Text Edit Changing!";
    checkSaveState();
}


void MainWindow::actionSave_File_triggered()
{
    auto tab = qobject_cast<CustomTextEdit *>(ui->tabWidget->currentWidget());
    if (tab == nullptr)
        return;
    tab->save();
    setWindowModified(false);

    currentFileName = tab->getFileName();
    currentFilePath = tab->getFilePath();

    modifyWindowTitle();
    ui->tabWidget->setTabText(ui->tabWidget->currentIndex(),tab->getFileName());
}


void MainWindow::actionSave_As_triggered()
{
    auto tab = qobject_cast<CustomTextEdit *>(ui->tabWidget->currentWidget());
    if (tab == nullptr)
        return;
    tab->saveAs();
    setWindowModified(false);

    currentFileName = tab->getFileName();
    currentFilePath = tab->getFilePath();

    modifyWindowTitle();
    ui->tabWidget->setTabText(ui->tabWidget->currentIndex(),tab->getFileName());
}


void MainWindow::actionClose_File_triggered()
{
    auto tab = qobject_cast<CustomTextEdit *>(ui->tabWidget->currentWidget());
    if (tab == nullptr)
        return;

    if (!tab->saved() && ui->tabWidget->count() != 1)
    {
        auto decision = QMessageBox::question(this,"Confirmation","Are you sure you want to close the current file before saving?",QMessageBox::Yes,QMessageBox::No);
        if (decision == QMessageBox::No)
            return;
    }

    if (ui->tabWidget->count() == 1)
    {
        qApp->quit();
    }else
    {
        ui->tabWidget->removeTab(ui->tabWidget->currentIndex());
        tab->deleteLater();
    }
}

void MainWindow::on_actionAbout_Qt_triggered()
{
    QMessageBox::aboutQt(this);
}


void MainWindow::on_actionNew_File_2_triggered()
{
    actionNew_File_triggered();
}


void MainWindow::on_tabWidget_currentChanged()
{
    //currentTextEdit = qobject_cast<CustomTextEdit *>(ui->tabWidget->currentWidget());
    auto tab = qobject_cast<CustomTextEdit *>(ui->tabWidget->currentWidget());
    currentFileName = tab->getFileName();
    currentFilePath = tab->getFilePath();

    modifyWindowTitle();
    checkSaveState();
}


void MainWindow::on_actionOpen_File_2_triggered()
{
    actionOpen_File_triggered();
}


void MainWindow::on_actionSave_triggered()
{
    actionSave_File_triggered();
}


void MainWindow::on_actionSave_As_2_triggered()
{
    actionSave_As_triggered();
}


void MainWindow::on_actionClose_File_2_triggered()
{
    actionClose_File_triggered();
}


void MainWindow::on_actionExit_2_triggered()
{
    actionExit_triggered();
}


void MainWindow::on_actionUndo_triggered()
{
    auto tab = qobject_cast<CustomTextEdit *>(ui->tabWidget->currentWidget());
    tab->undo();
}


void MainWindow::on_actionRedo_triggered()
{
    auto tab = qobject_cast<CustomTextEdit *>(ui->tabWidget->currentWidget());
    tab->redo();
}


void MainWindow::on_actionCut_triggered()
{
    auto tab = qobject_cast<CustomTextEdit *>(ui->tabWidget->currentWidget());
    tab->cut();
}


void MainWindow::on_actionCopy_triggered()
{
    auto tab = qobject_cast<CustomTextEdit *>(ui->tabWidget->currentWidget());
    tab->copy();
}


void MainWindow::on_actionPaste_triggered()
{
    auto tab = qobject_cast<CustomTextEdit *>(ui->tabWidget->currentWidget());
    tab->paste();
}

void MainWindow::on_actionEnter_Distraction_Free_Mode_triggered()
{
    if (this->windowState() == Qt::WindowFullScreen)
    {
        this->showMaximized();
        ui->actionEnter_Distraction_Free_Mode->setText("Enter Distraction Free Mode");
    }else
    {
        this->showFullScreen();
        qDebug() << this->windowState();
        ui->actionEnter_Distraction_Free_Mode->setText("Exit Distraction Free Mode");
    }
}


void MainWindow::on_actionAbout_triggered()
{
    QMessageBox::about(this,"About Notepad--","<strong>Notepad-- v0.3 </strong><p>A simple text-editor built with Qt/C++</p>");
}


void MainWindow::on_actionFont_triggered()
{
    auto tab = qobject_cast<CustomTextEdit *>(ui->tabWidget->currentWidget());
    tab->setFont(QFontDialog::getFont(0, tab->font()));
}


void MainWindow::on_actionShow_Folder_View_triggered()
{
    if (ui->treeView->isVisible())
    {
        ui->treeView->setVisible(false);
        ui->actionShow_Folder_View->setText("Show Folder View");
    }else
    {
        ui->treeView->setVisible(true);
        ui->actionShow_Folder_View->setText("Hide Folder View");
    }
}


void MainWindow::on_actionOpen_Folder_triggered()
{
    if (!lastFolderOpened.isEmpty())
    {
        auto dir = QFileDialog::getExistingDirectory(this,"Open Folder",lastFolderOpened);
        if (!dir.isNull())
        {
            model->setRootPath(dir);
            ui->treeView->setRootIndex(model->setRootPath(dir));
        }
    }else
    {
        auto dir = QFileDialog::getExistingDirectory(this,"Open Folder");
        if (!dir.isNull())
        {
            model->setRootPath(dir);
            ui->treeView->setRootIndex(model->setRootPath(dir));
        }
    }

    if (!ui->treeView->isVisible())
    {
        ui->treeView->setVisible(true);
        ui->actionShow_Folder_View->setText("Hide Folder View");
    }
}

void MainWindow::on_actionPaste_to_Pastebin_triggered()
{
    auto tab = qobject_cast<CustomTextEdit *>(ui->tabWidget->currentWidget());
    if (!tab->toPlainText().isEmpty())
    {
        if (pasteBin->setUpPasteData(tab->toPlainText()))
        {
            pasteBin->paste();
            connect(pasteBin,&QPasteBin::complete,[&]{
                edit.setText(pasteBin->getLink());
            });
        }else
        {
            edit.setText("Couldn't set up paste data");
        }
    }
}

