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

    // connects signal from login page (loginSuccesful) to slot function (showUserListPage) which changes the current displayed
    // stacked widget to User List Page
    connect(loginPage, &LoginPage::loginSuccessful, this, &MainWindow::showUserListPage);

    // Function to connect UserListPage to LogInPage ?
    connect(userListPage, &UserListPage::userClicked, this, &MainWindow::showChatPage);

    /*{
        // Disble Button if user is busy

        if(userListPage->getWidget(username)){
            userListPage->getWidget(username)->setEnabled(false);
        }

        ui->stackedWidget->setCurrentIndex(1);
        qDebug() << "User selected:" << username;
    });*/

    // Function to connect Exit Button to LogIn - NEEDS WORK
    connect(userListPage, &UserListPage::userLogOut, this, [this](){
      //  this->currentUser ;
        ui->stackedWidget->setCurrentIndex(0);
        qDebug()<<"User has logged out. Session CLeared";
    });
}

// MainWindow destructor
MainWindow::~MainWindow()
{
    delete ui;
}

// MainWindow slot function to show Login Page
void MainWindow::showLoginPage() {
    ui->stackedWidget->setCurrentWidget(loginPage);
}

// MainWindow slot function to show User List Page
void MainWindow::showUserListPage() {
    ui->stackedWidget->setCurrentWidget(userListPage);
}

// MainWindow function to show Chat Page
void MainWindow::showChatPage() {
    ui->stackedWidget->setCurrentWidget(chatPage);
}
