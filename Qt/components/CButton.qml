import QtQuick 2.6
import QtQuick.Controls 2.0

Rectangle {
    id: button
    signal clicked
    property alias text: text.text
    border.width: 1
    border.color: mainAppColor
    radius: 3
    property real textHeight: height - 2
    property real fontHeight: 0.3
    property bool pressed: mouse.pressed
    property real implicitMargin: (width - text.implicitWidth) / 2

    Text {
        id: text
        anchors.left: parent.left
        anchors.right: parent.right
        anchors.top: parent.top
        height: parent.textHeight
        horizontalAlignment: Text.AlignHCenter
        verticalAlignment: Text.AlignVCenter
        font.pixelSize: height * fontHeight
        color: "#ffffff"
        font.family: alibaba.name
    }

    MouseArea {
        id: mouse
        anchors.fill: parent
        onClicked: button.clicked()
    }
}
