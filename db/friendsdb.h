#ifndef FRIENDSDB_H
#define FRIENDSDB_H

#include <QSqlDatabase>
#include <QSqlQuery>
#include <QJsonObject>

class FriendsDB
{
public:
    FriendsDB();

    bool addFriend(QString accountId, QString friendId);  // 添加好友
    bool removeFriend(QString accountId, QString friendId);  // 删除好友
    QJsonObject selectFriend(QString accountId);  // 获取所有好友

    QJsonObject selectGroup(QString groupId);
    QString createGroup(QString accountId, QString groupName);  // 建群
    QString findGroup(QString groupId);
    QString getGroupMembers(QString groupId);  // 获取所有群成员

    bool joinGroupInGroups(QString accountId, QString groupId);  // 加群  groups表操作
    bool quitGroupInGroups(QString accountId, QString groupId);  // 退群  groups表操作

    void joinGroupInFriends(QString accountId, QString groupId);  // 加群  friends表操作
    void quitGroupInFriends(QString accountId, QString groupId);  // 退群  friends表操作
private:
    QSqlDatabase db;

    QString generateGroupID();
    bool isGroupIDExists(QString groupId);
};

#endif // FRIENDSDB_H
