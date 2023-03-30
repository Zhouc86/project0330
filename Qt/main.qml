import QtQuick
import QtQuick.Layouts
import QtQuick.Controls
//import QtDataVisualization 1.2
import "components"

Item {
    id: rootItem
    width: 1920
    height: 1080

    property string textTitle: '湛江钢铁非视距远程操控系统'
    property string mainTextColor: '#29b6fb'
    property string mainAppColor: '#29b6fb'
    property string textClickedColor: '#7ad4ff'
    property string textColorGreen: '#00FFFF'
    property color popupBgColorErr: "#b44"
    property color popupBgColorWarn: "#f7bd3e"
    property color popupBgColorOK: "#29b6fb"
    property color popupTextColor: "#ffffff"

    FontLoader {
        id: alibaba
        source: "qrc:/components/font/Alibaba-PuHuiTi-Bold.ttf"
    }

    FontLoader {
        id: fontawesome
        source: "qrc:/components/font/fontawesome-webfont.ttf"
    }

    FontLoader {
        id: digtalFont
        source: "qrc:/components/font/DS-DIGIB.ttf"
    }

    Popup {
        id: popup
        property alias popMessage: message.text
        property alias popColor: popupBg.color

        background: Rectangle {
            id: popupBg
            implicitWidth: rootItem.width
            implicitHeight: 60
        }
        y: (rootItem.height - 60)
        modal: true
        focus: true
        closePolicy: Popup.CloseOnPressOutside
        Text {
            id: message
            anchors.centerIn: parent
            font.pointSize: 12
            color: popupTextColor
            font.family: '黑体'
        }
        onOpened: popupClose.start()
    }

    signal qmlSignalMaxWindow
    signal qmlSignalQuit

    state: "loading"

    states: [
        State {
            name: "loading"
            PropertyChanges {
                target: panelView
                opacity: 0
            }
        },
        State {
            name: "loaded"
            PropertyChanges {
                target: panelView
                opacity: 1
            }
        }
    ]

    Item {
        id: mainView
        //        MainWindow {}
    }

    Item {
        id: panelView
        width: 1920
        height: 1080
        visible: true
        x: 0
        y: 0

        Dialog {
            id: closeDialog
            visible: false
            modal: true
            x: (parent.width - width) / 2
            y: (parent.height - height) / 2
            closePolicy: Popup.NoAutoClose
            implicitWidth: 500
            implicitHeight: 300

            Text {
                text: "该账号已登录，程序将自动退出"
                font.pointSize: 24
                font.family: alibaba.name
                color: mainAppColor
                anchors.horizontalCenter: parent.horizontalCenter
                y: 100
            }

            onOpened: {
                closeWinTimer.start()
            }
        }

        Dialog {
            id: panelDialog
            visible: false
            modal: true
            x: (parent.width - width) / 2
            y: (parent.height - height) / 2
            closePolicy: Popup.NoAutoClose
            implicitWidth: 500
            implicitHeight: 300

            Text {
                text: "确认退出吗?"
                font.pointSize: 24
                font.family: alibaba.name
                color: mainAppColor
                anchors.horizontalCenter: parent.horizontalCenter
                y: 100
            }

            Row {
                height: parent.height
                anchors.horizontalCenter: parent.horizontalCenter
                y: 200

                CButton {
                    id: cancelBtn
                    textHeight: 50
                    fontHeight: 0.4
                    width: 150
                    height: 50
                    color: pressed ? textClickedColor : mainAppColor
                    text: "取消"
                    onClicked: panelDialog.visible = false
                }

                spacing: 50

                CButton {
                    id: quitButton
                    textHeight: 50
                    fontHeight: 0.4
                    width: 150
                    height: 50
                    color: pressed ? textClickedColor : mainAppColor
                    text: "退出"
                    onClicked: {
                        videoWindow.close()
                        rootItem.qmlSignalQuit()
                    }
                }
            }
        }

        PanelWindow {
            id: subPage
            focus: true
            Keys.enabled: true
            Keys.onEscapePressed: {
                panelDialog.visible = true
            }
        }

        Window {
            id: videoWindow
            property int wndIndex: 0
            visible: true
            width: 3840
            height: 1080
            x: 1920
            title: qsTr("Hyper vision remote control System")
            flags: Qt.Window | Qt.FramelessWindowHint
            screen: Qt.application.screens[wndIndex]
            MainWindow {}

            Component.onCompleted: {
                appModel.radar1 = 0
                appModel.radar2 = 0
                appModel.radar3 = 0
                appModel.radar4 = 0
                appModel.radar5 = 0
                appModel.radar6 = 0
                appModel.ping = 0
                appModel.ready = 0
                appModel.imux = 0
                appModel.imuy = 0
                appModel.username = ''
                appModel.password = ''
            }
        }
    }

    Timer {
        id: splashScreenClose
        interval: 1000
        onTriggered: {
            splashScreenLoader.item.visible = false
            splashScreenLoader.source = ""
            rootItem.qmlSignalMaxWindow()
            rootItem.state = "loaded"
        }
    }

    Loader {
        id: splashScreenLoader
        source: "qrc:/components/SplashScreen.qml"
        onLoaded: {
            splashScreenClose.start()
        }
    }

    //    Component.onCompleted: {
    //                videoWin.createObject(rootItem, {
    //                                          "wndIndex": 0
    //                                      })
    //    }

    //    Component {
    //        id: videoWin
    //        Window {
    //            id: videoWindow
    //            property int wndIndex: 0
    //            visible: true
    //            width: 3840
    //            height: 1080
    //            x: 2560
    //            title: qsTr("Hyper vision remote control System")
    //            flags: Qt.Window | Qt.FramelessWindowHint
    //            screen: Qt.application.screens[wndIndex]
    //            MainWindow {}

    //            Component.onCompleted: {
    //                appModel.radar1 = 0
    //                appModel.radar2 = 0
    //                appModel.radar3 = 0
    //                appModel.radar4 = 0
    //                appModel.radar5 = 0
    //                appModel.radar6 = 0
    //                appModel.delay = 0
    //                appModel.ready = 0
    //            }
    //        }
    //    }
}
