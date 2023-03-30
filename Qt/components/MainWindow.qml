import QtQml 2.3
import QtQuick 2.6
import QtQuick.Window 2.2
import QtQuick.Controls 2.0

//import QtDataVisualization 1.2
//import QtQuick3D
Item {
    id: mainWindow
    visible: true
    width: 3840
    height: 1080

    //    signal imageEvent(string params)

    //    onImageEvent: {
    //        if (params === "redrawImage") {
    //            cam_front_rear.signalRedraw()
    //        }
    //    }
    //    property int mainHeight: 1000
    //    property int lineWidth: 20
    //    property int statusWidth: 467
    //    property int leftCamWidth: 540
    //    property int frontCamWidth: 1706
    //    property int rightCamWidth: 540
    //    property int radarWidth: 467
    //    property int radarSize: 120
    property int mainHeight: 1000
    property int lineWidth: 20
    property int statusWidth: 417
    property int leftCamWidth: 490
    property int frontCamWidth: 1906
    property int rightCamWidth: 490
    property int radarWidth: 417
    property int radarSize: 120

    //    property int topCamHeight: 500
    //    property int bottomCamHeight: 500
    property int radarRedRange: 2000
    property int radarGreenRange: 4000

    property string radarRightFGreenImg: 'qrc:/images/right_f_green.png'
    property string radarRightFYellowImg: 'qrc:/images/right_f_yellow.png'
    property string radarRightFRedImg: 'qrc:/images/right_f_red.png'

    property string radarRightRGreenImg: 'qrc:/images/right_r_green.png'
    property string radarRightRYellowImg: 'qrc:/images/right_r_yellow.png'
    property string radarRightRRedImg: 'qrc:/images/right_r_red.png'

    property string radarLeftFGreenImg: 'qrc:/images/left_f_green.png'
    property string radarLeftFYellowImg: 'qrc:/images/left_f_yellow.png'
    property string radarLeftFRedImg: 'qrc:/images/left_f_red.png'

    property string radarLeftRGreenImg: 'qrc:/images/left_r_green.png'
    property string radarLeftRYellowImg: 'qrc:/images/left_r_yellow.png'
    property string radarLeftRRedImg: 'qrc:/images/left_r_red.png'

    property string radarBottomGreenImg: 'qrc:/images/bottom_green.png'
    property string radarBottomYellowImg: 'qrc:/images/bottom_yellow.png'
    property string radarBottomRedImg: 'qrc:/images/bottom_red.png'

    property int radarRightMargin: 40
    property int radarTopMargin: 350
    property int radarBottomMargin: 280
    property int radarleftMargin: 200
    property int radarTxtSize: 18

    FontLoader {
        id: alibaba
        source: "qrc:/components/font/Alibaba-PuHuiTi-Bold.ttf"
    }

    FontLoader {
        id: fontawesome
        source: "qrc:/components/font/fontawesome-webfont.ttf"
    }

    Rectangle {
        id: mainForm
        anchors.fill: parent

        Image {
            fillMode: Image.PreserveAspectFit
            source: 'qrc:/images/bg4k.png'
        }

        Rectangle {
            id: mainFormHeader
            width: mainForm.width
            height: 80
            color: 'transparent'
            Image {
                id: mainFormBannerBg
                anchors.top: mainFormHeader.top
                anchors.horizontalCenter: mainFormHeader.horizontalCenter
                fillMode: Image.PreserveAspectFit
                source: 'qrc:/images/banner.png'
            }
            Text {
                id: mainFormTitle
                width: mainFormHeader.width
                height: mainFormHeader.height
                horizontalAlignment: Text.AlignHCenter
                verticalAlignment: Text.AlignVCenter
                text: textTitle
                font.family: alibaba.name
                font.pointSize: 24
                color: mainTextColor
            }

            Text {
                id: delayms
                text: '网络延迟： ' + (appModel.ping / 2).toFixed(1) + ' ms'
                color: appModel.ping > 200 ? popupBgColorErr : mainTextColor
                font.family: alibaba.name
                font.pointSize: 12
                anchors.right: mainFormHeader.right
                anchors.rightMargin: 50
                anchors.top: mainFormHeader.top
                anchors.topMargin: 50
            }
        }

        Rectangle {
            id: mainFormContent
            width: mainForm.width
            height: mainForm.height - 80
            anchors.top: mainFormHeader.bottom
            color: 'transparent'

            Row {
                Rectangle {
                    id: statusData
                    width: statusWidth + lineWidth
                    height: mainHeight
                    color: 'transparent'

                    Image {
                        width: lineWidth
                        height: mainHeight
                        anchors.left: statusData.left
                        anchors.leftMargin: 20
                        fillMode: Image.Stretch
                        source: 'qrc:/images/border_l.png'
                    }

                    Column {
                        id: carDataForm
                        width: statusData.width - 60
                        height: 300
                        anchors.top: statusData.top
                        anchors.topMargin: 20
                        anchors.horizontalCenter: statusData.horizontalCenter

                        Text {
                            id: posText
                            text: "车辆姿态:"
                            color: mainTextColor
                            anchors.left: statusData.left
                            anchors.leftMargin: 20
                            font.family: alibaba.name
                            font.pointSize: 20
                            anchors.horizontalCenter: statusData.horizontalCenter
                        }

                        Image {
                            id: posSideView
                            anchors.top: posText.bottom
                            anchors.topMargin: 50
                            width: 360
                            height: 240
                            fillMode: Image.Stretch
                            source: 'qrc:/images/side_view.png'
                            anchors.horizontalCenter: statusData.horizontalCenter
                            //transformOrigin: Item.Center
                            Component.onCompleted: {
                                posSideTimer.start()
                            }
                        }

                        RotationAnimation {
                            id: posSideAnim
                            target: posSideView
                            from: 0
                            to: 0
                            duration: 100
                        }

                        Timer {
                            id: posSideTimer
                            interval: 2000
                            repeat: true
                            onTriggered: {
                                posSideAnim.from = 0
                                posSideAnim.to = appModel.imux
                                posSideAnim.start()
                            }
                        }

                        Row {
                            anchors.top: posSideView.bottom
                            anchors.topMargin: 20
                            Text {
                                id: xAngle
                                text: "横向倾角(度):"
                                color: mainTextColor
                                anchors.left: statusData.left
                                anchors.leftMargin: 20
                                font.family: alibaba.name
                                font.pointSize: 20
                                anchors.horizontalCenter: statusData.horizontalCenter
                            }
                            spacing: 50
                            Text {
                                id: xAngleValue
                                text: appModel.imux
                                color: appModel.imux >= 30 ? 'red' : '#FFFFFF'
                                font.family: digtalFont.name
                                font.pointSize: 36
                            }
                        }

                        Image {
                            id: posRearView
                            anchors.top: posSideView.bottom
                            anchors.topMargin: 80
                            width: 360
                            height: 240
                            fillMode: Image.Stretch
                            source: 'qrc:/images/rear_view.png'
                            anchors.horizontalCenter: statusData.horizontalCenter
                            Component.onCompleted: {
                                posRearTimer.start()
                            }
                        }

                        RotationAnimation {
                            id: posRearAnim
                            target: posRearView
                            from: 0
                            to: 0
                            duration: 100
                        }

                        Timer {
                            id: posRearTimer
                            interval: 2000
                            repeat: true
                            onTriggered: {
                                posRearAnim.from = 0
                                posRearAnim.to = appModel.imuy
                                posRearAnim.start()
                            }
                        }

                        Row {
                            anchors.top: posRearView.bottom
                            anchors.topMargin: 20
                            Text {
                                id: yAngle
                                text: "纵向倾角(度):"
                                color: mainTextColor
                                anchors.left: statusData.left
                                anchors.leftMargin: 20
                                font.family: alibaba.name
                                font.pointSize: 20
                                anchors.horizontalCenter: statusData.horizontalCenter
                            }
                            spacing: 50
                            Text {
                                id: yAngleValue
                                text: appModel.imuy
                                color: appModel.imuy >= 30 ? 'red' : '#FFFFFF'
                                font.family: digtalFont.name
                                font.pointSize: 36
                            }
                        }
                    }

                    Rectangle {
                        id: refreshDate
                        width: statusData.width - 60
                        height: 50
                        color: mainAppColor
                        anchors.bottom: statusData.bottom
                        anchors.bottomMargin: 30
                        anchors.horizontalCenter: statusData.horizontalCenter
                        anchors.horizontalCenterOffset: 10
                        Text {
                            id: localTime
                            width: 140
                            height: 50
                            anchors.horizontalCenter: refreshDate.horizontalCenter
                            color: '#FFFFFF'
                            font.family: digtalFont.name
                            font.pointSize: 36
                            text: new Date().toLocaleTimeString()
                            Component.onCompleted: {
                                localTimeRefresh.start()
                            }
                        }
                        Timer {
                            id: localTimeRefresh
                            interval: 1000
                            repeat: true
                            onTriggered: {
                                localTime.text = new Date().toLocaleTimeString()
                            }
                        }
                    }

                    Image {
                        width: lineWidth
                        height: mainHeight
                        anchors.right: statusData.right
                        fillMode: Image.Stretch
                        source: 'qrc:/images/border_r.png'
                    }
                }

                Rectangle {
                    id: leftCam
                    width: leftCamWidth + lineWidth
                    height: mainHeight
                    color: 'transparent'

                    Image {
                        width: lineWidth
                        height: mainHeight
                        anchors.left: leftCam.left
                        anchors.leftMargin: 20
                        fillMode: Image.Stretch
                        source: 'qrc:/images/border_l.png'
                        z: 99
                    }

                    Column {
                        anchors.left: leftCam.left
                        anchors.leftMargin: 20

                        //                        // CAM ForwardRight
                        //                        Rectangle {
                        //                            width: rightCamWidth
                        //                            height: topCamHeight
                        //                            color: 'transparent'
                        //                            Image {
                        //                                id: forwardRightImg
                        //                                width: leftCamWidth
                        //                                height: bottomCamHeight
                        //                                //fillMode: Image.Stretch
                        //                                fillMode: Image.PreserveAspectCrop
                        //                            }
                        //                            Connections {
                        //                                target: ForwardRightImage
                        //                                onCallQmlRefeshForwardRightImg: {
                        //                                    forwardRightImg.source = ""
                        //                                    forwardRightImg.source = "image://ForwardRightImage"
                        //                                }
                        //                            }
                        //                        }
                        Rectangle {
                            width: leftCamWidth
                            height: mainHeight
                            color: 'transparent'
                            anchors.left: leftCam.left
                            anchors.leftMargin: 20

                            // CAM Left
                            Image {
                                id: leftImg
                                width: leftCamWidth
                                height: mainHeight
                                fillMode: Image.Stretch
                            }
                            Connections {
                                target: LeftImage
                                onCallQmlRefeshLeftImg: {
                                    leftImg.source = ""
                                    leftImg.source = "image://LeftImage"
                                }
                            }
                        }
                    }

                    Image {
                        width: lineWidth
                        height: mainHeight
                        anchors.right: leftCam.right
                        fillMode: Image.Stretch
                        source: 'qrc:/images/border_r.png'
                        z: 99
                    }
                }

                Rectangle {
                    id: centerPanel
                    width: frontCamWidth + 2 * lineWidth
                    height: mainHeight
                    anchors.top: mainFormContent.top
                    anchors.horizontalCenter: mainFormContent.horizontalCenter
                    color: 'transparent'

                    MouseArea {
                        anchors.fill: parent
                        drag.target: backImgRect
                        drag.axis: Drag.XandYAxis
                        drag.minimumX: 20
                        drag.maximumX: centerPanel.width - backImgRect.width - 20
                        drag.minimumY: 0
                        drag.maximumY: centerPanel.height - backImgRect.height
                    }

                    Image {
                        width: lineWidth
                        height: mainHeight
                        anchors.left: centerPanel.left
                        anchors.leftMargin: 20
                        fillMode: Image.Stretch
                        source: 'qrc:/images/border_l.png'
                        z: 99
                    }

                    Rectangle {
                        width: frontCamWidth
                        height: mainHeight
                        color: 'transparent'
                        anchors.left: centerPanel.left
                        anchors.leftMargin: 20

                        // CAM FRONT
                        Image {
                            id: img
                            anchors.topMargin: 140
                            width: frontCamWidth
                            height: mainHeight
                            fillMode: Image.Stretch
                            z: 97
                        }
                        Connections {
                            target: FrontImage
                            onCallQmlRefeshImg: {
                                img.source = ""
                                img.source = "image://FrontImage"
                            }
                        }
                    }

                    Rectangle {
                        id: backImgRect
                        width: frontCamWidth / 3
                        height: mainHeight / 3
                        color: 'transparent'
                        x: 25
                        border.color: mainAppColor
                        border.width: 1

                        Image {
                            id: cautionLine
                            anchors.horizontalCenter: parent.horizontalCenter
                            width: parent.width / 1.5
                            height: parent.height / 3
                            fillMode: Image.Stretch
                            z: 99
                            y: 220
                            source: 'qrc:/images/caution.png'
                        }

                        // CAM BACK REAR
                        Image {
                            id: backImg
                            anchors.topMargin: 140
                            width: parent.width
                            height: parent.height
                            fillMode: Image.Stretch
                            z: 98
                        }

                        Connections {
                            target: BackImage
                            onCallQmlRefeshBackImg: {
                                backImg.source = ""
                                backImg.source = "image://BackImage"
                            }
                        }
                    }

                    Image {
                        width: lineWidth
                        height: mainHeight
                        anchors.right: centerPanel.right
                        anchors.rightMargin: 20
                        fillMode: Image.Stretch
                        source: 'qrc:/images/border_r.png'
                        z: 99
                    }
                }

                Rectangle {
                    id: rightCam
                    width: rightCamWidth + lineWidth
                    height: mainHeight
                    color: 'transparent'

                    Image {
                        width: lineWidth
                        height: mainHeight
                        anchors.left: rightCam.left
                        fillMode: Image.Stretch
                        source: 'qrc:/images/border_l.png'
                        z: 99
                    }

                    Column {
                        anchors.right: rightCam.right
                        anchors.rightMargin: 20

                        //                        // CAM ForwardRight
                        //                        Rectangle {
                        //                            width: rightCamWidth
                        //                            height: topCamHeight
                        //                            color: 'transparent'
                        //                            Image {
                        //                                id: forwardRightImg
                        //                                width: leftCamWidth
                        //                                height: bottomCamHeight
                        //                                //fillMode: Image.Stretch
                        //                                fillMode: Image.PreserveAspectCrop
                        //                            }
                        //                            Connections {
                        //                                target: ForwardRightImage
                        //                                onCallQmlRefeshForwardRightImg: {
                        //                                    forwardRightImg.source = ""
                        //                                    forwardRightImg.source = "image://ForwardRightImage"
                        //                                }
                        //                            }
                        //                        }
                        Rectangle {
                            width: rightCamWidth
                            height: mainHeight
                            color: 'transparent'
                            anchors.left: rightCam.left

                            // CAM Right
                            Image {
                                id: rightImg
                                width: rightCamWidth
                                height: mainHeight
                                fillMode: Image.Stretch
                            }
                            Connections {
                                target: RightImage
                                onCallQmlRefeshRightImg: {
                                    rightImg.source = ""
                                    rightImg.source = "image://RightImage"
                                }
                            }
                        }
                    }

                    Image {
                        width: lineWidth
                        height: mainHeight
                        anchors.right: rightCam.right
                        anchors.rightMargin: 20
                        fillMode: Image.Stretch
                        source: 'qrc:/images/border_r.png'
                        z: 99
                    }
                }

                Rectangle {
                    id: radarData
                    width: radarWidth
                    height: mainHeight
                    color: 'transparent'

                    Image {
                        width: lineWidth
                        height: mainHeight
                        anchors.left: radarData.left
                        fillMode: Image.Stretch
                        source: 'qrc:/images/border_l.png'
                    }

                    Image {
                        id: radar_car
                        width: 600
                        height: 800
                        anchors.horizontalCenter: radarData.horizontalCenter
                        anchors.verticalCenter: radarData.verticalCenter
                        fillMode: Image.Stretch
                        source: 'qrc:/images/radar_car.png'
                    }

                    //------------ radarRightF
                    Image {
                        id: radarRightF0
                        width: radarSize
                        height: radarSize
                        anchors.right: radarData.right
                        anchors.rightMargin: radarRightMargin
                        anchors.top: radarData.top
                        anchors.topMargin: radarTopMargin
                        fillMode: Image.PreserveAspectFit
                        source: radarRightFGreenImg
                        visible: appModel.radar1 >= radarGreenRange
                    }
                    Image {
                        id: radarRightF1
                        width: radarSize
                        height: radarSize
                        anchors.right: radarData.right
                        anchors.rightMargin: radarRightMargin
                        anchors.top: radarData.top
                        anchors.topMargin: radarTopMargin
                        fillMode: Image.PreserveAspectFit
                        source: radarRightFYellowImg
                        visible: appModel.radar1 < radarGreenRange
                                 && appModel.radar1 > radarRedRange
                    }
                    Image {
                        id: radarRightF2
                        width: radarSize
                        height: radarSize
                        anchors.right: radarData.right
                        anchors.rightMargin: radarRightMargin
                        anchors.top: radarData.top
                        anchors.topMargin: radarTopMargin
                        fillMode: Image.PreserveAspectFit
                        source: radarRightFRedImg
                        visible: appModel.radar1 <= radarRedRange
                    }

                    //------------ radarRightR
                    Image {
                        id: radarRightR0
                        width: radarSize
                        height: radarSize
                        anchors.bottom: radarData.bottom
                        anchors.bottomMargin: radarTopMargin
                        anchors.right: radarData.right
                        anchors.rightMargin: radarRightMargin
                        fillMode: Image.PreserveAspectFit
                        source: radarRightRGreenImg
                        visible: appModel.radar2 >= radarGreenRange
                    }
                    Image {
                        id: radarRightR1
                        width: radarSize
                        height: radarSize
                        anchors.bottom: radarData.bottom
                        anchors.bottomMargin: radarTopMargin
                        anchors.right: radarData.right
                        anchors.rightMargin: radarRightMargin
                        fillMode: Image.PreserveAspectFit
                        source: radarRightRYellowImg
                        visible: appModel.radar2 < radarGreenRange
                                 && appModel.radar2 > radarRedRange
                    }
                    Image {
                        id: radarRightR2
                        width: radarSize
                        height: radarSize
                        anchors.bottom: radarData.bottom
                        anchors.bottomMargin: radarTopMargin
                        anchors.right: radarData.right
                        anchors.rightMargin: radarRightMargin
                        fillMode: Image.PreserveAspectFit
                        source: radarRightRRedImg
                        visible: appModel.radar2 <= radarRedRange
                    }

                    //------------ radarLeftF
                    Image {
                        id: radarLeftF0
                        width: radarSize
                        height: radarSize
                        anchors.left: radarData.left
                        anchors.leftMargin: radarRightMargin
                        anchors.top: radarData.top
                        anchors.topMargin: radarTopMargin
                        fillMode: Image.PreserveAspectFit
                        source: radarLeftFGreenImg
                        visible: appModel.radar3 >= radarGreenRange
                    }
                    Image {
                        id: radarLeftF1
                        width: radarSize
                        height: radarSize
                        anchors.left: radarData.left
                        anchors.leftMargin: radarRightMargin
                        anchors.top: radarData.top
                        anchors.topMargin: radarTopMargin
                        fillMode: Image.PreserveAspectFit
                        source: radarLeftFYellowImg
                        visible: appModel.radar3 < radarGreenRange
                                 && appModel.radar3 > radarRedRange
                    }
                    Image {
                        id: radarLeftF2
                        width: radarSize
                        height: radarSize
                        anchors.left: radarData.left
                        anchors.leftMargin: radarRightMargin
                        anchors.top: radarData.top
                        anchors.topMargin: radarTopMargin
                        fillMode: Image.PreserveAspectFit
                        source: radarLeftFRedImg
                        visible: appModel.radar3 <= radarRedRange
                    }

                    //------------ radarLeftR
                    Image {
                        id: radarLeftR0
                        width: radarSize
                        height: radarSize
                        anchors.bottom: radarData.bottom
                        anchors.bottomMargin: radarTopMargin
                        anchors.left: radarData.left
                        anchors.leftMargin: radarRightMargin
                        fillMode: Image.PreserveAspectFit
                        source: radarLeftRGreenImg
                        visible: appModel.radar4 >= radarGreenRange
                    }

                    Image {
                        id: radarLeftR1
                        width: radarSize
                        height: radarSize
                        anchors.bottom: radarData.bottom
                        anchors.bottomMargin: radarTopMargin
                        anchors.left: radarData.left
                        anchors.leftMargin: radarRightMargin
                        fillMode: Image.PreserveAspectFit
                        source: radarLeftRYellowImg
                        visible: appModel.radar4 < radarGreenRange
                                 && appModel.radar4 > radarRedRange
                    }

                    Image {
                        id: radarLeftR2
                        width: radarSize
                        height: radarSize
                        anchors.bottom: radarData.bottom
                        anchors.bottomMargin: radarTopMargin
                        anchors.left: radarData.left
                        anchors.leftMargin: radarRightMargin
                        fillMode: Image.PreserveAspectFit
                        source: radarLeftRRedImg
                        visible: appModel.radar4 <= radarRedRange
                    }

                    //------------ radarBottomL
                    Image {
                        id: radarBottomL0
                        width: radarSize
                        height: radarSize
                        anchors.bottom: radarData.bottom
                        anchors.bottomMargin: radarBottomMargin
                        anchors.left: radarData.left
                        anchors.leftMargin: radarleftMargin
                        fillMode: Image.PreserveAspectFit
                        source: radarBottomGreenImg
                        visible: appModel.radar5 >= radarGreenRange
                    }

                    Image {
                        id: radarBottomL1
                        width: radarSize
                        height: radarSize
                        anchors.bottom: radarData.bottom
                        anchors.bottomMargin: radarBottomMargin
                        anchors.left: radarData.left
                        anchors.leftMargin: radarleftMargin
                        fillMode: Image.PreserveAspectFit
                        source: radarBottomYellowImg
                        visible: appModel.radar5 < radarGreenRange
                                 && appModel.radar5 > radarRedRange
                    }

                    Image {
                        id: radarBottomL2
                        width: radarSize
                        height: radarSize
                        anchors.bottom: radarData.bottom
                        anchors.bottomMargin: radarBottomMargin
                        anchors.left: radarData.left
                        anchors.leftMargin: radarleftMargin
                        fillMode: Image.PreserveAspectFit
                        source: radarBottomRedImg
                        visible: appModel.radar5 <= radarRedRange
                    }

                    //------------ radarBottomR
                    Image {
                        id: radarBottomR0
                        width: radarSize
                        height: radarSize
                        anchors.bottom: radarData.bottom
                        anchors.bottomMargin: radarBottomMargin
                        anchors.right: radarData.right
                        anchors.rightMargin: radarleftMargin
                        fillMode: Image.PreserveAspectFit
                        source: radarBottomGreenImg
                        visible: appModel.radar6 >= radarGreenRange
                    }

                    Image {
                        id: radarBottomR1
                        width: radarSize
                        height: radarSize
                        anchors.bottom: radarData.bottom
                        anchors.bottomMargin: radarBottomMargin
                        anchors.right: radarData.right
                        anchors.rightMargin: radarleftMargin
                        fillMode: Image.PreserveAspectFit
                        source: radarBottomYellowImg
                        visible: appModel.radar6 < radarGreenRange
                                 && appModel.radar6 > radarRedRange
                    }

                    Image {
                        id: radarBottomR2
                        width: radarSize
                        height: radarSize
                        anchors.bottom: radarData.bottom
                        anchors.bottomMargin: radarBottomMargin
                        anchors.right: radarData.right
                        anchors.rightMargin: radarleftMargin
                        fillMode: Image.PreserveAspectFit
                        source: radarBottomRedImg
                        visible: appModel.radar6 <= radarRedRange
                    }

                    //                    Image {
                    //                        id: radarBottomL
                    //                        width: radarSize
                    //                        height: radarSize
                    //                        anchors.bottom: radarData.bottom
                    //                        anchors.bottomMargin: 240
                    //                        anchors.left: radarData.left
                    //                        anchors.leftMargin: 200
                    //                        fillMode: Image.Stretch
                    //                    }
                    //                    Connections {
                    //                        target: RadarImage
                    //                        onCallQmlRefeshRadar2: {
                    //                            radarBottomL.source = "image://RadarImage/" + Math.random()
                    //                        }
                    //                    }

                    //                    Image {
                    //                        id: radarBottomR
                    //                        width: radarSize
                    //                        height: radarSize
                    //                        anchors.bottom: radarData.bottom
                    //                        anchors.bottomMargin: 240
                    //                        anchors.right: radarData.right
                    //                        anchors.rightMargin: 200
                    //                        fillMode: Image.Stretch
                    //                    }
                    //                    Connections {
                    //                        target: RadarImage
                    //                        onCallQmlRefeshRadar1: {
                    //                            radarBottomR.source = "image://RadarImage/" + Math.random()
                    //                        }
                    //                    }
                    Row {
                        height: parent.height
                        anchors.horizontalCenter: parent.horizontalCenter
                        anchors.top: radarData.top
                        anchors.topMargin: 20

                        Text {
                            text: "雷达1："
                            color: mainTextColor
                            anchors.left: radarData.left
                            anchors.leftMargin: 20
                            font.family: alibaba.name
                            font.pointSize: radarTxtSize
                        }

                        Text {
                            id: radar1
                            text: (appModel.radar1 / 1000).toFixed(2)
                            color: mainTextColor
                            anchors.left: radarData.left
                            anchors.leftMargin: 20
                            font.family: alibaba.name
                            font.pointSize: radarTxtSize
                        }

                        spacing: 30

                        Text {
                            text: "雷达2："
                            color: mainTextColor
                            anchors.left: radarData.left
                            anchors.leftMargin: 20
                            font.family: alibaba.name
                            font.pointSize: radarTxtSize
                        }

                        Text {
                            id: radar2
                            text: (appModel.radar2 / 1000).toFixed(2)
                            color: mainTextColor
                            anchors.left: radarData.left
                            anchors.leftMargin: 60
                            font.family: alibaba.name
                            font.pointSize: radarTxtSize
                        }
                    }

                    Row {
                        height: parent.height
                        anchors.top: radarData.top
                        anchors.topMargin: 80
                        anchors.horizontalCenter: parent.horizontalCenter
                        Text {
                            text: "雷达3："
                            color: mainTextColor
                            anchors.left: radarData.left
                            anchors.leftMargin: 20
                            font.family: alibaba.name
                            font.pointSize: radarTxtSize
                        }
                        Text {
                            id: radar3
                            text: (appModel.radar3 / 1000).toFixed(2)
                            color: mainTextColor
                            anchors.left: radarData.left
                            anchors.leftMargin: 100
                            font.family: alibaba.name
                            font.pointSize: radarTxtSize
                        }

                        spacing: 30

                        Text {
                            text: "雷达4："
                            color: mainTextColor
                            anchors.left: radarData.left
                            anchors.leftMargin: 20
                            font.family: alibaba.name
                            font.pointSize: radarTxtSize
                        }

                        Text {
                            id: radar4
                            text: (appModel.radar4 / 1000).toFixed(2)
                            color: mainTextColor
                            anchors.left: radarData.left
                            anchors.leftMargin: 140
                            font.family: alibaba.name
                            font.pointSize: radarTxtSize
                        }
                    }
                    Row {
                        height: parent.height
                        anchors.horizontalCenter: parent.horizontalCenter
                        anchors.top: radarData.top
                        anchors.topMargin: 140

                        Text {
                            text: "雷达5："
                            color: mainTextColor
                            anchors.left: radarData.left
                            anchors.leftMargin: 20
                            font.family: alibaba.name
                            font.pointSize: radarTxtSize
                        }

                        Text {
                            id: radar5
                            text: (appModel.radar5 / 1000).toFixed(2)
                            color: mainTextColor
                            anchors.left: radarData.left
                            anchors.leftMargin: 180
                            font.family: alibaba.name
                            font.pointSize: radarTxtSize
                        }

                        spacing: 30

                        Text {
                            text: "雷达6："
                            color: mainTextColor
                            anchors.left: radarData.left
                            anchors.leftMargin: 20
                            font.family: alibaba.name
                            font.pointSize: radarTxtSize
                        }

                        Text {
                            id: radar6
                            text: (appModel.radar6 / 1000).toFixed(2)
                            color: mainTextColor
                            anchors.left: radarData.left
                            anchors.leftMargin: 220
                            font.family: alibaba.name
                            font.pointSize: radarTxtSize
                        }
                    }

                    Rectangle {
                        id: warning
                        width: radarData.width - 60
                        height: 50
                        color: popupBgColorErr
                        anchors.bottom: radarData.bottom
                        anchors.bottomMargin: 30
                        anchors.horizontalCenter: radarData.horizontalCenter
                        Text {
                            id: warningtxt
                            text: qsTr("注意保持车距")
                            color: '#FFFFFF'
                            font.family: alibaba.name
                            font.pointSize: 24
                            anchors.horizontalCenter: warning.horizontalCenter
                        }
                        visible: appModel.radar1 < radarRedRange
                                 || appModel.radar2 < radarRedRange
                                 || appModel.radar3 < radarRedRange
                                 || appModel.radar4 < radarRedRange
                                 || appModel.radar5 < radarRedRange
                                 || appModel.radar6 < radarRedRange

                        //                        AudioEngine {
                        //                            id: audioengine

                        //                            AudioSample {
                        //                                name: "beep1"
                        //                                source: "qrc:/sound/beep-1.wav"
                        //                            }

                        //                            Sound {
                        //                                name: "explosion"
                        //                                playType: Sound.Random
                        //                                PlayVariation {
                        //                                    sample: "explosion01"
                        //                                    minPitch: 0.8
                        //                                    maxPitch: 1.1
                        //                                }
                        //                                PlayVariation {
                        //                                    sample: "explosion02"
                        //                                    minGain: 1.1
                        //                                    maxGain: 1.5
                        //                                }
                        //                            }
                        //                        }
                        //                        SoundEffect {
                        //                            id: clickSound
                        //                            source: "clickSound.wav"
                        //                            loops: SoundEffect.Infinite
                        //                        }
                        //                        MouseArea {
                        //                            id: playArea
                        //                            anchors.fill: parent
                        //                            onPressed: {
                        //                                clickSound.play()
                        //                            }
                        //                        }

                        //                        Audio {
                        //                            id: audioPlayer
                        //                            source: "qrc:/sound/beep-1.wav"
                        //                        }

                        //                        MouseArea {
                        //                            id: playArea
                        //                            anchors.fill: parent
                        //                            onPressed: {
                        //                                audioPlayer.play()
                        //                            }
                        //                        }
                    }

                    //                        Rectangle {
                    //                            width: radarData.width
                    //                            height: rightPanel.height
                    //                            color: 'transparent'

                    //                            Rectangle {
                    //                                width: radarData.width - 300
                    //                                height: radarData.height
                    //                                color: 'lightblue'

                    //                                Image {
                    //                                    width: 800
                    //                                    height: 1200
                    //                                    anchors.left: radarData.left
                    //                                    fillMode: Image.Stretch
                    //                                    source: 'qrc:/images/radar_car.png'
                    //                                }
                    //                            }
                    //                        }
                    Image {
                        width: lineWidth
                        height: mainHeight
                        anchors.right: radarData.right
                        fillMode: Image.Stretch
                        source: 'qrc:/images/border_r.png'
                    }
                }
            }
        }
    }
}
