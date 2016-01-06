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

#include "applicationui.hpp"
#include "readability/qtreadability.h"
#include <bb/cascades/Application>
#include <bb/cascades/GroupDataModel>
#include <bb/cascades/ProgressIndicator>
#include <bb/cascades/QmlDocument>
#include <bb/cascades/AbstractPane>
#include <bb/cascades/WebView>
#include <bb/cascades/LocaleHandler>
#include <bb/cascades/Header>
#include <bb/cascades/Shortcut>
#include <bb/cascades/GestureHandler>
#include <bb/cascades/ListScrollStateHandler>
#include <QtSql/QSqlDatabase>
#include <QtSql/QSqlQuery>
#include <QtSql/QSqlRecord>
#include <QtSql/QSqlError>
#include <QtSql/QtSql>
#include "database.hpp"
#include <qt4/QtCore/qdebug.h>
#include <bb/data/JsonDataAccess>
using namespace bb::cascades;
using namespace bb::data;

//Good Reads Keys
//key: WrXepjkcLeJ3hYtdzAnBQQ
//secret: 6c02plpnIsPD5Q7SCbYNiiC3umhP6MOKmKtOGSzqg



ApplicationUI::ApplicationUI() :
        QObject(), db(new Database(this)), readability(
                new QtReadability("kanishkablack", "EBBVwRtv96GresMV8bmQDZ7bvk8CpssF", this)), instapaper(
                new QtInstapaper("3d5d0d2b052d46889a0d0ac6ee414eaa",
                        "f432112a27694049a6c8b9c29af6c803", this)), pocket(
                new QtPocket("36072-ebe38c859105c01114f5fe25", this))
{
    // prepare the localization
    m_pTranslator = new QTranslator(this);
    m_pLocaleHandler = new LocaleHandler(this);

    bool res = QObject::connect(m_pLocaleHandler, SIGNAL(systemLanguageChanged()), this,
            SLOT(onSystemLanguageChanged()));
    // This is only available in Debug builds
    Q_ASSERT(res);
    // Since the variable is not used in the app, this is added to avoid a
    // compiler warning
    Q_UNUSED(res);

    // initial load
    onSystemLanguageChanged();

//Database class making
    ok = db->initDatabase();
    // Create scene document from main.qml asset, the parent is set
    // to ensure the document gets destroyed properly at shut down.
    QmlDocument *qml = QmlDocument::create("asset:///main.qml").parent(this);
    qml->setContextProperty("app", this);
    // Create root object for the UI
    AbstractPane *root = qml->createRootObject<AbstractPane>();

    // Set created root object as the application scene
    Application::instance()->setScene(root);
    db->getRowCount("auths");
    switchme();
    connect(pocket, SIGNAL(requestTokenReceived(QString)), this, SIGNAL(pocketcodeemit(QString)));
    connect(pocket, SIGNAL(accessTokenReceived(QMap<QString,QString>)), this,
            SLOT(pocketAccessTokenReceived(QMap<QString,QString>)));
    connect(readability, SIGNAL(accessTokenReceived(QMap<QString,QString>)), this,
            SLOT(readabilityAccessTokenReceived(QMap<QString,QString>)));
    connect(instapaper, SIGNAL(accessTokenReceived(QMap<QString,QString>)), this,
            SLOT(instapaperAccessTokenReceived(QMap<QString,QString>)));
    connect(readability, SIGNAL(responseReceived(QByteArray,QString,qint32)), this,
                            SLOT(readabilityresponsereceived(QByteArray,QString,qint32)));
    connect(pocket, SIGNAL(responseReceived(QByteArray)), this,
                                SLOT(pocketresponsereceived(QByteArray)));
}

void ApplicationUI::onSystemLanguageChanged()
{
    QCoreApplication::instance()->removeTranslator(m_pTranslator);
    // Initiate, load and install the application translation files.
    QString locale_string = QLocale().name();
    QString file_name = QString("PaperClip_%1").arg(locale_string);
    if (m_pTranslator->load(file_name, "app/native/qm")) {
        QCoreApplication::instance()->installTranslator(m_pTranslator);
    }
}

void ApplicationUI::readabilityLogin(QString username, QString password)
{
    readability->xAuthLogin(username, password);

}

