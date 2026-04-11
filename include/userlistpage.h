#ifndef USERLISTPAGE_H
#define USERLISTPAGE_H

#include <QWidget>
#include <QHash>
#include <QLabel>

class ActiveUsersManager;
class SessionManager;

namespace Ui {
class UserListPage;
}

class UserListPage : public QWidget
{
    Q_OBJECT

public:
    explicit UserListPage(QWidget *parent = nullptr, SessionManager *sessionManager = nullptr, ActiveUsersManager *activeUsersManager = nullptr);
    ~UserListPage();
    // Functions
    void removeActiveUser(const QString &username);
    void addUserToList(const QString &username, const QString &public_key);

signals:
    void userClicked(const QString &username);
    // signal for user exiting app
    void logoutRequested();
    void chatRequested(const QString &username, const QString &public_key);

private slots:
   void on_pushButton_clicked(); // Exit button
   void displayActiveUsers();

private:
    Ui::UserListPage *ui;
    QHash<QString, QWidget*> activeUserLabels;
    SessionManager *m_sessionManager; // pointer to sessionManager
    ActiveUsersManager *m_activeUsersManager; // pointer to activeUsersManager

};

#endif // USERLISTPAGE_H
