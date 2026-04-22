#include "chatpage.h"
#include "ui_chatpage.h"
#include <QLabel>
#include <QWidget>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QPushButton>

// Constructor
ChatPage::ChatPage(QWidget *parent, SessionManager *sessionManager, ActiveUsersManager *activeUsersManager, BackendClient *backendClient, User partner)
    : QWidget(parent)
    , ui(new Ui::ChatPage)
    , m_sessionManager{sessionManager}
    , m_activeUsersManager{activeUsersManager}
    , m_backendClient{backendClient}
    , m_currentPartner{partner}

// Constructor body
{
    ui->setupUi(this);

    // Initialize layouts for the dynamic scroll areas
    chatScrollAreaLayout= new QVBoxLayout(ui->chatScrollAreaContent);
    userListScrollAreaLayout= new QVBoxLayout(ui->userListScrollAreaContent);

    // Sidebar configuration to align users at the top
    userListScrollAreaLayout->setAlignment(Qt::AlignTop);
    userListScrollAreaLayout->setContentsMargins(0, 0, 0, 0);
    userListScrollAreaLayout->setSpacing(2);

    ui->frame_2->setMinimumWidth(160);
    ui->userListScrollAreaContent->setStyleSheet("background-color: #3e405a;");
    ui->userListScrollArea->setWidgetResizable(true);

    // Attribute allows page to have its own background color
    this->setAttribute(Qt::WA_StyledBackground, true);

    // Header: Creates the top bar containing partner name and actions
     ui->chatHeader->setMinimumHeight(60);
     ui->chatHeader->setStyleSheet("background-color: #54566a;");
    QHBoxLayout* headerLayout = new QHBoxLayout(ui->chatHeader);

    // Visual: Create a white Bubble for the current chat partner's name
    QFrame* bubble = new QFrame(ui->chatHeader);
    bubble->setStyleSheet("background-color: white;" "border-radius: 8px;" "padding: 2px 20px;");
    QHBoxLayout* bubbleLayout = new QHBoxLayout(bubble);

    chatPartnerLabel = new QLabel(m_currentPartner.getUsername(), bubble);
    chatPartnerLabel->setStyleSheet("color: black; font-size: 18pt; font-weight: bold;");
    bubbleLayout->addWidget(chatPartnerLabel);

    // Add chat partner's label to header layout
    headerLayout->addWidget(bubble);

    // Report Button Setup
    reportBtn = new QPushButton("Report", ui->chatHeader);
    reportBtn->setFixedSize(80,40);
    reportBtn->setCursor(Qt::PointingHandCursor);

    reportBtn->setStyleSheet(R"(
        QPushButton {
            background-color: #bacba8;
            color: black;
            font-weight: bold;
        }
        QPushButton:hover {
            background-color: #95a681;
        }
        QPushButton:pressed {
            background-color: #7a8a68;
        }
        QPushButton:disabled {
            background-color: #d1d9c7; /* Color for when it says 'Reported' */
        }
    )");

    headerLayout->addWidget(reportBtn);

    // Notify backend and visually disable the button
    connect(reportBtn, &QPushButton::clicked, this,[this](){
        QString partnerName = m_currentPartner.getUsername();
        if(!partnerName.isEmpty()){
            m_backendClient->reportUser(partnerName.toStdString());
            m_sessionManager->reportUser(partnerName);
            // Visual
            QPushButton* btn = qobject_cast<QPushButton*>(sender());
            btn->setText("Reported");
            btn->setEnabled(false);
        }
    });

    // Spacer to header layout user to push partner label and report to the left
    headerLayout->addStretch();

    // Exit Button Setup
    ui->pushButton->setFixedSize(120, 40);
    ui->pushButton->setCursor(Qt::PointingHandCursor);

    ui->pushButton->setStyleSheet(R"(
        QPushButton {
            background-color: #bacba8;
            color: black;
            font-weight: bold;
        }
        QPushButton:hover {
            background-color: #95a681;
        }
        QPushButton:pressed {
            background-color: #7a8a68;
        }
    )");

    headerLayout->addWidget(ui->pushButton);

    // Handle sending messages
    connect(ui->sendMessageButton, &QPushButton::clicked, this, &ChatPage::sendMessage);

    // Message Input Styling
    ui->messageLineEdit->setPlaceholderText("Send message...");
    ui->sendMessageButton->setCursor(Qt::PointingHandCursor);

    // Global styling for chat frames and scroll area labels
    this->setStyleSheet(R"(
        QWidget#ChatPage {
        background-color: #2a2c47;
        }

        #chatFrame, #frame_2{
        background-color: #323451;
        }

        #userListHeader{
        background-color: #3e405a;
        }

        #chatScrollAreaContent QLabel {
            font-size: 18pt;
        }

        #chatScrollArea {
            background-color: #54566a;
        }

        #chatScrollAreaContent, #chatFooter {
            background-color: #54566a;
        }

        #messageLineEdit, #sendMessageButton {
        background-color: white;
        color: black;
        font-weight: bold;
        }

        #sendMessageButton:hover {
        background-color: #d9d9d9;
        }

        #userListScrollAreaContent QLabel {
            color: #3e405a;
            font-size: 18pt;
        }
    )");


