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

#ifndef QTPOCKET_P_H
#define QTPOCKET_P_H


#include "qtpocket.h"

class QtPocketAuth;
class QEventLoop;

class QTPOCKETSHARED_EXPORT QtPocketPrivate
{
public:
    QtPocketPrivate(QtPocket *parent);
    virtual ~QtPocketPrivate();
    QString token;
    void executeRequest(const QUrl &requestUrl, const QString &httpMethod = "GET", const QtPocketParams &requestParams = QtPocketParams());
    QByteArray paramsToByteArray(const QtPocketParams &params);
    void setApiRequest(QtPocket::ApiRequest request);
    void clearTokens();

    QString consumerKey;
    QString code;
    QUrl callbackUrl;
    QtPocket::OAuthError error;
    QString errorString;
    qint32 errorCode;
    QtPocketHeaders replyHeaders;
    QtPocket::ApiRequest apiRequest;
    bool synchronous;

    QtPocketAuth *auth;
    QNetworkAccessManager *manager;
    QEventLoop *loop;


protected:
    QtPocket *const q_ptr;
    Q_DECLARE_PUBLIC(QtPocket)
};

#endif // QTPOCKET_P_H
