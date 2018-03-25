#ifndef USERDB_H
#define USERDB_H

#include <QSqlDatabase>
#include <QSqlQuery>


class UserDB
{
public:
    UserDB();
    void addUser(QString userName, QString password);  // 添加用户
    void removeUser(QString accountId);  // 删除用户
    QSqlQuery* selectAllUsers();  // 查找所有用户
    QSqlQuery* selectUser(QString accountId);  // 查找用户
    void updateUser(QString accountId, QString userName, QString password);  // 修改用户

private:
    QString generateAcountID();  // 生成账号ID
    bool isAccountIDExists(QString accountId);  // 判断ID 是否已经存在

    QSqlDatabase db;
};

#endif // USERDB_H
