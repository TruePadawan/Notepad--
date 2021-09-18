#include "fileinstance.h"

FileInstance::FileInstance(QString name, QString path, bool savestate)
{
    FileName = name;
    FilePath = path;
    _saveState = savestate;
}

QString FileInstance::getFileName()
{
    return FileName;
}

QString FileInstance::getFilePath()
{
    return FilePath;
}

bool FileInstance::getSaveState()
{
    return _saveState;
}

void FileInstance::setSaveState(bool state)
{
    _saveState = state;
}

void FileInstance::setName_and_Path(QString name, QString path)
{
    FileName = name;
    FilePath = path;
}

bool FileInstance::operator==(const FileInstance &rval) const
{
    if (this->FileName == rval.FileName && this->FilePath == rval.FilePath)
        return true;
    return false;
}
