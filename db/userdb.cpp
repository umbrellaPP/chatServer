#include "userdb.h"
#include <QDebug>
#include <time.h>

UserDB::UserDB() {
    this->db = QSqlDatabase::database();
    this->db.setHostName("localhost");
    this->db.setPort(3306);
    this->db.setDatabaseName("chat");
    this->db.setUserName("root");
    this->db.setPassword("");
    if (this->db.open()){
        qDebug() << "数据库打开成功";
    }else {
        qDebug() << "数据库打开失败";
    }
}

void UserDB::addUser(QString userName, QString password) {
    QString accountId = this->generateAcountID();
    QSqlQuery query(this->db);
    QString queryCmd = QString("insert into user values(NULL, '%1', '%2', '%3')")
            .arg(accountId).arg(userName).arg(password);
    qDebug() << queryCmd;

    if (query.exec(queryCmd)){
        qDebug() << "添加用户成功";
    }else {
        qDebug() << "添加用户失败" << query.executedQuery();
    }
}

void UserDB::removeUser(QString accountId) {
    QSqlQuery *query = new QSqlQuery(this->db);
    QString queryCmd = QString("delete from user where accountId = '%1'").arg(accountId);
    if (query->exec(queryCmd)) {
        qDebug() << QString("删除用户'%1'成功").arg(accountId);
    } else {
        qDebug() << QString("删除用户'%1'失败").arg(accountId) << query->executedQuery();
    }
}

QSqlQuery* UserDB::selectAllUsers() {
    QSqlQuery *query = new QSqlQuery(this->db);
    if (query->exec("select * from user")) {
        qDebug() << "查询所有用户成功";
        return query;
    } else {
        qDebug() << "查询所有用户失败" << query->executedQuery();
        return Q_NULLPTR;
    }
}

QSqlQuery* UserDB::selectUser(QString accountId) {
    QSqlQuery *query = new QSqlQuery(this->db);
    QString queryCmd = QString("select * from user where accountId = '%1'").arg(accountId);
    if (query->exec(queryCmd)) {
        qDebug() << QString("查询用户'%1'成功").arg(accountId);
        return query;
    } else {
        qDebug() << QString("查询用户'%1'失败").arg(accountId) << query->executedQuery();
        return Q_NULLPTR;
    }
}

void UserDB::updateUser(QString accountId, QString userName, QString password) {
    QSqlQuery *query = new QSqlQuery(this->db);
    QString queryCmd = QString("update user set userName = '%1', password = '%2' where accountId = '%3'")
            .arg(userName).arg(password).arg(accountId);
    if (query->exec(queryCmd)) {
        qDebug() << QString("更新用户'%1'成功").arg(accountId);
    } else {
        qDebug() << QString("更新用户'%1'失败").arg(accountId) << query->executedQuery();
    }
}

QString UserDB::generateAcountID() {
    srand(time(0));
    int id;
    bool isExists = false;
    do {
        id = qrand();
        isExists = isAccountIDExists(QString::number(id));
    } while((id < 1000 || id > 9999) && isExists);
    return QString::number(id);
}

bool UserDB::isAccountIDExists(QString accountId) {
    QSqlQuery* query = this->selectAllUsers();
    while (query->next()) {
        if (accountId == query->value("accountId").toString()) {
            return true;
        }
    }
    return false;
}