void ApplicationUI::instapaperLogin(QString username, QString password)
{
    instapaper->xAuthLogin(username, password);

}

void ApplicationUI::pocketlogin(QString username, QString password)
{

    pocket->getRequestToken();

}

void ApplicationUI::readabilityAccessTokenReceived(QMap<QString, QString> response)
{

    if (response.size() > 0 && !response.value("oauth_token").isNull()
            && !response.value("oauth_token_secret").isNull()) {
        if (ok) {
            QVariantMap data;
            data["token"] = response.value("oauth_token");
            data["tokensecret"] = response.value("oauth_token_secret");
            data["auth"] = 1;
            data["service"] = "readability";
            readability->setToken(response.value("oauth_token"));
            readability->setTokenSecret(response.value("oauth_token_secret"));
            bool insert =
                    db->insertQuery(
                            "insert into auths(service,auth,token,tokensecret)values(:service,:auth,:token,:tokensecret)",
                            data);
            if (!insert) {
                Q_EMIT readabilityloginstate(1);
                qDebug() << "data inserted successfully";
            }
            switchme();
            getUserInfo();
            getReadabilityInitial();
        }
    } else
        Q_EMIT readabilityloginstate(0);
}
;
void ApplicationUI::instapaperAccessTokenReceived(QMap<QString, QString> response)
{
    Q_FOREACH(QString key,response.keys()){
    qDebug() << "date " << key  << response.value(key);
}

    if (response. size() > 0 && !response. value( "oauth_token" ).isNull ()
           && !response. value( "oauth_token_secret" ).isNull ()) {
       if ( ok) {
            QVariantMap data;
            data["token"] = response.value("oauth_token");
            data["tokensecret"] = response.value("oauth_token_secret");
            data["auth"] = 1;
            data["service"] = "instapaper";
            instapaper->setToken(response.value("oauth_token"));
            instapaper->setTokenSecret(response.value("oauth_token_secret"));
            bool insert =
                    db->insertQuery(
                            "insert into auths(service,auth,token,tokensecret)values(:service,:auth,:token,:tokensecret )",
                            data);
            if (!insert) {
                Q_EMIT instapaperloginstate(1);
            }
            switchme();
            instapaper->getUserInfo();
       }
   } else
       Q_EMIT instapaperloginstate (0);
}
;
void ApplicationUI::pocketAccessTokenReceived(QMap<QString, QString> response)
{

    if (response.size() > 0 && !response.value("access_token").isNull()) {
        if (ok) {
            QVariantMap data;
            data["token"] = response.value("oauth_token");

            data["auth"] = 1;
            data["username"] = response.value("username");
            data["service"] = "pocket";
            bool insert =
                    db->insertQuery(
                            "insert into auths(service,username,auth,token)values(:service,:username,:auth,:token)",
                            data);
            if (!insert) {
                qDebug() << "data inserted successfully";
                Q_EMIT pocketAccessTokenSuccess();
            } else {
                Q_EMIT pocketAccessTokenError();
            }
            switchme();
            getPocketInital();
        }
    } else {
        Q_EMIT pocketAccessTokenError();
    }
}
;

void ApplicationUI::pocketAuthorise(QString code)
{
    if (code != "") {
        pocket->getAccessToken(code);
    }
}
void ApplicationUI::loadAuths()
{
    QString sstr = "select * from auths";
    QVariant v = db->executeQuery(sstr);
    qDebug() << "Query Execured";
    Q_EMIT loadAuthstoQML(v);
}
void ApplicationUI::switchme()
{
    QSqlQuery query = db->executeSqlQuery("select * from auths");
    while (query.next()) {
        QString service = query.value(0).toString();
        QStringList myOptions;
        int tokenIndex = query.record().indexOf("token");
        int tokenSecretIndex = query.record().indexOf("tokensecret");
        myOptions << "readability" << "pocket" << "instapaper";
        switch (myOptions.indexOf(service)) {
            case 0:
                readability->setToken(query.value(tokenIndex).toString());
                readability->setTokenSecret(query.value(tokenSecretIndex).toString());
                break;
            case 1:
                pocket->setToken(query.value(tokenIndex).toString());
                break;
            case 2:
                instapaper->setToken(query.value(tokenIndex).toString());
                instapaper->setTokenSecret(query.value(tokenSecretIndex).toString());
                break;
        }
    }
}

