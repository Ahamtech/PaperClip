#include "qtpocket.h"
#include "qtpocket_p.h"
#include "qtpocketauth.h"
#include <QtNetwork>
#include <QSslError>
#include <QMap>
#include <QUrl>
#include <QEventLoop>

const char* const REQUEST_TOKEN = "http://getpocket.com/v3/oauth/request";
const char* const ACCESS_TOKEN = "https://getpocket.com/v3/oauth/authorize/";
// API Endpoint
const char* const API_ENDPOINT = "https://getpocket.com/v3/";

QtPocketPrivate::QtPocketPrivate(QtPocket *parent) :
        synchronous(true), auth(new QtPocketAuth), manager(new QNetworkAccessManager), loop(
                new QEventLoop), q_ptr(parent)
{
    Q_Q(QtPocket);
    manager->setParent(q);
    auth->setParent(q);
    loop->setParent(q);

    q->connect(manager, SIGNAL(finished(QNetworkReply*)), q, SLOT(replyReceived(QNetworkReply*)));
    q->connect(manager, SIGNAL(sslErrors(QNetworkReply*,QList<QSslError>)), q,
            SLOT(sslErrors(QNetworkReply*,QList<QSslError>)));
    q->connect(manager, SIGNAL(finished(QNetworkReply*)), loop, SLOT(quit()));
}

QtPocketPrivate::~QtPocketPrivate()
{
}

void QtPocketPrivate::executeRequest(const QUrl &requestUrl, const QString &httpMethod,
        const QtPocketParams &requestParams)
{
    QNetworkRequest request(requestUrl);
    request.setHeader(QNetworkRequest::ContentTypeHeader, "application/x-www-form-urlencoded");
    if (!requestParams.isEmpty()) {
        Q_FOREACH(QString key,requestParams.keys()){
        qDebug() << key << " : -- : " << requestParams.value(key);
    }
    manager->post(request, paramsToByteArray(requestParams));
    qDebug()<< "url is "<<request.url().toString();
} else {
    manager->post(request, QByteArray());
}
}

QByteArray QtPocketPrivate::paramsToByteArray(const QtPocketParams &params)
{
    QByteArray postParams;
    QtPocketParams::const_iterator i = params.constBegin();
    while (i != params.end()) {
        postParams += i.key() + "=" + i.value() + "&";
        ++i;
    }
    postParams.chop(1);
    return postParams;
}

void QtPocketPrivate::setApiRequest(QtPocket::ApiRequest request)
{
    this->apiRequest = request;
}

QtPocket::QtPocket(QObject *parent) :
        QObject(parent), d_ptr(new QtPocketPrivate(this))
{
}

QtPocket::QtPocket(const QString &consumerKey, QObject *parent) :
        QObject(parent), d_ptr(new QtPocketPrivate(this))
{
    Q_D(QtPocket);

    d->consumerKey = consumerKey;
    d->auth->setConsumerKey(d->consumerKey);
}

QtPocket::QtPocket(const QString &consumerKey, const QUrl &callbackUrl, QObject *parent) :
        QObject(parent), d_ptr(new QtPocketPrivate(this))
{
    Q_D(QtPocket);

    d->consumerKey = consumerKey;
    d->callbackUrl = callbackUrl;
    d->auth->setConsumerKey(d->consumerKey);
    d->auth->setCallbackUrl(d->callbackUrl);
}

QtPocket::~QtPocket()
{
    delete d_ptr;
}

void QtPocket::getRequestToken()
{
    Q_D(QtPocket);
    d->auth->setType(QtPocketAuth::REQUEST_TOKEN);
    QtPocketParams param;
    param.insert("consumer_key", "36072-ebe38c859105c01114f5fe25");
    param.insert("redirect_uri", "http://localhost");
    d->executeRequest(QUrl(REQUEST_TOKEN), "POST", param);
}
void QtPocket::getAccessToken(QString code)
{
    Q_D(QtPocket);
    d->auth->setType(QtPocketAuth::ACCESS_TOKEN);
    QtPocketParams param;
    param.insert("consumer_key", "36072-ebe38c859105c01114f5fe25");
    param.insert("code", code);
    d->executeRequest(QUrl(ACCESS_TOKEN), "POST", param);
}

