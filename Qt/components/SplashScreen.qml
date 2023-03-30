import QtQuick 2.0
import QtQuick.Controls 2.0
import QtQuick.Window 2.2
import QtQuick.Controls.Universal 2.12

Window {
    id: splashScreen
    modality: Qt.ApplicationModal
    flags: Qt.SplashScreen
    width: 500
    height: 500
    x: 710
    y: 290

    FontLoader {
        id: alibaba
        source: "qrc:/components/font/Alibaba-PuHuiTi-Bold.ttf"
    }

    Rectangle {
        id: splashRect
        anchors.fill: parent

        border.width: 1
        border.color: "black"
        color: 'transparent'

        Image {
            fillMode: Image.PreserveAspectFit
            source: 'qrc:/images/bg.png'
        }

        Text {
            anchors.horizontalCenter: parent.horizontalCenter
            anchors.top: parent.top
            anchors.topMargin: 150
            //            width: 500
            //            height: 500
            //            horizontalAlignment: Text.AlignHCenter
            //            verticalAlignment: Text.AlignVCenter
            text: textTitle
            font.pointSize: 24
            font.bold: true
            color: mainTextColor
            font.family: alibaba.name
        }

        BusyIndicator {
            id: busyAnimation
            anchors.horizontalCenter: parent.horizontalCenter
            anchors.bottom: parent.bottom
            anchors.bottomMargin: parent.height / 8
            width: parent.width / 6
            height: width
            running: true
            Universal.accent: Universal.Cyan
        }
    }

    Component.onCompleted: visible = true
}
