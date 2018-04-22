#include "handler.h"
#include "net/server.h"
#include <QDebug>

Handler::Handler() {
    // 注册路由
    this->handlerDict.insert(CodeNS::C2S_LOGIN, Handler::handleLogin);
    this->handlerDict.insert(CodeNS::C2S_REGISTER, Handler::handleRegister);
    this->handlerDict.insert(CodeNS::C2S_GET_FRIENDS_LIST, Handler::handleGetFriendsList);
    this->handlerDict.insert(CodeNS::C2S_FIND_FRIEND, Handler::handleFindFriend);
    this->handlerDict.insert(CodeNS::C2S_ADD_FRIEND, Handler::handleAddFirend);
    this->handlerDict.insert(CodeNS::C2S_REMOVE_FRIEND, Handler::handleRemoveFriend);
    this->handlerDict.insert(CodeNS::C2S_SEND_MSG, Handler::handleSendMsg);

    this->handlerDict.insert(CodeNS::C2S_GET_GROUP_LIST, Handler::handleGetGroupList);
    this->handlerDict.insert(CodeNS::C2S_CREATE_GROUP, Handler::handleCreateGroup);
    this->handlerDict.insert(CodeNS::C2S_FIND_GROUP, Handler::handleFindGroup);
    this->handlerDict.insert(CodeNS::C2S_JOIN_GROUP, Handler::handleJoinGroup);
    this->handlerDict.insert(CodeNS::C2S_UPDATE_MEMBERS, Handler::handleUpdataMember);
    this->handlerDict.insert(CodeNS::C2S_SEND_GROUP_MSG, Handler::handleSendGroupMsg);
    this->handlerDict.insert(CodeNS::C2S_QUIT_GROUP, Handler::handleQuitGroup);
}

void Handler::handle(QTcpSocket *socket, QByteArray package) {
    Code code;
    QJsonObject obj;
    JsonTool->getPackageData(package, &code, &obj);
    HandleFunc handleFunc = this->handlerDict.value(code);
    if (handleFunc) {
        (this->*handleFunc)(socket, obj);
    } else {
        qDebug() << "找不到对应的处理器处理 code " + code;
    }

    //    qDebug() << "用户: " << Net->socketToAccountId(socket) << " - " << code << obj;
    logPackage(socket, code, obj);
}

void Handler::logPackage(QTcpSocket *socket, QString code, QJsonObject obj) {
    qDebug() << "用户: " << Net->socketToAccountId(socket) << " - " << code;
    for (auto key: obj.keys()) {
        qDebug() << key << ": " << obj.value(key).toString();
    }
    qDebug() << '\n';
}

QString Handler::getUserNames(QString accountIds) {
    QStringList userNameList;
    for (auto id: accountIds.split(',')) {
        auto result = userDB.selectUser(id);
        if (!result.isEmpty()) {
            auto userName = result.value("userName").toString();
            userNameList.push_back(userName);
        }
    }
    return userNameList.join(',');
}

QString Handler::getGroupNames(QString groupIds) {
    QStringList grouNameList;
    for (auto id: groupIds.split(',')) {
        auto result = friendDB.selectGroup(id);
        if (!result.isEmpty()) {
            auto groupName = result.value("groupName").toString();
            grouNameList.push_back(groupName);
        }
    }
    return grouNameList.join(',');
}

void Handler::handleLogin(QTcpSocket *socket, QJsonObject data) {
    QJsonObject obj;
    QString accountId = data.value("accountId").toString();
    QString password = data.value("password").toString();

    QJsonObject queryResult = userDB.selectUser(accountId);
    if (!queryResult.value("accountId").isUndefined()) {
        if (password == queryResult.value("password").toString()) {
            if (Net->isOnline(accountId)) {
                // 重复登录
                obj.insert("result", 3);
            }else {
                // 登录成功
                obj.insert("result", 0);
                obj.insert("userName", queryResult.value("userName").toString());
                obj.insert("accountId", accountId);
                Net->onlineMap.insert(accountId, socket);  // accountId  绑定 socket
            }
        } else {
            // 密码不正确
            obj.insert("result", 2);
        }
    } else {
        // todo 不存在用户id
        obj.insert("result", 1);
    }
    this->send(socket, CodeNS::S2C_LOGIN, obj);
}

