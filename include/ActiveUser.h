#ifndef ACTIVEUSER_H
#define ACTIVEUSER_H

#include <QString>

// This will change later for now this is ok (plan to change to class)
// Struct for store an active user
struct ActiveUser {
public:
    QString username;
    QString public_key;
};

#endif // ACTIVEUSER_H
