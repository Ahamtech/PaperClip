#include "qtinstapaper.h"
#include "qtinstapaper_p.h"
#include "qtinstapaperauth.h"
#include <QtNetwork>
#include <QSslError>
#include <QMap>
#include <QUrl>
#include <QEventLoop>


const char* const REQUEST_TOKEN = "https://www.instapaper.com/api/1.1/oauth/request_token/";
const char* const AUTHORIZE = "https://www.instapaper.com/api/1.1/oauth/authorize/";
const char* const ACCESS_TOKEN = "https://www.instapaper.com/api/1.1/oauth/access_token/";
// XAUTH url
const char* const XAUTH_ENDPOINT = "https://www.instapaper.com/api/1/oauth/access_token";
// API Endpoint
const char* const API_ENDPOINT = "https://www.instapaper.com/api/1.1/";


QtInstapaperPrivate::QtInstapaperPrivate(QtInstapaper *parent) :
    synchronous(true),
    auth(new QtInstapaperAuth),
    manager(new QNetworkAccessManager),
    loop(new QEventLoop),
    q_ptr(parent)
{
    Q_Q(QtInstapaper);
    manager->setParent(q);
    auth->setParent(q);
    loop->setParent(q);

    q->connect(manager, SIGNAL(finished(QNetworkReply*)),
            q,
            SLOT(replyReceived(QNetworkReply*)));
    q->connect(manager, SIGNAL(sslErrors(QNetworkReply*,QList<QSslError>)),
            q,
            SLOT(sslErrors(QNetworkReply*,QList<QSslError>)));
    q->connect(manager, SIGNAL(finished(QNetworkReply*)),
               loop,
               SLOT(quit()));
}


QtInstapaperPrivate::~QtInstapaperPrivate()
{
}


void QtInstapaperPrivate::executeRequest(const QUrl &requestUrl, const QString &httpMethod, const QtInstapaperParams &requestParams)
{
    QNetworkRequest request(requestUrl);

    if (httpMethod == "GET"){
        request.setRawHeader("Authorization", auth->generateAuthHeader(request.url()));
        manager->get(request);
    }else if (httpMethod == "POST"){
        request.setHeader(QNetworkRequest::ContentTypeHeader, "application/x-www-form-urlencoded");
        if (!requestParams.isEmpty()){
            request.setRawHeader("Authorization", auth->generateAuthHeader(request.url(), QtInstapaperAuth::POST, requestParams));
            Q_FOREACH(QString key,requestParams.keys()){
                            qDebug() << key << " : -- : " << requestParams.value(key);
                        }
            manager->post(request, paramsToByteArray(requestParams));
        }else{
            request.setRawHeader("Authorization", auth->generateAuthHeader(request.url(), QtInstapaperAuth::POST));

            manager->post(request, QByteArray());
        }
    }else if (httpMethod == "PUT"){
        manager->put(request, paramsToByteArray(requestParams));
    }else if (httpMethod == "DELETE"){
        request.setRawHeader("Authorization", auth->generateAuthHeader(request.url(), QtInstapaperAuth::DELETE));
        manager->deleteResource(request);
    }else if (httpMethod == "HEAD"){
        manager->head(request);
    }
    if (synchronous)
        loop->exec();
}


QByteArray QtInstapaperPrivate::paramsToByteArray(const QtInstapaperParams &params)
{
    QByteArray postParams;
    QtInstapaperParams::const_iterator i = params.constBegin();
    while(i != params.end()){
        postParams += i.key()+"="+i.value()+"&";
        ++i;
    }
    postParams.chop(1);
    return postParams;
}

void QtInstapaperPrivate::setApiRequest(QtInstapaper::ApiRequest request)
{
    this->apiRequest = request;
}

void QtInstapaperPrivate::clearTokens()
{
    auth->setToken("");
    auth->setTokenSecret("");
}


QtInstapaper::QtInstapaper(QObject *parent) :
    QObject(parent),
    d_ptr(new QtInstapaperPrivate(this))
{
}

QtInstapaper::QtInstapaper(const QString &consumerKey,
                             const QString &consumerSecret,
                             QObject *parent) :
    QObject(parent),
    d_ptr(new QtInstapaperPrivate(this))
{
    Q_D(QtInstapaper);

    d->consumerKey = consumerKey;
    d->consumerSecret = consumerSecret;
    d->auth->setConsumerKey(d->consumerKey);
    d->auth->setConsumerSecret(d->consumerSecret);
}


