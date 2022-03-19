#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QComboBox>
#include <QLineEdit>
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

    void newTab(CustomTextEdit *widget) const;

    void openFile();
    ~MainWindow();

private:
    Ui::MainWindow *ui;
    MainController *controller;
    QComboBox *langsComboBox;
    QLineEdit *pastebinLinkLineEdit;

    void connectSignalsToSlotsForMenuBar();
    bool isAnyTabModified();
};
#endif // MAINWINDOW_H
