#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#pragma once
#include <QMainWindow>

// Foward declaration of classes used
class LoginPage;
class UserListPage;
class ChatPage;
class User;

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
    Ui::MainWindow *ui;


    // Delcaration of pointers for each QWidget
    LoginPage *loginPage;
    UserListPage *userListPage;
    ChatPage *chatPage;

    User* currentUser;
};

#endif // MAINWINDOW_H
