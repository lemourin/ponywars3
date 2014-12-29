import QtQuick 2.2
import QtQuick.Particles 2.0

ParticleSystem {
    property Item lightSystem
    property Item player: world ? world.player : null
    property Item world: lightSystem ? lightSystem.world : null

    Rectangle {
        anchors.fill: parent
        color: Qt.rgba(0, 0, 0, 1)
    }

    LightParticle {
        normalmap: lightSystem ? lightSystem.normalMap : null
        color: Qt.rgba(1, 0.5, 0.5)
    }

    Connections {
        target: world
        onExplosion: emitter.burst(20, pos.x, pos.y)
    }

    Emitter {
        id: emitter
        enabled: false
        maximumEmitted: 100
        sizeVariation: 5
        velocityFromMovement: 10
        velocity: PointDirection {
            x: 0
            y: -10
            xVariation: 10
            yVariation: 10
        }
    }

    Emitter {
        enabled: player ? player.focus : false
        emitRate: 100
        x: player ? player.x+player.hand.x : 0
        y: player ? player.y+player.hand.y : 0
        size: 0.5
        sizeVariation: 5
        velocityFromMovement: 5
        velocity: PointDirection {
            x: 0
            y: -1
            xVariation: 1
            yVariation: 1
        }
    }
}