QtInstapaper::QtInstapaper(const QString &consumerKey,
                             const QString &consumerSecret,
                             const QUrl &callbackUrl,
                             QObject *parent) :
    QObject(parent),
    d_ptr(new QtInstapaperPrivate(this))
{
    Q_D(QtInstapaper);

    d->consumerKey = consumerKey;
    d->consumerSecret = consumerSecret;
    d->callbackUrl = callbackUrl;
    d->auth->setConsumerKey(d->consumerKey);
    d->auth->setConsumerSecret(d->consumerSecret);
    d->auth->setCallbackUrl(d->callbackUrl);
}


QtInstapaper::~QtInstapaper()
{
    delete d_ptr;
}

void QtInstapaper::xAuthLogin(const QString &username, const QString &password)
{
    Q_D(QtInstapaper);
    d->auth->setType(QtInstapaperAuth::XAUTH_LOGIN);
    QUrl url(XAUTH_ENDPOINT);
    QtInstapaperParams param;
    param.insert("x_auth_username",username);
    param.insert("x_auth_password",password);
    param.insert("x_auth_mode","client_auth");
    d->executeRequest(url, "POST",param);
}

void QtInstapaper::getArticle(const QString &articleId)
{
    Q_D(QtInstapaper);
    d->setApiRequest(QtInstapaper::GET_ARTICLE);
    d->auth->setType(QtInstapaperAuth::AUTHORIZED);
    QUrl url(QString("%1articles/%2").arg(API_ENDPOINT).arg(articleId));

    d->executeRequest(url);
}

void QtInstapaper::getBookmarks(const QtInstapaperParams &filters)
{
    Q_D(QtInstapaper);
    d->setApiRequest(QtInstapaper::GET_BOOKMARKS);
    d->auth->setType(QtInstapaperAuth::AUTHORIZED);
    QString query;
    QtInstapaperParams::const_iterator i = filters.constBegin();
    while (i != filters.constEnd()){
        if (!QString(i.value()).isEmpty())
            query += i.key() + "=" + i.value() + "&";
        ++i;
    }
    if (!query.isEmpty()){
        query.prepend("?");
        query.chop(1);
    }

    QUrl url(QString("%1bookmarks%2").arg(API_ENDPOINT).arg(query));

    d->executeRequest(url);
}


void QtInstapaper::addBookmark(const QString &anyUrl,
                                const int &favorite,
                                const int &archive,
                                const int allow_duplicates)
{
    Q_D(QtInstapaper);
    d->setApiRequest(QtInstapaper::ADD_BOOKMARK);
    d->auth->setType(QtInstapaperAuth::AUTHORIZED);

    QUrl url(QString("%1bookmarks").arg(API_ENDPOINT));

    QtInstapaperParams params;
    params.insert("url", anyUrl.toUtf8().toPercentEncoding());
    params.insert("favorite", QString::number(favorite));
    params.insert("archive", QString::number(archive));
    params.insert("allow_duplicates", QString::number(allow_duplicates));

    d->executeRequest(url, "POST", params);
}

void QtInstapaper::getBookmark(const QString &bookmarkId)
{
    Q_D(QtInstapaper);
    d->setApiRequest(QtInstapaper::GET_BOOKMARK);
    d->auth->setType(QtInstapaperAuth::AUTHORIZED);

    QUrl url(QString("%1bookmarks/%2").arg(API_ENDPOINT).arg(bookmarkId));

    d->executeRequest(url);
}

void QtInstapaper::updateBookmark(const QString &bookmarkId,
                                   const int &favorite,
                                   const int &archive,
                                   const float &read_percent)
{
    Q_D(QtInstapaper);
    d->setApiRequest(QtInstapaper::UPDATE_BOOKMARK);
    d->auth->setType(QtInstapaperAuth::AUTHORIZED);

    QUrl url(QString("%1bookmarks/%2").arg(API_ENDPOINT).arg(bookmarkId));

    QtInstapaperParams params;
    params.insert("favorite", QString::number(favorite));
    params.insert("archive", QString::number(archive));
    params.insert("read_percent", QString::number(read_percent));

    d->executeRequest(url, "POST", params);
}


void QtInstapaper::deleteBookmark(const QString &bookmarkId)
{
    Q_D(QtInstapaper);
    d->setApiRequest(QtInstapaper::DELETE_BOOKMARK);
    d->auth->setType(QtInstapaperAuth::AUTHORIZED);

    QUrl url(QString("%1bookmarks/%2").arg(API_ENDPOINT).arg(bookmarkId));

    d->executeRequest(url, "DELETE");
}


