#ifndef HANDLER_H
#define HANDLER_H

#include "../define.h"
#include "../util/jsonparser.h"
#include "../db/userdb.h"
#include "../db/friendsdb.h"
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

    void handle(QTcpSocket *socket, QByteArray package);  // 处理器总入口

private:
    QMap<Code, HandleFunc> handlerDict;
    UserDB userDB;
    FriendsDB friendDB;

    void handleLogin(QTcpSocket *socket, QJsonObject data);
    void handleRegister(QTcpSocket *socket, QJsonObject data);
    void handleGetFriendsList(QTcpSocket *socket, QJsonObject data);    
    void updateFriendList(QTcpSocket *socket, QString accountId, QString friendId = "");
    void handleFindFriend(QTcpSocket *socket, QJsonObject data);
    void handleAddFirend(QTcpSocket *socket, QJsonObject data);
    void handleRemoveFriend(QTcpSocket *socket, QJsonObject data);
    void handleSendMsg(QTcpSocket *socket, QJsonObject data);

    void handleGetGroupList(QTcpSocket *socket, QJsonObject data);
    void handleCreateGroup(QTcpSocket *socket, QJsonObject data);
    void updateGroupList(QTcpSocket *socket, QString accountId);
    void handleFindGroup(QTcpSocket *socket, QJsonObject data);
    void handleJoinGroup(QTcpSocket *socket, QJsonObject data);
    void handleUpdataMember(QTcpSocket *socket, QJsonObject data);
    void handleSendGroupMsg(QTcpSocket *socket, QJsonObject data);
    void handleQuitGroup(QTcpSocket *socket, QJsonObject data);

    void send(QTcpSocket *socket, Code code, QJsonObject data);
};

#endif // HANDLER_H
