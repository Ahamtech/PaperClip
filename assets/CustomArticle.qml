import bb.cascades 1.3

Container {
    property alias text: title.text
    property alias img: image.imageSource
    layout: StackLayout {
        orientation: LayoutOrientation.LeftToRight
    }
    Container {
        layout: StackLayout {
            orientation: LayoutOrientation.RightToLeft
        }
        Container {
            preferredHeight: handler.layoutFrame.height
            //            background: Color.create("#7f7c7c")
            layout: StackLayout {
                orientation: LayoutOrientation.TopToBottom
            }
            horizontalAlignment: HorizontalAlignment.Fill

            Container {
                attachedObjects: [
                    LayoutUpdateHandler {
                        id: starhandler
                    }
                ]
                Container {
                    verticalAlignment: VerticalAlignment.Center
                    horizontalAlignment: HorizontalAlignment.Center
                    layoutProperties: StackLayoutProperties {
                        spaceQuota: 5
                    }
                    
                    Label {
                        text: qsTr("Time")
                        

                    }

                }

            }

        }

        Container {
            layout: StackLayout {
                orientation: LayoutOrientation.TopToBottom
            }
            attachedObjects: [
                LayoutUpdateHandler {
                    id: handler
                }
            ]

            Container {
                leftPadding: 20
                Label {
                    id: title
                    text: "Article Headinj fda; al;jf k"
                    multiline: true
                    textStyle.fontSize: FontSize.Large
                    maxHeight: starhandler.layoutFrame.height
                    minHeight: imagehandler.layoutFrame.height

                }
                Container {
                    Label {
                        text: "Article Website eadingArticle kasd fads fja ;dfla; dfja;lj fda;lkj fda;ljf a;lsjkfd a;lsjf da;lkj fda;lk"
                        multiline: true
                        textStyle.fontSize: FontSize.XSmall

                    }
                }
                Container {
                    Label {
                        text: "Tag Tag Tag Tag"
                        multiline: true
                        textStyle.fontSize: FontSize.XSmall
                    }
                }
            }

        }
        Container {
            background: Color.Gray
            preferredHeight: handler.layoutFrame.height
            minWidth: 10
        }

    }
    Divider {

    }

}
