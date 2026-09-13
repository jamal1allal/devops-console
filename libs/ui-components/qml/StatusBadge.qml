import QtQuick

// Indicateur de santé compact, pensé comme un "voyant" plutôt que
// comme un chip Material générique : forme d'ampoule (cercle + halo),
// pas de badge rectangulaire à coins arrondis passe-partout.
Item {
    id: root
    property string health: "unknown"  // healthy | degraded | down | unknown
    implicitWidth: 96
    implicitHeight: 28

    readonly property color dotColor: {
        switch (root.health) {
            case "healthy":  return "#2E7D5B"
            case "degraded": return "#B8862B"
            case "down":     return "#B23B3B"
            default:         return "#7A7A7A"
        }
    }
    readonly property string label: {
        switch (root.health) {
            case "healthy":  return "opérationnel"
            case "degraded": return "dégradé"
            case "down":     return "en panne"
            default:         return "inconnu"
        }
    }

    Row {
        anchors.verticalCenter: parent.verticalCenter
        spacing: 8

        // Halo doux derrière le point, pas un simple cercle plat
        Item {
            width: 12; height: 12
            anchors.verticalCenter: parent.verticalCenter

            Rectangle {
                anchors.centerIn: parent
                width: 12; height: 12
                radius: 6
                color: root.dotColor
                opacity: 0.25
                scale: root.health === "down" ? 1.6 : 1.0
                Behavior on scale { NumberAnimation { duration: 600; easing.type: Easing.InOutQuad } }
            }
            Rectangle {
                anchors.centerIn: parent
                width: 8; height: 8
                radius: 4
                color: root.dotColor
            }
        }

        Text {
            text: root.label
            font.pixelSize: 13
            color: "#3A3A38"
        }
    }
}
