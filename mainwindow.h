#pragma once

#include <QMainWindow>
#include <QList>
#include <QTextEdit>
#include <fileinstance.h>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    void modifyWindowTitle();
    int newTab();
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

    void on_actionAbout_Qt_triggered();

    void on_actionNew_File_2_triggered();

    void on_tabWidget_currentChanged(int index);

    void on_actionOpen_File_2_triggered();

    void on_actionSave_triggered();

    void on_actionSave_As_2_triggered();

    void on_actionClose_File_2_triggered();

    void on_actionExit_2_triggered();

    void on_actionUndo_triggered();

    void on_actionRedo_triggered();

    void on_actionCut_triggered();

    void on_actionCopy_triggered();

    void on_actionPaste_triggered();

private:
    Ui::MainWindow *ui;
    QList<FileInstance> fileInstances;
    QString currentFileName;
    QString currentFilePath;
    QTextEdit *currentTextEdit;
    bool _savestate{true};
};