void ApplicationUI::getUserInfo()
{
    readability->getUserInfo();
}
void ApplicationUI::readabilityresponsereceived(QByteArray buffer, QString url, qint32 error)
{
    qDebug() << error << url;
if(error == 200 ){
    JsonDataAccess jda;
    QVariant databuffer = jda.loadFromBuffer(buffer);
    QVariantMap map = databuffer.value<QVariantMap>();

    if (url.contains("initial")) {
        Q_EMIT readabilityInitial(databuffer);
    } else if (url.contains("bookmarks")) {
//        qDebug() << "total article found";
        Q_EMIT readabilityBookmark(databuffer);
    } else if (url.contains("_current")) {
        qDebug() << "Getting Current User Info";
        db->updateusername(map.value("username").toString(), "readability");
        Q_EMIT readabilityloginstate(1);
    } else if (url.contains("article")) {
      Q_EMIT readablityarticle(databuffer);
    }
}
else{
    switch (error){
        case 204:{
            qDebug() << url;
            QStringList list  = url.split("/");
            QString listval = QString(list.value(list.size()-1));
            Q_EMIT deletearticled(qint32(listval.toInt()));
            db->executeQuery(QString("delete from content where bookid=\""+listval+"\""));
        }
    }
}
}
int ApplicationUI::getNumberOfAccounts()
{
    return db->getRowCount("auths");
}

int ApplicationUI::getNumberOfArticles()
{
    return db->getRowCount("content");
}
int ApplicationUI::getNumberOfQueues()
{
    return db->getRowCount("queue");
}

void ApplicationUI::getReadabilityInitial()
{
    QtReadabilityParams filters;
    filters.insert("page", "1");
    filters.insert("per_page", "5");
    filters.insert("initial", "5");
    readability->getBookmarks(filters);
}
void ApplicationUI::insertPageToQueue(QString service, int addeddate, QString type, int content)
{
    QVariantMap data;
    data.insert("service", service);
    data.insert("addeddate", addeddate);
    data.insert("type", type);
    data.insert("content", content);
    db->insertQueue(data, type);
}
void ApplicationUI::insertArticleToQueue(QString service, int addeddate, QString type, int id)
{
    QVariantMap data;
    data.insert("service", service);
    data.insert("addeddate", addeddate);
    data.insert("type", type);
    data.insert("id", id);
    db->insertQueue(data, type);
}
void ApplicationUI::lastUpdateDate(QString service, int num)
{
    qDebug() << service << num;
    db->updateLastUpdate(service, num);
    QSqlQuery query = db->executeSqlQuery("select *from auths");
    while (query.next()) {
        qDebug() << query.value(0) << query.value(4).toString();
    }
}
void ApplicationUI::insertArticleMetaData(QString id, QString service, int date, QString title,
        QString url, QString snippet, QString image, bool favourite, bool archive,QString author,QString wordcount,QString bookid)
{
    QVariantMap cmap;
    cmap.insert("id", id);
    cmap.insert("favourite", favourite);
    cmap.insert("archive", archive);
    cmap.insert("service", service);
    cmap.insert("date", date);
    cmap.insert("title", title);
    cmap.insert("url", url);
    cmap.insert("snippet", snippet);
    cmap.insert("image", image);
    cmap.insert("author", author);
    cmap.insert("wordcount", wordcount);
    cmap.insert("bookid", bookid);
    int count = db->getTableSizeByQuery(QString("select count(*) from content where id = \""+id+"\""));
    if(count>0)
    db->insertQuery("update content set favourite = :favourite ,archive=:archive,service=:service,date=:date,title=:title,url=:url,snippet=:snippet,image=:image,author=:author,wordcount=:wordcount,bookid=:bookid where id = :id",cmap);
    else
    db->insertContent(cmap);

}
void ApplicationUI::flushQueue()
{
    db->executeSqlQuery("delete from queue");
}
void ApplicationUI::updateArticleIntoContent(QString id, QString message)
{
    QVariantMap data;
    data.insert("id", id);
    data.insert("message", message);
    db->insertQuery("update content set message = :message where id = :id",data);
    Q_EMIT articleUpdate(id);
}
QVariant ApplicationUI::queueTop()
{
    return db->executeQuery("select * from queue order by date desc limit 1");
}
void ApplicationUI::fetchReadabilityBookmark(int page)
{
    QtReadabilityParams filters;
    filters.insert("page", QString::number(page));
    filters.insert("per_page", "50");
    readability->getBookmarks(filters);
}
void ApplicationUI::fetchReadabilityArticle(QString id)
{
    readability->getArticle(id);
}
void ApplicationUI::deleteQueue(QString service, QString count)
{
    QString query = "delete from queue where service = \"";
    query.append(service);
    query.append("\" and content = \"");
    query.append(count);
    query.append("\"");
    db->executeQuery(query);
    qDebug() << "this is query" << query;
}
QVariantList ApplicationUI::getArticlesModel(QString offset,QString nullstring,QString type)
{
    if(!nullstring.isEmpty()){

        if(!type.isEmpty()){
            QVariant var = db->executeQuery(QString("select * from content where service = \""+nullstring+"\" and archive = \"+  +\" order by date desc limit 30 offset %1").arg(offset));
        }
        else{
            QVariant var = db->executeQuery(QString("select * from content where service = \""+nullstring+"\" and archive = \"false\" order by date desc limit 30 offset %1").arg(offset));
        }
    }
    QVariant var = db->executeQuery(QString("select * from content order by date desc limit 30 offset %1").arg(offset));
// Add the data to the model
    return var.value<QVariantList>();
}
void ApplicationUI::flushContent()
{
    db->executeQuery("delete from content");
}
bool ApplicationUI::isAccountExist(QString accname){
    int count = db->getTableSizeByQuery("select count(*) from auths where service = \""+accname+"\"");
    return count>0 ? true:false;
}

