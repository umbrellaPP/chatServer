#ifndef DEFINE_H
#define DEFINE_H

enum Code {
    C2S_LOGIN = 1,
    C2S_REGISTER,


    S2C_LOGIN = 1000,  // result 0成功 1用户id错误 2密码错误
    S2C_REGISTER
};

#endif // DEFINE_H
