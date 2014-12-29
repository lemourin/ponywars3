import QtQuick 2.2
import QtQuick.Controls 1.1

FocusScope {
    id: loadMap
    width: childrenRect.width
    height: childrenRect.height

    Row {
        Button {
            text: "LoadMap"
            onClicked: loadMap.focus ^= 1
        }

        Row {
            id: options
            visible: loadMap.focus

            Column {
                Row {
                    Button {
                        text: "Load from file:"
                        onClicked: world.loadMap("qml/Maps/"+fileName.text);
                    }

                    TextField {
                        id: fileName
                        text: "ItemSet.qml"
                    }
                }
            }
        }
    }
}
