#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QMessageBox>
#include <QFileDialog>
#include <QHash>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
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

    ui->treeView->setModel(filesystemModel);

    ui->treeView->setRootIndex(filesystemModel->setRootPath(QDir::currentPath()));

    for (int i = 1; i <= 3; i++)
    {
        ui->treeView->hideColumn(i);
    }
}

void MainWindow::newTab(CustomTextEdit *widget) const
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
}

void MainWindow::newFile()
{
    CustomTextEdit *widget = controller->newWidget(ui->tabWidget);
    newTab(widget);

    // CHECK IF MENU ACTIONS SHOULD BE ENABLED
    toggleActionsMenuActionsAndComboBox();
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

void MainWindow::openFolder()
{
    QString folderPath = QFileDialog::getExistingDirectory(this,"Select Folder");

    if (!folderPath.isEmpty())
    {
        ui->treeView->setRootIndex(filesystemModel->setRootPath(folderPath));
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

        syntaxComboBox->setEnabled(false);
    }
    else if (numberOfTabs > 0 && !ui->actionSave->isEnabled())
    {
        ui->actionSave->setEnabled(true);
        ui->actionSave_As->setEnabled(true);
        ui->actionClose_File->setEnabled(true);

        syntaxComboBox->setEnabled(true);
    }
}

MainWindow::~MainWindow()
{
    delete ui;
}
