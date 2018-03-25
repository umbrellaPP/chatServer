#include "server.h"
#include <QDebug>

const QString ip = "127.0.0.1";
const int port = 8090;

Server::Server() {

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
            socket->deleteLater();
            this->socketList.removeAt(i--);
        }
    }
}

void Server::receivePackage() {
    for (QTcpSocket *socket: this->socketList){
        if (socket->canReadLine()){
            QByteArray package = socket->readAll();
            if (!package.isEmpty()) {
                this->handler.handle(socket, package);
            } else {
                qDebug() << "不存在数据包";
            }
        }
    }
}
