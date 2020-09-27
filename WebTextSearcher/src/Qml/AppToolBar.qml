import QtQuick 2.11
import QtQuick.Controls 2.2
import QtQuick.Layouts 1.3

ToolBar {
    id: root

    property int defMargin: -1
    signal startClicked
    signal stopClicked

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
        anchors.left: parent.left

        ToolButton {
            text: qsTr('Start')
            onClicked: root.startClicked()
        }

        ToolButton {
            text: qsTr('Stop')
            onClicked: root.stopClicked()
        }
    }
}
