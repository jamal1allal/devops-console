import QtQuick
import QtQuick.Controls.Basic
import QtQuick.Layouts
import DevopsUi

// Vue "tableau de bord" : un en-tête façon terminal (police mono pour le
// titre, pas de bandeau eyebrow générique) au-dessus d'une grille de
// ServiceCard. Volontairement pas de sidebar/nav superflue : ce projet
// n'a qu'un seul écran pour l'instant.
Item {
    id: root

    readonly property var vm: ServicesViewModel {}

    Component.onCompleted: vm.refresh()

    Column {
        anchors.fill: parent
        anchors.margins: 32
        spacing: 24

        RowLayout {
            width: parent.width
            spacing: 16

            Text {
                text: "devops-console"
                font.family: "Courier New"
                font.pixelSize: 22
                font.weight: Font.Bold
                color: "#2A2A28"
            }

            Item { Layout.fillWidth: true }

            Button {
                text: vm.loading ? "Actualisation..." : "Actualiser"
                enabled: !vm.loading
                onClicked: vm.refresh()
            }
        }

        Text {
            visible: vm.errorMessage.length > 0
            text: "Erreur : " + vm.errorMessage
            color: "#B23B3B"
            font.pixelSize: 13
        }

        Text {
            visible: !vm.loading && vm.services.length === 0 && vm.errorMessage.length === 0
            text: "Aucun service à afficher pour le moment."
            color: "#8A8A82"
            font.pixelSize: 13
        }

        Flow {
            width: parent.width
            spacing: 16

            Repeater {
                model: vm.services

                ServiceCard {
                    serviceName: modelData.name
                    health: modelData.health
                    version: modelData.version
                    lastDeployedAt: modelData.lastDeployedAt
                    onClicked: console.log("Détail du service :", modelData.id)
                }
            }
        }
    }
}
