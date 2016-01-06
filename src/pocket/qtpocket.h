/*
*	QtPocket - A Pocket.com library for Qt
*
*	Copyright (c) 2014 Zoltán Benke (benecore@devpda.net)
*                      	 http://devpda.net
*
*	The MIT License (MIT)
*
*	Permission is hereby granted, free of charge, to any person obtaining a copy of
*	this software and associated documentation files (the "Software"), to deal in
*	the Software without restriction, including without limitation the rights to
*	use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of
*	the Software, and to permit persons to whom the Software is furnished to do so,
*	subject to the following conditions:
*
*	The above copyright notice and this permission notice shall be included in all
*	copies or substantial portions of the Software.
*
*	THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
*	IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS
*	FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR
*	COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER
*	IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN
*	CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
*/

#ifndef QTPOCKET_H
#define QTPOCKET_H

#include "qtpocket_export.h"
#include <QObject>
#include <QUrl>
#include <QMultiMap>
#include <QMap>

class QNetworkReply;
class QNetworkAccessManager;
class QSslError;

typedef QMultiMap<QString, QString> QtPocketParams;
typedef QMap<QString, QString> QtPocketHeaders;

class QtPocketPrivate;
class QTPOCKETSHARED_EXPORT QtPocket : public QObject
{
    Q_OBJECT
public:
     QtPocket(QObject *parent = 0);
     QtPocket(const QString &consumerKey,QObject *parent = 0);
     QtPocket(const QString &consumerKey, const QUrl &callbackUrl, QObject *parent = 0);
     void setToken(const QString &token);
    virtual ~QtPocket();


    enum OAuthError{
        NoError = 0,
        NetworkError = 1,
        RequestUnauthorized = 2
    };


    enum ApiRequest{

        GET_BOOKMARKS,
        ADD_BOOKMARK,
        GET_BOOKMARK,
        UPDATE_BOOKMARK,
        DELETE_BOOKMARK,
        GET_BOOKMARK_TAGS,
        ADD_TAGS_TO_BOOKMARK,
        DELETE_TAG_FROM_BOOKMARK
    };


    Q_INVOKABLE void getRequestToken();
    Q_INVOKABLE
    void getAccessToken(QString);

    Q_INVOKABLE
    void getBookmarks(const QtPocketParams &filters = QtPocketParams());
    Q_INVOKABLE
    void addBookmark(const QString &anyUrl, const int &favorite = 0, const int &archive = 0, const int allow_duplicates = 0);
    Q_INVOKABLE
    void getBookmark(const QString &bookmarkId);
    Q_INVOKABLE
    void updateBookmark(const QString &bookmarkId, const int &favorite, const int &archive, const float &read_percent = 0.0);
    Q_INVOKABLE
    void deleteBookmark(const QString &bookmarkId);
    Q_INVOKABLE
    void getBookmarkTags(const QString &bookmarkId);
    void addTagsToBookmark(const QString&, const QString&);
    void deleteTagFromBookmark(const QString&, const QString&);
    void getInitial();



    ApiRequest apiRequest() const;
    // Last error
    OAuthError lastError() const;
    // Error string
    QString errorString() const;
    // Http erro code
    qint32 errorCode() const;
    // Reply headers
    QtPocketHeaders replyHeaders() const;
    // Get networkManager instance
    QNetworkAccessManager *networkManager() const;
    // is requests synchronous
    bool synchronous() const;


signals:
    void requestTokenReceived(QString);
    void authorizationUrlReceived(QUrl authorizationUrl);
    void accessTokenReceived(QMap<QString, QString> response);
    void responseReceived(QByteArray response);


public slots:
    // Setters
    void setNetworkManager(QNetworkAccessManager *manager);
    void setSynchronous(const bool &synchronousRequests);
    void setConsumerKey(const QString &consumerKey);
    void setCallbackUrl(const QUrl &callbackUrl);

private slots:
    void replyReceived(QNetworkReply *reply);
    void sslErrors(QNetworkReply *reply, const QList<QSslError> & errors);


private:
    QtPocketPrivate *const d_ptr;
    Q_DECLARE_PRIVATE(QtPocket)
    Q_DISABLE_COPY(QtPocket)
};

#endif // QTPOCKET_H
