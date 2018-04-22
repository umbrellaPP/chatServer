#ifndef DEFINE_H
#define DEFINE_H

enum Code {
    C2S_LOGIN = 1,
    C2S_REGISTER,
    C2S_GET_FRIENDS_LIST,
    C2S_FIND_FRIEND,
    C2S_ADD_FRIEND,
    C2S_REMOVE_FRIEND,
    C2S_SEND_MSG,
    C2S_GET_GROUP_LIST,
    C2S_CREATE_GROUP,
    C2S_FIND_GROUP,
    C2S_JOIN_GROUP,
    C2S_UPDATE_MEMBERS,
    C2S_SEND_GROUP_MSG,
    C2S_QUIT_GROUP,

    S2C_LOGIN = 1000,  // result 0成功 1用户id错误 2密码错误 3重复登录
    S2C_REGISTER,   // result 0成功 1注册失败
    S2C_UPDATE_FRIENDS_LIST,  // result 0成功 1获取好友列表失败
    S2C_FIND_FRIEND,  // result 0成功 1查无此用户
    S2C_ADD_FRIEND,  // result 0成功 1已经是好友 2不能添加自己为好友
    S2C_REMOVE_FRIEND,  // result 0成功 1好友不存在
    S2C_SEND_MSG,  // result 0成功 1对方不在线
    S2C_NEW_MSG,  // 略
    S2C_CREATE_GROUP,  // result 0成功 1失败
    S2C_FIND_GROUP,  // result 0成功 1查无此群
    S2C_JOIN_GROUP,  // result 0成功 1已经加入
    S2C_UPDATE_GROUPS_LIST,  // 略
    S2C_UPDATE_MEMBERS,  // 略
    S2C_NEW_GROUP_MSG,  // 略
    S2C_QUIT_GROUP,  // result 0解散 1退群
};

#endif // DEFINE_H
