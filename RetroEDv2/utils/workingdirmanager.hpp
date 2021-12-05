#ifndef WORKINGDIRMANAGER_H
#define WORKINGDIRMANAGER_H

class WorkingDirManager
{
public:
    WorkingDirManager() {}

    static bool FileExists(QString dataPath, QString fullPath);
    static QString GetPath(QString dataPath, QString fullPath);

    static QString workingDir;
};

#endif // WORKINGDIRMANAGER_H
