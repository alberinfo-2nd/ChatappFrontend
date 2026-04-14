#include "chatpage.h"
#include "ui_chatpage.h"
#include <QLabel>
#include <QWidget>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QPushButton>

// constructor
ChatPage::ChatPage(QWidget *parent, SessionManager *sessionManager, ActiveUsersManager *activeUsersManager, BackendClient *backendClient, User partner)
    : QWidget(parent)
    , ui(new Ui::ChatPage)
    , m_sessionManager{sessionManager}
    , m_activeUsersManager{activeUsersManager}
    , m_backendClient{backendClient}
    , m_currentPartner{partner}
// constructor body
{
    ui->setupUi(this);

    //removed from constructor due to bug - readded here
    chatScrollAreaLayout= new QVBoxLayout(ui->chatScrollAreaContent);
    userListScrollAreaLayout= new QVBoxLayout(ui->userListScrollAreaContent);

    // layout
    userListScrollAreaLayout->setAlignment(Qt::AlignTop);
    userListScrollAreaLayout->setContentsMargins(0, 0, 0, 0);
    userListScrollAreaLayout->setSpacing(2);
    ui->frame_2->setMinimumWidth(160);
    ui->userListScrollAreaContent->setStyleSheet("background-color: #3e405a;");
    ui->userListScrollArea->setWidgetResizable(true);

    this->setAttribute(Qt::WA_StyledBackground, true); // fix background problem

    // Layout for header
     ui->chatHeader->setMinimumHeight(60);
     ui->chatHeader->setStyleSheet("background-color: #54566a;");

    QHBoxLayout* headerLayout = new QHBoxLayout(ui->chatHeader);

    // Bubble for chatPartnerLabel
    QFrame* bubble = new QFrame(ui->chatHeader);
    bubble->setStyleSheet("background-color: white;" "border-radius: 20px;" "padding: 2px 20px;");
    QHBoxLayout* bubbleLayout = new QHBoxLayout(bubble);

    chatPartnerLabel = new QLabel(m_currentPartner.getUsername(), bubble);
    chatPartnerLabel->setStyleSheet("color: black; font-size: 18pt; font-weight: bold;");
    bubbleLayout->addWidget(chatPartnerLabel);

    headerLayout->addWidget(bubble);
    headerLayout->addStretch();

    // make exit button show up
    ui->pushButton->setFixedSize(80, 35);
    ui->pushButton->setStyleSheet("background-color: #bacba8; color: black; font-weight: bold;");
    headerLayout->addWidget(ui->pushButton);

    // connect send message button to slot function send message
    connect(ui->sendMessageButton, &QPushButton::clicked, this, &ChatPage::sendMessage);

    // set place holder text for line edit in chat footer
    ui->messageLineEdit->setPlaceholderText("Send message...");

    // style sheet for chat page
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

        #userListScrollAreaContent QLabel {
            color: #3e405a;
            font-size: 18pt;
        }
    )");


// exit button
    connect(ui->pushButton, &QPushButton::clicked, this, [this]() {
        clearDisplayedMessages();
        emit backToUserListRequested();
    });
    displayActiveUsers();

    // connect signal from active users manager to display active users everytime the list is updated
    connect(m_activeUsersManager, &ActiveUsersManager::activeUsersUpdated, this, &ChatPage::displayActiveUsers);
    connect(m_sessionManager, &SessionManager::inboxUpdated, this, &ChatPage::displayReceivedMessage);

}


// destructor
ChatPage::~ChatPage()
{
    delete ui;
}

// Message Display Logic
// creates a new label for a message
QLabel* ChatPage::createNewMessageLabel(const QString &message) {
    QLabel* messageLabel = new QLabel(message, ui->chatScrollAreaContent);
    messageLabel->setSizePolicy(QSizePolicy::Maximum, QSizePolicy::Maximum);
    return messageLabel;
}

