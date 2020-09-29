import QtQuick 2.9
import QtQuick.Layouts 1.3
import QtQuick.Controls 2.2
import SearchStatusType 1.0
import SearchManager 1.0
import SearchManagerStatus 1.0

Rectangle {
    id: root
    border { width: 3; color: 'gray' }
    color: 'lightgray'
    radius: defMargin
    implicitHeight: searchedUrlsLv.implicitHeight + 30

    property int defMargin: -1
    property var searchManager: null

    Item {
        id: margedRoot
        anchors.fill: root
        anchors.margins: 2*defMargin

        ListView {
            id: searchedUrlsLv
            anchors.left: margedRoot.left
            anchors.right: margedRoot.right
            anchors.top: margedRoot.top
            anchors.bottom: progress.top
            anchors.bottomMargin: defMargin

            spacing: 0
            model: searchManager.serchedUrlsModel
            clip: true

            delegate: Rectangle {
                anchors.left: parent.left
                anchors.right: parent.right
                anchors.rightMargin: scrollBar.visible ? scrollBar.width + defMargin : 0
                height: searchedUrlsRl.implicitHeight
                //border { width: 2; color: 'red' }
                color: 'transparent'

                RowLayout {
                    id: searchedUrlsRl
                    anchors.fill: parent

                    Label {
                        id: indexLb
                        Layout.minimumWidth: 35 // max length for '999999'
                        Layout.maximumWidth: 35
                        Layout.alignment: Qt.AlignLeft
                        verticalAlignment: Text.AlignVCenter
                        horizontalAlignment: Text.AlignLeft
                        text: index + 1
                        font: linkTxt.font
                    }

                    TextField {
                        id: linkTxt
                        Layout.maximumWidth: searchedUrlsRl.width
                                             - indexLb.width
                                             - statusUrlLoadingTxtWraper.width
                                             - 3*defMargin
                        Layout.alignment: Qt.AlignLeft
                        verticalAlignment: Text.AlignVCenter
                        horizontalAlignment: Text.AlignLeft

                        text: qsTr(model.url)
                        selectByMouse: true

                        background: Rectangle {
                            color: root.color
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

                    Item {
                        Layout.fillWidth: true
                    }

                    Item {
                        id: statusUrlLoadingTxtWraper
                        Layout.maximumWidth: 200
                        Layout.minimumWidth: 200
                        Layout.alignment: Qt.AlignRight

                        Text {
                            id: statusUrlLoadingTxt
                            anchors.fill: statusUrlLoadingTxtWraper
                            verticalAlignment: Text.AlignVCenter
                            horizontalAlignment: Text.AlignLeft

                            text: qsTr('Status: '
                                       + toStringTextSearcherStatus(model.status)
                                       + (model.status === SearchStatusType.Error ? ': ' + model.error : ''))

                            font {
                                pointSize: 8
                            }
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

        ProgressBar {
            id: progress
            anchors.left: margedRoot.left
            anchors.right: margedRoot.right
            anchors.bottom: margedRoot.bottom
            padding: 2
            value: searchManager.status == SearchManagerStatus.Stopped
                   ? 0
                   : searchedUrlsLv.model.rowCount

            from: 0
            to: searchManager.status == SearchManagerStatus.Stopped
                ? searchManager.maxUrlsNum
                : progress.to

            background: Rectangle {
                implicitWidth: 200
                implicitHeight: 6
                color: "transparent"
                radius: 3
            }

            contentItem: Item {
                implicitWidth: 200
                implicitHeight: 4

                Rectangle {
                    width: progress.visualPosition * parent.width
                    height: parent.height
                    radius: 2
                    color: "#17a81a"
                }
            }
        }
    }

    function toStringTextSearcherStatus(status) {
        switch(status){
            case SearchStatusType.NotSet: return "NotSet"
            case SearchStatusType.Found: return "Found"
            case SearchStatusType.NotFound: return "NotFound"
            case SearchStatusType.Downloading: return "Downloading"
            case SearchStatusType.Error: return "Error"
        }
    }
}
