#ifndef LOGINPAGE_H
#define LOGINPAGE_H

#include <QWidget>

class BackendClient;

namespace Ui {
class LoginPage;
}

class LoginPage : public QWidget
{
    Q_OBJECT

public:
    explicit LoginPage(QWidget *parent = nullptr, BackendClient *backendClient = nullptr);
    ~LoginPage();

signals:
    // signal for main window to listen for
    void loginSuccessful(QString username, QString public_key, QString authorizationToken);

private slots:
    // slot for toggling between User and Admin mode
    void toggleLoginMode();
    // slot for handling login emits loginSuccesful signal
    void handleLogin();

private:
    Ui::LoginPage *ui;

    // Modes for login page (used for readability)
    enum class LoginMode {
        User,
        Admin

    };

    // Data member to hold current mode (default is user)
    LoginMode m_mode{LoginMode::User};

    // used to upate UI based on current mode
    void updateLoginView();

    BackendClient *m_backendClient;


};

#endif // LOGINPAGE_H
