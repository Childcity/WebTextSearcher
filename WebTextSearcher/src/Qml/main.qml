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
    minimumWidth: gridLayout.implicitWidth + 2*defMargin

    property int defMargin: 5
    property int controlHeight: 30

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

    ColumnLayout{
        anchors.fill: parent
        anchors.margins: defMargin

        GridLayout {
            id: gridLayout
            Layout.fillWidth: true
            enabled: true
            columnSpacing: defMargin
            rowSpacing: defMargin

            rows: 5
            columns: 4

            Label {
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
                Layout.preferredHeight: controlHeight
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
                Layout.preferredHeight: controlHeight
                placeholderText: qsTr("qthread")
            }

            Label {
                Layout.row: 3
                Layout.column: 1
                text: "Max threads:"
            }

            SpinBox {
                id: maxThreadsNum
                Layout.preferredHeight: controlHeight
                Layout.row: 3
                Layout.column: 2
                editable: true
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
                Layout.preferredHeight: controlHeight
                Layout.row: 4
                Layout.column: 2
                editable: true
                value: 500
                from: 1
                to: 100000
            }

            Row {
                Layout.row: 4
                Layout.column: 4
                Layout.alignment: Qt.AlignRight

                Label {
                    height: parent.height
                    text: "Timeout:"
                    verticalAlignment: Text.AlignVCenter
                }

                SpinBox {
                    id: maxUrlTimeout
                    height: controlHeight
                    editable: true
                    value: 500
                    from: 1
                    to: 100000
                }
            }
        }

        Rectangle {
            Layout.fillWidth: true
            Layout.fillHeight: true
            border { width: 3; color: 'gray' }
            color: 'lightgray'
            radius: defMargin

            ListView {
                anchors.fill: parent
                anchors.margins: 2*defMargin
                spacing: defMargin
                model: 10

                delegate: Rectangle {
                    anchors.left: parent.left
                    anchors.right: parent.right
                    height: scannedUrlsRl.implicitHeight + defMargin
                    border { width: 2; color: 'red' }
                    color: 'transparent'

                    RowLayout {
                        id: scannedUrlsRl
                        anchors.fill: parent

                        TextEdit {
                            text: qsTr("http://www.google.com/search?q=qthread")
                            Layout.maximumWidth: scannedUrlsRl.width/2
                            Layout.minimumWidth: scannedUrlsRl.width/2
                            clip: true
                            wrapMode: TextEdit.NoWrap
                            Layout.alignment: Qt.AlignLeft
                            verticalAlignment: Text.AlignVCenter
                            horizontalAlignment: Text.AlignLeft
                        }

                        Text {
                            text: qsTr("Status: Loading....")
                            Layout.minimumWidth: scannedUrlsRl.width/2
                            //anchors.horizontalCenter: parent.horizontalCenter
                            //anchors.verticalCenter: parent.verticalCenter
                            verticalAlignment: Text.AlignVCenter
                            horizontalAlignment: Text.AlignLeft
                        }
                    }
                }

                ScrollBar.vertical: ScrollBar {
                    id: scrollBar
                    policy: ScrollBar.AsNeeded
                    contentItem: Rectangle {
                        color: scrollBar.pressed ? Qt.lighter("#7b7f7b") : "#7b7f7b"
                        implicitWidth: 10
                        radius: width / 2
                    }
                }
            }
        }

    }
}
