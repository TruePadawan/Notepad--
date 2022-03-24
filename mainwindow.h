#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QComboBox>
#include <QLineEdit>
#include <QFileSystemModel>
#include "maincontroller.h"
#include "customtextedit.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    void setupStatusBarWidgets();
    void setupStatusBar();
    void setupSideBar();

    void newTab(CustomTextEdit *widget) const;

    void newFile();
    void openFile();
    void openFolder();

    void saveFile();
    void saveFileAs();

    void closeFile(int indexOfTab);
    void quitProgram();
    ~MainWindow();

private:
    Ui::MainWindow *ui;
    MainController *controller;
    QComboBox *syntaxComboBox;
    QLineEdit *pastebinLinkLineEdit;
    QStringList nameFilters;
    QFileSystemModel *filesystemModel;

    void toggleActionsMenuActionsAndComboBox();

    void sendCurrentTabToController(int indexOfCurrentTab);

    void connectSignalsToSlotsForMenuBar();
    void connectSignalsToSlotsForTabWidget();
    void connectSignalsToSlotsForController();
    void connectSignalsToSlotsForComboBox();

    void saved(bool saved, QString fileName);

    bool isAnyTabModified();
};
#endif // MAINWINDOW_H