// displays recieved message (different color, align left)
void ChatPage::displayReceivedMessage() {
    auto& inbox = m_sessionManager->getInbox();
    for (size_t i{0}; i < inbox.size(); ) {
        Message message = inbox.at(i);
        if (message.getSender() == m_currentPartner.getUsername()) {
            QLabel* messageLabel = createNewMessageLabel(message.getMessage());
            chatScrollAreaLayout->setAlignment(messageLabel, Qt::AlignLeft);
            chatScrollAreaLayout->addWidget(messageLabel);
            messageLabel->setStyleSheet("background-color: #7f906c; color:white; border-radius: 12px; padding: 8px 15px;");
            m_sessionManager->removeMessage(i);
        } else {
            ++i;
        }
    }
}
// dsiplays sent message (different color, align right)
void ChatPage::displaySentMessage(const QString &message) {
    QLabel* messageLabel = createNewMessageLabel(message);
    chatScrollAreaLayout->addWidget(messageLabel);
    chatScrollAreaLayout->setAlignment(messageLabel, Qt::AlignRight);
    messageLabel->setStyleSheet("background-color: #abbc99; color:white; border-radius: 12px; padding: 8px 15px;");
}

// slot function for sending message
void ChatPage::sendMessage() {
    const QString message = ui->messageLineEdit->text();
    ui->messageLineEdit->clear();
    if (!message.isEmpty()) {
        displaySentMessage(message);
        m_backendClient->sendMessage(m_currentPartner.getUsername().toStdString(), message.toStdString());
    }
}

// UserList Logic
// used to display a label for each active user
void ChatPage::displayActiveUsers() {
    clearActiveUserList();
    QString myName = m_sessionManager->getUsername();
    auto& inbox = m_sessionManager->getInbox();


    for (const auto& user : m_activeUsersManager->getActiveUsers()) {
        QString username = user.getUsername();
        // dont show self/ user talking to
       if (username == myName || username == m_currentPartner.getUsername()) continue;

       // Row container (replaces old userLabel)
       QWidget* rowWidget = new QWidget(ui->userListScrollAreaContent);
       rowWidget->setMinimumSize(120, 50);
       rowWidget->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);

       QHBoxLayout* rowLayout = new QHBoxLayout(rowWidget);

       // button for the list of users
       QPushButton* userBtn = new QPushButton(username, rowWidget);
       userBtn->setStyleSheet("background: white; color: black; font-size: 16pt;");
       userBtn->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Preferred);

       rowLayout->addWidget(userBtn);
       rowLayout->addStretch();

       // connect button to switch to another user
        connect(userBtn, &QPushButton::clicked, this, [this, username]() {
             switchToNewChat(username);
        });

        userListScrollAreaLayout->addWidget(rowWidget);
        activeUserLabels.insert(username, rowWidget);
        rowWidget->show();
    }
    ui->userListScrollAreaContent->adjustSize();
    alternateLabelStyle();


}

// used to delete label assiociated to an active user
void ChatPage::removeActiveUser(const QString &username) {
    auto iterator = activeUserLabels.find(username);
    if (iterator == activeUserLabels.end()) {
        return;
    }    
// changed to handle row instead of old code
    QWidget* rowWidget = iterator.value();
    rowWidget->deleteLater();
    activeUserLabels.erase(iterator);

    alternateLabelStyle();
}

// clear user box
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

// clear chat box
void ChatPage::clearDisplayedMessages() {
    QLayoutItem* item;
    while ((item = chatScrollAreaLayout->takeAt(0)) != nullptr) {
        if (item->widget()) {
            item->widget()->deleteLater();
        }
        delete item;
    }
}

// switch to new chat
void ChatPage::switchToNewChat(const QString &username) {
    clearDisplayedMessages();
    chatScrollAreaLayout->addStretch();
    m_currentPartner.setUsername(username);
    chatPartnerLabel->setText(username);
}

// stylinig for ative user labels (every other labels gets different color)
 void ChatPage::alternateLabelStyle() {
    for (int i{0}; i < userListScrollAreaLayout->count(); ++i) {
        QLayoutItem* item = userListScrollAreaLayout->itemAt(i);
        if (!item || !item->widget()) {
            continue;
        }

    // changed to include QWidget
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






