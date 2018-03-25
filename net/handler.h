#ifndef HANDLER_H
#define HANDLER_H

#include "../define.h"
#include "../util/jsonparser.h"
#include "../db/userdb.h"
#include <QByteArray>
#include <QJsonObject>
#include <QSqlQuery>
#include <QMap>
#include <QTcpSocket>

class Handler;
typedef void (Handler::*HandleFunc)(QTcpSocket *socket, QJsonObject data);

class Handler
{
public:
    Handler();

    void handle(QTcpSocket *socket, QByteArray package);

private:
    QMap<Code, HandleFunc> handlerDict;
    UserDB userDB;

    void handleLogin(QTcpSocket *socket, QJsonObject data);
    void handleRegister(QTcpSocket *socket, QJsonObject data);
};

#endif // HANDLER_H
