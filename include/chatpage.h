#ifndef CHATPAGE_H
#define CHATPAGE_H

#include <QWidget>

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

    void displayReceivedMessage(const QString &message);
    void displaySentMessage(const QString &message);
    void displayCurrentUsers();

private slots:
    void sendMessage();

private:
    Ui::ChatPage *ui;
    QVBoxLayout* chatScrollAreaLayout;
    QVBoxLayout* userListScrollAreaLayout;

    QLabel* createNewMessageLabel(const QString &message);

};

#endif // CHATPAGE_H
