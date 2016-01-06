import bb.cascades 1.3
import bb.data 1.0
Page {
    id: accounts
    onCreationCompleted: {
        app.loadAuthstoQML.connect(renderauth)
        app.authsupdated.connect(auths)
        auths()
        navigationPane.topChanged.connect(auths)
    }
    function auths() {
        resetauths()
        app.loadAuths()
    }
    function resetauths(){
        readabilityloginbtn.visible = true
        readabilityusername.visible = false
       pocketlogoutbtn.visible = false;
       readabilitylogoutbtn.visible = false
       instapaperlogoutbtn.visible = false
        pocketloginbtn.visible = true
        pocketusername.visible = false
        instapaperloginbtn.visible = true
        instapaperusername.visible = false
    }
    
    function renderauth(data) {
        for (var i = 0; i < data.length; i ++) {
            switch (data[i].service) {
                case "readability":
                    renderauthreadablity(data[i])
                    break;
                case "pocket":
                    renderauthpocket(data[i])
                    break;
                case "instapaper":
                    renderauthinstapaper(data[i])
                    break;
            }
        }
    }
    function renderauthreadablity(data){
        readabilityloginbtn.visible = false
        readabilityusername.visible = true
        readabilityusername.text = data.username
        readabilitylogoutbtn.visible = true
    }
    function renderauthpocket(data){
        pocketloginbtn.visible = false
        pocketusername.visible = true
        pocketusername.text = data.username
        pocketlogoutbtn.visible = true
    }
    function renderauthinstapaper(data){
        instapaperloginbtn.visible = false
        instapaperusername.visible = true
        instapaperusername.text = data.username
        instapaperlogoutbtn.visible = true
    }
    titleBar: TitleBar {
        title: "Accounts information"
    }
    Container {
        Container {
            margin.bottomOffset: 30.0
            margin.leftOffset: 20
            margin.topOffset: 50.0
            margin.rightOffset: 20.0
            layout: GridLayout {
                columnCount: 3
            }
            horizontalAlignment: HorizontalAlignment.Fill
            verticalAlignment: VerticalAlignment.Center
            Label {
                text: "Pocket"
                verticalAlignment: VerticalAlignment.Center
                horizontalAlignment: HorizontalAlignment.Left
            }
            Label {
                id: pocketusername
                verticalAlignment: VerticalAlignment.Center
                horizontalAlignment: HorizontalAlignment.Right
            }
            Button {
                id: pocketloginbtn
                text: "pocket"
                onClicked: {
                    navigationPane.push(pocketpage.createObject());
                }
                appearance: ControlAppearance.Primary
                visible: true
            }
            
        }
        Container {
            id:pocketlogoutbtn
            margin.leftOffset: 20
            margin.bottomOffset: 30.0
            margin.rightOffset: 20.0
            layout: DockLayout {

            }
            horizontalAlignment: HorizontalAlignment.Right
            visible: false
            Button {
                text: "logout"
                horizontalAlignment: HorizontalAlignment.Right
                verticalAlignment: VerticalAlignment.Center
                onClicked: {
                    app.deleteAccount('pocket')
                }
            }
        } Container {
            preferredHeight: 1 // 1 pixel in height
            
            verticalAlignment: VerticalAlignment.Bottom // docks at bottom of line row
            horizontalAlignment: HorizontalAlignment.Fill // full width
        }
        Divider {
        }
        Container {
            margin.bottomOffset: 30.0
            margin.leftOffset: 20
            margin.topOffset: 50.0
            margin.rightOffset: 20.0
            layout: GridLayout {
                columnCount: 3
            }

            horizontalAlignment: HorizontalAlignment.Fill
            verticalAlignment: VerticalAlignment.Center
            Label {
                text: "Readability"
                verticalAlignment: VerticalAlignment.Center
                horizontalAlignment: HorizontalAlignment.Left
            }
            Label {
                id: readabilityusername
                verticalAlignment: VerticalAlignment.Center
                horizontalAlignment: HorizontalAlignment.Right
            }
            Button {
                id: readabilityloginbtn
                text: "readbility"
                onClicked: {
                    var page = readabilitypage.createObject();
                    navigationPane.push(page);
                }
                appearance: ControlAppearance.Primary
            }
        }Container {
            id:readabilitylogoutbtn
            margin.bottomOffset: 30
            margin.leftOffset: 20
            margin.rightOffset: 20.0
            layout: DockLayout {
            
            }
            horizontalAlignment: HorizontalAlignment.Right
            visible: false
            
            Button {
                text: "logout"
                horizontalAlignment: HorizontalAlignment.Right
                verticalAlignment: VerticalAlignment.Center
                onClicked: {
                    app.deleteAccount('readability')
                }
            }
        }
        Divider {

        }
        Container {
            margin.bottomOffset: 30.0
            margin.leftOffset: 20
            margin.topOffset: 50.0
            margin.rightOffset: 20.0
            layout: GridLayout {
                columnCount: 3
            }

            horizontalAlignment: HorizontalAlignment.Fill
            verticalAlignment: VerticalAlignment.Center
            Label {
                text: "Instapaper"
                verticalAlignment: VerticalAlignment.Center
                horizontalAlignment: HorizontalAlignment.Left
            }
            Label {
                id: instapaperusername
                verticalAlignment: VerticalAlignment.Center
                horizontalAlignment: HorizontalAlignment.Right
            }
            Button {
                text: "instapaper"
                id: instapaperloginbtn
                onClicked: {
                    var page = instapaperpage.createObject();
                    navigationPane.push(page);
                }
                appearance: ControlAppearance.Primary
            }
        }Container {
            id:instapaperlogoutbtn
            margin.leftOffset: 20
            margin.rightOffset: 20.0
            margin.bottomOffset: 30
            layout: DockLayout {
            
            }
            horizontalAlignment: HorizontalAlignment.Right
            visible: false
            Button {
                text: "logout"
                horizontalAlignment: HorizontalAlignment.Right
                verticalAlignment: VerticalAlignment.Center
                onClicked: {
                    app.deleteAccount('instapaper')
                }
            }
        }
        Divider {

        }

    }
    attachedObjects: [
        ComponentDefinition {
            id: pocketpage
            source: "login/pocket.qml"
        },
        ComponentDefinition {
            id: readabilitypage
            source: "login/readability.qml"
        },
        ComponentDefinition {
            id: instapaperpage
            source: "login/instapaper.qml"
        }
    ]
}
