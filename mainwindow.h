#pragma once

#include <QMainWindow>
#include <QList>
#include <QTextEdit>
#include <QFileSystemModel>
#include <QItemSelection>
#include <QCloseEvent>
#include <QLineEdit>
#include <highlighter.h>
#include <customtextedit.h>
#include <qpastebin.h>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    void modifyWindowTitle();
    void checkSaveState();
    bool allSaved();
    void saveMechanism(CustomTextEdit *cust);
    void printFiles();
    void configureFolderView();
    void fileClicked();
    bool checkFileCollision(QString file);
    void closeEvent(QCloseEvent *ev) override;
    ~MainWindow();

private slots:
    void actionNew_File_triggered();

    void actionExit_triggered();

    void actionOpen_File_triggered();

    void textEdit_Modified();

    void actionSave_File_triggered();

    void actionSave_As_triggered();

    void actionClose_File_triggered();

    void on_actionAbout_Qt_triggered();

    void on_actionNew_File_2_triggered();

    void on_tabWidget_currentChanged();

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

    void on_actionPaste_to_Pastebin_triggered();

private:
    Ui::MainWindow *ui;
    QString currentFileName;
    QString currentFilePath;
    QFileSystemModel *model;
    QString lastFolderOpened;
    QLineEdit edit;
    QPasteBin *pasteBin;
};