void QtInstapaper::getBookmarkTags(const QString &bookmarkId)
{
    Q_D(QtInstapaper);
    d->setApiRequest(QtInstapaper::GET_BOOKMARK_TAGS);
    d->auth->setType(QtInstapaperAuth::AUTHORIZED);

    QUrl url(QString("%1bookmarks/%2/tags").arg(API_ENDPOINT).arg(bookmarkId));

    d->executeRequest(url);
}

void QtInstapaper::addTagsToBookmark(const QString &bookmarkId, const QString &tags)
{
    Q_D(QtInstapaper);
    d->setApiRequest(QtInstapaper::ADD_TAGS_TO_BOOKMARK);
    d->auth->setType(QtInstapaperAuth::AUTHORIZED);

    QUrl url(QString("%1bookmarks/%2/tags").arg(API_ENDPOINT).arg(bookmarkId));

    QtInstapaperParams params;
    params.insert("tags", QUrl::toPercentEncoding(tags));

    d->executeRequest(url, "POST", params);
}


void QtInstapaper::deleteTagFromBookmark(const QString &bookmarkId, const QString &tagId)
{
    Q_D(QtInstapaper);
    d->setApiRequest(QtInstapaper::DELETE_TAG_FROM_BOOKMARK);
    d->auth->setType(QtInstapaperAuth::AUTHORIZED);

    QUrl url(QString("%1bookmarks/%2/tags/%3").arg(API_ENDPOINT).arg(bookmarkId).arg(tagId));

    d->executeRequest(url, "DELETE");
}

void QtInstapaper::getTags()
{
    Q_D(QtInstapaper);
    d->setApiRequest(QtInstapaper::GET_TAGS);
    d->auth->setType(QtInstapaperAuth::AUTHORIZED);

    QUrl url(QString("%1tags").arg(API_ENDPOINT));

    d->executeRequest(url);
}

void QtInstapaper::getTag(const QString &tagId)
{
    Q_D(QtInstapaper);
    d->setApiRequest(QtInstapaper::GET_TAG);
    d->auth->setType(QtInstapaperAuth::AUTHORIZED);

    QUrl url(QString("%1tags/%2").arg(API_ENDPOINT).arg(tagId));

    d->executeRequest(url);
}

void QtInstapaper::deleteTag(const QString &tagId)
{
    Q_D(QtInstapaper);
    d->setApiRequest(QtInstapaper::DELETE_TAG);
    d->auth->setType(QtInstapaperAuth::AUTHORIZED);

    QUrl url(QString("%1tags/%2").arg(API_ENDPOINT).arg(tagId));

    d->executeRequest(url, "DELETE");
}


void QtInstapaper::getUserInfo()
{
    Q_D(QtInstapaper);
    d->setApiRequest(QtInstapaper::GET_USER_INFO);
    d->auth->setType(QtInstapaperAuth::AUTHORIZED);

    QUrl url(QString("%1account/verify_credentials").arg(API_ENDPOINT));

    d->executeRequest(url);
}


void QtInstapaper::getShortUrl(const QString &sourceUrl)
{
    Q_D(QtInstapaper);
    d->auth->setType(QtInstapaperAuth::AUTHORIZED);
    d->setApiRequest(QtInstapaper::GET_SHORT_URL);
    QUrl url("http://www.instapaper.com/api/shortener/v1/urls");

    QtInstapaperParams params;
    params.insert("url", QUrl::toPercentEncoding(sourceUrl));

    d->executeRequest(url, "POST", params);
}


QtInstapaper::ApiRequest QtInstapaper::apiRequest() const
{
    Q_D(const QtInstapaper);
    return d->apiRequest;
}



QtInstapaper::OAuthError QtInstapaper::lastError() const
{
    Q_D(const QtInstapaper);
    return d->error;
}


qint32 QtInstapaper::errorCode() const
{
    Q_D(const QtInstapaper);
    return d->errorCode;
}


QtInstapaperHeaders QtInstapaper::replyHeaders() const
{
    Q_D(const QtInstapaper);
    return d->replyHeaders;
}

QNetworkAccessManager *QtInstapaper::networkManager() const
{
    Q_D(const QtInstapaper);
    return d->manager;
}


bool QtInstapaper::synchronous() const
{
    Q_D(const QtInstapaper);
    return d->synchronous;
}

void QtInstapaper::setNetworkManager(QNetworkAccessManager *manager)
{
    Q_D(QtInstapaper);
    d->manager = manager;
}


