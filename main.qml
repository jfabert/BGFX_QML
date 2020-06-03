import QtQuick 2.6
import QtQuick.Window 2.2
import QtQuick.Controls 2.14
import QtQuick.Layouts 1.14

import BGFX 1.0

Window
{
    visible: true
    width: 640
    height: 480
    title: qsTr("Hello BGFX")
    color: "lightBlue"
    id: bgfx_window

    BgfxItem
    {
        width: parent.width
        height: parent.height
    }

    RowLayout
    {
        anchors.bottom: parent.bottom
        anchors.horizontalCenter: parent.horizontalCenter

        Button{
            text: "QML Button"
        }
    }
}
