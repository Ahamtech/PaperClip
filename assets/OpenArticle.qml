import bb.cascades 1.3
import "js/moment.js" as Moment
Page {
    property variant id
    property variant type
    property variant bookid
    onCreationCompleted: {
        app.articleUpdate.connect(render)
        app.deletearticled.connect(deletearticle)
    }
    onTypeChanged: {
        render()
    }
    function deletearticle(delid){
        if(bookid == delid){
            navigationPane.pop()
        }
    }
    function render(){
        var data = app.getDataById(id);
        var info = data[0]
        title.text = info.title
        arturl.text = info.url
        readtime.text = info.wordcount
        author.text = info.author
        savedtime.text = Moment.moment.unix(info.date).format("D-M-YYYY hh:mm")
        console.log(info.message.length)
        bookid = info.bookid
        if (info["message"].length > 0) {
            message.html = info.message
        } else {
            app.fetchReadabilityArticle(id)
        }
    }
    actionBarVisibility: ChromeVisibility.Compact
    actionBarAutoHideBehavior: ActionBarAutoHideBehavior.HideOnScroll
    Container {
        Container {
            background: Color.Red
            Divider {

            }
        }
        Container {
            leftPadding: 20
            Container {
                layout: StackLayout {
                    orientation: LayoutOrientation.LeftToRight
                }
                horizontalAlignment: HorizontalAlignment.Right
                Container {
                    ImageView {
                        imageSource: "asset:///Images/BBicons/ic_all.png"
                        maxHeight: 40
                        maxWidth: 40
                    }
                }
                Container {
                    Label {
                        id: savedtime
                    }
                }
                Container {
                    ImageView {
                        imageSource: "asset:///Images/BBicons/ic_doctype_generic.png"
                        maxHeight: 40
                        maxWidth: 40
                    }
                }
                Container {
                    Label {
                        id: readtime
                    }

                }
            }
            Container {
                Label {
                    id: title
                    multiline: true
                }
            }
            Container {
                Container {
                    layout: StackLayout {
                        orientation: LayoutOrientation.LeftToRight
                    }
                    Label {
                        layoutProperties: StackLayoutProperties {
                            spaceQuota: + 1
                        }
                        id: arturl
                    }
                    Label {
                        layoutProperties: StackLayoutProperties {
                            spaceQuota: -1
                        }
                        id: author
                    }
                }
            }
            Container {
                visible: false
                horizontalAlignment: HorizontalAlignment.Right
                Label {
                    multiline: true
                    textStyle.fontSize: FontSize.XSmall
                }
            }
            Divider {

            }
            Container {
                ScrollView {
                    id: scrollView
                    scrollViewProperties {
                        scrollMode: ScrollMode.Both
                        pinchToZoomEnabled: true
                    }
                    WebView {
                        id: message
                        onMinContentScaleChanged: {
                            scrollView.scrollViewProperties.minContentScale = minContentScale;
                        }
                        
                        onMaxContentScaleChanged: {
                            scrollView.scrollViewProperties.maxContentScale = maxContentScale;
                        }
                    
                    }
                }
            }
        }

    }
    actions: [
        ActionItem {
            title: "Archive"
            onTriggered: {
                app.archiveArticle(bookid,type)
            }
        },
        ActionItem {
            title: "delete"
            onTriggered: {
                app.deleteArticle(bookid,type)
            }
        },
        ActionItem {
            title: "Fav"
            onTriggered: {
                app.archiveArticle(bookid,type)
            }
        }
    ]
}
