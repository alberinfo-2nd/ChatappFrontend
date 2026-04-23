#include "userlistpage.h"
#include "ui_userlistpage.h"
#include <QLabel>
#include <QVBoxLayout>
#include <QPushButton>
#include <QMessageBox>
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

    // UI scroll area and layout behavior
    ui->pushButton->setFixedSize(120, 40);
    ui->pushButton->setCursor(Qt::PointingHandCursor);
    ui->LeftScrollArea->setWidget(ui->LeftScrollWidget);
    ui->LeftScrollArea->setWidgetResizable(true);
    ui->LeftScrollWidget->setLayout(ui->verticalLayout_3);

    // Ensure user rows stay at the top
    ui->verticalLayout_3->setAlignment(Qt::AlignTop);
    ui->verticalLayout_3->setSpacing(5);

    // Attribute allows page to have its own background color
    this->setAttribute(Qt::WA_StyledBackground, true);

    this->setStyleSheet(R"(
    /* Styling for the main page and scroll elements */
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
    // Refresh the UI whenever the online list or the user's inbox changes
    connect(m_activeUsersManager, &ActiveUsersManager::activeUsersUpdated, this, &UserListPage::displayActiveUsers);
    connect(m_sessionManager, &SessionManager::inboxUpdated, this, &UserListPage::displayActiveUsers);
}

UserListPage::~UserListPage()
{
    delete ui;
}

// Function: display user list UI
// Updated to allow auto updates of active user list
void UserListPage::displayActiveUsers()
{

    if (m_sessionManager->getUsername().isEmpty())
    {
        return;
    }

    // Clear existing widgets from the layout to prevent duplicates
    QLayoutItem *item;
    while ((item = ui->verticalLayout_3->takeAt(0)) != nullptr)
    {
        if (item->widget())
        {
            item->widget()->deleteLater();
        }
        delete item;
    }

    if (m_sessionManager->getUsername().isEmpty())
    {
        return;
    }

    qDebug() << "Updating UI. Manager says there are"
             << m_activeUsersManager->getActiveUsers().size()
             << "users online.\n";

    QString myName = m_sessionManager->getUsername();
    auto &inbox = m_sessionManager->getInbox();

    // Keep track of unique users to show (username -> public_key)
    QHash<QString, QString> usersToDisplay;

    // Get currently online users
    for (const auto &user : m_activeUsersManager->getActiveUsers())
    {
        QString username = user.getUsername();
        QString public_key = user.getPublicKey();

        if (username == myName)
        {
            continue;
        }

        usersToDisplay.insert(username, public_key);
    }

    // Get users who have sent messages
    for (const auto &message : inbox)
    {
        QString sender = message.getSender();

        if (sender == myName)
        {
            continue;
        }

        if (!usersToDisplay.contains(sender))
        {
            usersToDisplay.insert(sender, message.getAdminPublicKey());
        }
    }

    // Create a row widget for each user in the list
    for (auto it = usersToDisplay.begin(); it != usersToDisplay.end(); ++it)
    {
        addUserToList(it.key(), it.value());
    }
    ui->verticalLayout_3->addStretch(1);
}

// Function: Creates and adds a single user row (button + kick button for admins)
void UserListPage::addUserToList(const QString &username, const QString &public_key)
{
    auto &inbox = m_sessionManager->getInbox();

    // Container widget for the entire row
    QWidget *rowWidget = new QWidget(ui->LeftScrollWidget);
    rowWidget->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Maximum);
    rowWidget->setLayoutDirection(Qt::LeftToRight);

    // Horizontal layout to hold the Username button and the Kick button
    QHBoxLayout *rowLayout = new QHBoxLayout(rowWidget);
    rowLayout->setDirection(QBoxLayout::LeftToRight);
    rowLayout->setContentsMargins(0, 0, 0, 0);
    rowLayout->setSpacing(8);

    QPushButton *userBtn = new QPushButton(username, rowWidget);
    QPushButton *kickBtn = new QPushButton("Kick", rowWidget);
    kickBtn->setCursor(Qt::PointingHandCursor);
    userBtn->setCursor(Qt::PointingHandCursor);

    // Buttons resize
    userBtn->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);
    kickBtn->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
    rowLayout->setStretchFactor(userBtn, 3);
    rowLayout->setStretchFactor(kickBtn, 1);

    // Button styling
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
            border-radius: 5px;
            padding: 5px 10px;
        }
        QPushButton:hover {
            background-color: #d9d9d9;
        }
    )");
    // Count unread messages from this specific user
    int numberOfMessages{0};
    for (size_t i{1}; i <= inbox.size(); ++i)
    {
        const QString sender = inbox.at(i - 1).getSender();
        if (sender == username)
        {
            numberOfMessages++;
        }
    }
    // Append message count to the button text if any exist
    if (numberOfMessages != 0)
    {
        userBtn->setText(username + "   [" + QString::number(numberOfMessages) + "]");
    }

    rowLayout->addWidget(userBtn);
    rowLayout->addWidget(kickBtn);
    // Kick button is only visible to Admin users
    kickBtn->hide();

    if (m_sessionManager->getIsAdmin())
    {
        kickBtn->show();
        connect(kickBtn, &QPushButton::clicked, this, [this, username]()
        {   std::string message = m_backendClient->kick(username.toStdString());
            QMessageBox::information(this, "Kick User Request", QString::fromStdString(message));
        });
    }

    // Button to open a chat when a user row is clicked
    connect(userBtn, &QPushButton::clicked, this, [this, username, public_key]()
            { emit chatRequested(username, public_key); });
    ui->verticalLayout_3->insertWidget(0, rowWidget);
}

// Function: Handles the logout button click
void UserListPage::on_pushButton_clicked()
{
    QLayoutItem *item;
    while ((item = ui->verticalLayout_3->takeAt(0)) != nullptr)
    {
        if (item->widget())
            item->widget()->deleteLater();
        delete item;
    }

    emit logoutRequested();
}
