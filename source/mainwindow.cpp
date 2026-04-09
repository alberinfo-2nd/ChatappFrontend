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
    , chatPage(new ChatPage(this))

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
    connect(loginPage, &LoginPage::loginSuccessful, this, &MainWindow::handleSuccessfulLogin);



    // Function to connect UserListPage to LogInPage ?
    connect(userListPage, &UserListPage::chatRequested, this, &MainWindow::handleChatRequest);

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

// MainWindow function to show Chat Page
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
    ui->stackedWidget->setCurrentWidget(userListPage);
    backendClient->requestActiveUsers();
    backendClient->startActiveUserPolling();
    connect(backendClient, &BackendClient::activeUsersReceived, activeUsersManager, &ActiveUsersManager::setActiveUsers);
}

//used to handle chat requests
//TODO no impementation add yet
void MainWindow::handleChatRequest(const QString &username, const QString &public_key) {
    ui->stackedWidget->setCurrentWidget(chatPage);
    backendClient->stopActiveUserPolling();
}
