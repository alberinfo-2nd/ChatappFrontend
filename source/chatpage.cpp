#include "chatpage.h"
#include "ui_chatpage.h"

#include <QLabel>
#include <QVBoxLayout>


ChatPage::ChatPage(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::ChatPage)
    , chatScrollAreaLayout(new QVBoxLayout())
    , userListScrollAreaLayout(new QVBoxLayout())
{
    ui->setupUi(this);

    // set layout inside scroll area widget
    ui->chatScrollAreaContent->setLayout(chatScrollAreaLayout);
    ui->userListScrollAreaContent->setLayout(userListScrollAreaLayout);

    connect(ui->sendMessageButton, &QPushButton::clicked, this, &ChatPage::sendMessage);

    ui->messageLineEdit->setPlaceholderText("Send message...");

    displayReceivedMessage("Hello");
    displayReceivedMessage("How are you doing");
    displayReceivedMessage("How are you doing");
    displayReceivedMessage("How are you doing");
    displayReceivedMessage("How are you doing");
    displaySentMessage("How are you doing");
    displayReceivedMessage("How are you doing");
    displaySentMessage("How are you doing");
    displaySentMessage("How are you doing");
    displayReceivedMessage("How are you doing");
    displayReceivedMessage("How are you doing");
    displayReceivedMessage("How are you doing");
    displayReceivedMessage("How are you doing");
    displayReceivedMessage("How are you doing");
    displayReceivedMessage("How are you doing");
    displayReceivedMessage("How are you doing");
    displayReceivedMessage("How are you doing");
    displayReceivedMessage("How are you doing");
    displayReceivedMessage("How are you doing");
    displayReceivedMessage("How are you doing");
    displayReceivedMessage("How are you doing");
    displayReceivedMessage("How are you doing");
    displayReceivedMessage("How are you doing");

    std::vector<std::string> testUsers{"Kolby", "Halie", "John"};
    displayActiveUsers(testUsers);


    this->setStyleSheet(R"(
        #chatScrollAreaContent QLabel {
            color: red;
            font-size: 18pt;
        }

        #chatScrollArea {
            background-color: white;
        }

        #chatScrollAreaContent {
            background-color: white;
        }

        #userListScrollAreaContent QLabel {
            color: blue;
            font-size: 18pt;
        }
    )");
}

ChatPage::~ChatPage()
{
    delete ui;
}

QLabel* ChatPage::createNewMessageLabel(const QString &message) {
    QLabel* messageLabel = new QLabel(message, ui->chatScrollArea);
    messageLabel->setSizePolicy(QSizePolicy::Maximum, QSizePolicy::Maximum);
    return messageLabel;

}

void ChatPage::displayReceivedMessage(const QString &message) {
    QLabel* messageLabel = createNewMessageLabel(message);
    chatScrollAreaLayout->setAlignment(messageLabel, Qt::AlignLeft);
    chatScrollAreaLayout->addWidget(messageLabel);


    messageLabel->setStyleSheet("background-color: blue;");
}

void ChatPage::displaySentMessage(const QString &message) {
    QLabel* messageLabel = createNewMessageLabel(message);
    chatScrollAreaLayout->addWidget(messageLabel);
    chatScrollAreaLayout->setAlignment(messageLabel, Qt::AlignRight);

    messageLabel->setStyleSheet("background-color: green;");
}

void ChatPage::sendMessage() {
    const QString message = ui->messageLineEdit->text();
    ui->messageLineEdit->clear();
    if (!message.isEmpty()) {
        displaySentMessage(message);
    }
}

void ChatPage::displayActiveUsers(const std::vector<std::string> &users) {
    for (const auto& user : users) {
        QString username = QString::fromStdString(user);
        QLabel* userLabel = new QLabel(username, ui->userListScrollArea);
        userLabel->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Maximum);
        userListScrollAreaLayout->setAlignment(userLabel, Qt::AlignCenter);
        userLabel->setAlignment(Qt::AlignCenter);
        userListScrollAreaLayout->addWidget(userLabel);
        activeUserLabels.insert(username, userLabel);

        alternateLabelStyle();
    }
}

void ChatPage::removeActiveUser(const QString &username) {
    auto iterator = activeUserLabels.find(username);
    if (iterator == activeUserLabels.end()) {
        return;
    }

    QLabel* userLabel = iterator.value();
    userLabel->deleteLater();
    activeUserLabels.erase(iterator);

    alternateLabelStyle();
}

void ChatPage::clearDisplayedMessages() {
    QLayoutItem* item;
    while ((item = chatScrollAreaLayout->takeAt(0)) != nullptr) {
        item->widget()->deleteLater();
        delete item;
    }

}

void ChatPage::alternateLabelStyle() {
    for (int i{0}; i < userListScrollAreaLayout->count(); ++i) {
        QLayoutItem* item = userListScrollAreaLayout->itemAt(i);
        if (!item) {
            continue;
        }

        QLabel* userLabel = qobject_cast<QLabel*>(item->widget());

        if (i % 2 == 0) {
            userLabel->setStyleSheet(R"(
                background-color: yellow;
            )");
        } else {
            userLabel->setStyleSheet(R"(
                background-color: green;
            )");
        }
    }
}




