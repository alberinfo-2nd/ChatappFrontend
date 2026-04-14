#include "mainwindow.h"
#include "./ui_mainwindow.h"
#include "chatpage.h"
#include "loginpage.h"
#include "userlistpage.h"
#include "SessionManager.h"
#include "ActiveUsersManager.h"
#include "BackendClient.h"

// Constructor of MainWindow Creates a pointer for each QWidget object(page)
MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
    , sessionManager(new SessionManager())
    , activeUsersManager(new ActiveUsersManager(this))
    , backendClient(new BackendClient(this, sessionManager))
    , loginPage(new LoginPage(this, backendClient))
    , userListPage(new UserListPage(this, sessionManager, activeUsersManager))
    , chatPage(nullptr)

{
    ui->setupUi(this);

    // add each QWidget object(page) to stackedWidget
    ui->stackedWidget->addWidget(loginPage);
   // ui->stackedWidget->addWidget(chatPage);
   // ui->stackedWidget->addWidget(userListPage);

    // set Login Page as the default page to display
    ui->stackedWidget->setCurrentWidget(loginPage);

    // connects signal from login page (loginSuccesful) to slot function (showUserListPage) which changes the current displayed
    // stacked widget to User List Page
    connect(loginPage, &LoginPage::loginSuccessful, this, &MainWindow::handleSuccessfulLogin);
}

// MainWindow destructor
MainWindow::~MainWindow()
{
    delete ui;
    delete sessionManager;
}

// MainWindow slot function to show Login Page
void MainWindow::showLoginPage() {
    ui->stackedWidget->setCurrentWidget(loginPage);
}

// MainWindow slot function to show User List Page
void MainWindow::showUserListPage() {
    ui->stackedWidget->setCurrentWidget(userListPage);
}

//MainWindow function to show Chat Page
void MainWindow::showChatPage() {
    ui->stackedWidget->setCurrentWidget(chatPage);
}

// used to handle succesful logins
void MainWindow::handleSuccessfulLogin(const QString &username, const QString &public_key, const QString &authorizationToken) {
    sessionManager->setCurrentUser(username, public_key, authorizationToken);
    // testing
    /*
    std::cout << "Username: " << sessionManager->getUsername().toStdString() << std::endl <<
        "Public Key: " << sessionManager->getPublicKey().toStdString() << std::endl <<
        "Auth Token: " << sessionManager->getAuthorizationToken().toStdString() << std::endl;
    */

    // MOVED STUFF HERE - FIXED EXIT BUTTON
    ui->stackedWidget->addWidget(userListPage);
    ui->stackedWidget->setCurrentWidget(userListPage);
    connect(backendClient, &BackendClient::activeUsersReceived, activeUsersManager, &ActiveUsersManager::setActiveUsers);
    backendClient->requestActiveUsers();
    backendClient->startActiveUserPolling();
    connect(userListPage, &UserListPage::chatRequested, this, &MainWindow::handleChatRequest);
    connect(userListPage, &UserListPage::logoutRequested, this, [this](){
        qDebug()<<"User has logged out. Session Cleared";

        ui->stackedWidget->setCurrentIndex(-1);
        ui->stackedWidget->addWidget(loginPage);
        showLoginPage();
        connect(loginPage, &LoginPage::loginSuccessful, this, &MainWindow::handleSuccessfulLogin);
    });
}

//used to handle chat requests
//TODO no impementation add yet
void MainWindow::handleChatRequest(const QString &username, const QString &public_key) {
    User partner(username, public_key);


    if (!chatPage) {
        chatPage = new ChatPage(ui->stackedWidget, sessionManager, activeUsersManager, partner);
        ui->stackedWidget->addWidget(chatPage);

        connect(chatPage, &ChatPage::backToUserListRequested,
                this, &MainWindow::showUserListPage);
    }

    // update chat instead of recreating it
    chatPage->switchToNewChat(username);
    ui->stackedWidget->setCurrentWidget(chatPage);

}
