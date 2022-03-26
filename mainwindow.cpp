#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QMessageBox>
#include <QFileDialog>
#include <QHash>
#include <QModelIndex>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    _init();
}

void MainWindow::_init()
{
    setWindowIcon(QIcon(":/resources/notepad.ico"));

    syntaxComboBox = new QComboBox{ui->statusBar};
    pastebinLinkLineEdit = new QLineEdit{ui->statusBar};
    controller = new MainController(this);

    nameFilters << "*.h" << "*.cpp" << "*.txt";

    setupStatusBarWidgets();
    setupStatusBar();
    setupSideBar();

    connectSignalsToSlotsForMenuBar();
    connectSignalsToSlotsForTabWidget();
    connectSignalsToSlotsForController();
    connectSignalsToSlotsForComboBox();
    connectSignalsToSlotsForSidebar();
}

int MainWindow::isFileAlreadyOpen(QString filePath)
{
    const int NUMBER_OF_TABS = ui->tabWidget->count();

    if (NUMBER_OF_TABS == 0) return -1;

    for (int i = 0; i < NUMBER_OF_TABS; ++i)
    {
        CustomTextEdit *widget = qobject_cast<CustomTextEdit *>(ui->tabWidget->widget(i));
        QString pathOfFileInWidget = widget->getFilePath();

        if (filePath == pathOfFileInWidget)
        {
            return i;
        }
    }
    return -1;
}

void MainWindow::setupStatusBarWidgets()
{
    pastebinLinkLineEdit->setReadOnly(true);
    pastebinLinkLineEdit->setMaximumWidth(150);

    syntaxComboBox->addItem("Plain Text");
    syntaxComboBox->addItem("C++");

    syntaxComboBox->setEnabled(false);
}

void MainWindow::setupStatusBar()
{
    ui->statusBar->addWidget(pastebinLinkLineEdit);
    ui->statusBar->addPermanentWidget(syntaxComboBox);
}

void MainWindow::setupSideBar()
{
    filesystemModel = new QFileSystemModel(this);
    filesystemModel->setFilter(QDir::AllDirs | QDir::NoDotAndDotDot | QDir::Files);

    filesystemModel->setNameFilters(nameFilters);
    filesystemModel->setNameFilterDisables(false);

    ui->sideBar->setModel(filesystemModel);

    ui->sideBar->setRootIndex(filesystemModel->setRootPath(QDir::currentPath()));

    for (int i = 1; i <= 3; i++)
    {
        ui->sideBar->hideColumn(i);
    }


}

void MainWindow::newTab(CustomTextEdit *widget)
{
    int tabIndex = ui->tabWidget->addTab(widget, widget->getFileName());

    // SET THE TAB'S CURRENT INDEX TO THE NEWLY CREATED TAB EXCEPT WHEN ITS THE FIRST TAB IN THE PROGRAM
    if (ui->tabWidget->currentIndex() != tabIndex)
    {
        ui->tabWidget->setCurrentIndex(tabIndex);
    }

    // WHEN TEXT IN WIDGET'S TEXT-EDIT CHANGES, ADD AN INDICATOR TO THE TAB-BAR TITLE
    connect(widget,&CustomTextEdit::textChanged,this,[&] () {
        int currentTabIndex = ui->tabWidget->currentIndex();
        QString tabTitle = ui->tabWidget->tabText(currentTabIndex);

        if (tabTitle[tabTitle.size()-1] != '*')
        {
            tabTitle.append("*");
            ui->tabWidget->setTabText(currentTabIndex,tabTitle);
        }
    });

    // CHECK IF MENU ACTIONS SHOULD BE ENABLED
    toggleActionsMenuActionsAndComboBox();
}

void MainWindow::newFile()
{
    CustomTextEdit *widget = controller->newWidget(ui->tabWidget);
    newTab(widget);
}