void QtPocket::getInitial()
{
    Q_D(QtPocket);
    d->setApiRequest(QtPocket::ADD_BOOKMARK);
    d->auth->setType(QtPocketAuth::AUTHORIZED);
    QUrl url(QString("%1get").arg(API_ENDPOINT));
    QtPocketParams param;
    param.insert("state", "all");
    param.insert("detailType", "complete");
    d->executeRequest(url, "POST", param);
}

void QtPocket::getBookmarks(const QtPocketParams &filters)
{
    Q_D(QtPocket);
    d->setApiRequest(QtPocket::GET_BOOKMARKS);
    d->auth->setType(QtPocketAuth::AUTHORIZED);
    QString query;
    QtPocketParams::const_iterator i = filters.constBegin();
    while (i != filters.constEnd()) {
        if (!QString(i.value()).isEmpty())
            query += i.key() + "=" + i.value() + "&";
        ++i;
    }
    if (!query.isEmpty()) {
        query.prepend("?");
        query.chop(1);
    }

    QUrl url(QString("%1bookmarks%2").arg(API_ENDPOINT).arg(query));

    d->executeRequest(url);
}

void QtPocket::addBookmark(const QString &anyUrl, const int &favorite, const int &archive,
        const int allow_duplicates)
{
    Q_D(QtPocket);
    d->setApiRequest(QtPocket::ADD_BOOKMARK);
    d->auth->setType(QtPocketAuth::AUTHORIZED);

    QUrl url(QString("%1bookmarks").arg(API_ENDPOINT));

    QtPocketParams params;
    params.insert("url", anyUrl.toUtf8().toPercentEncoding());
    params.insert("favorite", QString::number(favorite));
    params.insert("archive", QString::number(archive));
    params.insert("allow_duplicates", QString::number(allow_duplicates));

    d->executeRequest(url, "POST", params);
}

void QtPocket::getBookmark(const QString &bookmarkId)
{
    Q_D(QtPocket);
    d->setApiRequest(QtPocket::GET_BOOKMARK);
    d->auth->setType(QtPocketAuth::AUTHORIZED);

    QUrl url(QString("%1bookmarks/%2").arg(API_ENDPOINT).arg(bookmarkId));

    d->executeRequest(url);
}

void QtPocket::updateBookmark(const QString &bookmarkId, const int &favorite, const int &archive,
        const float &read_percent)
{
    Q_D(QtPocket);
    d->setApiRequest(QtPocket::UPDATE_BOOKMARK);
    d->auth->setType(QtPocketAuth::AUTHORIZED);

    QUrl url(QString("%1bookmarks/%2").arg(API_ENDPOINT).arg(bookmarkId));

    QtPocketParams params;
    params.insert("favorite", QString::number(favorite));
    params.insert("archive", QString::number(archive));
    params.insert("read_percent", QString::number(read_percent));

    d->executeRequest(url, "POST", params);
}

void QtPocket::deleteBookmark(const QString &bookmarkId)
{
    Q_D(QtPocket);
    d->setApiRequest(QtPocket::DELETE_BOOKMARK);
    d->auth->setType(QtPocketAuth::AUTHORIZED);

    QUrl url(QString("%1bookmarks/%2").arg(API_ENDPOINT).arg(bookmarkId));

    d->executeRequest(url, "DELETE");
}

void QtPocket::getBookmarkTags(const QString &bookmarkId)
{
    Q_D(QtPocket);
    d->setApiRequest(QtPocket::GET_BOOKMARK_TAGS);
    d->auth->setType(QtPocketAuth::AUTHORIZED);

    QUrl url(QString("%1bookmarks/%2/tags").arg(API_ENDPOINT).arg(bookmarkId));

    d->executeRequest(url);
}

void QtPocket::addTagsToBookmark(const QString &bookmarkId, const QString &tags)
{
    Q_D(QtPocket);
    d->setApiRequest(QtPocket::ADD_TAGS_TO_BOOKMARK);
    d->auth->setType(QtPocketAuth::AUTHORIZED);

    QUrl url(QString("%1bookmarks/%2/tags").arg(API_ENDPOINT).arg(bookmarkId));

    QtPocketParams params;
    params.insert("tags", QUrl::toPercentEncoding(tags));

    d->executeRequest(url, "POST", params);
}

void QtPocket::deleteTagFromBookmark(const QString &bookmarkId, const QString &tagId)
{
    Q_D(QtPocket);
    d->setApiRequest(QtPocket::DELETE_TAG_FROM_BOOKMARK);
    d->auth->setType(QtPocketAuth::AUTHORIZED);

    QUrl url(QString("%1bookmarks/%2/tags/%3").arg(API_ENDPOINT).arg(bookmarkId).arg(tagId));

    d->executeRequest(url, "DELETE");
}

