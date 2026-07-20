#include "mediaplayer.h"
#include <qmediametadata.h>

Song::Song(QString sTitle, QString sArtist, QString sAlbum, QString sTrack, QString sGenre, QString sUrl, qint64 sDurationMs)
{
    setTitle(sTitle);
    setArtist(sArtist);
    setAlbum(sAlbum);
    setTrack(sTrack);
    setGenre(sGenre);
    setUrl(sUrl);
    setDurationMs(sDurationMs);
}

void Song::setTitle(const QString &sTitle) { title  = sTitle.isEmpty() ? "Unknown" : sTitle; }
void Song::setArtist(const QString &sArtist) { artist = sArtist.isEmpty() ? "Unknown" : sArtist; }
void Song::setAlbum(const QString &sAlbum) { album  = sAlbum.isEmpty() ? "Unknown" : sAlbum; }
void Song::setTrack(const QString &sTrack) { track  = sTrack.isEmpty() ? "Unknown" : sTrack; }
void Song::setGenre(const QString &sGenre) { genre  = sGenre.isEmpty() ? "Unknown" : sGenre; }
void Song::setUrl(const QString &sUrl) { url = sUrl.isEmpty() ? "Unknown" : sUrl; }

void Song::setDurationMs(qint64 sDurationMs) {
    durationMs = sDurationMs >= 0 ? sDurationMs : 0;
}

MediaPlayer::MediaPlayer(QObject *parent): QObject(parent), m_player(new QMediaPlayer(this)), m_audioOutput(new QAudioOutput(this)) {
    m_player->setAudioOutput(m_audioOutput);

    connect(m_player, &QMediaPlayer::mediaStatusChanged, this, &MediaPlayer::onMediaStatusChanged);
    connect(m_player, &QMediaPlayer::playbackStateChanged, this, &MediaPlayer::playbackStateChanged);
    connect(m_player, &QMediaPlayer::errorOccurred, this, &MediaPlayer::onErrorOccurred);
}

void MediaPlayer::play(const QUrl &url) {
    m_pendingPlayback = true;
    m_currentUrl = url;
    m_player->setSource(url);
}

void MediaPlayer::pause() { m_player->pause(); }
void MediaPlayer::stop() { m_player->stop(); }

void MediaPlayer::readMetadata(const QUrl &url) {
    m_pendingPlayback = false;
    m_currentUrl = url;
    m_player->setSource(url);
}

qint64 MediaPlayer::duration() const { return m_player->duration(); }
qint64 MediaPlayer::position() const { return m_player->position(); }
QMediaPlayer::PlaybackState MediaPlayer::playbackState() const { return m_player->playbackState(); }

Song MediaPlayer::extractMetadata() const {
    const QMediaMetaData metaData = m_player->metaData();

    return Song(
        metaData.value(QMediaMetaData::Title).toString(),
        metaData.value(QMediaMetaData::AlbumArtist).toString(),
        metaData.value(QMediaMetaData::AlbumTitle).toString(),
        metaData.value(QMediaMetaData::TrackNumber).toString(),
        metaData.value(QMediaMetaData::Genre).toString(),
        m_currentUrl.toString(),
        m_player->duration()
    );
}

void MediaPlayer::onMediaStatusChanged(QMediaPlayer::MediaStatus status) {
    if (status == QMediaPlayer::LoadedMedia || status == QMediaPlayer::BufferedMedia) {
        emit metadataReady(extractMetadata());

        if (m_pendingPlayback) {
            m_pendingPlayback = false;
            m_player->play();
        }
    }
}

void MediaPlayer::onErrorOccurred(QMediaPlayer::Error error, const QString &errorString) {
    Q_UNUSED(error);
    emit errorOccurred(errorString);
}
