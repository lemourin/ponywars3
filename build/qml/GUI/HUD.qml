import QtQuick 2.2
import QtQuick.Controls 1.1

Item {
    anchors.fill: parent

    ProgressBar {
        id: healthbar
        anchors.bottom: parent.bottom
        anchors.right: parent.right
        minimumValue: 0
        maximumValue: 100
        //visible: world.player !== null
        value: world.playerHealth
        //value: world.player ? world.player.health : 0
        Text {
            anchors.centerIn: parent
            text: "Health: "+healthbar.value+"%"
        }
    }

    ProgressBar {
        property Item weapon: world.player ? world.player.hand.grabbedWeapon : null

        id: ammobar
        anchors.bottom: healthbar.top
        anchors.right: parent.right
        visible: weapon !== null
        value: visible ? weapon.bulletCount : 0
        minimumValue: 0
        maximumValue: 15
        Text {
            anchors.centerIn: parent
            text: "Ammo: "+ammobar.value
        }
    }
}
