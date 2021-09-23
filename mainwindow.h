#pragma once

#include <QMainWindow>
#include <QList>
#include <QTextEdit>
#include <QFileSystemModel>
#include <QItemSelection>
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
    int newTab(QString fileName="Untitled", QString filePath=QString());
    void SaveAs();
    void Save();
    void setSaveState(bool state, int index);
    void checkSaveState();
    bool allSaved();
    void saveMechanism();
    void printFiles();
    void configureFolderView();
    void fileClicked();
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

    void on_actionEnter_Distraction_Free_Mode_triggered();

    void on_actionAbout_triggered();

    void on_actionFont_triggered();

    void on_actionShow_Folder_View_triggered();

    void on_actionOpen_Folder_triggered();

private:
    Ui::MainWindow *ui;
    QList<FileInstance> fileInstances;
    QList<bool> saveStates;
    QString currentFileName;
    QString currentFilePath;
    QTextEdit *currentTextEdit;
    QFileSystemModel *model;
    int currentTabIndex{0};
    bool _savestate{true};
};
