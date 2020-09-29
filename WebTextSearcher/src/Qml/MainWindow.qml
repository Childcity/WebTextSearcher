import QtQuick 2.9
import QtQuick.Window 2.3
import QtQuick.Controls 2.2
import QtQuick.Layouts 1.3
import Qt.labs.settings 1.0
import SerchedUrlsModel 1.0
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
                   + searcherResultList.implicitHeight
                   + 2*defaultMargin

    readonly property int defaultMargin: 5

    header: AppToolBar {
        id: toolBar
        padding: defaultMargin
        topPadding: 0
        onStartClicked: {
            searcherParamsForm.startClicked()
            if(searcherParamsForm.startUrl.length < 8 ){ // 8 because 'http://a'
                return
            }
            searchManager.slotStartSearcher()
        }
        onStopClicked: searchManager.slotStopSearcher()
        searchManagerStatus: searchManager.status
    }

    ColumnLayout {
        anchors.fill: parent
        anchors.margins: defaultMargin

        SearcherParamsForm {
            id: searcherParamsForm
            Layout.fillWidth: true
            defMargin: defaultMargin
        }

        SearcherResultList {
            id: searcherResultList
            Layout.fillWidth: true
            Layout.fillHeight: true
            searchManager: searchManager
            defMargin: defaultMargin
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

    onClosing: searchManager.slotStopSearcher()

    Settings {
        property alias appWndWidth: root.width
        property alias appWndHeight: root.height
    }
}

