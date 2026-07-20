#include <QString>
#include <QObject>
#include <QMediaPlayer>
#include <QAudioOutput>
#include <QUrl>

#ifndef MEDIAPLAYER_H
#define MEDIAPLAYER_H

class Song {
public:
    Song() = default;
    Song(QString sTitle, QString sArtist, QString sAlbum, QString sTrack, QString sGenre, QString sUrl, qint64 sDurationMs);

    QString getTitle() const { return title; }
    QString getArtist() const { return artist; }
    QString getAlbum() const { return album; }
    QString getTrack() const { return track; }
    QString getGenre() const { return genre; }
    QString getUrl() const { return url; }
    qint64 getDurationMs() const { return durationMs; }

    void setTitle(const QString &sTitle);
    void setArtist(const QString &sArtist);
    void setAlbum(const QString &sAlbum);
    void setTrack(const QString &sTrack);
    void setGenre(const QString &sGenre);
    void setUrl(const QString &sUrl);
    void setDurationMs(qint64 sDurationMs);

private:
    QString title;
    QString artist;
    QString album;
    QString track;
    QString genre;
    QString url;
    qint64 durationMs = 0;
};

class MediaPlayer : public QObject
{
    Q_OBJECT
public:
    explicit MediaPlayer(QObject *parent = nullptr);

    void play(const QUrl &url);
    void pause();
    void stop();
    void readMetadata(const QUrl &url);

    qint64 duration() const;
    qint64 position() const;
    QMediaPlayer::PlaybackState playbackState() const;

signals:
    void metadataReady(const Song &song);
    void playbackStateChanged(QMediaPlayer::PlaybackState state);
    void errorOccurred(const QString &message);

private slots:
    void onMediaStatusChanged(QMediaPlayer::MediaStatus status);
    void onErrorOccurred(QMediaPlayer::Error error, const QString &errorString);

private:
    Song extractMetadata() const;

    QMediaPlayer *m_player;
    QAudioOutput *m_audioOutput;
    bool m_pendingPlayback = false;
    QUrl m_currentUrl;
};

#endif // MEDIAPLAYER_H
