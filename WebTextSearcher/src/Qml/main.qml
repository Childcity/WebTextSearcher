import QtQuick 2.11
import QtQuick.Window 2.11
import QtQuick.Controls 2.2
import QtQuick.Layouts 1.3

ApplicationWindow {
    title: qsTr("Web Text Searcher")
    id: root
    visible: true
    width: 640
    height: 480

    property int defMargin: 5

    header: ToolBar {
        RowLayout {
            anchors.left: root.left
            ToolButton {
                text: qsTr("Start")
            }
            ToolButton {
                text: qsTr("Stop")
            }
        }
    }

//    ColumnLayout {
//        anchors.fill: parent
//        anchors.margins: defMargin
//        spacing: defMargin
//        //Rectangle {
//        //    Layout.alignment: Qt.AlignCenter
//        //    border.color: "black"
//        //    border.width: 2

//            RowLayout {
//                //Layout.alignment: Qt.AlignTop
//                Layout.fillWidth: true
//                Label{
//                    text: "sasas"
//                }
//                Label{
//                    text: "sasas"
//                }
//            }
//            RowLayout {
//                //Layout.alignment: Qt.AlignTop
//                Layout.fillWidth: true
//                Label{
//                    text: "sasas"
//                }
//                Label{
//                    text: "sasas"
//                }
//            }


//            Rectangle {
//                id: space
//                Layout.fillHeight: true
//                Layout.fillWidth: true
//                border.width: 1
//                border.color: 'red'
//                color: 'transparent'
//                Text {
//                    text: qsTr("space")
//                    anchors.horizontalCenter: parent.horizontalCenter
//                    anchors.verticalCenter: parent.verticalCenter
//                    verticalAlignment: Text.AlignVCenter
//                    horizontalAlignment: Text.AlignHCenter
//                }
//            }
//    }

    property int firstColWidth: 130

    GridLayout {
        id: gridLayout
        anchors.fill: parent
        anchors.margins: defMargin
        enabled: true
        columnSpacing: defMargin
        rowSpacing: defMargin
        rows: 5
        columns: 4

        Label {
            Layout.preferredWidth: firstColWidth
            Layout.row: 1
            Layout.column: 1
            text: "Start Url:"
        }

        TextField {
            id: startUrl
            Layout.row: 1
            Layout.column: 2
            Layout.columnSpan: 3
            Layout.fillWidth: true
            placeholderText: qsTr("http://www.google.com/search?q=qthread")
        }

        Label {
            Layout.row: 2
            Layout.column: 1
            text: "Text to serch:"
        }

        TextField {
            id: searchedText
            Layout.row: 2
            Layout.column: 2
            Layout.columnSpan: 3
            Layout.fillWidth: true
            placeholderText: qsTr("qthread")
        }

        Label {
            Layout.row: 3
            Layout.column: 1
            text: "Max threads:"
        }

        SpinBox {
            id: maxThreadsNum
            Layout.row: 3
            Layout.column: 2
            value: 100
            from: 1
            to: 1000
        }

        Label {
            Layout.row: 4
            Layout.column: 1
            text: "Max urls:"
        }

        SpinBox {
            id: maxUrlsNum
            Layout.row: 4
            Layout.column: 2
            value: 500
            from: 1
            to: 100000
        }


        Label {
            Layout.row: 4
            Layout.column: 3
            Layout.alignment: Qt.AlignRight
            text: "Timeout:"
        }

        SpinBox {
            id: maxUrlTimeout
            Layout.row: 4
            Layout.column: 4
            Layout.alignment: Qt.AlignLeft
            value: 500
            from: 1
            to: 100000
        }

        Rectangle {
            id: space
            Layout.row: gridLayout.rows
            Layout.column: 1
            Layout.columnSpan: gridLayout.columns
            Layout.fillHeight: true
            Layout.fillWidth: true
            border.width: 1
            border.color: 'red'
            color: 'transparent'
            Text {
                text: qsTr("space")
                anchors.horizontalCenter: parent.horizontalCenter
                anchors.verticalCenter: parent.verticalCenter
                verticalAlignment: Text.AlignVCenter
                horizontalAlignment: Text.AlignHCenter
            }
        }
    }
}
