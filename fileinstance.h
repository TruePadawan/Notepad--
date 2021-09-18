#pragma once
#include <QString>

class FileInstance
{
public:
    FileInstance(QString name, QString path);
    QString getFileName();
    QString getFilePath();
    void setName_and_Path(QString name, QString path);
private:
    QString FileName;
    QString FilePath;
};

