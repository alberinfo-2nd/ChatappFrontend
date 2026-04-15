#include "mainwindow.h"
#include "./ui_mainwindow.h"
#include "chatpage.h"
#include "loginpage.h"
#include "userlistpage.h"
#include "sessionManager.h"
#include "ActiveUsersManager.h"
#include "BackendClient.h"
#include <QMessageBox>

// Constructor of MainWindow Creates a pointer for each QWidget object(page)
MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
    , sessionManager(new SessionManager())
    , activeUsersManager(new ActiveUsersManager(this))
    , backendClient(new BackendClient(this, sessionManager))
    , loginPage(new LoginPage(this, backendClient))
    , userListPage(new UserListPage(this, sessionManager, activeUsersManager, backendClient))
    , chatPage(nullptr)

{
    ui->setupUi(this);

    // add each QWidget object(page) to stackedWidget
    ui->stackedWidget->addWidget(loginPage);
    ui->stackedWidget->addWidget(userListPage);

    // set Login Page as the default page to display
    ui->stackedWidget->setCurrentWidget(loginPage);

    // Lamda function for for handling logging out or being kicked
    auto handleLogout = [this](bool kicked = false){
        qDebug()<<"User has logged out. Session Cleared";
        backendClient->logout(sessionManager->getUsername().toStdString(), sessionManager->getAuthorizationToken().toStdString());
        sessionManager->clear();
        backendClient->stopPolling();
        showLoginPage();
        if (kicked) {
            QMessageBox::warning(this, "Warning", "You have been kicked by an admin!");
        }
    };
    connect(loginPage, &LoginPage::loginSuccessful, this, &MainWindow::handleSuccessfulLogin);
    connect(userListPage, &UserListPage::chatRequested, this, &MainWindow::handleChatRequest);
    connect(userListPage, &UserListPage::logoutRequested, this, handleLogout);
    connect(backendClient, &BackendClient::userKicked, this, handleLogout);
    connect(backendClient, &BackendClient::activeUsersReceived, activeUsersManager, &ActiveUsersManager::setActiveUsers);
    connect(backendClient, &BackendClient::messageReceived, sessionManager, &SessionManager::addMessages);
    // runs destructor when window is closed
    this->setAttribute(Qt::WA_DeleteOnClose);
}

// MainWindow destructor
MainWindow::~MainWindow()
{
    backendClient->logout(sessionManager->getUsername().toStdString(), sessionManager->getAuthorizationToken().toStdString());
    delete ui;
    delete sessionManager;
    delete activeUsersManager;
    delete backendClient;

}

// MainWindow slot function to show Login Page
void MainWindow::showLoginPage() {
    ui->stackedWidget->setCurrentWidget(loginPage);
}

// MainWindow slot function to show User List Page
void MainWindow::showUserListPage() {
    ui->stackedWidget->setCurrentWidget(userListPage);
    chatPage->setChatPartner("");
}

//MainWindow function to show Chat Page
void MainWindow::showChatPage() {
    ui->stackedWidget->setCurrentWidget(chatPage);
}

// used to handle succesful logins
void MainWindow::handleSuccessfulLogin(const QString &username, const QString &public_key, const QString &authorizationToken, const bool &isAdmin) {
    sessionManager->setCurrentUser(username, public_key, authorizationToken);
    if (isAdmin) {
        sessionManager->setAsAdmin();
    }
    // testing
    /*
    std::cout << "Username: " << sessionManager->getUsername().toStdString() << std::endl <<
        "Public Key: " << sessionManager->getPublicKey().toStdString() << std::endl <<
        "Auth Token: " << sessionManager->getAuthorizationToken().toStdString() << std::endl;
    */

    // MOVED STUFF HERE - FIXED EXIT BUTTON

    ui->stackedWidget->setCurrentWidget(userListPage);
    backendClient->requestActiveUsers();
    backendClient->startPolling();
}

//used to handle chat requests
void MainWindow::handleChatRequest(const QString &username, const QString &public_key) {
    User partner(username, public_key);


    if (!chatPage) {
        chatPage = new ChatPage(ui->stackedWidget, sessionManager, activeUsersManager, backendClient, partner);
        ui->stackedWidget->addWidget(chatPage);

        connect(chatPage, &ChatPage::backToUserListRequested,
                this, &MainWindow::showUserListPage);
    }

    // update chat instead of recreating it
    chatPage->switchToNewChat(username);
    ui->stackedWidget->setCurrentWidget(chatPage);

}