void QtInstapaper::setSynchronous(const bool &synchronousRequests)
{
    Q_D(QtInstapaper);
    if (d->synchronous != synchronousRequests)
        d->synchronous = synchronousRequests;
}


QString QtInstapaper::errorString() const
{
    Q_D(const QtInstapaper);
    return d->errorString;
}

void QtInstapaper::setConsumerKey(const QString &consumerKey)
{
    Q_D(QtInstapaper);
    d->consumerKey = consumerKey;
    d->auth->setConsumerKey(d->consumerKey);
}

void QtInstapaper::setConsumerSecret(const QString &consumerSecret)
{
    Q_D(QtInstapaper);
    d->consumerSecret = consumerSecret;
    d->auth->setConsumerSecret(d->consumerSecret);
}

void QtInstapaper::setVerifier(const QString &verifier)
{
    Q_D(QtInstapaper);
    d->verifier = verifier;
}

void QtInstapaper::setCallbackUrl(const QUrl &callbackUrl)
{
    Q_D(QtInstapaper);
    d->callbackUrl = callbackUrl;
    d->auth->setCallbackUrl(d->callbackUrl);
}

void QtInstapaper::setToken(const QString &token)
{
    Q_D(QtInstapaper);
    d->token = token;
    d->auth->setToken(d->token);
}

void QtInstapaper::setTokenSecret(const QString &tokenSecret)
{
    Q_D(QtInstapaper);
    d->tokenSecret = tokenSecret;
    d->auth->setTokenSecret(d->tokenSecret);
}


void QtInstapaper::replyReceived(QNetworkReply *reply)
{
    Q_D(QtInstapaper);
    switch (reply->error()) {
    case QNetworkReply::NoError:
        d->error = QtInstapaper::NoError;
        break;

    case QNetworkReply::ContentAccessDenied:
    case QNetworkReply::ContentOperationNotPermittedError:
    case QNetworkReply::AuthenticationRequiredError:
    case QNetworkReply::ProtocolFailure:
        d->error = QtInstapaper::RequestUnauthorized;
        break;

    default:
        d->error = QtInstapaper::NetworkError;
        break;
    }

    d->errorCode = reply->attribute(QNetworkRequest::HttpStatusCodeAttribute).toInt();
    d->errorString = reply->errorString();
    qDebug() << d->errorCode << d->errorString;
    QByteArray replyString = reply->readAll();

    if ( d->auth->type() == QtInstapaperAuth::ACCESS_TOKEN || d->auth->type() == QtInstapaperAuth::XAUTH_LOGIN){
        QMap<QString, QString> response;
        QStringList parameters = QString(replyString).split('&', QString::SkipEmptyParts);
        foreach (const QString &pair, parameters) {
            QStringList p = pair.split('=');
            if (p.count() == 2){
                response.insert(p[0], QUrl::fromPercentEncoding(p[1].toAscii()));
            }
        }
        if (d->error == QtInstapaper::NoError
                && (response["oauth_token"].isEmpty() || response["oauth_token_secret"].isEmpty())) {
            d->error = QtInstapaper::RequestUnauthorized;
        }

        d->token = response["oauth_token"];
        d->tokenSecret = response["oauth_token_secret"];
        d->auth->setToken(d->token);
        d->auth->setTokenSecret(d->tokenSecret);

        switch (d->auth->type()){
        case QtInstapaperAuth::REQUEST_TOKEN:
            if (d->error == QtInstapaper::NoError)
                d->auth->setType(QtInstapaperAuth::ACCESS_TOKEN);
            emit requestTokenReceived(response);
            break;
        case QtInstapaperAuth::ACCESS_TOKEN:
            if (d->error == QtInstapaper::NoError)
                d->auth->setType(QtInstapaperAuth::AUTHORIZED);
            emit accessTokenReceived(response);
            break;
        case QtInstapaperAuth::XAUTH_LOGIN:
            if (d->error == QtInstapaper::NoError)
                d->auth->setType(QtInstapaperAuth::AUTHORIZED);
            emit accessTokenReceived(response);
            break;
        default:
            break;
        }
    }else{

        // Reply headers
        QList<QByteArray> headers = reply->rawHeaderList();
        foreach(const QByteArray &header, headers){
            d->replyHeaders.insert(header, reply->rawHeader(header));
        }

        emit responseReceived(replyString);

    }

    reply->deleteLater();
}


void QtInstapaper::sslErrors(QNetworkReply *reply, const QList<QSslError> & errors)
{
    reply->ignoreSslErrors(errors);
}
