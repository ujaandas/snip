import QtQuick 2.15
import QtQuick.Controls 2.15
import QtQuick.Layouts 1.15 

Item {
    id: root

    Column {
        anchors.fill: parent

        TabBar {
            id: tabBar
            width: parent.width

            Repeater {
                model: tabManager
                
                TabButton {
                    text: tabTitle 
                    
                    Button {
                        text: "x"
                        width: 20; height: 20
                        anchors.right: parent.right
                        anchors.verticalCenter: parent.verticalCenter
                        onClicked: tabManager.closeTab(index)
                    }
                }
            }
        }

        StackLayout {
            width: parent.width
            height: parent.height - tabBar.height
            currentIndex: tabBar.currentIndex

            Repeater {
                model: tabManager
                
                EditorView {
                    SplitView.fillHeight: true
                    controller: editorController
                }
            }
        }
    }
}