// Connect back to user list
    connect(ui->pushButton, &QPushButton::clicked, this, [this]() {
        clearDisplayedMessages();
        emit backToUserListRequested();
    });

    // Refresh UI on user list or inbox updates
    displayActiveUsers();
    connect(m_activeUsersManager, &ActiveUsersManager::activeUsersUpdated, this, &ChatPage::displayActiveUsers);
    connect(m_sessionManager, &SessionManager::inboxUpdated, this, &ChatPage::displayReceivedMessage);
    connect(m_sessionManager, &SessionManager::inboxUpdated, this, &ChatPage::displayActiveUsers);
    connect(m_sessionManager, &SessionManager::chatSessionChanged, this, &ChatPage::updateChatDisplay);

}


// Destructor
ChatPage::~ChatPage()
{
    delete ui;
}

// Set chat partners username
void ChatPage::setChatPartner(QString username) {
    m_currentPartner.setUsername(username);
}

// Message Display Logic
// Helper to create consistent message bubbles
QLabel* ChatPage::createNewMessageLabel(const QString &message) {
    QLabel* messageLabel = new QLabel(message, ui->chatScrollAreaContent);
    messageLabel->setSizePolicy(QSizePolicy::Maximum, QSizePolicy::Maximum);
    return messageLabel;
}

// Processes  inbox and displays messages from the current partner (Aligned Left)
void ChatPage::displayReceivedMessage() {
    auto& inbox = m_sessionManager->getInbox();
    for (size_t i{0}; i < inbox.size(); ) {
        Message message = inbox.at(i);
        if (message.getSender() == m_currentPartner.getUsername()) {
            QLabel* messageLabel = createNewMessageLabel(message.getMessage());
            chatScrollAreaLayout->setAlignment(messageLabel, Qt::AlignLeft);
            chatScrollAreaLayout->addWidget(messageLabel);
            messageLabel->setStyleSheet("background-color: #7f906c; color:white; border-radius: 12px; padding: 8px 15px;");
            m_sessionManager->removeMessage(i); // Clear message from session once displayed
        } else {
            ++i;
        }
    }
}

// Displays the user's own sent messages (Aligned Right)
void ChatPage::displaySentMessage(const QString &message) {
    QLabel* messageLabel = createNewMessageLabel(message);
    chatScrollAreaLayout->addWidget(messageLabel);
    chatScrollAreaLayout->setAlignment(messageLabel, Qt::AlignRight);
    messageLabel->setStyleSheet("background-color: #abbc99; color:white; border-radius: 12px; padding: 8px 15px;");
}

// Slot function for sending message
void ChatPage::sendMessage() {
    const QString message = ui->messageLineEdit->text();
    ui->messageLineEdit->clear();
    if (!message.isEmpty()) {
        displaySentMessage(message);
        m_backendClient->sendMessage(m_currentPartner.getUsername().toStdString(), message.toStdString());
    }
}

