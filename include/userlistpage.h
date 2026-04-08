#ifndef USERLISTPAGE_H
#define USERLISTPAGE_H

#include <QWidget>
#include <QHash>
#include <QLabel>

namespace Ui {
class UserListPage;
}

class UserListPage : public QWidget
{
    Q_OBJECT

public:
    explicit UserListPage(QWidget *parent = nullptr);
    ~UserListPage();
    // Functions
    void displayActiveUsers(const std::vector<std::string> &users);
    void removeActiveUser(const QString &username);
    void addUserToList(const QString &username);
    // Getter for activeUserLabels (in order to disable the button)
    QWidget* getWidget(QString name) { return activeUserLabels[name]; }

private slots:
   void on_pushButton_clicked(); // Exit button

private:
    Ui::UserListPage *ui;
    QHash<QString, QWidget*> activeUserLabels;


signals:
    // signal for user clicking username to chat
    void userClicked(const QString &username);
    // signal for user exiting app
    void userLogOut();
};

#endif // USERLISTPAGE_H
