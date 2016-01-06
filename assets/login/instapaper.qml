import bb.cascades 1.3
import bb.system 1.0

Page {
    property Page accpage
    function loginstate(info) {
        if (info == 1) {
            successinsta.show()
            navigationPane.pop()
            instapaperactivity.running = false
            instapaperactivity.visible=false
        } else {
            failinsta.show()
            instapaperactivity.running = false
            instapaperactivity.visible=false
        }
    }
    onCreationCompleted: {
        instapaperinput.requestFocus()
        app.instapaperloginstate.connect(loginstate)
    }
    ScrollView {

        Container {
            topPadding: ui.du(5)
            bottomPadding: ui.du(10)
            layout: DockLayout {

            }
            leftPadding: 100.0
            rightPadding: 100.0
            Container {
                
                verticalAlignment: VerticalAlignment.Center
                horizontalAlignment: HorizontalAlignment.Center
                ImageView {
                    verticalAlignment: VerticalAlignment.Center
                    horizontalAlignment: HorizontalAlignment.Center
                    imageSource: "asset:///Images/instapaper_real.png"
                    minHeight: 150
                    minWidth: 150
                    maxHeight: 150
                    maxWidth: 150

                }
                Label {
                    text: "Login to Instapaper"
                    verticalAlignment: VerticalAlignment.Center
                    horizontalAlignment: HorizontalAlignment.Center

                    textStyle.fontSizeValue: 0.0
                    textStyle.lineHeight: 2.5
                    textStyle.color: Color.DarkYellow
                    textStyle.fontSize: FontSize.Large
                }
                ActivityIndicator {
                    id: instapaperactivity
                    horizontalAlignment: HorizontalAlignment.Center
                    verticalAlignment: VerticalAlignment.Center
                    preferredHeight: 100
                    visible: false
                }
                TextField {
                    id: instapaperinput
                    input.submitKey: SubmitKey.Next
                    input.submitKeyFocusBehavior: SubmitKeyFocusBehavior.Next
                    inputMode: TextFieldInputMode.Default
                    hintText: "Enter Username/Email"
                    textStyle.color: Color.Yellow
                    bottomMargin: 20.0
                }
                TextField {
                    id: insatpaperpassword
                    inputMode: TextFieldInputMode.Password
                    hintText: "Enter Password"
                    textStyle.color: Color.Yellow
                    bottomMargin: 30.0
                    input.submitKey: SubmitKey.Submit
                    input {
                        onSubmitted: {
                            instapaperactivity.visible=true
                            instapaperactivity.running=true
                            app.instapaperLogin(instapaperinput.text, insatpaperpassword.text)
                        }
                    }
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
                    text: qsTr("Get Credentials")
                    onClicked: {
                        instapaperactivity.visible=true
                        instapaperactivity.running=true
                        app.instapaperLogin(instapaperinput.text, insatpaperpassword.text)
                    }
                }
            }
        }
        attachedObjects: [
            SystemToast {
                id: successinsta
                body: "Account added Sucessfully"
            },
            SystemToast {
                id: failinsta
                body: "Username and password doesn't match please try again"
            },
            ComponentDefinition {
                source: "../accounts.qml"
                id: accpage
            }
        ]
    }
}
