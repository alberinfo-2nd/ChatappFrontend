#ifndef CHATPAGE_H
#define CHATPAGE_H

#include <QWidget>

class QVBoxLayout;

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

private:
    Ui::ChatPage *ui;
    QVBoxLayout* chatScrollAreaLayout;

};

#endif // CHATPAGE_H
