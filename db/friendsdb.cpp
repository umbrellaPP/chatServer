#include "friendsdb.h"
#include <QStringList>
#include <QDebug>
#include <time.h>

FriendsDB::FriendsDB() {
    this->db = QSqlDatabase::database();
}

bool FriendsDB::addFriend(QString accountId, QString friendId) {
    QString friends = this->selectFriend(accountId).value("friendId").toString();
    QString queryCmd;
    if (!friends.isEmpty()) {
        queryCmd = QString("update friends set friendId='%1' where accountId='%2';")
                .arg(friends+","+friendId).arg(accountId);
    } else {
        queryCmd = QString("insert into friends values(NULL, '%1', '%2')")
                .arg(accountId).arg(friendId);
    }
    qDebug() << queryCmd;
    QSqlQuery query(this->db);

    if (query.exec(queryCmd)) {
//        qDebug() << "添加好友成功";
        return true;
    } else {
//        qDebug() << "添加好友失败";
        return false;
    }
}

bool FriendsDB::removeFriend(QString accountId, QString friendId) {
    QString friends = this->selectFriend(accountId).value("friendId").toString();
    if (friends.indexOf(friendId) < 0) {
        return false;
    }
    QStringList friendList = friends.split(',');
    for (int i = 0; i < friendList.count(); ++i) {
        if (friendList[i] == friendId) {
            friendList.removeAt(i);
            break;
        }
    }
    friends = friendList.join(',');
    QString queryCmd = QString("update friends set friendId = '%1' where accountId = '%2'")
            .arg(friends).arg(accountId);
    QSqlQuery query(this->db);
    query.exec(queryCmd);
    return true;
}

QJsonObject FriendsDB::selectFriend(QString accountId) {
    QString queryCmd = QString("select * from friends where accountId = '%1';")
            .arg(accountId);
    QSqlQuery query(this->db);
    QJsonObject obj;

    if (query.exec(queryCmd)) {
        if (query.next()) {
            QString friendId = query.value("friendId").toString();
            QString groupId = query.value("groupId").toString();
            obj.insert("accountId", accountId);
            obj.insert("friendId", friendId);
            obj.insert("groupId", groupId);
        }
    }
    return obj;
}

QJsonObject FriendsDB::selectGroup(QString groupId) {
    QSqlQuery query(this->db);
    QString queryCmd = QString("select * from groups where groupId = '%1'")
            .arg(groupId);
    QJsonObject obj;
    query.exec(queryCmd);
    if (query.next()){
        QString groupName = query.value("groupName").toString();
        QString memberId = query.value("memberId").toString();
        obj.insert("groupId", groupId);
        obj.insert("groupName", groupName);
        obj.insert("memberId", memberId);
    }
    return obj;
}

QString FriendsDB::createGroup(QString accountId, QString groupName) {
    QString groupId = this->generateGroupID();
    QSqlQuery query(this->db);
    QString queryCmd = QString("insert into groups(groupId, groupName, memberId) values('%1', '%2', '%3')")
            .arg(groupId).arg(groupName).arg(accountId);
    if (query.exec(queryCmd)){
//        qDebug() << "建群成功";
        return groupId;
    }else {
//        qDebug() << "建群失败" << query.executedQuery();
        return "";
    }

}

QString FriendsDB::findGroup(QString groupId) {
    QSqlQuery query(this->db);
    QString queryCmd = QString("select groupName from groups where groupId = '%1';")
            .arg(groupId);
    query.exec(queryCmd);

    if (query.next()) {
        return query.value("groupName").toString();
    }else {
        return "";
    }
}

QString FriendsDB::getGroupMembers(QString groupId) {
    QString members = this->selectGroup(groupId).value("memberId").toString();
    return members;
}


bool FriendsDB::joinGroupInGroups(QString accountId, QString groupId) {
    QString memberId = this->getGroupMembers(groupId);
    if (memberId.indexOf(accountId) != -1) {
        return false;
    } else if (memberId.isEmpty()){
        memberId += accountId;
    } else {
        memberId += "," + accountId;
    }
    QSqlQuery query(this->db);
    QString queryCmd = QString("update groups set memberId = '%1' where groupId = '%2'")
            .arg(memberId).arg(groupId);
    query.exec(queryCmd);
    return true;
}

bool FriendsDB::quitGroupInGroups(QString accountId, QString groupId) {
    QString memberId = this->getGroupMembers(groupId);
    QStringList memberList = memberId.split(',');
    for (int i = 0; i < memberList.count(); ++i) {
        if (memberList[i] == accountId) {
            memberList.removeAt(i);
            break;
        }
    }
    memberId = memberList.join(',');
    QSqlQuery query(this->db);
    QString queryCmd;
    if (memberId.isEmpty()){  // 解散
        queryCmd = QString("delete from groups where groupId = '%1'")
                    .arg(groupId);
        query.exec(queryCmd);
        return true;
    }else {  // 退群
        queryCmd = QString("update groups set memberId='%1' where groupId='%2'")
                    .arg(memberId).arg(groupId);
        query.exec(queryCmd);
        return false;
    }
}

void FriendsDB::joinGroupInFriends(QString accountId, QString groupId) {
    QString groups = this->selectFriend(accountId).value("groupId").toString();
    if (groups.isEmpty()) {
        groups = groupId;
    } else {
        groups += ',' + groupId;
    }

    QSqlQuery *query = new QSqlQuery(this->db);
    QString queryCmd = QString("update friends set groupId = '%1' where accountId = '%2'")
            .arg(groups).arg(accountId);
    query->exec(queryCmd);
}

void FriendsDB::quitGroupInFriends(QString accountId, QString groupId) {
    QString groups = this->selectFriend(accountId).value("groupId").toString();
    QStringList groupList = groups.split(',');
    for (int i = 0; i < groupList.count(); ++i) {
        if (groupId == groupList[i]) {
            groupList.removeAt(i);
            break;
        }
    }
    groups = groupList.join(',');

    QSqlQuery *query = new QSqlQuery(this->db);
    QString queryCmd = QString("update friends set groupId = '%1' where accountId = '%2'")
            .arg(groups).arg(accountId);
    query->exec(queryCmd);
}

QString FriendsDB::generateGroupID() {
    srand(time(0));
    int id;
    bool isExists = false;
    do {
        id = qrand();
        isExists = isGroupIDExists(QString::number(id));
    } while((id < 1000 || id > 9999) && isExists);
    return QString::number(id);
}

bool FriendsDB::isGroupIDExists(QString groupId) {
    QString queryCmd = QString("select * from groups");
    QSqlQuery query(this->db);
    query.exec(queryCmd);
    while (query.next()) {
        if (groupId == query.value("groupId").toString()) {
            return true;
        }
    }
    return false;
}
