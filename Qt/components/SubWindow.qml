import QtQuick
import QtQuick.Controls 2.5
import QtQuick.Layouts 1.3
import "backend.js" as Backend

Item {
    width: 1920
    height: 1080

    FontLoader {
        id: alibaba
        source: "qrc:/components/font/Alibaba-PuHuiTi-Bold.ttf"
    }

    FontLoader {
        id: fontawesome
        source: "qrc:/components/font/fontawesome-webfont.ttf"
    }

    signal loginSuccess(var userName, var password)

    //    signal carSeleted(var id)
    function loginUser(uname, pword) {
        var ret = Backend.validateUserCredentials(uname, pword)
        var message = ""
        if (ret) {
            message = "用户名或密码输入错误"
            subPopup.popMessage = message
            subPopup.open()
            return
        }
        //        loginSuccess(uname, pword)
        appModel.userLogin(uname, pword)

        page_car.visible = true
        page_car.stack = stack
        stack.push(page_car)
    }

    function carSelect(id) {
        appModel.connectCar(id)
    }

    Popup {
        id: subPopup

        property alias popMessage: message.text

        background: Rectangle {
            id: subPopupBg
            implicitWidth: 1920
            implicitHeight: 60
            color: popupBgColorErr
        }
        y: (parent.height - 60)
        modal: true
        focus: true
        closePolicy: Popup.CloseOnPressOutside
        Text {
            id: message
            anchors.centerIn: parent
            font.pointSize: 12
            color: popupTextColor
            font.family: alibaba.name
        }
        onOpened: subPopupClose.start()
    }

    Timer {
        id: subPopupClose
        interval: 3000
        onTriggered: subPopup.close()
    }

    Rectangle {
        anchors.fill: parent
        Image {
            fillMode: Image.PreserveAspectFit
            source: 'qrc:/images/bg.png'
        }
        color: 'transparent'
    }

    Rectangle {
        width: parent.width
        height: 80
        color: 'transparent'
        z: 99

        Image {
            anchors.horizontalCenter: parent.horizontalCenter
            fillMode: Image.PreserveAspectFit
            source: 'qrc:/images/banner.png'
        }
        Text {
            anchors.horizontalCenter: parent.horizontalCenter
            anchors.verticalCenter: parent.verticalCenter
            text: '中冶非视距远程操控系统'
            font.family: alibaba.name
            font.pointSize: 24
            color: mainTextColor
        }
    }

    StackView {
        id: stack
        initialItem: page_login
        anchors.fill: parent
    }

    LoginPage {
        id: page_login
        visible: false
    }

    CarPage {
        id: page_car
        visible: false
    }

    DetailPage {
        id: page_detail
        visible: false
    }
}
