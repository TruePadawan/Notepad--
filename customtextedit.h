#ifndef CUSTOMTEXTEDIT_H
#define CUSTOMTEXTEDIT_H

#include <QObject>
#include <QTextStream>
#include <QTextEdit>
#include <QFile>
#include <highlighter.h>

class CustomTextEdit : public QTextEdit
{
    Q_OBJECT
public:
    explicit CustomTextEdit(QWidget *parent = nullptr);
    CustomTextEdit(QFile &file, QWidget *parent);

    void loadTextFromFile(QFile &file);
    void save();
    void saveAs();
    void configureTextEdit();

    void setFileType(const QString &fileExtension);
    void setIsTextEditDataSaved(bool state);
    void setFileNameAndPath(const QString &name, const QString &path = QString());

    QString getFileName();
    QString getFilePath();
    QString getFileType();
    bool getIsTextEditDataSaved();
    ~CustomTextEdit();

private:
    QString fileName;
    QString filePath;
    QString fileType;
    bool isTextEditDataSaved{true};
    Highlighter highlighterObject;

signals:
    void textEditDataSaved(bool saved);
    void fileTypeChanged(QString fileType);
};

#endif // CUSTOMTEXTEDIT_H
