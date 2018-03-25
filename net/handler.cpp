#include "handler.h"
#include <QDebug>

Handler::Handler() {
    this->handlerDict.insert(Code::C2S_LOGIN, Handler::handleLogin);
    this->handlerDict.insert(Code::C2S_REGISTER, Handler::handleRegister);
}

void Handler::handle(QTcpSocket *socket, QByteArray package) {
    Code code;
    QJsonObject obj;
    qDebug() << "package: " << package;
    JsonTool->getPackageData(package, &code, &obj);
    HandleFunc handleFunc = this->handlerDict.value(code);
    if (handleFunc){
        (this->*handleFunc)(socket, obj);
    } else {
        qDebug() << "找不到对应的处理器处理 code " + code;
    }
}

void Handler::handleLogin(QTcpSocket *socket, QJsonObject data) {
    QJsonObject obj;
    QString accountId = data.value("accountId").toString();
    QString password = data.value("password").toString();
    qDebug() << accountId << password;

    QSqlQuery *query = userDB.selectUser(accountId);
    if (query && query->next()) {
        if (password == query->value("password").toString()) {
            // 登录成功
            obj.insert("result", 0);
            obj.insert("userName", query->value("userName").toString());
            obj.insert("accountId", accountId);
            obj.insert("password", password);
        } else {
            // 密码不正确
            obj.insert("result", 2);
        }
    } else {
        // todo 不存在用户id
        obj.insert("result", 1);
    }
    QByteArray result = JsonTool->generatePackage(Code::S2C_LOGIN, obj);
    socket->write(result);  // 服务器返回
}

void Handler::handleRegister(QTcpSocket *socket, QJsonObject data) {
    qDebug() << "handleRegister";
}


