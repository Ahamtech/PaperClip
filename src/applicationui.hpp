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

#ifndef ApplicationUI_HPP_
#define ApplicationUI_HPP_

#include <QObject>
#include <QMap>
#include "readability/qtreadability.h"
#include "pocket/qtpocket.h"
#include "instapaper/qtinstapaper.h"
#include <bb/cascades/Page>
#include <bb/cascades/GroupDataModel>
#include <bb/cascades/WebView>
#include "database.hpp"
#include <QtSql/QSqlDatabase>
#include <QtSql/QSqlQuery>
#include <QtSql/QSqlRecord>
#include <QtSql/QSqlError>
#include <QtSql/QtSql>
namespace bb
{
    namespace cascades
    {
        class LocaleHandler;

    }
}
using namespace bb::cascades;
class GroupDataModel;
class QTranslator;
class QtReadability;
class QtInstapaper;
class QtPocket;
class Database;
class WebView;
class QSqlQuery;
/*!
 * @brief Application UI object
 *
 * Use this object to create and init app UI, to create context objects, to register the new meta types etc.
 */
class ApplicationUI: public QObject
{
Q_OBJECT
public:
    ApplicationUI();
    virtual ~ApplicationUI()
    {
    }
    Q_INVOKABLE void readabilityLogin(QString username, QString password);
    Q_INVOKABLE void pocketlogin(QString username, QString password);
    Q_INVOKABLE void instapaperLogin(QString username, QString password);
    Q_INVOKABLE void loadAuths();
    Q_INVOKABLE void getUserInfo();
    Q_INVOKABLE void deleteAccount(QString);
    Q_INVOKABLE int getNumberOfAccounts();
    Q_INVOKABLE int getNumberOfArticles();
    Q_INVOKABLE int getNumberOfQueues();
    Q_INVOKABLE void getReadabilityInitial();
    Q_INVOKABLE void lastUpdateDate(QString service,int num);
    Q_INVOKABLE void insertPageToQueue(QString service,int addeddate,QString type,int content );
    Q_INVOKABLE void insertArticleToQueue(QString service,int addeddate,QString type,int id );
    Q_INVOKABLE void insertArticleMetaData(QString id,QString service,int date,QString title,QString url,QString snippet,QString image,bool favourite,bool archive,QString author,QString wordcount,QString bookid);
    Q_INVOKABLE void flushQueue();
    Q_INVOKABLE QVariant queueTop();
    Q_INVOKABLE void updateArticleIntoContent(QString id ,QString message);
    Q_INVOKABLE void fetchReadabilityBookmark(int);
    Q_INVOKABLE void fetchReadabilityArticle(QString);
    Q_INVOKABLE void deleteQueue(QString service,QString count);
    Q_INVOKABLE void flushContent();
    Q_INVOKABLE void pocketAuthorise(QString);
    Q_INVOKABLE QVariant getDataById(QString);
    Q_INVOKABLE bool isAccountExist(QString);
    Q_INVOKABLE int getTotalAccounts();
    Q_INVOKABLE void getUpdates(QString time,QString type);
    Q_INVOKABLE void deleteArticle(QString,QString);
    Q_INVOKABLE void archiveArticle(QString,QString);
    Q_INVOKABLE void favouriteArticle(QString,QString);


    Q_INVOKABLE QVariant getAuths();
    Q_INVOKABLE QString getCurrentLanguage();

//    Q_INVOKABLE void updateArticleToQueue();
    QtReadability *readability;
    QtPocket *pocket;
    QtInstapaper *instapaper;
    Database *db;
    QString pocketKey,pocketSecret,readabilityKey,readabilitySecret,instapaperKey,instapaperSecret;
    Q_INVOKABLE QVariantList getArticlesModel(QString,QString,QString);
    void switchme();
    void getPocketInital();

    signals:
    void readabilityloginstate(bool);
    void instapaperloginstate(bool);
    void loadAuthstoQML(QVariant);
    void authsupdated();
    void readabilityInitial(QVariant);
    void readabilityBookmark(QVariant);
    void pocketcodeemit(QString);
    void pocketAccessTokenError();
    void pocketAccessTokenSuccess();
    void readablityarticle(QVariant);
    void articleUpdate(QString);
    void deletearticled(qint32);
    void favArticle(QString);
    void archiveArticle(QString);

private slots:
    void onSystemLanguageChanged();

private:
    QTranslator* m_pTranslator;
    bb::cascades::LocaleHandler* m_pLocaleHandler;
    bool ok;
private Q_SLOTS:
//    void readablityaccessTokenReceived(QMap<QString, QString> response);
    void pocketAccessTokenReceived(QMap<QString,QString>);
    void instapaperAccessTokenReceived(QMap<QString,QString>);
    void readabilityAccessTokenReceived(QMap<QString,QString> response);
    void readabilityresponsereceived(QByteArray,QString,qint32);


};

#endif /* ApplicationUI_HPP_ */
