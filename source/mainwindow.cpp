#include "mainwindow.h"
#include "./ui_mainwindow.h"
#include "chatpage.h"
#include "loginpage.h"
#include "userlistpage.h"

// Constructor of MainWindow Creates a pointer for each QWidget object(page)
MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
    , loginPage(new LoginPage(this))
    , chatPage(new ChatPage(this))
    , userListPage(new UserListPage(this))

{
    ui->setupUi(this);

    // add each QWidget object(page) to stackedWidget
    ui->stackedWidget->addWidget(loginPage);
    ui->stackedWidget->addWidget(chatPage);
    ui->stackedWidget->addWidget(userListPage);

    // set Login Page as the default page to display
    ui->stackedWidget->setCurrentWidget(loginPage);
}

// MainWindow destructor
MainWindow::~MainWindow()
{
    delete ui;
}

// MainWindow member function to show Login Page
void MainWindow::showLoginPage() {
    ui->stackedWidget->setCurrentWidget(loginPage);
}

// MainWindow member function to show User List Page
void MainWindow::showUserListPage() {
    ui->stackedWidget->setCurrentWidget(userListPage);
}

// MainWindow member function to show Chat Page
void MainWindow::showChatPage() {
    ui->stackedWidget->setCurrentWidget(chatPage);
}