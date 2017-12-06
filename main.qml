import QtQuick 2.7
import QtQuick.Window 2.2
import QtQuick.Dialogs 1.2
import demo.kmlDialog 1.0
import QtQuick.Controls 2.0
import QtQuick.Controls 1.4

Window {
    visible: true
    width: 640
    height: 480
    title: qsTr("Hello World")

    KMLDialogQml {  id:kmlDialog }
    Button
    {
        id: button
        x: 29
        y: 27
        width: 150
        height: 30
        text: qsTr("导入KML文件")
        onClicked: fileDialog.open();
    }

    Button
    {
        id: button1
        anchors.left: button.right
        anchors.leftMargin: 30
        anchors.top: button.top
        width: 150
        height: 30
        text: qsTr("导出KML文件")
        //参数分别为路径 文件名 文件描述 航线描述  航点类型  航点经纬高信息
        onClicked: kmlDialog.exportKML("/Users/apple/Desktop/"+"qtTest.kml","qtTest","doc 描述","placemark 描述","LineString","\n\t\t\t\t114.285633370142,30.59428022180686,0 114.3186977097045,30.58113997090438,0 114.3004506225153,30.56888490893557,0 114.2856206721545,30.58320206840937,0 114.3235811215027,30.59207470271208,0 114.3282941885463,30.5793147654806,0 114.333595915555,30.59716126111831,0 \n\t\t\t")
    }

     Button
     {
          id:toKmlBtn
          anchors.left: button.left
          anchors.top: button.bottom
          anchors.topMargin: 50
          width: 150
          height: 30
          text:"json转Kml"
          onClicked:jsonToKmlD.open();
     }
    Label
    {
        anchors.left: button1.right
        anchors.leftMargin: 20
        y:button.y
        width: 200
        anchors.horizontalCenter: button.horizontalCenter
        height: button.height
        text:fileDialog.fileUrl?fileDialog.fileUrl:""
    }

    Text
    {
        id:showText
        width:  400
        height:  300
        anchors.top: button.bottom
        anchors.topMargin:  20
        anchors.left: parent.left
        anchors.leftMargin:  20
        color: "red"
    }

    FileDialog {
        id: fileDialog
        title: "请选择目标文件"
        folder: shortcuts.desktop
       nameFilters: ["*.kml"]  //BUG  ：此处过滤有点问题 。。。
        onAccepted: {
            kmlDialog.parseKML(fileDialog.fileUrl);
            showText.text = kmlDialog.longitudeList[0] +" " +kmlDialog.latitudeList[0]+" " +kmlDialog.altitudeList[0]
        }
        onRejected: {
         fileDialog.close();
        }
    }
    FileDialog
    {
        id:jsonToKmlD
        title:"选取json"
        folder: shortcuts.desktop
        nameFilters: ["*plan"]
        onAccepted:
        {
            kmlDialog.jsonToKml(jsonToKmlD.fileUrl,"","hello");
        }
        onRejected:
        {
            kmlDialog.close();
        }
    }
}
