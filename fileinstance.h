#pragma once
#include <QString>

class FileInstance
{
public:
    FileInstance(QString name="Untitled", QString path=QString(), bool savestate=true);
    QString getFileName();
    QString getFilePath();
    bool getSaveState();
    void setSaveState(bool state);
    void setName_and_Path(QString name, QString path);
    bool operator==(const FileInstance &rval) const;
private:
    QString FileName;
    QString FilePath;
    bool _saveState;
};
