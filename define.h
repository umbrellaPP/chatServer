#ifndef DEFINE_H
#define DEFINE_H
#include <QString>

typedef QString Code;

namespace CodeNS {
    const QString C2S_LOGIN = "C2S_LOGIN";
    const QString C2S_REGISTER = "C2S_REGISTER";
    const QString C2S_GET_FRIENDS_LIST = "C2S_GET_FRIENDS_LIST";
    const QString C2S_FIND_FRIEND = "C2S_FIND_FRIEND";
    const QString C2S_ADD_FRIEND = "C2S_ADD_FRIEND";
    const QString C2S_REMOVE_FRIEND = "C2S_REMOVE_FRIEND";
    const QString C2S_SEND_MSG = "C2S_SEND_MSG";
    const QString C2S_GET_GROUP_LIST = "C2S_GET_GROUP_LIST";
    const QString C2S_CREATE_GROUP = "C2S_CREATE_GROUP";
    const QString C2S_FIND_GROUP = "C2S_FIND_GROUP";
    const QString C2S_JOIN_GROUP = "C2S_JOIN_GROUP";
    const QString C2S_UPDATE_MEMBERS = "C2S_UPDATE_MEMBERS";
    const QString C2S_SEND_GROUP_MSG = "C2S_SEND_GROUP_MSG";
    const QString C2S_QUIT_GROUP = "C2S_QUIT_GROUP";

    const QString S2C_LOGIN = "S2C_LOGIN";  // result 0成功 1用户id错误 2密码错误 3重复登录
    const QString S2C_REGISTER = "S2C_REGISTER";   // result 0成功 1注册失败
    const QString S2C_UPDATE_FRIENDS_LIST = "S2C_UPDATE_FRIENDS_LIST";  // result 0成功 1获取好友列表失败
    const QString S2C_FIND_FRIEND = "S2C_FIND_FRIEND";  // result 0成功 1查无此用户
    const QString S2C_ADD_FRIEND = "S2C_ADD_FRIEND";  // result 0成功 1已经是好友 2不能添加自己为好友
    const QString S2C_REMOVE_FRIEND = "S2C_REMOVE_FRIEND";  // result 0成功 1好友不存在
    const QString S2C_SEND_MSG = "S2C_SEND_MSG";  // result 0成功 1对方不在线
    const QString S2C_NEW_MSG = "S2C_NEW_MSG";  // 略
    const QString S2C_CREATE_GROUP = "S2C_CREATE_GROUP";  // result 0成功 1失败
    const QString S2C_FIND_GROUP = "S2C_FIND_GROUP";  // result 0成功 1查无此群
    const QString S2C_JOIN_GROUP = "S2C_JOIN_GROUP";  // result 0成功 1已经加入
    const QString S2C_UPDATE_GROUPS_LIST = "S2C_UPDATE_GROUPS_LIST";  // 略
    const QString S2C_UPDATE_MEMBERS = "S2C_UPDATE_MEMBERS";  // 略
    const QString S2C_NEW_GROUP_MSG = "S2C_NEW_GROUP_MSG";  // 略
    const QString S2C_QUIT_GROUP = "S2C_QUIT_GROUP";  // result 0解散 1退群
}

#endif // DEFINE_H
