import QtQuick 2.11
import QtQuick.Layouts 1.3
import QtQuick.Controls 2.2
import Qt.labs.settings 1.0

GridLayout {
    id: root

    property int defMargin: -1
    property alias startUrl: startUrlTf.text
    property alias serchedText: searchedTextTf.text
    property alias maxThreadsNum: maxThreadsNumSb.value
    property alias maxUrlsNum: maxUrlsNumSb.value
    property alias urlDownloadingTimeout: maxUrlTimeoutSb.value
    signal startClicked
    signal stopClicked

    readonly property int controlHeight: 30

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
        Layout.row: 1
        Layout.column: 2
        Layout.columnSpan: 3
        Layout.fillWidth: true
        Layout.preferredHeight: controlHeight
        placeholderText: qsTr('https://en.cppreference.com/w/cpp/algorithm/search')
        validator: RegExpValidator { regExp: /https*:\/\/[-a-zA-Z0-9@:%._\+~#=]{1,256}\.[a-zA-Z0-9()]{1,6}\b([-a-zA-Z0-9()@:%_\+.~#?&\/\/=]*)/ }
        selectByMouse: true

        SequentialAnimation {
            id: startUrlNotPresentAnim
            loops: 2
            onStarted: startUrlTf.focus = true
            onStopped: { startUrlTf.focus = false; startUrlTf.focus = true }
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
        selectByMouse: true
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
        stepSize: 50
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
        value: 1000
        from: 1
        to: 999999
        stepSize: 1000
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
            stepSize: 500
        }
    }

    onStartClicked: {
        if(startUrlTf.text.length < 8 ){ // 8 because 'http://a'
            startUrlNotPresentAnim.running = true
        }
    }

    Settings {
        property alias startUrl: startUrlTf.text
        property alias searchedText: searchedTextTf.text
        property alias maxThreadsNum: maxThreadsNumSb.value
        property alias maxUrlsNum: maxUrlsNumSb.value
        property alias maxUrlTimeout: maxUrlTimeoutSb.value
    }
}
