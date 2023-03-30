import QtQuick
import QtQuick.Controls 2.5
import QtQuick.Layouts 1.3

Rectangle {
    id: loginForm
    width: parent.width
    height: parent.height
    color: 'transparent'

    property StackView stack: null

    Component.onCompleted: {
        appModel.loadUser()
    }

    Rectangle {
        id: loginFormRect
        width: parent.width
        height: 500
        anchors.top: parent.top
        anchors.topMargin: 250
        color: 'transparent'

        Image {
            id: loginBg
            width: 960
            height: 500
            anchors.fill: parent
            fillMode: Image.PreserveAspectFit
            source: 'qrc:/images/login_bg.png'
        }

        Rectangle {
            id: inputColumn
            width: 960
            height: 500
            anchors.top: parent.top
            anchors.topMargin: 150
            anchors.fill: parent
            color: 'transparent'

            TextField {
                id: loginUsername
                width: 450
                height: 50
                anchors.horizontalCenter: parent.horizontalCenter
                color: mainTextColor
                font.pointSize: 22
                font.family: fontawesome.name
                leftPadding: 50
                text: appModel.username
                background: Rectangle {
                    implicitWidth: 50
                    implicitHeight: 50
                    radius: implicitHeight / 2
                    color: "transparent"
                    Text {
                        text: "\uf007"
                        font.pointSize: 24
                        font.family: fontawesome.name
                        color: mainAppColor
                        anchors.left: parent.left
                        anchors.verticalCenter: parent.verticalCenter
                        leftPadding: 10
                    }
                    Rectangle {
                        width: parent.width - 10
                        height: 1
                        anchors.horizontalCenter: parent.horizontalCenter
                        anchors.bottom: parent.bottom
                        color: mainTextColor
                    }
                    Rectangle {
                        width: 25
                        height: 25
                        anchors.left: parent.right
                        anchors.verticalCenter: parent.verticalCenter
                        color: 'transparent'
                        Text {
                            anchors.fill: parent
                            text: "\uf05c"
                            font.pointSize: 20
                            font.family: fontawesome.name
                            color: "#003399"
                        }
                        MouseArea {
                            anchors.fill: parent
                            onClicked: {
                                loginUsername.text = ''
                            }
                        }
                    }
                }
            }

            TextField {
                id: loginPassword
                anchors.top: loginUsername.bottom
                anchors.topMargin: 20
                width: 450
                height: 50
                anchors.horizontalCenter: parent.horizontalCenter
                color: mainTextColor
                font.pointSize: 22
                font.family: fontawesome.name
                leftPadding: 50
                echoMode: TextField.Password
                text: appModel.password
                onAccepted: loginUser(loginUsername.text, loginPassword.text)
                Keys.enabled: true
                Keys.onEnterPressed: console.log("enter")

                background: Rectangle {
                    implicitWidth: 50
                    implicitHeight: 50
                    radius: implicitHeight / 2
                    color: "transparent"
                    Text {
                        text: "\uf023"
                        font.pointSize: 24
                        font.family: fontawesome.name
                        color: mainAppColor
                        anchors.left: parent.left
                        anchors.verticalCenter: parent.verticalCenter
                        leftPadding: 10
                    }
                    Rectangle {
                        width: parent.width - 10
                        height: 1
                        anchors.horizontalCenter: parent.horizontalCenter
                        anchors.bottom: parent.bottom
                        color: mainTextColor
                    }
                    Rectangle {
                        width: 25
                        height: 25
                        anchors.left: parent.right
                        anchors.verticalCenter: parent.verticalCenter
                        color: 'transparent'
                        Text {
                            anchors.fill: parent
                            text: "\uf05c"
                            font.pointSize: 20
                            font.family: fontawesome.name
                            color: "#003399"
                        }
                        MouseArea {
                            anchors.fill: parent
                            onClicked: {
                                loginPassword.text = ''
                            }
                        }
                    }
                }
            }

            CButton {
                id: resultButton
                anchors.top: loginPassword.bottom
                anchors.topMargin: 50
                anchors.horizontalCenter: parent.horizontalCenter
                textHeight: 50
                fontHeight: 0.4
                width: 450
                height: 50
                color: pressed ? textClickedColor : mainAppColor
                text: "登 录"
                onClicked: loginUser(loginUsername.text, loginPassword.text)
            }
        }
    }
}
