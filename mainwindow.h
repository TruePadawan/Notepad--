#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QComboBox>
#include <QLineEdit>
#include <QFileSystemModel>
#include "qpastebindialog.h"
#include "maincontroller.h"
#include "customtextedit.h"
#include "qpastebin.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    CustomTextEdit *convertWidgetToCustomTextEdit(QWidget *widget);

    void setupStatusBarWidgets();
    void setupStatusBar();
    void setupSideBar();

    void newTab(CustomTextEdit *widget);

    void newFile();
    void openFile();
    void openFileFromSidebar();
    void openFolder();

    void saveFile();
    void saveFileAs();

    void closeFile(int indexOfTab);

    void setFontForWidget();

    void quitProgram();
    ~MainWindow();

private:
    Ui::MainWindow *ui;
    MainController *controller;
    QComboBox *syntaxComboBox;
    QLineEdit *pastebinLinkLineEdit;
    QStringList nameFilters;
    QFileSystemModel *filesystemModel;
    QPastebinDialog pastebinDialog;
    QPasteBin pastebin;

    void _init();

    int isFileAlreadyOpen(QString filePath);

    void toggleActionsMenuActionsAndComboBox();
    void toggleSideBar();
    void toggleZenMode();

    void setFontToAllWidgets(QFont &font);

    void sendCurrentTabToController(int indexOfCurrentTab);

    void connectSignalsToSlotsForMenuBar();
    void connectSignalsToSlotsForTabWidget();
    void connectSignalsToSlotsForController();
    void connectSignalsToSlotsForComboBox();
    void connectSignalsToSlotsForSidebar();
    void connectSignalsToSlotsForPastebin();

    void saved(bool saved, QString fileName);

    bool isAnyTabModified();
};
#endif // MAINWINDOW_H
