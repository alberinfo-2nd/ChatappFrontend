#ifndef CHATPAGE_H
#define CHATPAGE_H

#pragma once
#include <QWidget>
#include <vector>
#include <string>

class QVBoxLayout;
class QLabel;
class User;

namespace Ui {
class ChatPage;
}

class ChatPage : public QWidget
{
    Q_OBJECT

public:
    // constructor
    ChatPage(QWidget *parent = nullptr, User* currentUser = nullptr);
    ~ChatPage();



private slots:
    // slot functions
    void sendMessage();
    void clearDisplayedMessages();

private:
    // member attriubutes
    Ui::ChatPage *ui;
    QVBoxLayout* chatScrollAreaLayout;
    QVBoxLayout* userListScrollAreaLayout;
    QHash<QString, QLabel*> activeUserLabels;
    User* m_currentUser;

    //helper functions
    QLabel* createNewMessageLabel(const QString &message);
    void displayReceivedMessage(const QString &message);
    void displaySentMessage(const QString &message);
    void displayActiveUsers(const std::vector<std::string> &users);
    void removeActiveUser(const QString &username);
    void alternateLabelStyle();
};

#endif // CHATPAGE_H
