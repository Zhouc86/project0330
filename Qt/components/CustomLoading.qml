import QtQuick 2.12
import QtQuick.Controls 2.12

Item {
    id: loadingControl

    //item圆圈个数
    property int itemCount: 10
    //item圆圈直径大小
    property int itemSize: 10
    //item变大范围
    property int itemExpand: 10
    //item圆圈颜色
    property color itemColor: mainAppColor
    //当前item，配合动画
    property int itemIndex: 0
    //轮转一次的时长
    property int duration: 1500
    //
    property bool running: visible

    implicitHeight: 100
    implicitWidth: 100

    NumberAnimation{
        target: loadingControl
        property: "itemIndex"
        from: 0
        to: loadingControl.itemCount-1
        loops: Animation.Infinite
        duration: loadingControl.duration
        running: loadingControl.running
    }

    //加一层item.margin来容纳item大小变化
    Item{
        id: content
        anchors.fill: parent
        anchors.margins: loadingControl.itemExpand/2+1

        Repeater{
            id: repeater
            model: loadingControl.itemCount
            Rectangle{
                id: item
                height: loadingControl.itemSize
                width: height
                x: content.width/2 - width/2
                y: content.height/2 - height/2
                radius: height/2
                color: loadingControl.itemColor

                //环绕在中心
                transform: [
                    Translate {
                        y: content.height/2-height/2
                    },
                    Rotation {
                        angle: index / repeater.count * 360
                        origin.x: width/2
                        origin.y: height/2
                    }
                ]

                //轮转到当前item时就切换状态
                state: loadingControl.itemIndex===index?"current":"normal"
                states: [
                    State {
                        name: "current"
                        PropertyChanges {
                            target: item
                            opacity: 1
                            height: loadingControl.itemSize+loadingControl.itemExpand
                        }
                    },
                    State {
                        name: "normal"
                        PropertyChanges {
                            target: item
                            opacity: 0.1
                            height: loadingControl.itemSize
                        }
                    }
                ]

                //大小和透明度的状态过渡
                transitions: [
                    Transition {
                        from: "current"
                        to: "normal"
                        NumberAnimation{
                            properties: "opacity,height"
                            duration: loadingControl.duration
                        }
                    },
                    Transition {
                        from: "normal"
                        to: "current"
                        NumberAnimation{
                            properties: "opacity,height"
                            duration: 0
                        }
                    }
                ]
            }
        }
    }
}