QtPocket::ApiRequest QtPocket::apiRequest() const
{
    Q_D(const QtPocket);
    return d->apiRequest;
}

QtPocket::OAuthError QtPocket::lastError() const
{
    Q_D(const QtPocket);
    return d->error;
}

qint32 QtPocket::errorCode() const
{
    Q_D(const QtPocket);
    return d->errorCode;
}

QtPocketHeaders QtPocket::replyHeaders() const
{
    Q_D(const QtPocket);
    return d->replyHeaders;
}

QNetworkAccessManager *QtPocket::networkManager() const
{
    Q_D(const QtPocket);
    return d->manager;
}

bool QtPocket::synchronous() const
{
    Q_D(const QtPocket);
    return d->synchronous;
}

void QtPocket::setNetworkManager(QNetworkAccessManager *manager)
{
    Q_D(QtPocket);
    d->manager = manager;
}

void QtPocket::setSynchronous(const bool &synchronousRequests)
{
    Q_D(QtPocket);
    if (d->synchronous != synchronousRequests)
        d->synchronous = synchronousRequests;
}

QString QtPocket::errorString() const
{
    Q_D(const QtPocket);
    return d->errorString;
}

void QtPocket::setConsumerKey(const QString &consumerKey)
{
    Q_D(QtPocket);
    d->consumerKey = consumerKey;
    d->auth->setConsumerKey(d->consumerKey);
}

void QtPocket::setCallbackUrl(const QUrl &callbackUrl)
{
    Q_D(QtPocket);
    d->callbackUrl = callbackUrl;
    d->auth->setCallbackUrl(d->callbackUrl);
}

void QtPocket::replyReceived(QNetworkReply *reply)
{
    Q_D(QtPocket);
    switch (reply->error()) {
        case QNetworkReply::NoError:
            d->error = QtPocket::NoError;
            break;

        case QNetworkReply::ContentAccessDenied:
        case QNetworkReply::ContentOperationNotPermittedError:
        case QNetworkReply::AuthenticationRequiredError:
        case QNetworkReply::ProtocolFailure:
            d->error = QtPocket::RequestUnauthorized;
            break;

        default:
            d->error = QtPocket::NetworkError;
            break;
    }

    d->errorCode = reply->attribute(QNetworkRequest::HttpStatusCodeAttribute).toInt();
    d->errorString = reply->errorString();
    qDebug() << d->errorCode << d->errorString;
    QByteArray replyString = reply->readAll();
    qDebug() << QString::fromAscii(replyString) << "bytearrraydata";
    if (d->auth->type() == QtPocketAuth::REQUEST_TOKEN
            || d->auth->type() == QtPocketAuth::ACCESS_TOKEN) {
        QMap<QString, QString> response;

        QStringList parameters = QString(replyString).split('&', QString::SkipEmptyParts);
        foreach (const QString &pair, parameters){
        QStringList p = pair.split('=');

        if (p.count() == 2) {
            response.insert(p[0], QUrl::fromPercentEncoding(p[1].toAscii()));
        }
    }

        if (d->error == QtPocket::NoError
                && (response["oauth_token"].isEmpty() || response["oauth_token_secret"].isEmpty())) {
            d->error = QtPocket::RequestUnauthorized;
        }
        switch (d->auth->type()) {
            case QtPocketAuth::REQUEST_TOKEN:
                emit requestTokenReceived(response.value("code"));
                qDebug() << "emit requestTokenReceived";
                break;
            case QtPocketAuth::ACCESS_TOKEN:
                emit accessTokenReceived(response);
                qDebug() << "emit AccessTokenReceived";

                break;
        }
    } else {

        // Reply headers
        QList<QByteArray> headers = reply->rawHeaderList();
        foreach(const QByteArray &header, headers){
        d->replyHeaders.insert(header, reply->rawHeader(header));
    }
        qDebug() << QString(replyString);
        emit responseReceived(replyString);

    }

    reply->deleteLater();
}

void QtPocket::sslErrors(QNetworkReply *reply, const QList<QSslError> & errors)
{
    reply->ignoreSslErrors(errors);
}

void QtPocket::setToken(const QString &token)
{
    Q_D(QtPocket);
    d->token = token;
    d->auth->setToken(d->token);
}
