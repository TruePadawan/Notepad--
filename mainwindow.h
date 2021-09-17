#pragma once

#include <QMainWindow>
#include <customtabwidget.h>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    void modifyWindowTitle();
    void SaveAs();
    void Save();
    void setSaveState(bool state);
    ~MainWindow();

private slots:
    void on_actionNew_File_triggered();

    void on_actionExit_triggered();

    void on_actionOpen_File_triggered();

    void on_textEdit_textChanged();

    void on_actionSave_File_triggered();

    void on_actionSave_As_triggered();

    void on_actionClose_File_triggered();

private:
    Ui::MainWindow *ui;
//    CustomTabWidget *tabWidget;
//    QVBoxLayout *layout;
    QString currentFileName;
    QString currentFilePath;
    bool _savestate{true};
};