// Sidebar/UserList Logic
// Updates the sidebar with online users and message notifications
void ChatPage::displayActiveUsers() {
    clearActiveUserList();
    QString myName = m_sessionManager->getUsername();
    auto& inbox = m_sessionManager->getInbox();

    QSet<QString> usersToDisplay;
    // Filter out self and current partner from the sidebar
    for (const auto& user : m_activeUsersManager->getActiveUsers()) {
        QString username = user.getUsername();

        if (username == myName || username == m_currentPartner.getUsername())
            continue;

        usersToDisplay.insert(username);
    }

    for (const auto& message : inbox) {
        QString sender = message.getSender();

        if (sender == myName || sender == m_currentPartner.getUsername())
            continue;

        usersToDisplay.insert(sender);
    }

    for (const QString& username : usersToDisplay) {

       QWidget* rowWidget = new QWidget(ui->userListScrollAreaContent);
       rowWidget->setMinimumSize(120, 50);
       rowWidget->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);

       QHBoxLayout* rowLayout = new QHBoxLayout(rowWidget);

       QPushButton* userBtn = new QPushButton(username, rowWidget);
       userBtn->setObjectName("userListButton");

       // Styling for sidebar buttons
       userBtn->setStyleSheet(R"(
        QPushButton#userListButton {
            background-color: white;
            color: black;
            font-size: 16pt;
            border-radius: 5px;
        }
        QPushButton#userListButton:hover {
            background-color: #d9d9d9;
        }
        QPushButton#userListButton:pressed {
            background-color: #cccccc;
        }
        )");

       userBtn->setCursor(Qt::PointingHandCursor);

       userBtn->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Preferred);
       QPushButton* kickBtn = new QPushButton("Kick", rowWidget);
       kickBtn->setCursor(Qt::PointingHandCursor);
       kickBtn->setStyleSheet("background: white; color: black; font-size: 16pt;");
       kickBtn->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);

       // Check for unread messages to display a notification count
       int numberOfMessages{0};
       for (size_t i{1}; i <= inbox.size(); ++i) {
           const QString sender = inbox.at(i - 1).getSender();
           if (sender == username && sender != m_currentPartner.getUsername()) {
               numberOfMessages++;
           }
       }

       if (numberOfMessages != 0) {
           userBtn->setText(username + "   [" + QString::number(numberOfMessages) + "]");
       }

       rowLayout->addWidget(userBtn);
       rowLayout->addWidget(kickBtn);

       rowLayout->setStretchFactor(userBtn, 4);
       rowLayout->setStretchFactor(kickBtn, 1);

       // Admin check for Kick privileges
       kickBtn->hide();
       if (m_sessionManager->getIsAdmin()) {
           kickBtn->show();
           connect(kickBtn, &QPushButton::clicked, this, [this, username]() {
               m_backendClient->kick(username.toStdString());
           });

       }

       // Switches the main chat view to the selected user
        connect(userBtn, &QPushButton::clicked, this, [this, username]() {
            m_sessionManager->switchToNewChat(username);
        });

        userListScrollAreaLayout->addWidget(rowWidget);
        activeUserLabels.insert(username, rowWidget);
        rowWidget->show();
    }
    ui->userListScrollAreaContent->adjustSize();
    alternateLabelStyle();


}

// Delete label assiociated to an active user
void ChatPage::removeActiveUser(const QString &username) {
    auto iterator = activeUserLabels.find(username);
    if (iterator == activeUserLabels.end()) {
        return;
    }    
    QWidget* rowWidget = iterator.value();
    rowWidget->deleteLater();
    activeUserLabels.erase(iterator);

    alternateLabelStyle();
}

// Clear user box
void ChatPage::clearActiveUserList() {
    QLayoutItem* item;
    while ((item = userListScrollAreaLayout->takeAt(0)) != nullptr) {
        if (item->widget()){
            item->widget()->hide();
            item->widget()->deleteLater();
        }
        delete item;
    }
    activeUserLabels.clear();
}

// Clear chat box
void ChatPage::clearDisplayedMessages() {
    QLayoutItem* item;
    while ((item = chatScrollAreaLayout->takeAt(0)) != nullptr) {
        if (item->widget()) {
            item->widget()->deleteLater();
        }
        delete item;
    }
}

// Transitions the page to a conversation with a different user
void ChatPage::updateChatDisplay(const QString &username, bool isReported) {
    clearDisplayedMessages();
    chatScrollAreaLayout->addStretch();
    m_currentPartner.setUsername(username);
    chatPartnerLabel->setText(username);

    // Reset report button state for the new partner
    if(reportBtn){
        QString style = reportBtn->styleSheet(); // Reuse existing style
        // Check if reported user is in list
        if(isReported){
            reportBtn->setText("Reported");
            reportBtn->setEnabled(false);
        } else {
        // Keep regular button layout
            reportBtn->setText("Report");
            reportBtn->setEnabled(true);
        } }

    displayActiveUsers();
    displayReceivedMessage();
}


// Stylinig for ative user labels (every other labels gets different color)
 void ChatPage::alternateLabelStyle() {
    for (int i{0}; i < userListScrollAreaLayout->count(); ++i) {
        QLayoutItem* item = userListScrollAreaLayout->itemAt(i);
        if (!item || !item->widget()) {
            continue;
        }

    QWidget* rowWidget = item->widget();
        if (i % 2 == 0) {
            rowWidget->setStyleSheet(R"(
                background-color: #3e405a;
            )");
        } else {
            rowWidget->setStyleSheet(R"(
                background-color: #323451;
            )");
        }
    }
}






