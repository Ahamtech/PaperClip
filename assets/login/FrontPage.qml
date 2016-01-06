import bb.cascades 1.4

NavigationPane {
    id: navigationpane
    Page {
        Container {
            layout: AbsoluteLayout {}
            objectName: "Untitled-1"
            preferredWidth: 720
            preferredHeight: 720
            ImageView {
                objectName: "instapaperbackground"
                imageSource: "asset:///Images/frontpageimages/instapaperbackground.png"
                layoutProperties: AbsoluteLayoutProperties {
                    positionX: 0 
                    positionY: 478
                    
                }
                preferredWidth: 720
                preferredHeight: 240
                onTouch: {
                    navigationpane.push(instapaperpage.createObject())
                }
            }
            ImageView {
                objectName: "readabilitybackground"
                imageSource: "asset:///Images/frontpageimages/readabilitybackground.png"
                layoutProperties: AbsoluteLayoutProperties {
                    positionX: 0 
                    positionY: 0
                }
                onTouch: {
                    navigationpane.push(readabilitypage.createObject())
                }
                preferredWidth: 720
                preferredHeight: 242
            }
            ImageView {
                objectName: "pocketbackground"
                imageSource: "asset:///Images/frontpageimages/pocketbackground.png"
                layoutProperties: AbsoluteLayoutProperties {
                    positionX: 0 
                    positionY: 206
                }
                onTouch: {
                    navigationpane.push(pocketpage.createObject())
                }
                preferredWidth: 720
                preferredHeight: 274
            }
            Label { 
                objectName: "READABILITY"
                text: "READABILITY" 
                
                textStyle.fontSize: FontSize.PointValue
                textStyle.fontSizeValue: 17.8
                textStyle.fontFamily: "Opal"
                textStyle.fontStyle: FontStyle.Normal
                layoutProperties: AbsoluteLayoutProperties {
                    positionX: 248 
                    positionY: 70
                }
            }
            Label { 
                objectName: "POCKET"
                text: "POCKET" 
                
                textStyle.fontSize: FontSize.PointValue
                textStyle.fontSizeValue: 17.8
                textStyle.fontFamily: "Opal"
                textStyle.fontStyle: FontStyle.Normal
                layoutProperties: AbsoluteLayoutProperties {
                    positionX: 239 
                    positionY: 304
                }
            }
            Label { 
                objectName: "INSTAPAPER"
                text: "INSTAPAPER" 
              
                textStyle.fontSize: FontSize.PointValue
                textStyle.fontSizeValue: 17.8
                textStyle.fontFamily: "Opal"
                textStyle.fontStyle: FontStyle.Normal
                layoutProperties: AbsoluteLayoutProperties {
                    positionX: 238 
                    positionY: 548
                }
            }
            ImageView {
                objectName: "instapaperlogo"
                imageSource: "asset:///Images/frontpageimages/instapaperlogo.png"
                layoutProperties: AbsoluteLayoutProperties {
                    positionX: 0 
                    positionY: 480
                }
                preferredWidth: 207
                preferredHeight: 240
            }
            ImageView {
                objectName: "pocketlogo"
                imageSource: "asset:///Images/frontpageimages/pocketlogo.png"
                layoutProperties: AbsoluteLayoutProperties {
                    positionX: 0 
                    positionY: 241
                }
                preferredWidth: 207
                preferredHeight: 239
            }
            ImageView {
                objectName: "readabilitylogo"
                imageSource: "asset:///Images/frontpageimages/readabilitylogo.png"
                layoutProperties: AbsoluteLayoutProperties {
                    positionX: 0 
                    positionY: 0
                }
                preferredWidth: 207
                preferredHeight: 242
            }
        }
        attachedObjects: [
            ComponentDefinition {
                id: instapaperpage
                source: "asset:///login/instapaper.qml"
            },
            ComponentDefinition {
                id: pocketpage
                source: "asset:///login/pocket.qml"
            },
            ComponentDefinition {
                id: readabilitypage
                source: "asset:///login/readability.qml"
            }
        ]
    }
}
