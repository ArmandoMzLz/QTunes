#include "getfolder.h"

#include <QStandardPaths>
#include <QDir>
#include <QDirIterator>
#include <QFileInfo>
#include <QSqlDatabase>
#include <QSqlQuery>

QString getFolder::getMainFolder() {
    QString musicPath = QStandardPaths::writableLocation(QStandardPaths::MusicLocation);
    QDir dir(musicPath);


    if(!dir.exists())
        if(!dir.mkpath(musicPath))
            qWarning() << "Failed to create folder";

    return musicPath;
}

QVector<QString> getFolder::findFilesNeedingScan() {
    getFolder getfolder;
    QVector<QString> pending;
    QString musicPath = getfolder.getMainFolder();

    static const QSet<QString> extensions = {"mp3", "flac", "wav", "m4a", "ogg"};

    QSqlQuery existing (QSqlDatabase::database());
    existing.prepare("SELECT last_modified, file_size song WHERE file_path = ?");

    QDirIterator it(musicPath, QDir::Files, QDirIterator::Subdirectories);
    while (it.hasNext()) {
        QString path = it.next();
        QFileInfo info(path);

        if(!extensions.contains(info.suffix().toLower()))
            continue;

        qint64 mtime = info.lastModified().toSecsSinceEpoch();
        qint64 size = info.size();

        existing.bindValue(0, path);
        existing.exec();

        if(!existing.next())
            pending.append(path);
        else {
            bool changed = existing.value(0).toLongLong() != mtime || existing.value(1).toLongLong() != size;

            if(changed)
                pending.append(path);
        }
    }

    return pending;
}
