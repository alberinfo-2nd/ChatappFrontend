#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "userlistpage.h"
#include <QMainWindow>

// Foward declaration of classes used
class LoginPage;
class UserListPage;
class ChatPage;


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

// Declaring Member attributes
private:
    QString storedUsername; // temporary store for testing
    Ui::MainWindow *ui;

    // Delcaration of pointers for each QWidget
    LoginPage *loginPage;
    UserListPage *userListPage = new UserListPage(this);
    ChatPage *chatPage;
};
#endif // MAINWINDOW_H
