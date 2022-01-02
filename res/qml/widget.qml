import QtQuick 2.15
import QtQuick.Controls 2.15
import QtQuick.Layouts 1.15

import Karunit 1.0
import KarunitPlugins 1.0

Item {
    anchors.fill: parent

    function call(number) {
        KUPOfonoPluginConnector.call(number)
    }

    function hangupAll() {
        KUPOfonoPluginConnector.hangupAll()
    }

    Connections {
        target: KUPOfonoPluginConnector
        function onCallSignal(number) {
            stack.currentIndex = 0
            textinput.text = number
        }
    }

    ColumnLayout {
        anchors.fill: parent

        TabBar {
            Layout.fillWidth: true
            id: bar

            TabButton {
                text: qsTr("Home")
            }

            Repeater {
                model: KUPOfonoPluginConnector.callsAliases
                TabButton {
                    text: KUPOfonoPluginConnector.callsAliases
                          && KUPOfonoPluginConnector.lineIdentification(
                              modelData)
                }
            }
        }

        StackLayout {
            id: stack
            Layout.fillWidth: true
            Layout.fillHeight: true
            currentIndex: bar.currentIndex

            Rectangle {
                color: "salmon"

                TextField {
                    width: parent.width
                    height: 50
                    id: textinput
                    placeholderText: qsTr("Number to call")

                    RoundButton {
                        anchors.verticalCenter: parent.verticalCenter
                        anchors.right: parent.right
                        anchors.rightMargin: 5
                        icon.name: "edit-clear"
                        onClicked: {
                            textinput.clear()
                        }
                    }
                }

                Grid {
                    anchors.centerIn: parent
                    columns: 3
                    spacing: 5

                    Repeater {
                        model: 9
                        Button {
                            text: modelData
                            onClicked: {
                                textinput.text += modelData
                            }
                        }
                    }
                    Repeater {
                        model: ["+", "0", "#"]
                        Button {
                            text: modelData
                            onClicked: {
                                textinput.text += modelData
                            }
                        }
                    }
                    Button {
                        icon.name: "call-start"
                        onClicked: {
                            call(textinput.text)
                        }
                    }
                    Button {
                        enabled: false
                    }
                    Button {
                        icon.name: "call-stop"
                        onClicked: {
                            hangupAll()
                        }
                    }
                }
            }

            Repeater {
                model: KUPOfonoPluginConnector.callsAliases

                Rectangle {
                    color: "beige"
                    Column {
                        anchors.fill: parent
                        spacing: 5
                        Label {
                            anchors.horizontalCenter: parent.horizontalCenter
                            text: qsTr(
                                      "Status") + " " + (KUPOfonoPluginConnector.callsAliases
                                                         && KUPOfonoPluginConnector.state(
                                                             modelData))
                            horizontalAlignment: Qt.AlignHCenter
                        }
                        Row {
                            anchors.horizontalCenter: parent.horizontalCenter
                            spacing: 5
                            Button {
                                icon.name: "call-start"
                                onClicked: {
                                    KUPOfonoPluginConnector.answer(modelData)
                                }
                            }
                            Button {
                                icon.name: "call-stop"
                                onClicked: {
                                    KUPOfonoPluginConnector.hangup(modelData)
                                }
                            }
                        }
                    }
                }
            }
        }
    }
}
