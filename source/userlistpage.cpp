#include "userlistpage.h"
#include "ui_userlistpage.h"
#include <QLabel>
#include <QVBoxLayout>
#include <QPushButton>
#include <SessionManager.h>
#include <ActiveUsersManager.h>

UserListPage::UserListPage(QWidget *parent, SessionManager *sessionManager, ActiveUsersManager *activeUsersManager)
    : QWidget(parent)
    , ui(new Ui::UserListPage)
    , m_sessionManager{sessionManager}
    , m_activeUsersManager{activeUsersManager}

{
    ui->setupUi(this);
    ui->pushButton->setCursor(Qt::PointingHandCursor);
    ui->LeftScrollArea->setWidget(ui->LeftScrollWidget);
    ui->LeftScrollArea->setWidgetResizable(true);
    ui->LeftScrollWidget->setLayout(ui->verticalLayout_3);

    this->setAttribute(Qt::WA_StyledBackground, true); // fixes background problem

    this->setStyleSheet(R"(
    #UserListPage {
        background-color: #54566a;
    }

    #OnlineUserLabel {
       color: white;
       font-size: 30pt;
       font-weight: bold;
       border-radius: 15px;
       padding: 8px 20px;
    }

    #LeftScrollArea, #LeftScrollWidget {
        background-color: #bacba8;
        border-radius: 20px;
        padding: 10px;
        margin: 4px;
    }

    #LeftScrollWidget QPushButton{
        background-color: white;
        color: black;
        border-radius: 10px;
        padding: 5px 10px;
        margin: 2px;
        font-weight: 500;
        font-size: 18pt;
    }
    #pushButton{
        background-color: white;
        color: black;
        font-size: 14pt;
        padding: 5px 20px;
        min-width: 20px;
    }
    #pushButton::hover{
        background-color: #d9d9d9;
    }
)");
    // connect signal from active users manager to display active users everytime the list is updated
    connect(m_activeUsersManager, &ActiveUsersManager::activeUsersUpdated, this, &UserListPage::displayActiveUsers);
}

UserListPage::~UserListPage()
{
    delete ui;
}

// Function to Display List
// Updated to allow auto updates of active user list
void UserListPage::displayActiveUsers() {
     qDebug() << "Updating UI. Manager says these users are online:" << m_activeUsersManager->getActiveUsers().size();
    if (m_sessionManager->getUsername().isEmpty()) {
        return;
    }

    QSet<QString> currentUsers;
    for(const auto& user : m_activeUsersManager->getActiveUsers()){
        QString username = user.getUsername();
        QString public_key = user.getPublicKey();

        if (username == m_sessionManager->getUsername()) {
            continue;
        }

        currentUsers.insert(username);

        if(!(activeUserLabels.contains(username))) {
            addUserToList(username, public_key);
        }
    }
// changed
    QStringList usersToRemove;
    for (auto it = activeUserLabels.begin(); it != activeUserLabels.end(); ++it) {
        if(!currentUsers.contains(it.key())) {
            usersToRemove.append(it.key());
        }
    }
    for (const QString& name : usersToRemove) {
        removeActiveUser(name);
    }
}

// Funtion to Add User
void UserListPage::addUserToList(const QString &username, const QString &public_key){    
    QPushButton* userBtn = new QPushButton(username, ui->LeftScrollWidget);
    userBtn->setCursor(Qt::PointingHandCursor);
    userBtn->setStyleSheet(R"(
        QPushButton {
            background-color: white;
        }
        QPushButton::hover {
            background-color: #d9d9d9;
        }
    )");
    // Button to click user
    connect(userBtn, &QPushButton::clicked, this, [this, username, public_key]() {
        emit chatRequested(username, public_key);
    });
    ui-> verticalLayout_3->insertWidget(0,userBtn);
    activeUserLabels.insert(username, userBtn);
}

// Funtion to Remove Users (copied from chat branch)
void UserListPage::removeActiveUser(const QString &username) {
    auto iterator = activeUserLabels.find(username);
    if (iterator == activeUserLabels.end()) {
        return;
    }

    QWidget* userWidget = iterator.value();
    userWidget->deleteLater();
    activeUserLabels.erase(iterator);
}

// Function for Exit Button
void UserListPage::on_pushButton_clicked() {
    // Clear Session Data ??
    m_activeUsersManager->clearActiveUsers();
    m_sessionManager->clear();
    emit logoutRequested();
}

