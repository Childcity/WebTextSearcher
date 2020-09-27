import QtQuick 2.11
import QtQuick.Window 2.11
import QtQuick.Controls 2.2
import QtQuick.Layouts 1.3
import SerchedUrlsModel 1.0
import SearchStatusType 1.0
import SearchManager 1.0

ApplicationWindow {
    title: qsTr('Web Text Searcher')
    id: root
    visible: true
    width: 640
    height: 480
    minimumWidth: searcherParamsForm.implicitWidth + 2*defaultMargin
    minimumHeight: toolBar.implicitHeight
                   + searcherParamsForm.implicitHeight
                   + backgroundForUrls.implicitHeight
                   + 2*defaultMargin

    readonly property int defaultMargin: 5
    readonly property int controlHeight: 30

    header: AppToolBar {
        id: toolBar
        leftPadding: defaultMargin
        bottomPadding: defaultMargin
        onStartClicked: {
            if(startUrlTf.text.length < 8 ){ // 8 because 'http://a'
                startUrlTfAnim.running = true
                return
            }
            searchManager.slotStartSearcher()
        }
        onStopClicked: searchManager.slotStopSearcher()
    }

    ColumnLayout {
        anchors.fill: parent
        anchors.margins: defaultMargin

        SearcherParamsForm {
            id: searcherParamsForm
            Layout.fillWidth: true
            defMargin: defaultMargin
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
                                       + (model.status === SearchStatusType.Error ? ': ' + model.error : ''))
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
            case SearchStatusType.NotSet: return "NotSet"
            case SearchStatusType.Found: return "Found"
            case SearchStatusType.NotFound: return "NotFound"
            case SearchStatusType.Downloading: return "Downloading"
            case SearchStatusType.Error: return "Error"
        }
    }

    SearchManager {
        id: searchManager
        startUrl: searcherParamsForm.startUrl
        serchedText: searcherParamsForm.serchedText
        maxThreadsNum: searcherParamsForm.maxThreadsNum
        maxUrlsNum: searcherParamsForm.maxUrlsNum
        urlDownloadingTimeout: searcherParamsForm.urlDownloadingTimeout
        serchedUrlsModel: SerchedUrlsModel {
        }
    }
}

