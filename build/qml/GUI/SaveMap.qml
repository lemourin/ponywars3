import QtQuick 2.2
import QtQuick.Controls 1.0

FocusScope {
    id: saveMap
    width: childrenRect.width
    height: childrenRect.height

    Row {
        Button {
            text: "SaveMap"
            onClicked: saveMap.focus ^= 1
        }

        Row {
            id: options
            visible: saveMap.focus

            Column {
                Row {
                    Button {
                        text: "Save to file:"
                        onClicked: root.world.itemSet.dump("qml/Maps/"+fileName.text);
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
