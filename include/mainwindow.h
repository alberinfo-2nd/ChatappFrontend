#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "user.h"

// Foward declaration of classes used
class LoginPage;
class UserListPage;
class ChatPage;
class BackendClient;
class SessionManager;
class ActiveUsersManager;


QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

// MainWindow Class
class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow() override;

// slots used for showing each page
private slots:
    void showLoginPage();
    void showUserListPage();
    void showChatPage();
    void handleSuccessfulLogin(const QString &username, const QString &public_key, const QString &authorizationToken);
    void handleChatRequest(const QString &username, const QString &publick_key);

// Declaring Member attributes
private:
    Ui::MainWindow *ui;
    User m_currentUser;
    SessionManager *sessionManager;
    ActiveUsersManager *activeUsersManager;
    BackendClient *backendClient;

    // Delcaration of pointers for each QWidget
    LoginPage *loginPage;
    UserListPage *userListPage;
    ChatPage *chatPage;
    ChatPage* currentChatPage = nullptr;
};
#endif // MAINWINDOW_H
