#ifndef USERLISTPAGE_H
#define USERLISTPAGE_H

#include <QWidget>
#include <QHash>
#include <QLabel>

// Forward declarations
class ActiveUsersManager;
class SessionManager;
class BackendClient;

namespace Ui {
class UserListPage;
}

class UserListPage : public QWidget
{
    Q_OBJECT

public:
    explicit UserListPage(QWidget *parent = nullptr, SessionManager *sessionManager = nullptr, ActiveUsersManager *activeUsersManager = nullptr, BackendClient *backendClient = nullptr);
    ~UserListPage();
    // UI Management: Updates the scroll area
    void removeActiveUser(const QString &username);
    void addUserToList(const QString &username, const QString &public_key);

signals:
    // Navigation signals sent to MainWindow
    void userClicked(const QString &username);
    void logoutRequested();
    void chatRequested(const QString &username, const QString &public_key);

private slots:
   void on_pushButton_clicked(); // Exit/logout button handler
   void displayActiveUsers(); // Refresh logic for the user list

private:
    Ui::UserListPage *ui;
    // Mapping  usernames to their QWidget rows
    QHash<QString, QWidget*> activeUserLabels;
    // Core logic managers
    SessionManager *m_sessionManager;
    ActiveUsersManager *m_activeUsersManager;
    BackendClient *m_backendClient;

};

#endif // USERLISTPAGE_H
