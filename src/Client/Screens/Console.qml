import QtQuick
import QtQuick.Controls
import QtQuick.Controls.FluentWinUI3
import AS

Pane {
    id: main
    visible: true
    anchors.fill: parent;
    ScrollView {
        id: scroll
        anchors.left: parent.left
        anchors.right: parent.right
        anchors.top: parent.top
        anchors.bottom: cmd.bottom
        ScrollBar.horizontal.policy: ScrollBar.AlwaysOff
        ScrollBar.vertical.policy: ScrollBar.AlwaysOff
        anchors.bottomMargin: 35
        TextArea {
            id: logs
            anchors.fill: parent
            objectName: "logs"
            readOnly: true
            onTextChanged: {
                scroll.contentItem.contentY = scroll.contentItem.contentHeight - scroll.contentItem.height;
                cursorPosition = length;
            }
        }
    }
    TextField{
        id: cmd
        focus: true
        anchors.bottom: parent.bottom
        anchors.left: parent.left
        anchors.right: parent.right
        anchors.bottomMargin: 5
    }
}