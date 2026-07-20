#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QDebug>
#include "mediaplayer.h"

int main(int argc, char *argv[])
{
    QGuiApplication app(argc, argv);
    QQmlApplicationEngine engine;
    QObject::connect(
        &engine,
        &QQmlApplicationEngine::objectCreationFailed,
        &app,
        []() { QCoreApplication::exit(-1); },
        Qt::QueuedConnection);
    engine.loadFromModule("QTunes", "Main");

    MediaPlayer *player = new MediaPlayer(&app);
    QObject::connect(player, &MediaPlayer::metadataReady, &app, [](const Song &song) {
        qDebug() << song.getTitle() << song.getArtist() << song.getDurationMs();
    });

    player->readMetadata(QUrl::fromLocalFile("C:/Users/Administrador/Desktop/LiftYourSkinnyFistsLikeAntennastoHeaven/Storm.mp3"));
    player->play(QUrl::fromLocalFile("C:/Users/Administrador/Desktop/LiftYourSkinnyFistsLikeAntennastoHeaven/Storm.mp3"));

    return app.exec();
}
