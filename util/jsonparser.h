#ifndef JSONPARSER_H
#define JSONPARSER_H

#include <QJsonObject>
#include <QJsonArray>
#include <QJsonValue>
#include <QJsonDocument>

#include "../define.h"

#define JsonTool JsonParser::getInstance()

class JsonParser
{
public:
    static JsonParser* getInstance() {
        static JsonParser *_instance;
        return _instance;
    }

    void getPackageData(QByteArray package, Code *code, QJsonObject *obj);
    QByteArray generatePackage(Code code, QJsonObject obj);

private:
    QJsonObject toJson(QByteArray text);  // 从json 文本到 json
    QByteArray toString(QJsonObject obj);  // 从json 到json文本

private:
    JsonParser();
};

#endif // JSONPARSER_H
