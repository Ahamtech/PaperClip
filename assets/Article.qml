import bb.cascades 1.4

Container {
    property alias text: title.text
    property alias favitem: fav.visible
    property alias des: description.text
    property alias time: timestamp.text
    property variant type
    property bool read
    layout: StackLayout {
        orientation: LayoutOrientation.TopToBottom

    }
//    background: read? Color.Gray:"" 
    Container {
        layout: StackLayout {
            orientation: LayoutOrientation.LeftToRight

        }
        Container {
            layoutProperties: StackLayoutProperties {
                spaceQuota: 1
            }
            id: typeing
            maxWidth: 10
            minHeight: texthandler.layoutFrame.height
            onCreationCompleted: {
                switch (type) {
                    case "pocket":
                        background = Color.DarkRed
                        break;
                    case "instapaper":
                        background = Color.DarkGreen
                        break;
                    case "readability":
                        background = Color.DarkMagenta
                        break;
                }
            }
        }
        Container {
            layout: StackLayout {
                orientation: LayoutOrientation.LeftToRight

            }

            Container {
                layout: StackLayout {
                    orientation: LayoutOrientation.LeftToRight
                }
                Container {
                    layoutProperties: StackLayoutProperties {
                        spaceQuota: 10
                    }
                    leftPadding: ui.du(3)
                    rightPadding: ui.du(2)
                    topPadding: ui.du(1)
                    bottomPadding: ui.du(2)
                    attachedObjects: [
                        LayoutUpdateHandler {
                            id: texthandler
                        }
                    ]
                    Container {
                        layoutProperties: StackLayoutProperties {
                            spaceQuota: 10
                        }
                        topPadding: 0
                        layout: StackLayout {
                            orientation: LayoutOrientation.TopToBottom
                        }
                        Container {
                           Label {
                                id: title
                                maxHeight: ui.du(10)
                                text: "hello o finish our app, we need to connect the buttons so that the displayed layout changes when the corresponding button is clicked. You learn more about event handling in other tutorials, but here you get a taste of how to respond to simple events in QML."
                                multiline: true
                            }
                           topPadding: ui.du(1)
                        }
                        Container {
                            topPadding: ui.du(2)
                            Label {
                                id: description
                                maxHeight: ui.du(10)
                                text: "hello Cascades uses a signals and slots model that is inherited from the underlying Qt framework. All UI controls in Cascades emit signals when particular properties change (such as size andt"
                                multiline: true
                            }
                        }
                    }
                }
                Container {
                    topPadding: ui.du(2)
                    layout: StackLayout {
                        orientation: LayoutOrientation.TopToBottom
                    }
                    horizontalAlignment: HorizontalAlignment.Right
                    Label {
                        textStyle.rules: [
                            FontFaceRule {
                                id: styleFontRule
                                source: "asset:///fonts/avenir/AvenirNextLTPro-CnIt.woff"
                                fontFamily: "AvenirNextLTPro-CnIt"
                            }
                        ]
                        textStyle.fontFamily: "AvenirNextLTPro-CnIt"
                        horizontalAlignment: HorizontalAlignment.Center
                        id: timestamp
                        textStyle.textAlign: TextAlign.Left
                    }
                    ImageView {
                        horizontalAlignment: HorizontalAlignment.Center

                        imageSource: "asset:///Images/BBicons/ic_favorite.png"
                        scalingMethod: ScalingMethod.None
                        loadEffect: ImageViewLoadEffect.FadeZoom
                        filterColor: Color.DarkGray
                        id: fav
                    }

                }

            }
            Divider {

            }
        }
    }
}