void Handler::handleRegister(QTcpSocket *socket, QJsonObject data) {
    QString userName = data.value("userName").toString();
    QString password = data.value("password").toString();
    QJsonObject obj;

    QString accountId = userDB.addUser(userName, password);
    qDebug() << "test " << accountId;
    if (!accountId.isEmpty()) {
        obj.insert("result", 0);
        obj.insert("accountId", accountId);
        obj.insert("userName", userName);
        obj.insert("password", password);
    } else {
        obj.insert("result", 1);
    }
    this->send(socket, CodeNS::S2C_REGISTER, obj);
}

void Handler::handleGetFriendsList(QTcpSocket *socket, QJsonObject data) {
    QString accountId = data.value("accountId").toString();

    this->updateFriendList(socket, accountId);
}

void Handler::updateFriendList(QTcpSocket *socket, QString accountId, QString friendId) {
    // 更新好友列表
    QJsonObject obj;
    QString friendIds = friendDB.selectFriend(accountId).value("friendId").toString();
    auto friendUserNames = this->getUserNames(friendIds);
    obj.insert("friendIds", friendIds);
    obj.insert("friendUserNames", friendUserNames);
    this->send(socket, CodeNS::S2C_UPDATE_FRIENDS_LIST, obj);

    // 更新对方好友列表
    if (friendId.isEmpty()){
        return;
    }
    QJsonObject obj2;
    QTcpSocket *socket2 = Net->onlineMap.value(friendId, nullptr);
    if (socket2){  // 如果在线
        QString friendIds2 = friendDB.selectFriend(friendId).value("friendId").toString();
        auto friendUserNames2 = this->getUserNames(friendIds2);
        obj2.insert("friendIds", friendIds2);
        obj2.insert("friendUserNames", friendUserNames2);
        this->send(socket2, CodeNS::S2C_UPDATE_FRIENDS_LIST, obj2);
    }
}

void Handler::handleFindFriend(QTcpSocket *socket, QJsonObject data) {
    QString accountId = data.value("accountId").toString();
    QJsonObject queryResult = userDB.selectUser(accountId);
    QJsonObject obj;

    if (!queryResult.value("accountId").toString().isEmpty()) {
        // 查找成功
        obj.insert("result", 0);
        accountId = queryResult.value("accountId").toString();
        QString userName = queryResult.value("userName").toString();
        obj.insert("accountId", accountId);
        obj.insert("userName", userName);
    } else {
        // 查无此用户
        obj.insert("result", 1);
    }
    this->send(socket, CodeNS::S2C_FIND_FRIEND, obj);
}

void Handler::handleAddFirend(QTcpSocket *socket, QJsonObject data) {
    QString accountId = data.value("accountId").toString();
    QString friendId = data.value("friendId").toString();
    QJsonObject obj;

    // 判断是否存在好友
    QString friends = friendDB.selectFriend(accountId).value("friendId").toString();
    if (accountId == friendId) {
        obj.insert("result", 2);  // 不能添加自己为好友
    } else if (friends.split(',').contains(friendId)) {
        obj.insert("result", 1);  // 好友已经存在
    } else {
        friendDB.addFriend(accountId, friendId);
        friendDB.addFriend(friendId, accountId);
        obj.insert("result", 0);
        obj.insert("accountId", accountId);
        obj.insert("friendId", friendId);

        this->updateFriendList(socket, accountId, friendId);
    }
    this->send(socket, CodeNS::S2C_ADD_FRIEND, obj);
}

