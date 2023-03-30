import QtQuick
import QtQuick.Controls 2.5
import QtQuick.Layouts 1.3

Rectangle {
    id: carForm
    width: parent.width
    height: parent.height
    color: 'transparent'

    property StackView stack: null

    Component.onCompleted: {

    }

    Rectangle {
        width: parent.width
        height: parent.height
        anchors.top: parent.top
        anchors.horizontalCenter: parent.horizontalCenter
        anchors.verticalCenter: parent.verticalCenter
        color: 'transparent'

        //        ListModel {
        //            id: carList
        //            ListElement {
        //                name: "Music"
        //                icon: "qrc:/images/car1.png"
        //            }
        //            ListElement {
        //                name: "Movies"
        //                icon: "qrc:/images/car1.png"
        //            }
        //            ListElement {
        //                name: "Camera"
        //                icon: "qrc:/images/car1.png"
        //            }
        //        }
        //        Rectangle {
        //            width: 100
        //            height: 100
        //            Text {
        //                text: appModel.city
        //            }
        //        }
        //        Rectangle {
        //            width: 200
        //            height: 100
        //            Text {
        //                anchors {
        //                    top: myIcon.bottom
        //                    topMargin: 10
        //                    horizontalCenter: parent.horizontalCenter
        //                }
        //                text: "test" + 1000004
        //                color: '#eaf4fc'
        //                font.family: alibaba.name
        //                font.pointSize: 20
        //            }

        //            //            Button {
        //            //                width: 100
        //            //                height: 100
        //            //            }
        //            MouseArea {
        //                anchors.fill: parent
        //                onDoubleClicked: {
        //                    carSelect(1000004)
        //                }
        //            }
        //        }
        GridView {
            id: carGrid
            anchors.fill: parent
            cellWidth: 400
            cellHeight: 400
            focus: true
            anchors.leftMargin: 160
            anchors.rightMargin: 160
            anchors.topMargin: 160
            anchors.bottomMargin: 160

            model: appModel.car

            Rectangle {
                width: 350
                height: 350
                radius: 5
                //                border.color: mainAppColor
                color: 'transparent'
            }

            delegate: Item {
                required property string name
                required property int uid

                width: 350
                height: 350

                Image {
                    id: myIcon
                    width: 200
                    height: 200
                    anchors.horizontalCenter: parent.horizontalCenter
                    anchors.verticalCenter: parent.verticalCenter
                    fillMode: Image.PreserveAspectFit
                    source: 'qrc:/images/car1.png'
                }

                Text {
                    anchors {
                        top: myIcon.bottom
                        topMargin: 10
                        horizontalCenter: parent.horizontalCenter
                    }
                    text: parent.name + "--" + parent.uid
                    color: '#eaf4fc'
                    font.family: alibaba.name
                    font.pointSize: 20
                }

                MouseArea {
                    anchors.fill: parent
                    //                    onClicked: parent.GridView.view.currentIndex = parent.uid
                    onClicked: {
                        parent.GridView.view.currentIndex = parent.uid
                        console.log(parent.GridView.view.currentInde)
                        //carSelect(parent.GridView.view.currentIndex)
                        page_detail.visible = true
                        page_detail.stack = stack
                        stack.push(page_detail, {
                                       "name": parent.name,
                                       "uid": parent.uid
                                   })
                    }
                }
            }
        }

        //        CButton {
        //            id: resultButton
        //            anchors.top: carGrid.bottom
        //            anchors.topMargin: 50
        //            anchors.horizontalCenter: parent.horizontalCenter
        //            textHeight: 50
        //            fontHeight: 0.4
        //            width: 250
        //            height: 50
        //            color: pressed ? textClickedColor : mainAppColor
        //            text: "登 出"
        //            onClicked: {
        //                appModel.logout()
        //                stack.pop()
        //            }
        //        }
    }
}
