#include "server.h"
#include <QDebug>
#include <QList>

const QString ip = "127.0.0.1";
const int port = 8090;

Server::Server() {
    this->handler = new Handler();
}

void Server::initDB() {
    QSqlDatabase db = QSqlDatabase::addDatabase("QMYSQL");
    db.setHostName("localhost");
    db.setPort(3306);
    db.setDatabaseName("chat");
    db.setUserName("root");
    db.setPassword("");
    if (db.open()){
        qDebug() << "数据库打开成功";
    }else {
        qDebug() << "数据库打开失败";
    }
}

void Server::initWeb() {
    this->server = new QTcpServer();
    if (this->server->listen(QHostAddress(ip), port)) {
        qDebug() << "服务器开始监听";
        connect(this->server, SIGNAL(newConnection()), this, SLOT(newConnection()));
    } else {
        qDebug() << "服务器监听失败";
    }
}

// deprecated
//void Server::sendPackage(Code code, QJsonObject obj) {
//    QByteArray package = JsonTool->generatePackage(code, obj);
//    m_socket->write(package);
//}

bool Server::isOnline(QString accountId) {
    return onlineMap.value(accountId, nullptr) != nullptr;
}

QString Server::socketToAccountId(QTcpSocket *socket){
    for (auto key : onlineMap.keys()){
        if (socket == onlineMap.value(key)){
            return key;
        }
    }
    return "";
}

void Server::offline(QTcpSocket *socket) {
    QStringList keys = onlineMap.keys();
    for (QString key: keys){
        if (socket == onlineMap.value(key)) {
            onlineMap.remove(key);
            qDebug() << key << " 已经下线";
            return;
        }
    }
}

void Server::newConnection() {
    qDebug() << "新链接";
    QTcpSocket *socket = this->server->nextPendingConnection();
    connect(socket, SIGNAL(readyRead()), this, SLOT(receivePackage()));
    connect(socket, SIGNAL(disconnected()), this, SLOT(disconnected()));
    this->socketList.push_back(socket);
}

void Server::disconnected() {
    QTcpSocket *socket = static_cast<QTcpSocket*>(this->sender());
    for (int i = 0; i < this->socketList.size(); ++i){
        if (socket == this->socketList[i]) {
            this->offline(socket);
            socket->deleteLater();
            this->socketList.removeAt(i--);
        }
    }
}

void Server::receivePackage() {
    for (QTcpSocket *socket: this->socketList){
        if (socket->canReadLine()){
            QByteArray content = socket->readAll();
            QList<QByteArray> list = content.split(';');
            list.pop_back();
            for (auto package: list) {
                if (!package.isEmpty()) {
                    this->handler->handle(socket, package);
                } else {
                    qDebug() << "不存在数据包";
                }
            }
        }
    }
}