void Handler::handleRemoveFriend(QTcpSocket *socket, QJsonObject data) {
    QString accountId = data.value("accountId").toString();
    QString friendId = data.value("friendId").toString();
    QJsonObject obj;

    if (friendDB.removeFriend(accountId, friendId)) {
        obj.insert("result", 0);
        obj.insert("accountId", accountId);
        obj.insert("friendId", friendId);
        obj.insert("friendUserName", userDB.selectUser(friendId).value("userName").toString());
    } else {
        obj.insert("result", 1);  // 好友不存在
    }
    this->send(socket, CodeNS::S2C_REMOVE_FRIEND, obj);

    // 双向删除好友
    friendDB.removeFriend(friendId, accountId);

    // 更新好友列表
    this->updateFriendList(socket, accountId, friendId);

    // 通知被删除
    auto mSocket = Net->onlineMap.value(friendId, nullptr);
    if (mSocket) {
        QJsonObject obj2;
        obj2.insert("result", 0);
        obj2.insert("accountId", friendId);
        obj2.insert("friendId", accountId);
        obj2.insert("friendUserName", userDB.selectUser(friendId).value("userName").toString());
        this->send(mSocket, CodeNS::S2C_REMOVE_FRIEND, obj2);
    }
}

void Handler::handleSendMsg(QTcpSocket *socket, QJsonObject data) {
    QString fromId = data.value("fromId").toString();
    QString toId = data.value("toId").toString();
    QString msg = data.value("msg").toString();
    QString fromUserName = userDB.selectUser(fromId).value("userName").toString();
    QJsonObject obj;

    QTcpSocket *toSocket = Net->onlineMap.value(toId, nullptr);
    if (toSocket) {
        obj.insert("result", 0);
        obj.insert("fromId", fromId);
        obj.insert("fromUserName", fromUserName);
        obj.insert("toId", toId);
        obj.insert("msg", msg);
        this->send(toSocket, CodeNS::S2C_NEW_MSG, obj);
    } else {
        obj.insert("result", 1);
    }
    this->send(socket, CodeNS::S2C_SEND_MSG, obj);
}

void Handler::handleGetGroupList(QTcpSocket *socket, QJsonObject data) {
    QString accountId = data.value("accountId").toString();

    this->updateGroupList(socket, accountId);
}

void Handler::handleCreateGroup(QTcpSocket *socket, QJsonObject data) {
    QString groupName = data.value("groupName").toString();
    QString accountId = data.value("accountId").toString();
    QJsonObject obj;

    QString groupId = friendDB.createGroup(accountId, groupName);
    friendDB.joinGroupInFriends(accountId, groupId);  // friends表操作
    if (!groupId.isEmpty()) {
        obj.insert("result", 0);
        obj.insert("groupName", groupName);
        obj.insert("accountId", accountId);
        obj.insert("groupId", groupId);
    }else {
        obj.insert("result", 1);
    }
    this->send(socket, CodeNS::S2C_CREATE_GROUP, obj);

    // 更新群列表
    this->updateGroupList(socket, accountId);
}

void Handler::updateGroupList(QTcpSocket *socket, QString accountId) {
    QJsonObject obj;
    QString groups = friendDB.selectFriend(accountId).value("groupId").toString();
    auto groupNames = this->getGroupNames(groups);

    obj.insert("groups", groups);
    obj.insert("groupNames", groupNames);
    this->send(socket, CodeNS::S2C_UPDATE_GROUPS_LIST, obj);
}

void Handler::handleFindGroup(QTcpSocket *socket, QJsonObject data) {
    QString groupId = data.value("groupId").toString();
    QJsonObject obj;

    QString groupName = friendDB.findGroup(groupId);
    if (!groupName.isEmpty()) {
        obj.insert("result", 0);
        obj.insert("groupId", groupId);
        obj.insert("groupName", groupName);
    } else {
        obj.insert("result", 1);
    }
    this->send(socket, CodeNS::S2C_FIND_GROUP, obj);
}

