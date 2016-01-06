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

#ifndef QTPOCKETAUTH_H
#define QTPOCKETAUTH_H

#include "qtpocket_export.h"
#include <QObject>
#include <QUrl>
#ifdef QT5
#include <QUrlQuery>
#endif
#include <QMultiMap>

class QtPocketAuthPrivate;
class QTPOCKETSHARED_EXPORT QtPocketAuth : public QObject
{
    Q_OBJECT
public:
    explicit QtPocketAuth(QObject *parent = 0);
    virtual ~QtPocketAuth();
    void setToken(const QString& token);


    enum RequestType{
        REQUEST_TOKEN,
        ACCESS_TOKEN,
        AUTHORIZED
    };

    enum HttpMethod{
        GET,
        POST
    };


public slots:
    // Setters
    void setType(RequestType type);
    void setConsumerKey(const QString& consumerKey);
    void setCallbackUrl(const QUrl& callbackUrl);

    // Getters
    QtPocketAuth::RequestType type() const;

private:
    QtPocketAuthPrivate * const d_ptr;
    Q_DECLARE_PRIVATE(QtPocketAuth)
    Q_DISABLE_COPY(QtPocketAuth)
};

#endif // QTPOCKETAUTH_H
