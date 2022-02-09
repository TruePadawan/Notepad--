#ifndef CUSTOMTEXTEDIT_H
#define CUSTOMTEXTEDIT_H

#include <QTextEdit>
#include <highlighter.h>
#include <QTextStream>
#include <QFileDialog>

class CustomTextEdit : public QTextEdit
{
    Q_OBJECT
public:
    CustomTextEdit(QWidget * parent = nullptr);
    CustomTextEdit(const QString &filepath, QWidget * parent = nullptr);
    void openFile(QString filepath);
    void save();
    void saveAs();
    bool saved();
    void setSaveState(bool state);
    QString getFileName() const;
    QString getFilePath() const;

protected:
    QString fileName;
    QString filePath;
    Highlighter highlighterObject;
    bool saveState{true};
};

#endif // CUSTOMTEXTEDIT_H
