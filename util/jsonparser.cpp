#include "jsonparser.h"

#include <QStringList>
#include <QDebug>
#include <QJsonParseError>

JsonParser::JsonParser() {

}

void JsonParser::getPackageData(QByteArray package, Code *code, QJsonObject *obj) {
    QJsonObject json = this->toJson(package);
    *code = json.keys()[0];
    *obj = json.value(*code).toObject();
}

QByteArray JsonParser::generatePackage(Code code, QJsonObject obj) {
    QByteArray data = QJsonDocument(obj).toJson();
    QString json = QString("{\"%1\":%2}").arg(code).arg(QString(data));
    return json.toUtf8();
}

QJsonObject JsonParser::toJson(QByteArray text) {
    QJsonDocument doc = QJsonDocument::fromJson(text);
    QJsonObject obj = doc.object();
    return obj;
}

QByteArray JsonParser::toString(QJsonObject obj) {
    QJsonDocument doc(obj);
    return doc.toJson();
}
