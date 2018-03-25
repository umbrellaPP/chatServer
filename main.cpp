#include <QCoreApplication>
#include <QtSql/QSqlDatabase>
#include <QtSql/QSqlQuery>
#include <QDebug>
#include "db/userdb.h"
#include "net/server.h"
#include "net/handler.h"

#include <QTcpServer>
#include <QHostAddress>

#include <QSqlQuery>

#include "util/jsonparser.h"

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);
    QSqlDatabase::addDatabase("QMYSQL");

    Net->initWeb();

    return a.exec();
}
