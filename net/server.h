#ifndef SERVER_H
#define SERVER_H

#include <QObject>
#include <QList>
#include <QMap>
#include <QSqlDatabase>
#include <QTcpServer>
#include <QTcpSocket>
#include <QJsonObject>
#include "../define.h"
#include "../util/jsonparser.h"
#include "handler.h"

#define Net Server::getInstance()

class Server: public QObject
{
    Q_OBJECT
public:
    static Server* getInstance() {
        static Server* _instance = new Server();
        return _instance;
    }
    void init() {
        this->initDB();
        this->initWeb();
    }

    bool isOnline(QString accountId);  // 判断用户是否在线
    QString socketToAccountId(QTcpSocket *socket);
    void offline(QTcpSocket *socket);  // 下线操作

    QMap<QString, QTcpSocket*> onlineMap;  // accountId 映射 socket

//    void sendPackage(Code code, QJsonObject obj);  // 发送数据包  deprecated
public slots:
    void newConnection();
    void disconnected();
    void receivePackage();  // 接收数据包

private:
    Server();
    void initDB();
    void initWeb();

    QTcpServer *server;
    Handler *handler;
//    QTcpSocket *m_socket;
    QList<QTcpSocket*> socketList;

};

#endif // SERVER_H
