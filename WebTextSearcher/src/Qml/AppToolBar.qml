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

    enabled: searchManagerStatus != SearchManagerStatus.Starting
             && searchManagerStatus != SearchManagerStatus.Stopping

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

        Custom.RoundBusyIndicator {
            Layout.maximumHeight: 30
            Layout.alignment: Qt.AlignRight
            scale: 0.5
            visible: searchManagerStatus === SearchManagerStatus.Running
                     || searchManagerStatus === SearchManagerStatus.Stopping
        }
    }
}
