import QtQuick
import QtQuick.Controls 2.5
import QtQuick.Layouts 1.3
import "backend.js" as Backend

Rectangle {
    id: detailForm
    width: parent.width
    height: parent.height
    color: 'transparent'

    property StackView stack: null
    property string name: ""
    property string uid: ""

    property int statue: 0
    property int loadingCount: 0

    Rectangle {
        width: 1440
        height: 820
        anchors.horizontalCenter: parent.horizontalCenter
        anchors.verticalCenter: parent.verticalCenter
        color: 'transparent'
        border.color: mainAppColor
        border.width: 1
        radius: 5

        Rectangle {
            width: 1280
            height: 720
            color: 'transparent'
            //color: 'blue'
            anchors.top: parent.top
            anchors.topMargin: 50
            anchors.horizontalCenter: parent.horizontalCenter
            // anchors.verticalCenter: parent.verticalCenter

            // CAM Panel
            Image {
                id: panelImg
                width: 1280
                height: 720
                fillMode: Image.Stretch
            }
            Connections {
                target: PanelImage
                onCallQmlRefeshPanelImg: {
                    panelImg.source = ""
                    panelImg.source = "image://PanelImage"
                }
            }
        }

        Text {
            id: carName
            anchors {
                top: myIcon.bottom
                topMargin: 10
                horizontalCenter: parent.horizontalCenter
            }
            text: name
            color: '#eaf4fc'
            font.family: alibaba.name
            //            anchors.topMargin: 5
            font.pointSize: 20
        }

        //        Text {
        //            id: carConnState
        //            anchors {
        //                top: carName.bottom
        //                topMargin: 10
        //                horizontalCenter: parent.horizontalCenter
        //            }
        //            text: !appModel.ready ? "未连接" : "已连接"
        //            color: '#eaf4fc'
        //            font.family: alibaba.name
        //            font.pointSize: 20
        //        }
        Row {
            anchors.top: parent.bottom
            anchors.topMargin: 50
            anchors.horizontalCenter: parent.horizontalCenter

            CButton {
                id: connectButton
                textHeight: 50
                fontHeight: 0.4
                width: 180
                height: 50
                color: pressed ? textClickedColor : mainAppColor
                text: appModel.ready ? "断开" : "连接"
                onClicked: {
                    if (!appModel.ready) {
                        connectButton.visible = false
                        loading.start()
                        carSelect(uid)
                        backButton.visible = false
                    } else {
                        connectButton.visible = false
                        disconn.start()
                        appModel.leaveCar()
                        appModel.ready = false
                    }
                }
            }

            spacing: 50

            CButton {
                id: backButton
                textHeight: 50
                fontHeight: 0.4
                width: 180
                height: 50
                color: pressed ? textClickedColor : mainAppColor
                text: "返回"
                onClicked: {
                    connectButton.enabled = true
                    //                carConnState.text = ""
                    stack.pop()
                }
                visible: true
            }
        }

        Timer {
            id: loading
            interval: 5000
            onTriggered: {
                connectButton.visible = true
                //                loadingCount++
                //                carConnState.text = "连接中" + Backend.dot(loadingCount)
                //                if (loadingCount === 4)
                //                    loadingCount = 0
            }
        }

        Timer {
            id: disconn
            interval: 5000
            onTriggered: {
                connectButton.visible = true
                backButton.visible = true
            }
        }

        Component.onCompleted: {

        }
    }
}
