#ifndef GETFOLDER_H
#define GETFOLDER_H

#include <QVector>

class getFolder
{
public:
    QString getMainFolder();
    QVector<QString> findFilesNeedingScan();
};

#endif // GETFOLDER_H
