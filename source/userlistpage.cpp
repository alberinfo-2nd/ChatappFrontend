#include "userlistpage.h"
#include "ui_userlistpage.h"
#include <QLabel>
#include <QVBoxLayout>
#include <QPushButton>
#include <SessionManager.h>
#include <ActiveUsersManager.h>
#include <BackendClient.h>

UserListPage::UserListPage(QWidget *parent, SessionManager *sessionManager, ActiveUsersManager *activeUsersManager, BackendClient *backendClient)
    : QWidget(parent)
    , ui(new Ui::UserListPage)
    , m_sessionManager{sessionManager}
    , m_activeUsersManager{activeUsersManager}
    , m_backendClient{backendClient}

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
    connect(m_sessionManager, &SessionManager::inboxUpdated, this, &UserListPage::displayActiveUsers);
}

UserListPage::~UserListPage()
{
    delete ui;
}

// Function to Display List
// Updated to allow auto updates of active user list
void UserListPage::displayActiveUsers() {
    qDebug() << "Updating UI. Manager says these users are online:"
             << m_activeUsersManager->getActiveUsers().size();
    qDebug() << "Is admin:" << m_sessionManager->getIsAdmin();

    if (m_sessionManager->getUsername().isEmpty()) {
        return;
    }

    QLayoutItem* item;
    while ((item = ui->verticalLayout_3->takeAt(0)) != nullptr) {
        if (item->widget()) {
            item->widget()->deleteLater();
        }
        delete item;
    }

    activeUserLabels.clear();

    QString myName = m_sessionManager->getUsername();
    auto& inbox = m_sessionManager->getInbox();

    // username -> public_key
    QHash<QString, QString> usersToDisplay;

    // 1. Add normal active users
    for (const auto& user : m_activeUsersManager->getActiveUsers()) {
        QString username = user.getUsername();
        QString public_key = user.getPublicKey();

        if (username == myName) {
            continue;
        }

        usersToDisplay.insert(username, public_key);
    }

    // 2. Add inbox senders (ex: admin who messaged user)
    for (const auto& message : inbox) {
        QString sender = message.getSender();

        if (sender == myName) {
            continue;
        }

        // Only add if not already present from active users
        if (!usersToDisplay.contains(sender)) {
            usersToDisplay.insert(sender, "");
        }
    }

    // 3. Build UI
    for (auto it = usersToDisplay.begin(); it != usersToDisplay.end(); ++it) {
        addUserToList(it.key(), it.value());
    }
}

// Funtion to Add User
void UserListPage::addUserToList(const QString &username, const QString &public_key){
    auto& inbox = m_sessionManager->getInbox();
    QWidget* rowWidget = new QWidget(ui->LeftScrollWidget);
    rowWidget->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Maximum);
    rowWidget->setLayoutDirection(Qt::LeftToRight);

    QHBoxLayout* rowLayout = new QHBoxLayout(rowWidget);
    rowLayout->setDirection(QBoxLayout::LeftToRight);
    rowLayout->setContentsMargins(0, 0, 0, 0);
    rowLayout->setSpacing(8);

    QPushButton* userBtn = new QPushButton(username, rowWidget);
    QPushButton* kickBtn = new QPushButton("Kick", rowWidget);

    userBtn->setCursor(Qt::PointingHandCursor);

    userBtn->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);
    kickBtn->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);

    rowLayout->setStretchFactor(userBtn, 3);
    rowLayout->setStretchFactor(kickBtn, 1);
    userBtn->setStyleSheet(R"(
        QPushButton {
            background-color: white;
        }
        QPushButton:hover {
            background-color: #d9d9d9;
        }
    )");
    kickBtn->setStyleSheet(R"(
        QPushButton {
            background-color: white;
        }
        QPushButton:hover {
            background-color: #d9d9d9;
        }
    )");

    int numberOfMessages{0};
    for (size_t i{1}; i <= inbox.size(); ++i) {
        const QString sender = inbox.at(i - 1).getSender();
        if (sender == username) {
            numberOfMessages++;
        }
    }

    if (numberOfMessages != 0) {
        userBtn->setText(username + "   [" + QString::number(numberOfMessages) + "]");
    }



    rowLayout->addWidget(userBtn);
    rowLayout->addWidget(kickBtn);
    kickBtn->hide();

    if (m_sessionManager->getIsAdmin()) {
        kickBtn->show();
        connect(kickBtn, &QPushButton::clicked, this, [this, username]() {
            m_backendClient->kick(username.toStdString());
        });

    }

    // Button to click user
    connect(userBtn, &QPushButton::clicked, this, [this, username, public_key]() {
        emit chatRequested(username, public_key);
    });
    ui-> verticalLayout_3->insertWidget(0, rowWidget);
    activeUserLabels.insert(username, rowWidget);
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
    emit logoutRequested();
}

