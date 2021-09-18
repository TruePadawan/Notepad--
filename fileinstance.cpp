#include "fileinstance.h"

FileInstance::FileInstance(QString name, QString path)
{
    FileName = name;
    FilePath = path;
}

QString FileInstance::getFileName()
{
    return FileName;
}

QString FileInstance::getFilePath()
{
    return FilePath;
}

void FileInstance::setName_and_Path(QString name, QString path)
{
    FileName = name;
    FilePath = path;
}