void MainWindow::openFile()
{
    QString filePath = QFileDialog::getOpenFileName(this,"Open",QDir::homePath(),"Text files (*.txt);;C++ files (*.h *.cpp)");
    if (!filePath.isNull())
    {
        CustomTextEdit *widget = controller->newWidget(filePath,ui->tabWidget);

        newTab(widget);
    }
}

void MainWindow::openFileFromSidebar()
{
    const QModelIndex indexOfSelectedItem = ui->sideBar->selectionModel()->currentIndex();

   // CHECK IF THE CURRENTLY SELECTED ITEM IS A DIRECTORY (IT NEEDS TO BE A FILE)
    if (indexOfSelectedItem.isValid() && !filesystemModel->isDir(indexOfSelectedItem))
    {
        QString pathOfSelectedFile = filesystemModel->filePath(indexOfSelectedItem);

        const int indexOfPossiblyOpenedFile = isFileAlreadyOpen(pathOfSelectedFile);

        if (indexOfPossiblyOpenedFile == -1)
        {
            CustomTextEdit *widget = controller->newWidget(pathOfSelectedFile,ui->tabWidget);
            newTab(widget);

            return;
        }
        // SET THE TAB TO THAT INDEX SINCE THE FILE IS ALREADY OPENED
        ui->tabWidget->setCurrentIndex(indexOfPossiblyOpenedFile);
    }
}

void MainWindow::openFolder()
{
    QString folderPath = QFileDialog::getExistingDirectory(this,"Select Folder");

    if (!folderPath.isEmpty())
    {
        ui->sideBar->setRootIndex(filesystemModel->setRootPath(folderPath));
    }
}

void MainWindow::saveFile()
{
    controller->save();
}

void MainWindow::saveFileAs()
{
    controller->saveAs();
}

void MainWindow::closeFile(int indexOfTab)
{
    ui->tabWidget->removeTab(indexOfTab);

    // CHECK IF MENU ACTIONS SHOULD BE ENABLED
    toggleActionsMenuActionsAndComboBox();
}

void MainWindow::quitProgram()
{
    if (!this->isAnyTabModified())
    {
        qApp->quit();
    }
}

void MainWindow::sendCurrentTabToController(int indexOfCurrentTab)
{
    if (indexOfCurrentTab != -1)
    {
        CustomTextEdit *actualWidget = qobject_cast<CustomTextEdit *>(ui->tabWidget->widget(indexOfCurrentTab));

        if (actualWidget != nullptr)
        {
            controller->setCurrentWidget(actualWidget);
        }else
        {
            qFatal("Casting Tab Widget to CustomTextEdit Failed!!!!");
        }
        return;
    }
    controller->setWidgetToNull(); // IF THERE IS NO WIDGET IN TABWIDGET
}

void MainWindow::connectSignalsToSlotsForMenuBar()
{
    // HELP MENU
    connect(ui->actionAbout_Qt,&QAction::triggered,this,[&] () {
       QMessageBox::aboutQt(this);
    });

    connect(ui->actionAbout, &QAction::triggered, this,[&] () {
        QMessageBox::about(this,"About Notepad--","<p>A simple text editor</p>");
    });

    // FILE MENU
    connect(ui->actionNew_File,&QAction::triggered,this,&MainWindow::newFile);
    connect(ui->actionOpen_File,&QAction::triggered,this,&MainWindow::openFile);

    connect(ui->actionOpen_Folder,&QAction::triggered,this,&MainWindow::openFolder);

    connect(ui->actionSave,&QAction::triggered,this,&MainWindow::saveFile);
    connect(ui->actionSave_As,&QAction::triggered,this,&MainWindow::saveFileAs);

    connect(ui->actionClose_File,&QAction::triggered,this, [&] () {
        int indexOfCurrentTab = ui->tabWidget->currentIndex();
        closeFile(indexOfCurrentTab);
    });

    connect(ui->actionExit, &QAction::triggered,this, &MainWindow::quitProgram);

    // EDIT MENU

    // VIEW MENU
    connect(ui->actionToggle_SideBar,&QAction::triggered,this, &MainWindow::toggleSideBar);
    connect(ui->actionToggle_Zen_Mode,&QAction::triggered,this, &MainWindow::toggleZenMode);
}

