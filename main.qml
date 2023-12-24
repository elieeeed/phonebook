import QtQuick 2.15
import QtQuick.Window 2.15
import QtQuick.Controls 2.15
import QtQuick.Layouts 1.15
import QtQuick.Dialogs

import MyModule 1.0

Window {
    id: window
    width: 640
    height: 480
    visible: true

    PhoneModel{
        id: phone_model
    }

    ColumnLayout {
        anchors.fill: parent

        ToolBar {
            id: toolBar
            Layout.alignment: Qt.AlignTop | Qt.AlignLeft | Qt.AlignRight

            Row{
                Column{
                    ToolButton {
                        id: add_tool_button
                        icon.source: "add.ico"
                        text: "Add contact"
                        font.bold: true
                        antialiasing: true
                        onClicked: {
                            scroll_list_view.Layout.alignment = Qt.AlignTop | Qt.AlignLeft | Qt.AlignRight;
                            rectangle_add_save.visible = true
                            but_add.visible=true
                            but_save.visible=false
                        }
                    }
                    ToolButton {
                        id: delete_tool_button
                        icon.source: "delete.png"
                        text: "Delete contact"
                        font.bold: true
                        antialiasing: true
                        onClicked: {
                            if(list_view.currentIndex>-1){
                                phone_model.remove_person(list_view.currentIndex)
                                phone_model.update()
                            }
                        }
                    }
                }
                Column{
                    ToolButton {
                        id: red_tool_button
                        icon.source: "red.png"
                        text: "Edit"
                        font.bold: true
                        antialiasing: true
                        onClicked: {
                            if(list_view.currentIndex>-1){
                                text_input_FIO.text=phone_model.get_person_name(list_view.currentIndex)
                                text_input_phone.text=phone_model.get_person_phone(list_view.currentIndex)
                                text_input_address.text=phone_model.get_person_address(list_view.currentIndex)

                                scroll_list_view.Layout.alignment = Qt.AlignTop | Qt.AlignLeft | Qt.AlignRight;
                                rectangle_add_save.visible = true
                                but_add.visible=false
                                but_save.visible=true
                            }
                        }
                    }

                    ToolButton {
                        id: clear_tool_button
                        icon.source: "clear.png"
                        text: "Clear all"
                        font.bold: true
                        antialiasing: true
                        onClicked: {
                            phone_model.clear()
                        }
                    }
                }
            }
        }

        Item {
            id: item_header
            height: 20
            Layout.fillWidth: true
            RowLayout {
                Row {
                Rectangle {
                    width: 50
                    height: 20
                    border.width: 1
                    border.color: "#808080"
                    Text {
                        font.bold: true
                        text: 'row '
                    }
                }
                Rectangle {
                    width: 180
                    height: 20
                    border.width: 1
                    border.color: "#808080"
                    Text {
                        font.bold: true
                        text: 'Name:'
                    }
                }
                Rectangle {
                    width: 180
                    height: 20
                    border.width: 1
                    border.color: "#808080"
                    Text {
                        font.bold: true
                        text: 'Phone:'
                    }
                }
                Rectangle {
                    width: 500
                    height: 20
                    border.width: 1
                    border.color: "#808080"
                    Text {
                        font.bold: true
                        text: 'Address:'
                    }
                }
            }
            }
        }

        ScrollView {
            id: scroll_list_view
            visible: true
            Layout.fillWidth: true
            Layout.fillHeight: true
            property int scrollSpeed: 20
            focus: true

            ListView {
                id: list_view
                Layout.alignment: Qt.AlignTop | Qt.AlignLeft | Qt.AlignRight
                clip: true
                model: phone_model.proxy_model_property
                focus: true

                delegate: Item {
                    id: item_delegate
                    width: 1000
                    height: 20

                    required property int index
                    required property string name
                    required property string phone
                    required property string address

                    Row {
                        Rectangle {
                            width: 50
                            height: 20
                            border.width: 1
                            border.color: "#808080"
                            color:  "#00ffffff"
                            Text {
                                text: index+1
                            }
                        }
                        Rectangle {
                            width: 180
                            height: 20
                            border.width: 1
                            border.color: "#808080"
                            color:  "#00ffffff"
                            Text {
                                text: name
                            }
                        }
                        Rectangle {
                            width: 180
                            height: 20
                            border.width: 1
                            border.color: "#808080"
                            color:  "#00ffffff"
                            Text {
                                text: phone
                            }
                        }
                        Rectangle {
                            width: 500
                            height: 20
                            border.width: 1
                            border.color: "#808080"
                            color:  "#00ffffff"
                            Text {
                                text: address
                            }
                        }
                    }

                    MouseArea {
                        id: mouse_area_delegate
                        onClicked: {
                            scroll_list_view.focus = true
                            list_view.focus = true
                            list_view.currentIndex = index

                            if(rectangle_add_save.visible){
                                text_input_FIO.text=phone_model.get_person_name(list_view.currentIndex)
                                text_input_phone.text=phone_model.get_person_phone(list_view.currentIndex)
                                text_input_address.text=phone_model.get_person_address(list_view.currentIndex)
                            }
                        }
                        onWheel: {
                            if (wheel.angleDelta.y > 0) {
                                list_view.contentY -= scroll_list_view.scrollSpeed
                                if (list_view.contentY < 0) {
                                    list_view.contentY = 0;
                                }
                            } else {
                                if(list_view.contentHeight>scroll_list_view.height){
                                    list_view.contentY += scroll_list_view.scrollSpeed;
                                    if (list_view.contentY + scroll_list_view.height > list_view.contentHeight) {
                                        list_view.contentY = list_view.contentHeight -  scroll_list_view.height;
                                    }
                                }
                            }
                        }
                    }
                }

                highlightMoveDuration : 200
                highlightMoveVelocity : 1000
                highlight: Rectangle { color: "lightsteelblue"; radius: 5 }
            }
        }


        Rectangle {
            id: rectangle_add_save
            height: 200
            visible: false
            color: "#ffffff"
            Layout.alignment: Qt.AlignBottom | Qt.AlignLeft | Qt.AlignRight
            border.width: 2
            border.color: "#000000"

            Column{
                x: 10
                y: 10
                spacing: 10
                Grid {
                    id: grid
                    columns: 2
                    spacing: 2

                    Text {
                        id: text_FIO
                        height: text_input_FIO.height
                        verticalAlignment: Text.AlignVCenter
                        text: qsTr("Name:")
                        font.pixelSize: 12
                    }

                    TextField  {
                        id: text_input_FIO
                        text: qsTr("Elahe yarahmadi")
                        font.pixelSize: 12
                        focus: true
                        selectByMouse: true
                    }

                    Text {
                        id: text_phone
                        height: text_input_phone.height
                        verticalAlignment: Text.AlignVCenter
                        text: qsTr("Phone:")
                        font.pixelSize: 12
                    }

                    TextField {
                        id: text_input_phone
                        text: qsTr("+989102298352")
                        font.pixelSize: 12
                        focus: true
                        selectByMouse: true
                    }

                    Text {
                        id: text_address
                        height: text_input_address.height
                        verticalAlignment: Text.AlignVCenter
                        text: qsTr("Address:")
                        font.pixelSize: 12
                    }

                    TextField  {
                        id: text_input_address
                        text: qsTr("tehranpars")
                        font.pixelSize: 12
                        focus: true
                        selectByMouse: true
                    }
                }
                Row{
                    spacing: 5
                    Button{
                        id: but_add
                        text: "Add"
                        font.bold: true
                        onClicked: {
                            phone_model.addPerson(text_input_FIO.text,text_input_phone.text,text_input_address.text)
                            phone_model.update()
                            list_view.currentIndex=phone_model.get_last_index()
                        }
                    }
                    Button{
                        id: but_save
                        text: "save"
                        font.bold: true
                        onClicked: {
                            phone_model.setPerson(list_view.currentIndex,text_input_FIO.text,text_input_phone.text,text_input_address.text)
                            list_view.currentIndex=phone_model.curent_index_property
                        }
                    }
                    Button{
                        text: "cancel"
                        font.bold: true
                        onClicked: {
                            rectangle_add_save.visible = false
                            scroll_list_view.Layout.alignment = Qt.AlignBottom | Qt.AlignLeft | Qt.AlignRight;                        }
                    }
                }
            }
        }
    }

}

