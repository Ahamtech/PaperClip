/* Copyright (c) 2012, 2013  BlackBerry Limited.
 * 
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 * 
 * http://www.apache.org/licenses/LICENSE-2.0
 * 
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

import bb.cascades 1.3
import "../js/ajaxmee.js" as Ajaxmee

Page {
    titleBar: TitleBar {
        id: pockettitle
        
    }
    property variant returnCode
    function startOAuth(inputcode) {
        var infor = {
            consumer_key: "36072-ebe38c859105c01114f5fe25",
            code: inputcode
        }
        console.log("----------------")
        console.log("" + JSON.stringify(infor))
        if (inputcode) {
            app.pocketAuthorise(inputcode)
        }
    }
    onCreationCompleted: {
        //        startOAuth()
        app.pocketcodeemit.connect(codereceived)
        
        app.pocketAccessTokenSuccess.connect(loginsuccess)
        app.pocketAccessTokenError.connect(loginerror)

    }

    function loginsuccess() {
        navigationPane.pop()
    }
    function logingerror() {

    }
    function codereceived(data) {
        returnCode = data
        var item = "https://getpocket.com/auth/authorize?request_token=" + data + "&redirect_uri=http://localhost"
        webWindow.url = item
    }
    Container {
        layout: DockLayout {

        }

        accessibilityMode: A11yMode.Default

        ScrollView {
            Container {
                layout: DockLayout {

                }
                onCreationCompleted: {
                    webWindow.storage.clear()
                }

                WebView {
                    id: webWindow

                    onCreationCompleted: {
                        storage.clear()
                        storage.clearCache()
                        storage.clearCookies()
                        storage.clearCredentials()
                    }
                    onUrlChanged: {

                        if (url.toString().indexOf("authorize") > 0 || url.toString().indexOf("approve_access") > 0) {
                            console.log(" if loop nav requ" + url)
                        } else {
                            console.log(" else loop" + url)
                            if (url.toString().indexOf("localhost") > 0) {
                                console.log("nav ready" + url)
                                startOAuth(returnCode)
                            }
                        }
                    }

                    onLoadProgressChanged: {
                        webviewprgoress.value = loadProgress / 100
                    }
                }

            }

        }

        ProgressIndicator {
            id: webviewprgoress
            value: 0.1
            horizontalAlignment: HorizontalAlignment.Fill
            verticalAlignment: VerticalAlignment.Bottom
            onProgressChanged: {
                if(progress>=0.87)
                {
                    pockettitle.title=""
                }
            }
            
        }
        Container {
            id: pocketloadpage
            verticalAlignment: VerticalAlignment.Center

            horizontalAlignment: HorizontalAlignment.Center
            ImageView {
                verticalAlignment: VerticalAlignment.Center
                horizontalAlignment: HorizontalAlignment.Center
                imageSource: "asset:///Images/pocket_tab.png"
                minHeight: 150
                minWidth: 150
                maxHeight: 150
                maxWidth: 150

            }
            Label {
                text: "Login to Pocket"
                verticalAlignment: VerticalAlignment.Center
                horizontalAlignment: HorizontalAlignment.Center

                textStyle.fontSizeValue: 0.0
                textStyle.lineHeight: 2.5
                textStyle.color: Color.DarkYellow
                textStyle.fontSize: FontSize.Large
            }
            Button {
                verticalAlignment: VerticalAlignment.Center
                horizontalAlignment: HorizontalAlignment.Center
                text: qsTr("Login Here") + Retranslate.onLanguageChanged
                onClicked: {
                    app.pocketlogin("as", "as");
                    pocketloadpage.visible=false
                    pockettitle.title=qsTr("please wait page is loading...") + Retranslate.onLanguageChanged
                }
            }
            Container {
                verticalAlignment: VerticalAlignment.Center
                horizontalAlignment: HorizontalAlignment.Center
                leftPadding: 60
                rightPadding: 60
                topPadding: 30
                
                Label {
                    text: qsTr("This will redirect to pocket website to authorise PAPERCLIP to access pocket.")
                    multiline: true

                }
            }
        }

    }

}
