#pragma once

class WorkingDirManager
{
public:
    WorkingDirManager() {}

    static bool FileExists(QString dataPath, QString fullPath);
    static QString GetPath(QString dataPath, QString fullPath);

    static QString workingDir;
};


