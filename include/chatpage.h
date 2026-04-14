#ifndef CHATPAGE_H
#define CHATPAGE_H

#include <QWidget>
#include "user.h"
#include <QObject>
#include "ActiveUsersManager.h"
#include "sessionManager.h"

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
    ChatPage(QWidget *parent = nullptr, SessionManager *sessionManager = nullptr, ActiveUsersManager *activeUsersManager = nullptr, User partnerName={});
    ~ChatPage();

signals:
    void backToUserListRequested(); // signal for exit button in chat

public slots:
    void displayActiveUsers();
    void switchToNewChat(const QString &username);


private slots:
    // slot functions
    void sendMessage();

private:
    // member attriubutes
    Ui::ChatPage *ui;
    QVBoxLayout *chatScrollAreaLayout = nullptr;
    QVBoxLayout *userListScrollAreaLayout = nullptr;
    QHash<QString, QWidget*> activeUserLabels; // changed to qwidget
    SessionManager *m_sessionManager;
    ActiveUsersManager *m_activeUsersManager;
    User m_currentPartnerName;
    QLabel* chatPartnerLabel;

    //helper functions
    QLabel* createNewMessageLabel(const QString &message);
    void displayReceivedMessage(const QString &message);
    void displaySentMessage(const QString &message);
    void removeActiveUser(const QString &username);
    void alternateLabelStyle();
    void clearDisplayedMessages();
    void clearActiveUserList();
};

#endif // CHATPAGE_H
