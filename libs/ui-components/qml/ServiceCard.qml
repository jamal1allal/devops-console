import QtQuick

// Carte de service : une bordure gauche épaisse colorée selon la santé
// fait office de marqueur de statut, plutôt qu'un badge dans un coin
// perdu au milieu d'une carte SaaS générique à coins arrondis uniformes.
Item {
    id: root
    property string serviceName: ""
    property string health: "unknown"
    property string version: ""
    property string lastDeployedAt: ""
    signal clicked()

    implicitWidth: 320
    implicitHeight: 96

    readonly property color accentColor: {
        switch (root.health) {
            case "healthy":  return "#2E7D5B"
            case "degraded": return "#B8862B"
            case "down":     return "#B23B3B"
            default:         return "#7A7A7A"
        }
    }

    Rectangle {
        anchors.fill: parent
        color: "#FAFAF8"
        border.color: "#E4E2DC"
        border.width: 1

        Rectangle {
            anchors.left: parent.left
            anchors.top: parent.top
            anchors.bottom: parent.bottom
            width: 4
            color: root.accentColor
        }

        Column {
            anchors.left: parent.left
            anchors.leftMargin: 20
            anchors.verticalCenter: parent.verticalCenter
            anchors.right: parent.right
            anchors.rightMargin: 16
            spacing: 6

            Text {
                text: root.serviceName
                font.pixelSize: 16
                font.weight: Font.DemiBold
                color: "#2A2A28"
            }

            Row {
                spacing: 16
                StatusBadge { health: root.health }
                Text {
                    visible: root.version.length > 0
                    text: "v" + root.version
                    font.pixelSize: 12
                    color: "#8A8A82"
                    anchors.verticalCenter: parent.verticalCenter
                }
            }
        }

        MouseArea {
            anchors.fill: parent
            onClicked: root.clicked()
            hoverEnabled: true
            cursorShape: Qt.PointingHandCursor
        }
    }
}
