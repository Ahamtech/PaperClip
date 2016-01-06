/*
 * Copyright (c) 2011-2014 BlackBerry Limited.
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
import bb.system 1.0
import "js/moment.js" as Moment
NavigationPane {
    id: navigationPane
    property int incoming
    Menu.definition: MenuDefinition {
        // Specify the actions that should be included in the menu
        actions: [
            ActionItem {
                title: "About"
                imageSource: "images/actionOneIcon.png"
            },
            ActionItem {
                title: "Accounts"
                onTriggered: {
                    navigationPane.push(accounts.createObject());
                }
            }
        ]
    }
    onPopTransitionEnded: {
        page.destroy();
    }
    onCreationCompleted: {
        app.readabilityInitial.connect(redinitial)
        app.readabilityBookmark.connect(redparseboomarks)
        app.readablityarticle.connect(redparsearticle)
        app.authsupdated.connect(getallarticles)

        getUpdates(app.getAuths())
    }
    Page {
        onCreationCompleted: {
            getallarticles();
            app.deletearticled.connect(deletearticl)
        }
        function deletearticl(id) {
            var delat = 0
            var delid;
            var size = mainlist.dataModel.size()
            console.log("delete ")
            for (var i = 0; i < size; i ++) {
                if (mainlist.dataModel.value(i).id == id || mainlist.dataModel.value(i).bookid == id) {
                    mainlist.dataModel.removeAt(i)
                }
            }
        }
        actions: [
            ActionItem {
                id: additem
                ActionBar.placement: ActionBarPlacement.InOverflow
                title: "Add"
                imageSource: "asset:///Images/BBicons/ic_add.png"
                enabled: true
            },
            ActionItem {
                ActionBar.placement: ActionBarPlacement.InOverflow
                title: "Refresh"
                enabled: true
            }
        ]
        titleBar: TitleBar {
            id: topacc
            onSelectedOptionChanged: {
                getallarticles()
            }
            kind: TitleBarKind.Segmented
            options: [
                Option {
                    text: "All"
                    value: "All"

                },
                Option {
                    text: "Pocket"
                    value: "Pocket"
                },
                Option {
                    text: "Instapaper"
                    value: "Instapaper"
                    image: "asset:///Images/instapaper_tab.png"
                },
                Option {
                    value: "Readability"
                    text: "Readability"
                }
            ]
        }
        Container {
            layout: DockLayout {

            }
            Container {
                id: progressbar
                Label {
                    text: "help"
                }
                visible: false
            }
            Container {
                verticalAlignment: VerticalAlignment.Center
                horizontalAlignment: HorizontalAlignment.Center
                implicitLayoutAnimationsEnabled: false
                Container {
                    leftPadding: ui.du(2)
                    rightPadding: ui.du(2)
                    topPadding: ui.du(1)
                    bottomPadding: ui.du(2)
                    Container {
                        id: pocketdrop
                        visible: false
                        DropDown {
                            id: pocketdropdown
                            visible: app.isAccountExist("pocket")
                            onSelectedOptionChanged: {
                                switch (selectedOption.value) {
                                    case "My List":
                                        break
                                    case "Favorites":
                                        break
                                    case "Archive":
                                        break
                                    case "All":
                                        break
                                    case "Articles":
                                        break
                                    case "Images":
                                        break
                                    case "Videos":
                                        break
                                }
                            }
                            title: "Select Category"
                            options: [
                                Option {
                                    text: "My List"
                                    value: "MyList"
                                },
                                Option {
                                    text: "Favorites"
                                    value: "Favorites"
                                },
                                Option {
                                    text: "Archive"
                                    value: "Archive"
                                },
                                Option {
                                    text: "All"
                                    value: "All"
                                },
                                Option {
                                    text: "Articles"
                                    value: "Articles"
                                },
                                Option {
                                    text: "Images"
                                    value: "Images"
                                },
                                Option {
                                    text: "Videos"
                                    value: "Videos"
                                }
                            ]
                        }
                    }
                    Container {
                        id: readablitydrop
                        visible: false
                        DropDown {
                            maxHeight: 1
                            onSelectedOptionChanged: {
                                switch (selectedOption.value) {
                                    case "All":
                                        break
                                    case "Archive":
                                        break
                                    case "Favorites":
                                        break
                                }
                            }
                            title: "Select Category"
                            options: [
                                Option {
                                    text: "All"
                                    value: "All"
                                },
                                Option {
                                    text: "Archive"
                                    value: "Archive"
                                },
                                Option {
                                    text: "Favorites"
                                    value: "Favorites"
                                }

                            ]
                        }
                    }
                    Container {
                        id: instapaperdrop
                        visible: false
                        DropDown {
                            onSelectedOptionChanged: {
                                switch (selectedOption.value) {
                                    case "Favorites":
                                        break
                                    case "Archive":
                                        break
                                }
                            }
                            title: "Select Category"
                            options: [
                                Option {
                                    text: "Favorites"
                                    value: "Favorites"
                                },
                                Option {
                                    text: "Archive"
                                    value: "Archive"
                                }

                            ]
                        }
                    }
                }
                ListView {
                    attachedObjects: [
                        // This handler is tracking the scroll state of the ListView.
                        ListScrollStateHandler {
                            id: scrollStateHandler
                            onScrollingChanged: {
                                if (scrolling) {
                                    if (atEnd)
                                        renderdata(1);
                                }
                            }
                        }
                    ]
                    id: mainlist
                    visible: false
                    dataModel: ArrayDataModel {
                    }
                    scrollRole: ScrollRole.Main
                    onTriggered: {
                        var atpage = articleviewpage.createObject()
                        var item = mainlist.dataModel.value(indexPath)
                        navigationPane.push(atpage)
                        atpage.id = item.id
                        atpage.type = item.service
                    }
                    function deleteArt(data) {
                        var delat = 0
                        var size = mainlist.dataModel.size()
                        for (var i = 0; i < size; i ++) {
                            if (mainlist.dataModel.value(i).id == data.id) {
                                delat = i
                            }
                        }
                        mainlist.dataModel.removeAt(delat)
                        switch (data.service) {
                            case "pocket":
                                break
                            case "readability":
                                app.deleteArticle(data.bookid, data.service)
                                break
                            case "instapaper":
                                break
                        }

                    }
                    function favart(data) {
                        var size = mainlist.dataModel.size()
                        for (var i = 0; i < size; i ++) {
                            if (mainlist.dataModel.value(i).id == data.id) {
                                var inf = mainlist.dataModel.value(i)
                                inf.favourite = "true"
                                mainlist.dataModel.replace(i, inf)
                            }
                        }
                        switch (data.service) {
                            case "pocket":
                                break
                            case "readability":
                                app.favouriteArticle(data.bookid, data.service)
                                break
                            case "instapaper":
                                break
                        }
                    }
                    function archart(data) {
                        //                        var size = mainlist.dataModel.size()
                        //                        for (var i = 0; i < size; i ++) {
                        //                            if (mainlist.dataModel.value(i).id == data.id) {
                        //                                var inf = mainlist.dataModel.value(i)
                        //                                info.fav = true
                        //                                mainlist.dataModel.replace(i, info)
                        //                            }
                        //                        }
                        switch (data.service) {
                            case "pocket":
                                break
                            case "readability":
                                app.archiveArticle(data.bookid, data.service)
                                break
                            case "instapaper":
                                break
                        }
                    }
                    listItemComponents: [
                        ListItemComponent {

                            type: ""
                            Article {
                                id: artcile
                                text: ListItemData.title
                                des: ListItemData.snippet
                                type: ListItemData.service
                                time: ListItemData.wordcount
                                favitem: if (ListItemData.favourite == "true") {
                                    return true
                                } else {
                                    return false
                                }
                                read: if (ListItemData.read == "true") {
                                    return true
                                } else {
                                    return false
                                }
                                contextActions: [

                                    ActionSet {
                                        id: longtap
                                        ActionItem {
                                            title: "Share"
                                            imageSource: "asset:///Images/BBicons/ic_share.png"
                                            onTriggered: {
                                                longtap.add()
                                            }
                                        }
                                        ActionItem {
                                            title: "Favorite"
                                            imageSource: "asset:///Images/BBicons/ic_favorite.png"
                                            onTriggered: {
                                                artcile.ListItem.view.favart(ListItemData)
                                            }
                                        }
                                        ActionItem {
                                            title: "Archive"
                                            imageSource: "asset:///Images/BBicons/ic_entry.png"
                                            onTriggered: {
                                                artcile.ListItem.view.archart(ListItemData)
                                            }
                                        }
                                        ActionItem {
                                            title: "Edit Tags"
                                            imageSource: "asset:///Images/BBicons/ic_edit.png"
                                        }
                                        ActionItem {
                                            title: "Delete"
                                            imageSource: "asset:///Images/BBicons/ic_delete.png"
                                            onTriggered: {
                                                artcile.ListItem.view.deleteArt(ListItemData)
                                            }
                                        }
                                    }
                                ]
                            }
                        }
                    ]

                }
            }

            Container {
                id: emptypages
                horizontalAlignment: HorizontalAlignment.Center
                verticalAlignment: VerticalAlignment.Center

                Button {
                    text: "Add Account"
                    onClicked: {
                        navigationPane.push(accountspage.createObject())
                    }
                }
            }
        }
        attachedObjects: [
            ComponentDefinition {
                id: articleviewpage
                source: qsTr("asset:///OpenArticle.qml")
            },
            ComponentDefinition {
                id: accountspage
                source: qsTr("asset:///accounts.qml")
            },
            ComponentDefinition {
                id: readabilityloginpage
                source: qsTr("asset:///login/readability.qml")
            },
            ComponentDefinition {
                id: instapaperloginpage
                source: qsTr("asset:///login/instapaper.qml")
            },
            ComponentDefinition {
                id: pocketloginpage
                source: qsTr("asset:///login/pocket.qml")
            }
        ]
        actionBarVisibility: ChromeVisibility.Compact
    }
    attachedObjects: [
        // Definition of the second Page, used to dynamically create the Page above.
        ComponentDefinition {
            id: accounts
            source: "accounts.qml"
        },
        SystemToast {
            id: toast
        }
    ]
    function getUpdates(data) {
        for (var i = 0; i < data.length; i ++) {
            console.log()
            app.getUpdates(Moment.moment(data[i].lastupdate).format("YYYY-MM-DD HH:MM:SS"), data[i].service)
        }
    }
    function getallarticles() {
        var count = app.getNumberOfAccounts()
        console.log("Get Nmumber of Accounts " + count)
        if (count > 0) {
            var sel = topacc.selectedValue
            console.log(sel)
            emptypages.visible = false
            pocketdrop.visible = false
            readablitydrop.visible = false
            instapaperdrop.visible = false
            switch (sel) {
                case "Instapaper":
                    if (app.isAccountExist("instapaper")) {
                        mainlist.visible = true
                        emptypages.visible = false
                        instapaperdrop.visible = true
                        renderdata()
                    } else {
                        emptypages.visible = true
                        mainlist.visible = false
                    }
                    break;
                case "Pocket":
                    if (app.isAccountExist("pocket")) {
                        mainlist.visible = true
                        emptypages.visible = false
                        pocketdrop.visible = true
                        renderdata()
                    } else {
                        emptypages.visible = true
                        mainlist.visible = false
                        console.log(mainlist.visible, emptypages.visible)
                    }
                    break;
                case "Readability":
                    if (app.isAccountExist("readability")) {
                        mainlist.visible = true
                        emptypages.visible = false
                        readablitydrop.visible = true
                        renderdata()
                    } else {
                        emptypages.visible = true
                        mainlist.visible = true
                    }
                    break;
                case "All":
                    emptypages.visible = false
                    mainlist.visible = true
                    renderdata()
            }
        } else {
            emptypages.visible = true
            mainlist.visible = false
        }
    }
    function renderdata(next) {
        var sel = topacc.selectedValue
        console.log("Rendering Data " + sel + next)
        if (sel == "All") {
            if (next == 1) {
                mainlist.dataModel.append(app.getArticlesModel(mainlist.dataModel.size(), "", ""))
            } else {
                mainlist.dataModel.clear()
                mainlist.dataModel.append(app.getArticlesModel(0, "", ""))
            }
        } else {
            var query
            switch (sel) {
                case "All":
                    {

                    }
            }
            if (next) {

            } else {
                mainlist.dataModel.clear()
                mainlist.dataModel.append(app.getArticlesModel(0, "", ""))
            }
        }
    }
    function redinitial(data) {
        console.log("Readablity - List of All Articles : ", data.meta.item_count_total)
        app.lastUpdateDate("readability", Moment.moment().unix());
        incoming += data.meta.item_count_total
        var pagescount = Math.ceil(data.meta.item_count_total / 50);
        for (var i = 1; i < pagescount + 1; i ++) {
            app.insertPageToQueue("readability", Moment.moment().unix(), "page", i);
        }
        startqueue()
    }
    function redparseboomarks(data) {
        console.log(JSON.stringify(data))
        //TODO after archive
        console.log("Readablity - In Current Page : " + data.bookmarks.length)
        for (var i = 0; i < data.bookmarks.length; i ++) {
            var temp = data.bookmarks[i];
            app.insertArticleMetaData(temp.article.id, "readability", Moment.moment(temp.date_added).unix(), temp.article.title, temp.article.url, temp.article.excerpt, temp.article.lead_image_url, temp.favorite, temp.archive, temp.article.author, Math.ceil(temp.article["word_count"] / 240), temp.id);
        }
        app.deleteQueue("readability", data.meta.page)
        incoming -= data.meta.item_count_total
        startqueue()
    }
    function redparsearticle(data) {
        console.log("Redability Parsing Article : " + data.id)
        app.updateArticleIntoContent(data.id, data.content)
    }
    function flushQueue() {
        app.flushQueue();
        var queuecount = app.getNumberOfQueues();
        showtoast("Number Of items in the Queue " + queuecount)
    }
    function showtoast(title) {
        toast.body = title
        toast.show()
    }
    function startqueue() {
        var data = app.queueTop();
        var item = data[0]

        if (item) switch (item.service) {
            case "readability":
                if (item.type == "page") {
                    console.log("Fetching Articles from page" + item.content)
                    app.fetchReadabilityBookmark(item.content)
                } else {
                    app.fetchReadabilityArticle(item.id)
                }
                break;
        }
    }
    
}
