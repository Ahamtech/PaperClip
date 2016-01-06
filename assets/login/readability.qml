import bb.cascades 1.3
import bb.system 1.0

Page {
    property Page accpage;
    function loginstate(info){
        if (info == 1){
            success.show()
            navigationPane.pop()
            readabilityactivity.running = false
        }
        else{
            fail.show()
            readabilityactivity.running = false
        }
    }
    onCreationCompleted: {
        readablityinput.requestFocus()
        app.readabilityloginstate.connect(loginstate)
    }
    
        
   
    
    Container {
       
       layout: DockLayout {
           
       }
        leftPadding: 100.0
        rightPadding: 100.0
       
        ScrollView {
            
         
        
        Container {
            topPadding: ui.du(10)
            verticalAlignment: VerticalAlignment.Center
            horizontalAlignment: HorizontalAlignment.Center
            ImageView {
                verticalAlignment: VerticalAlignment.Center
                horizontalAlignment: HorizontalAlignment.Center
                imageSource: "asset:///Images/readability_tab.png"
                minHeight: 150
                minWidth: 150
                maxHeight: 150
                maxWidth: 150
            }
            
            Label {
                text: "Login to Readability"
                verticalAlignment: VerticalAlignment.Center
                horizontalAlignment: HorizontalAlignment.Center

                textStyle.fontSizeValue: 0.0
                textStyle.lineHeight: 2.5
                textStyle.color: Color.DarkYellow
                textStyle.fontSize: FontSize.Large
            }
            ActivityIndicator {
                id:readabilityactivity
                horizontalAlignment: HorizontalAlignment.Center
                verticalAlignment: VerticalAlignment.Center
                preferredHeight: 100
                enabled: true
            }
            TextField {
                id: readablityinput
                input.submitKey: SubmitKey.Next
                input.submitKeyFocusBehavior: SubmitKeyFocusBehavior.Next
                inputMode: TextFieldInputMode.Default
                hintText: "Enter Username/Email"
                textStyle.color: Color.Yellow
                bottomMargin: 20.0
            }
            TextField {
                id: readablitypassword
                inputMode: TextFieldInputMode.Password
                hintText: "Enter Password"
                textStyle.color: Color.Yellow
                bottomMargin: 30.0
            }
            Label {
                gestureHandlers: TapHandler {
                    onTapped: {
                    
                    }
                }
                verticalAlignment: VerticalAlignment.Center
                horizontalAlignment: HorizontalAlignment.Right
                text: "Forgot Password?"
            }
            Button {
                horizontalAlignment: HorizontalAlignment.Fill
                text: "Get Credentials"
                onClicked: {
                    readabilityactivity.running = true
                    app.readabilityLogin(readablityinput.text,readablitypassword.text)
                }
            }
        }
   
}
    }


    attachedObjects: [
        SystemToast {
            id: success
            body: "Accounts added Sucessfully"
        },
        SystemToast {
            id: fail
            body: "Username and password doesn't match please try again"
        },
        ComponentDefinition {
            source: "../accounts.qml";
            id:acc
        }
    ] 
}
