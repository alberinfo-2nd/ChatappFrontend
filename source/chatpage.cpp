#include "chatpage.h"
#include "ui_chatpage.h"
#include <QLabel>
#include <QVBoxLayout>

ChatPage::ChatPage(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::ChatPage)
    , chatScrollAreaLayout(new QVBoxLayout())
{
    ui->setupUi(this);

    ui->chatScrollAreaContent->setLayout(chatScrollAreaLayout);

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
        }
    )");
}

ChatPage::~ChatPage()
{
    delete ui;
}

void ChatPage::displayReceivedMessage(const QString &message) {
    QLabel* messageBox = new QLabel(message, ui->chatScrollArea);
    messageBox->setSizePolicy(QSizePolicy::Maximum, QSizePolicy::Preferred);
    chatScrollAreaLayout->setAlignment(messageBox, Qt::AlignLeft);
    chatScrollAreaLayout->addWidget(messageBox);


    messageBox->setStyleSheet("font-size: 18pt;"
                              "background-color: black;");
}

void ChatPage::displaySentMessage(const QString &message) {
    QLabel* messageBox = new QLabel(message, ui->chatScrollArea);
    messageBox->setSizePolicy(QSizePolicy::Maximum, QSizePolicy::Preferred);
    chatScrollAreaLayout->addWidget(messageBox);
    chatScrollAreaLayout->setAlignment(messageBox, Qt::AlignRight);

    messageBox->setStyleSheet("font-size: 18pt;"
                              "background-color: white;");
}
