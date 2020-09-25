import QtQuick 2.11
import QtQuick.Window 2.11
import QtQuick.Controls 2.2
import QtQuick.Layouts 1.3
import SerchedUrlsModel 1.0
import TextSearcherStatus 1.0
import SearchManager 1.0

ApplicationWindow {
    title: qsTr('Web Text Searcher')
    id: root
    visible: true
    width: 640
    height: 480
    minimumWidth: gridLayout.implicitWidth + 2*defMargin
    minimumHeight: toolBar.implicitHeight
                   + gridLayout.implicitHeight
                   + backgroundForUrls.implicitHeight
                   + 2*defMargin

    property int defMargin: 5
    property int controlHeight: 30

    header: ToolBar {
        id: toolBar
        leftPadding: defMargin
        bottomPadding: defMargin

        background: Rectangle {
            color: '#eeeeee'

            Rectangle {
                anchors.bottom: parent.bottom
                width: parent.width
                radius: defMargin
                height: 3
                color: 'gray'
            }
        }

        RowLayout {
            anchors.left: root.left
            ToolButton {
                text: qsTr('Start')
                onClicked: {
                    if(startUrlTf.text.length < 8 /*http://a*/){
                        startUrlTfAnim.running = true
                        return
                    }
                    searchManager.slotStartSearcher()
                }
            }
            ToolButton {
                text: qsTr('Stop')
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
                text: qsTr('Start Url:')
            }

            TextField {
                id: startUrlTf
                background: Rectangle {
                    border.color: startUrlTf.focus ? '#0066ff' : '#bdbdbd'
                    border.width: startUrlTf.focus ? 2 : 1
                }
                SequentialAnimation {
                    id: startUrlTfAnim
                    loops: 2
                    PropertyAnimation {
                        target: startUrlTf.background.border
                        property: "color"
                        to: "red"
                        duration: 300

                    }
                    PropertyAnimation {
                        target: startUrlTf.background.border
                        property: "color"
                        to: '#bdbdbd'
                        duration: 300

                    }
                }
                Layout.row: 1
                Layout.column: 2
                Layout.columnSpan: 3
                Layout.fillWidth: true
                Layout.preferredHeight: controlHeight
                text: 'https://doc.qt.io/archives/qq'
                placeholderText: qsTr('http://www.google.com/search?q=qthread')
                validator: RegExpValidator { regExp: /https*:\/\/[-a-zA-Z0-9@:%._\+~#=]{1,256}\.[a-zA-Z0-9()]{1,6}\b([-a-zA-Z0-9()@:%_\+.~#?&\/\/=]*)/ }
            }

            Label {
                Layout.row: 2
                Layout.column: 1
                text: qsTr('Text to serch:')
            }

            TextField {
                id: searchedTextTf
                Layout.row: 2
                Layout.column: 2
                Layout.columnSpan: 3
                Layout.fillWidth: true
                Layout.preferredHeight: controlHeight
                placeholderText: qsTr('qthread')
            }

            Label {
                Layout.row: 3
                Layout.column: 1
                text: qsTr('Max threads:')
            }

            SpinBox {
                id: maxThreadsNumSb
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
                text: qsTr('Max urls:')
            }

            SpinBox {
                id: maxUrlsNumSb
                Layout.preferredHeight: controlHeight
                Layout.row: 4
                Layout.column: 2
                editable: true
                value: 20
                from: 1
                to: 100000
            }

            Row {
                Layout.row: 4
                Layout.column: 4
                Layout.alignment: Qt.AlignRight

                Label {
                    height: parent.height
                    text: qsTr('Timeout:')
                    verticalAlignment: Text.AlignVCenter
                }

                SpinBox {
                    id: maxUrlTimeoutSb
                    height: controlHeight
                    editable: true
                    value: 3000
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
                id: searchedUrlsLv
                anchors.fill: parent
                anchors.margins: 2*defMargin
                //spacing: 1
                model: searchManager.serchedUrlsModel
                clip: true

                delegate: Rectangle {
                    anchors.left: parent.left
                    anchors.right: parent.right
                    height: searchedUrlsRl.implicitHeight
                    anchors.rightMargin: scrollBar.visible ? scrollBar.width + defMargin : 0
                    //border { width: 2; color: 'red' }
                    color: 'transparent'

                    RowLayout {
                        id: searchedUrlsRl
                        anchors.fill: parent

                        TextField {
                            id: linkTxt
                            text: qsTr(model.url)
                            Layout.maximumWidth: searchedUrlsRl.width - statusUrlLoadingTxt.width - defMargin
                            //Layout.minimumWidth: searchedUrlsRl.width/2
                            horizontalAlignment: TextInput.AlignLeft

                            background: Rectangle {
                                color: backgroundForUrls.color
                                radius: defMargin
                                border.color: linkTxt.focus ? 'gray' : 'transparent'
                            }

                            palette { text: 'blue' }

                            font {
                                italic: true
                                pointSize: 8
                            }

                            Component.onCompleted: linkTxt.cursorPosition = 0
                        }

                        Text {
                            id: statusUrlLoadingTxt
                            text: qsTr('Status: '
                                       + toTextSearcherStatusString(model.status)
                                       + (model.status === TextSearcherStatus.Error ? ': ' + model.error : ''))
                            Layout.maximumWidth: 200
                            Layout.minimumWidth: 200
                            Layout.alignment: Qt.AlignRight
                            verticalAlignment: Text.AlignVCenter
                            horizontalAlignment: Text.AlignLeft

                            font {
                                pointSize: 8
                            }
                        }
                    }
                }

                ScrollBar.vertical: ScrollBar {
                    id: scrollBar
                    policy: searchedUrlsLv.contentHeight > searchedUrlsLv.height ? ScrollBar.AlwaysOn : ScrollBar.AlwaysOff
                    minimumSize: 0.05
                    contentItem: Rectangle {
                        color: scrollBar.pressed ? Qt.lighter('#7b7f7b') : '#7b7f7b'
                        implicitWidth: 10
                        radius: width / 2
                    }
                }
            }
        }
    }

    function toTextSearcherStatusString(status) {
        switch(status){
            case TextSearcherStatus.NotSet: return "NotSet"
            case TextSearcherStatus.Found: return "Found"
            case TextSearcherStatus.NotFound: return "NotFound"
            case TextSearcherStatus.Downloading: return "Downloading"
            case TextSearcherStatus.Error: return "Error"
        }
    }

    SearchManager {
        id: searchManager
        startUrl: startUrlTf.text
        serchedText: searchedTextTf.text
        maxThreadsNum: maxThreadsNumSb.value
        maxUrlsNum: maxUrlsNumSb.value
        urlDownloadingTimeout: maxUrlTimeoutSb.value
        serchedUrlsModel: SerchedUrlsModel {
        }
    }
}

