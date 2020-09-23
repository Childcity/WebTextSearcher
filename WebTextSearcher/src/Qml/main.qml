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
        leftPadding: defMargin
        bottomPadding: defMargin

        background: Rectangle {
            color: "#eeeeee"

            Rectangle {
                anchors.bottom: parent.bottom
                width: parent.width
                radius: defMargin
                height: 3
                color: "gray"
            }
        }

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

    ColumnLayout {
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
            id: backgroundForUrls
            Layout.fillWidth: true
            Layout.fillHeight: true
            border { width: 3; color: 'gray' }
            color: 'lightgray'
            radius: defMargin

            ListView {
                id: scannedUrlsLv
                anchors.fill: parent
                anchors.margins: 2*defMargin
                //spacing: 1
                model: 10
                clip: true

                delegate: Rectangle {
                    anchors.left: parent.left
                    anchors.right: parent.right
                    height: scannedUrlsRl.implicitHeight
                    //border { width: 2; color: 'red' }
                    color: 'transparent'

                    RowLayout {
                        id: scannedUrlsRl
                        anchors.fill: parent
                        clip: true

                        TextField {
                            id: linkTxt
                            text: qsTr("http://www.google.com/search?q=qthread")
                            Layout.maximumWidth: scannedUrlsRl.width - statusUrlLoadingTxt.width
                            //Layout.minimumWidth: scannedUrlsRl.width/2
                            horizontalAlignment: TextInput.AlignLeft

                            background: Rectangle {
                                color: backgroundForUrls.color
                                radius: defMargin
                                border.color: linkTxt.focus ? 'gray' : "transparent"
                            }

                            palette { text: "blue" }

                            font {
                                italic: true
                                pointSize: 8
                            }

                            Component.onCompleted: linkTxt.cursorPosition = 0
                        }

                        Text {
                            id: statusUrlLoadingTxt
                            text: qsTr("Status: Loading....")
                            Layout.maximumWidth: 150
                            Layout.minimumWidth: 150
                            Layout.alignment: Qt.AlignRight
                            verticalAlignment: Text.AlignVCenter
                            horizontalAlignment: Text.AlignRight

                            font {
                                pointSize: 8
                            }
                        }
                    }
                }

                ScrollBar.vertical: ScrollBar {
                    id: scrollBar
                    policy: scannedUrlsLv.contentHeight > scannedUrlsLv.height ? ScrollBar.AlwaysOn : ScrollBar.AlwaysOff
                    minimumSize: 0.05
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
