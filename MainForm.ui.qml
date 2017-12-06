import QtQuick 2.7
import QtQuick.Controls 2.0
import QtQuick.Controls 1.4

Rectangle {

    width: 360
    height: 360
    property alias button: button

    Button {
        id: button
        x: 29
        y: 27
        width: 115
        height: 29
        text: qsTr("打开文件")
    }

    Connections {
        target: button
        onClicked: print("clicked")
    }

    TableView {
        anchors.centerIn: parent
        width: 600
        height: 400
        TableViewColumn {
          //  role: "title"
            title: "经度"
            width: 200
        }
        TableViewColumn {
           // role: "author"
            title: "纬度"
            width: 200
        }
        TableViewColumn {
          //  role: "author"
            title: "高度"
            width: 200
        }
    }
}
