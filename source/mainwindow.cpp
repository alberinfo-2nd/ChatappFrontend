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
    , chatPage(new ChatPage(this, sessionManager, activeUsersManager, backendClient, User()))
{

    ui->setupUi(this);

    // Add each QWidget object(page) to stackedWidget
    ui->stackedWidget->addWidget(loginPage);
    ui->stackedWidget->addWidget(userListPage);
    ui->stackedWidget->addWidget(chatPage);

    // Set Login Page as the default page to display
    ui->stackedWidget->setCurrentWidget(loginPage);

    // Lamda function for for handling logging out or being kicked
    auto handleLogout = [this](bool kicked = false)
    {
        qDebug() << "User has logged out. Session Cleared";

        backendClient->logout();
        backendClient->stopPolling();
        sessionManager->clear();
        activeUsersManager->clearActiveUsers();
        showLoginPage();

        if (kicked)
        {
            QMessageBox::warning(this, "Warning", "You have been kicked by an admin!");
        }
    };
    // Global Signal/Slot Connections
    connect(loginPage, &LoginPage::loginSuccessful, this, &MainWindow::handleSuccessfulLogin);
    connect(userListPage, &UserListPage::chatRequested, this, &MainWindow::handleChatRequest);
    connect(userListPage, &UserListPage::logoutRequested, this, handleLogout);
    connect(chatPage, &ChatPage::backToUserListRequested, this, &MainWindow::showUserListPage);
    // Backend synchronization
    connect(backendClient, &BackendClient::userKicked, this, handleLogout);
    connect(backendClient, &BackendClient::activeUsersReceived, activeUsersManager, &ActiveUsersManager::setActiveUsers);
    connect(backendClient, &BackendClient::messageReceived, sessionManager, &SessionManager::addMessages);
    // Runs destructor when window is closed
    this->setAttribute(Qt::WA_DeleteOnClose);
}

// MainWindow destructor
MainWindow::~MainWindow()
{
    backendClient->logout();
    delete ui;
    delete sessionManager;
    delete activeUsersManager;
    delete backendClient;
}

// MainWindow slot function to show Login Page
void MainWindow::showLoginPage()
{
    ui->stackedWidget->setCurrentWidget(loginPage);
}

// MainWindow slot function to show User List Page
void MainWindow::showUserListPage()
{
    ui->stackedWidget->setCurrentWidget(userListPage);
    chatPage->setChatPartner("");
}

// MainWindow function to show Chat Page
void MainWindow::showChatPage()
{
    ui->stackedWidget->setCurrentWidget(chatPage);
}

// used to handle succesful logins
void MainWindow::handleSuccessfulLogin(const QString &username, const QString &publicKey, const QByteArray &privateKey, const QString &authorizationToken, const bool &isAdmin)
{
    sessionManager->setCurrentUser(username, publicKey, privateKey, authorizationToken);
    if (isAdmin)
    {
        sessionManager->setAsAdmin();
    }

    showUserListPage();
    backendClient->requestActiveUsers();
    backendClient->startPolling();
}

// Used to handle chat requests
// Updates the session data and switches the UI to the chat screen
void MainWindow::handleChatRequest(const QString &username, const QString &publicKey)
{
    sessionManager->switchToNewChat(username, publicKey);
    showChatPage();
}
