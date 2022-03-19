#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QMessageBox>
#include <QFileDialog>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    setWindowIcon(QIcon(":/resources/notepad.ico"));

    langsComboBox = new QComboBox{ui->statusBar};
    pastebinLinkLineEdit = new QLineEdit{ui->statusBar};
    controller = new MainController(this);

    setupStatusBarWidgets();
    setupStatusBar();
    connectSignalsToSlotsForMenuBar();
}

void MainWindow::setupStatusBarWidgets()
{
    pastebinLinkLineEdit->setReadOnly(true);
    pastebinLinkLineEdit->setMaximumWidth(150);

    langsComboBox->addItem("Plain Text");
    langsComboBox->addItem("C++");
    langsComboBox->addItem("Qt/C++");
}

void MainWindow::setupStatusBar()
{
    ui->statusBar->addWidget(pastebinLinkLineEdit);
    ui->statusBar->addPermanentWidget(langsComboBox);
}

void MainWindow::newTab(CustomTextEdit *widget) const
{
    int tabIndex = ui->tabWidget->addTab(widget, widget->getFileName());

    if (ui->tabWidget->currentIndex() != tabIndex)
    {
        ui->tabWidget->setCurrentIndex(tabIndex);
    }

    // WHEN TEXT IN WIDGET'S TEXT-EDIT CHANGES, ADD AN INDICATOR TO THE TAB-BAR TITLE
    connect(widget,&CustomTextEdit::textChanged,this,[&] () {
        int tabIndex = ui->tabWidget->currentIndex();
        QString tabTitle = ui->tabWidget->tabText(tabIndex);

        if (tabTitle[tabTitle.size()-1] != '*')
        {
            tabTitle.append("*");
            ui->tabWidget->setTabText(tabIndex,tabTitle);
        }
    });
}

void MainWindow::openFile()
{
    QString filePath = QFileDialog::getOpenFileName(this,"Open",QDir::homePath(),"Text files (*.txt);;C++ files (*.h *.cpp)");
    if (!filePath.isNull())
    {
        CustomTextEdit *tabWidget = controller->newWidget(filePath,ui->tabWidget);

        newTab(tabWidget);
    }
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::connectSignalsToSlotsForMenuBar()
{
    // HELP MENU
    connect(ui->actionAbout_Qt,&QAction::triggered,this,[&] () {
       QMessageBox::aboutQt(this);
    });

    connect(ui->actionAbout, &QAction::triggered, this, [&] () {
        QMessageBox::about(this,"About Notepad--","<p>A simple text editor</p>");
    });

    // FILE MENU
    connect(ui->actionOpen_File,&QAction::triggered,this,&MainWindow::openFile);

    connect(ui->actionExit, &QAction::triggered, this, [&] () {
       if (!this->isAnyTabModified())
       {
           qApp->quit();
       }
    });
}

bool MainWindow::isAnyTabModified()
{
    return false;
}

