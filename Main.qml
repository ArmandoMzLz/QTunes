import QtQuick
import QtQuick.Window
import QtQuick.Controls
import QtQuick.Layouts

Window {
    id: root
    width: 800
    height: 600
    visible: true
    title: qsTr("QTunes")
    visibility: Window.Maximized

    property color primaryColor: "#006466"
    property color secondaryColor: "#065a60"
    property color terciaryColor: "#0b525b"

    property color textColor: "#fefcff"
    property color backgroundColor: "#272640"

    property color darkBlue: "#144552"
    property color darkerBlue: "#1b3a4b"
    property color evenDarkerBlue: "#212f45"

    property color darkPurple: "#312244"
    property color darkerPurple: "#3e1f47"
    property color evenDarkerPurple: "#4d194d"

    color: backgroundColor;

    Rectangle {
        id: header
        anchors.top: parent.top
        width: parent.width
        height: 70
        color: darkPurple

        RowLayout {
            anchors.fill: parent
            anchors.leftMargin: 20
            anchors.rightMargin: 20
            spacing: 20

            Text {
                text: "QTunes"
            }
        }
    }
}
