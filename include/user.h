#ifndef USER_H
#define USER_H

#include <QString>

/****************************************
This will later be changed to a class
until then this works
*****************************************/

struct User {
    QString username;
    QString public_key;
    QString authorizationToken;
};

#endif