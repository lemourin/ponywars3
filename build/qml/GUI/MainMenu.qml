import QtQuick 2.2
import QtQuick.Controls 1.1
import Environment 1.0
import "Network"

Row {
    id: mainMenu

    Column {
        Button {
            text: "Quit"
            onClicked: Qt.quit()
        }

        Button {
            text: "MapEditor"
            onClicked: mapEditor.toggleFocus()
        }

        Button {
            text: "File"
            onClicked: fileOptions.focus ^= 1
        }

        Button {
            text: "Network"
            onClicked: networkOptions.focus ^= 1
        }

        Button {
            text: "Pause"
            onClicked: world.paused ^= 1
        }

        Button {
            visible: app.system !== Environment.Android
            text: "Toggle fullscreen"
            onClicked: app.fullscreen ^= 1
        }

        Component.onCompleted: {
            var max = 0, i
            for (i=0; i<children.length; i++)
                if (children[i].visible)
                    max = Math.max(children[i].width, max)
            for (i=0; i<children.length; i++)
                if (children[i].visible)
                    children[i].width = max
        }
    }

    /*Column {
        Item { id: networkOptions; parent: world }
        visible: networkOptions.focus

        NetworkOptions { }
    }*/

    Column {
        Item { id: fileOptions; parent: world }
        visible: fileOptions.focus

        SaveMap { }
        LoadMap { }
    }

    Row {
        visible: mapEditor ? mapEditor.focus : false

        Column {
            Button {
                width: parent.width
                text: "AddChain"
                onClicked: mapEditor.addChain.focus ^= 1
            }

            Button {
                text: "DeleteItem"
                onClicked: mapEditor.deleteItem.focus ^= 1
            }

            Button {
                width: parent.width
                text: "GrabItem"
                onClicked: mapEditor.grabItem.focus ^= 1
            }

            Button {
                width: parent.width
                text: "BodyEdit"
                onClicked: mapEditor.bodyEdit.focus ^= 1
            }

            Button {
                width: parent.width
                text: "AddBody"
                onClicked: mapEditor.addBody.focus ^= 1
            }
        }

        Column {
            visible: mapEditor ? mapEditor.addBody.focus : false

            Button {
                width: parent.width
                text: "AddPolygon"
                onClicked: mapEditor.addBody.addPolygon.focus ^= 1
            }

            Button {
                width: parent.width
                text: "AddCircle"
                onClicked: mapEditor.addBody.addCircle.focus ^= 1
            }

            Button {
                text: "AddRectangle"
                onClicked: mapEditor.addBody.addRectangle.focus ^= 1
            }

            Button {
                width: parent.width
                text: "FinishBody"
                onClicked: mapEditor.addBody.finished()
            }
        }

        Column {
            id: polygonOptions
            visible: mapEditor ? mapEditor.addBody.addPolygon.focus : false

            Button {
                text: "Finish"
                onClicked: mapEditor.addBody.addPolygon.finished()
            }
        }
    }
}
