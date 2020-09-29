import QtQuick 2.11
import QtQuick.Controls 2.2
import QtQuick.Layouts 1.3
import "CustomControls" as Custom
import SearchManagerStatus 1.0

ToolBar {
    id: root

    property int searchManagerStatus: SearchManagerStatus.Stopped
    property int defMargin: -1
    signal startClicked
    signal stopClicked

    enabled: searchManagerStatus !== SearchManagerStatus.Starting
             && searchManagerStatus !== SearchManagerStatus.Stopping

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
        anchors.fill: parent

        ToolButton {
            text: qsTr('Start')
            onClicked: root.startClicked()
        }

        ToolButton {
            text: qsTr('Stop')
            onClicked: root.stopClicked()
        }

        Item{
            Layout.fillWidth: true
        }

        Item {
            Layout.alignment: Qt.AlignRight
            Layout.maximumHeight: 30
            Layout.minimumWidth: 70

            Label {
                id: st
                anchors.fill: parent
                anchors.topMargin: 4
                text: toStringSearchManagerStatus(searchManagerStatus)
                font {
                    italic: true
                    pointSize: 8
                }
            }

            Custom.RoundBusyIndicator {
                anchors.fill: parent
                anchors.bottomMargin: 10
                scale: 0.3
                visible: searchManagerStatus !== SearchManagerStatus.Stopped
            }
        }
    }

    function toStringSearchManagerStatus(status) {
        switch(status){
            case SearchManagerStatus.Starting: return "starting..."
            case SearchManagerStatus.Running: return "runnning..."
            case SearchManagerStatus.Stopping: return "stopping..."
            case SearchManagerStatus.Stopped: return "stopped"
        }
    }
}
