#include "qtpocketauth.h"
#include "qtpocketauth_p.h"
#include <QCryptographicHash>
#include <QDateTime>
#include <QStringList>
#include <QDebug>

QtPocketAuthPrivate::QtPocketAuthPrivate()
{
    qsrand(QTime::currentTime().msec());
    this->callbackUrl = QUrl("http://example.com"); // Default callback url
}


QtPocketAuthPrivate::~QtPocketAuthPrivate()
{
    qDebug() << Q_FUNC_INFO;
}



QtPocketAuth::QtPocketAuth(QObject *parent) :
    QObject(parent),
    d_ptr(new QtPocketAuthPrivate)
{
}

QtPocketAuth::~QtPocketAuth()
{
    delete d_ptr;
}

// Setters
void QtPocketAuth::setType(QtPocketAuth::RequestType type)
{
    d_ptr->requestType = type;
}

void QtPocketAuth::setConsumerKey(const QString &consumerKey)
{
    d_ptr->consumerKey = consumerKey;
}

void QtPocketAuth::setCallbackUrl(const QUrl &callbackUrl)
{
    d_ptr->callbackUrl = callbackUrl;
}



// Getters
QtPocketAuth::RequestType QtPocketAuth::type() const
{
    return d_ptr->requestType;
}

void QtPocketAuth::setToken(const QString &token)
{
    d_ptr->oauthToken = token;
}

// Helper function to avoid writting "QString(QUrl::toPercentEncoding(xxx)" 10 times
inline QString encode(QString string) { return QString(QUrl::toPercentEncoding(string)); }
