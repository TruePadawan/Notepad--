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
    bool save();
    bool saveAs();
    void configureTextEdit();

    void setFileNameAndPath(const QString &name, const QString &path = QString());
    void setFileType(const QString &fileExtension);
    void setPreferredSyntax(const QString &syntax);
    void setIsTextEditDataSaved(bool state);

    QString getFileName();
    QString getFilePath();
    QString getFileType();
    QString getPreferredSyntax();
    bool getIsTextEditDataSaved();
    ~CustomTextEdit();

private:
    QString fileName;
    QString filePath;
    QString fileType;
    QString preferredSyntax;
    bool isTextEditDataSaved{true};
    Highlighter highlighterObject;
};

#endif // CUSTOMTEXTEDIT_H
