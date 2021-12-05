#include "includes.hpp"

QString WorkingDirManager::workingDir = "";

bool WorkingDirManager::FileExists(QString dataPath, QString fullPath)
{
    if (QFile::exists(fullPath))
        return true;
    else if (QFile::exists(workingDir + dataPath))
        return true;
    return false;
}

QString WorkingDirManager::GetPath(QString dataPath, QString fullPath)
{
    if (QFile::exists(fullPath))
        return fullPath;
    else if (QFile::exists(workingDir + dataPath))
        return workingDir + dataPath;
    return fullPath;
}