void MainWindow::connectSignalsToSlotsForTabWidget()
{
    connect(ui->tabWidget,&QTabWidget::currentChanged,this,&MainWindow::sendCurrentTabToController);

    connect(ui->tabWidget, &QTabWidget::tabCloseRequested,this,&MainWindow::closeFile);
}

void MainWindow::connectSignalsToSlotsForController()
{
    // CONNECTION FOR UPDATING THE WINDOW TITLE AND COMBO-BOX DISPLAYING PREFFERED SYNTAX
    connect(controller,&MainController::widgetChanged,this,[&] (QHash <QString,QString> titleAndPrefferedSyntax) {
        setWindowTitle(titleAndPrefferedSyntax.value("fileName"));

        if (titleAndPrefferedSyntax.contains("syntax"))
        {
            syntaxComboBox->setCurrentText(titleAndPrefferedSyntax.value("syntax"));
        }
    });

    // CONNECTION FOR UPDATING THE TAB TITLE AND WINDOW TITLE WHEN FILE IS SAVED
    connect(controller, &MainController::widgetTextSaved,this,&MainWindow::saved);

    connect(controller, &MainController::widgetTextSavedAs,this,&MainWindow::saved);
}

void MainWindow::connectSignalsToSlotsForComboBox()
{
    connect(syntaxComboBox,&QComboBox::currentTextChanged,this,[&] (QString currentText) {
        controller->updatePreferredSyntaxForTab(currentText);
    });
}

void MainWindow::connectSignalsToSlotsForSidebar()
{
    connect(ui->sideBar->selectionModel(),&QItemSelectionModel::currentChanged,this,&MainWindow::openFileFromSidebar);
}

void MainWindow::saved(bool saved, QString fileName)
{
    if (saved)
    {
        int currentTabIndex = ui->tabWidget->currentIndex();
        QString newWindowTitle{QString("Notepad-- (%1)").arg(fileName)};

        ui->tabWidget->setTabText(currentTabIndex,fileName);
        setWindowTitle(newWindowTitle);
    }
}

bool MainWindow::isAnyTabModified()
{
    return false;
}

void MainWindow::toggleActionsMenuActionsAndComboBox()
{
    int numberOfTabs = ui->tabWidget->count();

    if (numberOfTabs <= 0 && ui->actionSave->isEnabled())
    {
        ui->actionSave->setEnabled(false);
        ui->actionSave_As->setEnabled(false);
        ui->actionClose_File->setEnabled(false);

        ui->actionCopy->setEnabled(false);
        ui->actionCut->setEnabled(false);
        ui->actionPaste->setEnabled(false);
        ui->actionSelect_All->setEnabled(false);

        syntaxComboBox->setEnabled(false);
    }
    else if (numberOfTabs > 0 && !ui->actionSave->isEnabled())
    {
        ui->actionSave->setEnabled(true);
        ui->actionSave_As->setEnabled(true);
        ui->actionClose_File->setEnabled(true);

        ui->actionCopy->setEnabled(true);
        ui->actionCut->setEnabled(true);
        ui->actionPaste->setEnabled(true);
        ui->actionSelect_All->setEnabled(true);

        syntaxComboBox->setEnabled(true);
    }
}

void MainWindow::toggleSideBar()
{
    bool isSideBarVisible = ui->sideBar->isVisible();
    ui->sideBar->setVisible(!isSideBarVisible);
}

void MainWindow::toggleZenMode()
{
    if (this->windowState() == Qt::WindowFullScreen)
    {
        this->showMaximized();
    }else
    {
        this->showFullScreen();
    }
}

MainWindow::~MainWindow()
{
    delete ui;
}
