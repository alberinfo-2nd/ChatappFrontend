#ifndef CHATPAGE_H
#define CHATPAGE_H

#include <QWidget>
#include "user.h"
#include <QObject>
#include "ActiveUsersManager.h"
#include "sessionManager.h"
#include "BackendClient.h"
#include <QPushButton>

class QVBoxLayout;
class QLabel;

namespace Ui
{
    class ChatPage;
}

class ChatPage : public QWidget
{
    Q_OBJECT

public:
    // constructor
    ChatPage(QWidget *parent = nullptr, SessionManager *sessionManager = nullptr, ActiveUsersManager *activeUsersManager = nullptr, BackendClient *backendClient = nullptr, User partnerName = {});
    ~ChatPage();
    // Sets the target user for the current conversation
    void setChatPartner(QString username);

signals:
    void backToUserListRequested(); // Used to change views

public slots:
    void displayActiveUsers();                                        // Refreshes the side-bar user list
    void updateChatDisplay(const QString &username, const QString &publicKey, bool isReported); // Loads conversation history

private slots:
    // Action Handlers
    void sendMessage();

private:
    Ui::ChatPage *ui;
    // Layout Management
    QVBoxLayout *chatScrollAreaLayout = nullptr;
    QVBoxLayout *userListScrollAreaLayout = nullptr;
    // Maps usernames to their row widgets
    QHash<QString, QWidget *> activeUserLabels;
    // Core Managers
    SessionManager *m_sessionManager;
    ActiveUsersManager *m_activeUsersManager;
    BackendClient *m_backendClient;
    // Conversation State
    User m_currentPartner;
    QLabel *chatPartnerLabel;
    QPushButton *reportBtn;

    // UI helper functions
    QLabel *createNewMessageLabel(const QString &message);
    void displayReceivedMessage();
    void displaySentMessage(const QString &message);
    void removeActiveUser(const QString &username);
    void alternateLabelStyle();
    void clearDisplayedMessages();
    void clearActiveUserList();
    std::string encryptMessage(const QString &message, const QString &publicKey);
    QString decryptMessage(const QString &message);
};

#endif // CHATPAGE_H
