#include <QCoreApplication>
#include <QtSql/QSqlDatabase>
#include <QtSql/QSqlQuery>
#include <QDebug>
#include "db/userdb.h"

#include <QSqlQuery>

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);
    QSqlDatabase::addDatabase("QMYSQL");

    UserDB *userDB = new UserDB();
//    userDB->addUser("Test", "wqqqqqq");
//    userDB->removeUser("14533");
//    userDB->selectAllUsers();
//    userDB->selectUser("14569");
//    userDB->updateUser("14568", "PP", "wqnmlgsb");
    return a.exec();
}
