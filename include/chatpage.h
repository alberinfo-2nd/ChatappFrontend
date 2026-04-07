#ifndef CHATPAGE_H
#define CHATPAGE_H

#pragma once
#include <QWidget>
#include <vector>
#include <string>

class QVBoxLayout;
class QLabel;

namespace Ui {
class ChatPage;
}

class ChatPage : public QWidget
{
    Q_OBJECT

public:
    explicit ChatPage(QWidget *parent = nullptr);
    ~ChatPage();



private slots:
    void sendMessage();
    void clearDisplayedMessages();

private:
    Ui::ChatPage *ui;
    QVBoxLayout* chatScrollAreaLayout;
    QVBoxLayout* userListScrollAreaLayout;

    QLabel* createNewMessageLabel(const QString &message);

    QHash<QString, QLabel*> activeUserLabels;

    void displayReceivedMessage(const QString &message);
    void displaySentMessage(const QString &message);
    void displayActiveUsers(const std::vector<std::string> &users);
    void removeActiveUser(const QString &username);
    void alternateLabelStyle();
};

#endif // CHATPAGE_H
