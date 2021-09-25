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
    ui->tabWidget->setTabText(0,"Untitled");
    ui->toolBar->setVisible(false);
    ui->treeView->setVisible(false);

    currentTextEdit = ui->textEdit;
    Highlighter *syntax = new Highlighter(currentTextEdit->document());
    syntaxHighlight.append(syntax);
    //highlighter = new Highlighter(currentTextEdit->document());

    FileInstance defaultFile{};
    fileInstances.append(defaultFile);

    tmp = currentTextEdit->font();
    saveMechanism();
    configureFolderView();
    //QApplication::setQuitOnLastWindowClosed(true);
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

int MainWindow::newTab(QString fileName, QString filePath)
{
    FileInstance newFile{fileName,filePath};
    if (fileName != "Untitled" && fileInstances.contains(newFile))
    {
        return -1;
    }

    fileInstances.append(newFile);

    QWidget *page = new QWidget;
    QTextEdit *textarea = new QTextEdit(page);

    QVBoxLayout *layout = new QVBoxLayout;
    layout->addWidget(textarea);
    page->setLayout(layout);

    Highlighter *syntax = new Highlighter(textarea->document());
    syntaxHighlight.append(syntax);

    return ui->tabWidget->addTab(page,fileName);
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

void MainWindow::printFiles()
{
    foreach (FileInstance tmp, fileInstances) {
        qDebug() << "File Name: " << tmp.getFileName();
        qDebug() << "File Path: " << tmp.getFilePath();
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
            int index = newTab(model->fileName(currentIndex),model->filePath(currentIndex));
            if (index != -1)
            {

                currentFileName = model->fileName(currentIndex);
                currentFilePath = model->filePath(currentIndex);

                QFile file{model->filePath(currentIndex)};
                if (!file.open(QIODevice::ReadOnly)){
                    qWarning() << file.errorString();
                    return;
                }
                currentTextEdit = ui->tabWidget->widget(index)->findChild<QTextEdit *>();
                QTextStream stream{&file};
                while (!stream.atEnd())
                {
                    currentTextEdit->append(stream.readLine());
                }
                file.close();
                ui->tabWidget->setCurrentIndex(index);
            }
       }
    }
}

MainWindow::~MainWindow()
{
    delete ui;
    delete currentTextEdit;
    delete model;

    for (auto syn: syntaxHighlight)
    {
        qDebug() << "Delete";
        delete syn;
    }
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
            qApp->quit();
        }
    }else{
        qApp->quit();
    }
}


void MainWindow::on_actionOpen_File_triggered()
{
    QString filepath = QFileDialog::getOpenFileName(this,"Open File",QString(),tr("Text File (*.txt *.ini);;C++ Files (*.cpp *.h)"));
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

        int index = newTab(currentFileName,currentFilePath);

        FileInstance newFile{currentFileName,currentFilePath};

        if (index != -1)
        {
            //fileInstances.append(newFile);
            currentTextEdit = ui->tabWidget->widget(index)->findChild<QTextEdit *>();

            QTextStream stream{&file};
            while (!stream.atEnd())
            {
                currentTextEdit->append(stream.readLine());
                //ui->textEdit->append(stream.readLine());
            }
            ui->tabWidget->setCurrentIndex(index);
            qDebug() << "Pass";
            // APPEND THE FILENAME TO THE TITLE OF THE WINDOW
//            modifyWindowTitle();
        }
        else
        {
            auto tabIndex = fileInstances.indexOf(newFile);
            ui->tabWidget->setCurrentIndex(tabIndex);
        }

        printFiles();
        file.close();
    }
}


void MainWindow::on_textEdit_textChanged()
{
    qDebug() << "Text Edit Changing!";
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
        if (decision == QMessageBox::No)
        {
            return;

            //setWindowTitle("Notepad--");
            //currentTextEdit->clear();
            //ui->textEdit->clear();
        }
    }

    if (fileInstances.size() == 1)
    {
        qApp->quit();
        //exit(0);
    }
    fileInstances.removeAt(currentTabIndex);
    syntaxHighlight.removeAt(currentTabIndex);
    ui->tabWidget->removeTab(currentTabIndex);
//    currentFileName = "";
//    currentFilePath = "";
//    ui->tabWidget->setTabText(ui->tabWidget->currentIndex(),"Untitled");
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
    modifyWindowTitle();
    checkSaveState();
    //currentTextEdit = qobject_cast<QTextEdit *>(ui->tabWidget->widget(index));
    currentTextEdit = ui->tabWidget->widget(index)->findChild<QTextEdit *>();
    currentTextEdit->setFont(tmp);
    //highlighter->setDocument(currentTextEdit->document());
    if (currentTextEdit != nullptr)
    {
        qDebug() << "Not a nullptr";
        saveMechanism();
    }
    printFiles();
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
    QMessageBox::about(this,"About Notepad--","<strong>Notepad-- v0.2 </strong><p>A simple text-editor built with Qt/C++</p>");
}


void MainWindow::on_actionFont_triggered()
{
    currentTextEdit->setFont(QFontDialog::getFont(0, currentTextEdit->font()));
}


void MainWindow::on_actionShow_Folder_View_triggered()
{
    if (ui->treeView->isVisible())
    {
        ui->treeView->setVisible(false);
        ui->actionShow_Folder_View->setText("Hide Folder View");
    }else
    {
        ui->treeView->setVisible(true);
        ui->actionShow_Folder_View->setText("Show Folder View");
    }
}


void MainWindow::on_actionOpen_Folder_triggered()
{
    auto dir = QFileDialog::getExistingDirectory(this,"Open Folder");
    model->setRootPath(dir);
    ui->treeView->setRootIndex(model->setRootPath(dir));
}