int ApplicationUI::getTotalAccounts(){
    return db->getTableSize("auths");
}
void ApplicationUI::deleteAccount(QString service){
    QVariantMap map;
    map.insert("service",service);
db->insertQuery("delete from auths where service = :service",map);
db->insertQuery("delete from content where service = :service",map);
db->insertQuery("delete from queue where service = :service",map);

            Q_EMIT authsupdated();
}
QVariant ApplicationUI::getDataById(QString id){
  return db->executeQuery("select * from content where id = \""+id+"\"");
}

void ApplicationUI::getUpdates(QString time,QString type){
    if(type=="instapaper"){

    }
    if(type=="readability"){
        QtReadabilityParams filters;
        filters.insert("updated_since", time);
        filters.insert("per_page", "100");
        readability->getBookmarks(filters);
    }
    if(type=="pocket"){}
}
QVariant ApplicationUI::getAuths(){
    return db->executeQuery("select * from auths");
}
QString ApplicationUI::getCurrentLanguage(){
    return QLocale().languageToString(QLocale().language());
}
void ApplicationUI::archiveArticle(QString id,QString type){
    if(type=="instapaper"){

        }
        if(type=="readability"){
            readability->updateBookmark(id,0,1,0);
            db->insertQuery(QString("update content set archive = 1 where bookid = \""+id+"\""),QVariantMap());
            Q_EMIT archiveArticle(id);
        }
        if(type=="pocket"){}
}
void ApplicationUI::deleteArticle(QString id,QString type){
    if(type=="instapaper"){

        }
        if(type=="readability"){
            db->insertQuery(QString("delete from content where id = \""+id+"\""),QVariantMap());
            readability->deleteBookmark(id);
        }
        if(type=="pocket"){}
}
void ApplicationUI::favouriteArticle(QString id,QString type){
    if(type=="instapaper"){

        }
        if(type=="readability"){
            readability->updateBookmark(id,1,0,0);
            db->insertQuery(QString("update content set favourite = \"true\" where bookid = \""+id+"\""),QVariantMap());
            Q_EMIT favArticle(id);
        }
        if(type=="pocket"){}
}

void ApplicationUI::getPocketInital(){
    pocket->getInitial();
}

//void ApplicationUI::pocketresponsereceived(QByteArray buffer){
//
//}
