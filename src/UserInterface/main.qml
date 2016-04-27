import QtQuick 2.2
import QtQuick.Controls 1.1
import Environment 1.0
import "../GameEngine/UserInterface"

Item {
    id: root
    width: 800
    height: 600
    focus: true

    GameOver { }
    HUD { id: hud }
    AndroidUI {
        visible: app.system === Environment.Android
        opacity: 0.5
    }

    GameEngineConsole {
        id: gameEngineConsole
    }

    Keys.onPressed: {
        if (event.key === Qt.Key_Escape)
            Qt.quit();
        else if (event.key === Qt.Key_F1)
            gameEngineConsole.toggle()
        else if (event.key === Qt.Key_Comma)
            game.dump("")
    }

}