void Handler::handleJoinGroup(QTcpSocket *socket, QJsonObject data) {
    QString accountId = data.value("accountId").toString();
    QString groupId = data.value("groupId").toString();
    QString groupName = friendDB.findGroup(groupId);
    QJsonObject obj;

    if (friendDB.joinGroupInGroups(accountId, groupId)) {
        friendDB.joinGroupInFriends(accountId, groupId);  // friends表操作
        obj.insert("result", 0);
        obj.insert("groupName", groupName);
        obj.insert("groupId", groupId);
    } else {
        obj.insert("result", 1);
    }
    this->send(socket, CodeNS::S2C_JOIN_GROUP, obj);

    // 更新群列表
    this->updateGroupList(socket, accountId);

    // 通知所有群成员更新群成员 -_-!
    this->updateMembers(groupId);
}

void Handler::handleUpdataMember(QTcpSocket *socket, QJsonObject data) {
    QString groupId = data.value("groupId").toString();
    QString groupName = friendDB.selectGroup(groupId).value("groupName").toString();
    QJsonObject obj;

    QString members = friendDB.selectGroup(groupId).value("memberId").toString();
    auto memberUserNames = this->getUserNames(members);
    obj.insert("groupId", groupId);
    obj.insert("groupName", groupName);
    obj.insert("memberUserNames", memberUserNames);
    this->send(socket, CodeNS::S2C_UPDATE_MEMBERS, obj);
}

void Handler::handleSendGroupMsg(QTcpSocket *socket, QJsonObject data) {
    QString accountId = data.value("accountId").toString();
    QString groupId = data.value("groupId").toString();
    QString msg = data.value("msg").toString();
    QJsonObject obj;

    QString memberId = friendDB.getGroupMembers(groupId);
    QStringList memberIdList = memberId.split(',');
    for (QString toId: memberIdList) {
        auto mSocket = Net->onlineMap.value(toId);
        if (!mSocket)
            continue;
        QString fromUserName = userDB.selectUser(accountId).value("userName").toString();
        obj.insert("fromId", accountId);
        obj.insert("fromUserName", fromUserName);
        obj.insert("toId", toId);
        obj.insert("msg", msg);
        obj.insert("groupId", groupId);
        this->send(mSocket, CodeNS::S2C_NEW_GROUP_MSG, obj);
    }
}

void Handler::handleQuitGroup(QTcpSocket *socket, QJsonObject data) {
    QString accountId = data.value("accountId").toString();
    QString groupId = data.value("groupId").toString();
    QString groupName = friendDB.selectGroup(groupId).value("groupName").toString();
    QJsonObject obj;
    if (friendDB.quitGroupInGroups(accountId, groupId)){
        // 解散
        obj.insert("result", 0);
        obj.insert("groupId", groupId);
        obj.insert("groupName", groupName);
    } else {
        // 退群
        obj.insert("result", 1);
        obj.insert("groupId", groupId);
        obj.insert("groupName", groupName);
    }
    friendDB.quitGroupInFriends(accountId, groupId);  // friends表操作

    this->send(socket, CodeNS::S2C_QUIT_GROUP, obj);

    // 更新群列表
    this->updateGroupList(socket, accountId);

    // 通知所有群成员更新群成员 -_-!
    this->updateMembers(groupId);
}

void Handler::updateMembers(QString groupId) {
    auto result = friendDB.selectGroup(groupId);
    if (!result.isEmpty()) {
        // 获取所有群成员
        auto members = result.value("memberId").toString();
        for (auto memberId: members.split(',')) {
            // 通知所有群成员
            auto socket = Net->onlineMap.value(memberId, nullptr);
            if (socket) {
                QJsonObject obj;
                auto groupName = result.value("groupName").toString();
                // 获取成员昵称，通过成员id
                auto memberUserNames = this->getUserNames(members);
                obj.insert("memberUserNames", memberUserNames);
                obj.insert("groupName", groupName);
                obj.insert("groupId", groupId);
                this->send(socket, CodeNS::S2C_UPDATE_MEMBERS, obj);
            }
        }
    }
}

void Handler::send(QTcpSocket *socket, Code code, QJsonObject data) {
    QByteArray result = JsonTool->generatePackage(code, data);
    socket->write(result);
    socket->write(";");
}


