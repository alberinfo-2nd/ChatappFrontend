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

private slots:
    void on_pushButton_clicked(); // Exit button

private:
    Ui::UserListPage *ui;
    QHash<QString, QLabel*> activeUserLabels;
};

#endif // USERLISTPAGE_H
