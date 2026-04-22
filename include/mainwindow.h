#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

// Foward declaration of classes used
class LoginPage;
class UserListPage;
class ChatPage;
class BackendClient;
class SessionManager;
class ActiveUsersManager;

QT_BEGIN_NAMESPACE
namespace Ui
{
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

private slots:
    // Controls which page is currently visible in the stacked widget
    void showLoginPage();
    void showUserListPage();
    void showChatPage();
    void handleSuccessfulLogin(const QString &username, const QString &publicKey, const QByteArray &privateKey, const QString &authorizationToken, const bool &isAdmin);
    void handleChatRequest(const QString &username, const QString &publicKey);

    // Declaring Member attributes
private:
    Ui::MainWindow *ui;
    // Core Managers
    SessionManager *sessionManager;
    ActiveUsersManager *activeUsersManager;
    BackendClient *backendClient;

    // Delcaration of pointers for each QWidget
    LoginPage *loginPage;
    UserListPage *userListPage;
    ChatPage *chatPage;
};
#endif // MAINWINDOW_H
