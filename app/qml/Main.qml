import QtQuick
import QtQuick.Window
import DevopsApp

Window {
    width: 1080
    height: 720
    visible: true
    title: "DevOps Console"
    color: "#F5F3EE"

    DashboardView {
        anchors.fill: parent
    }
}
