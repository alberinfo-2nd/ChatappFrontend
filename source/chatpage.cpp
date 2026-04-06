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


    this->setStyleSheet(R"(
        #chatScrollAreaContent QLabel {
            color: red;
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
    messageLabel->setSizePolicy(QSizePolicy::Maximum, QSizePolicy::Preferred);
    return messageLabel;

}

void ChatPage::displayReceivedMessage(const QString &message) {
    QLabel* messageLabel = createNewMessageLabel(message);
    chatScrollAreaLayout->setAlignment(messageLabel, Qt::AlignLeft);
    chatScrollAreaLayout->addWidget(messageLabel);


    messageLabel->setStyleSheet("background-color: black;");
}

void ChatPage::displaySentMessage(const QString &message) {
    QLabel* messageLabel = createNewMessageLabel(message);
    chatScrollAreaLayout->addWidget(messageLabel);
    chatScrollAreaLayout->setAlignment(messageLabel, Qt::AlignRight);

    messageLabel->setStyleSheet("background-color: white;");
}

void ChatPage::sendMessage() {
    const QString message = ui->messageLineEdit->text();
    ui->messageLineEdit->clear();
    if (!message.isEmpty()) {
        displaySentMessage(message);
    }
}
