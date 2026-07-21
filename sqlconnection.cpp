#include "sqlconnection.h"

#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlError>
#include <QStandardPaths>
#include <QDir>

bool sqlconnection::createTables() {
    QSqlDatabase db = QSqlDatabase::addDatabase("QSQLITE");

    QString writePath = QStandardPaths::writableLocation(QStandardPaths::AppDataLocation);
    QDir().mkpath(writePath);
    QString dbPath = writePath + "/music.db";

    db.setDatabaseName(dbPath);

    if(!db.open()) {
        qWarning() << "Cannot open db: " << db.lastError().text();
        return false;
    }

    QSqlQuery query (db);

    if(!query.exec("PRAGMA foreign_keys = ON;")) {
        qWarning() << "Cannot get foreing_keys: " << query.lastError().text();
        return false;
    }

    if(!query.exec(R"(
        CREATE TABLE IF NOT EXISTS songs(
            id INTEGER PRIMARY KEY AUTOINCREMENT,
            file_path TEXT UNIQUE NOT NULL,
            title TEXT,
            artists TEXT,
            album TEXT,
            track TEXT,
            genre TEXT,
            duration_ms INTEGER,
            last_modified INTEGER NOT NULL,
            file_size INTEGER NOT NULL,
            play_count INTEGER NOT NULL DEFAULT 0,
            last_played INTEGER
        );
    )")) {
        qWarning() << "Error to create table songs: " << query.lastError().text();
        return false;
    }

    if (!query.exec(R"(
        CREATE TABLE IF NOT EXISTS play_history (
            id        INTEGER PRIMARY KEY AUTOINCREMENT,
            song_id   INTEGER NOT NULL,
            played_at INTEGER NOT NULL,
            FOREIGN KEY (song_id) REFERENCES songs(id) ON DELETE CASCADE
        );
    )")) {
        qWarning() << "Error to create table play_history: " << query.lastError().text();
        return false;
    }

    if (!query.exec("CREATE INDEX IF NOT EXISTS idx_play_history_song ON play_history(song_id);")) {
        qWarning() << "Error to create index idx_play_history_song: " << query.lastError().text();
        return false;
    }

    if (!query.exec(R"(
        CREATE TABLE IF NOT EXISTS playlists (
            id INTEGER PRIMARY KEY AUTOINCREMENT,
            name TEXT NOT NULL,
            created_at INTEGER NOT NULL
        );
    )")) {
        qWarning() << "Error to create table playlists: " << query.lastError().text();
        return false;
    }


    if (!query.exec(R"(
        CREATE TABLE IF NOT EXISTS playlist_songs (
            id INTEGER PRIMARY KEY AUTOINCREMENT,
            playlist_id INTEGER NOT NULL,
            song_id INTEGER NOT NULL,
            position INTEGER NOT NULL,
            added_at INTEGER NOT NULL,
            FOREIGN KEY (playlist_id) REFERENCES playlists(id) ON DELETE CASCADE,
            FOREIGN KEY (song_id) REFERENCES songs(id) ON DELETE CASCADE
        );
    )")) {
        qWarning() << "Error to create table playlist_songs" << query.lastError().text();
        return false;
    }

    if (!query.exec(R"(
        CREATE INDEX IF NOT EXISTS idx_playlist_songs_playlist ON playlist_songs(playlist_id, position);
    )")) {
        qWarning() << "Error to create index idx_playlists_songs_playlist: " << query.lastError().text();
        return false;
    }

    if (!query.exec(R"(
        CREATE TRIGGER IF NOT EXISTS trg_update_play_count
        AFTER INSERT ON play_history
        BEGIN
            UPDATE songs
            SET play_count = play_count + 1,
                last_played = NEW.played_at
            WHERE id = NEW.song_id;
        END;
    )")) {
        qWarning() << "Error to create trigger:" << query.lastError().text();
        return false;
    }

    return true;
}


