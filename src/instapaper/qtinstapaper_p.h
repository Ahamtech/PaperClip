/*
*	QtInstapaper - A Instapaper.com library for Qt
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

#ifndef QTINSTAPAPER_P_H
#define QTINSTAPAPER_P_H


#include "qtinstapaper.h"

class QtInstapaperAuth;
class QEventLoop;

class QTINSTAPAPERSHARED_EXPORT QtInstapaperPrivate
{
public:
    QtInstapaperPrivate(QtInstapaper *parent);
    virtual ~QtInstapaperPrivate();


    void executeRequest(const QUrl &requestUrl, const QString &httpMethod = "GET", const QtInstapaperParams &requestParams = QtInstapaperParams());
    QByteArray paramsToByteArray(const QtInstapaperParams &params);
    void setApiRequest(QtInstapaper::ApiRequest request);
    void clearTokens();

    QString consumerKey;
    QString consumerSecret;
    QString token;
    QString tokenSecret;
    QString verifier;
    QUrl callbackUrl;
    QtInstapaper::OAuthError error;
    QString errorString;
    qint32 errorCode;
    QtInstapaperHeaders replyHeaders;
    QtInstapaper::ApiRequest apiRequest;
    bool synchronous;

    QtInstapaperAuth *auth;
    QNetworkAccessManager *manager;
    QEventLoop *loop;


protected:
    QtInstapaper *const q_ptr;
    Q_DECLARE_PUBLIC(QtInstapaper)
};

#endif // QTINSTAPAPER_P_H
