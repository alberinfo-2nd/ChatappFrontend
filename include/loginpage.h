#ifndef LOGINPAGE_H
#define LOGINPAGE_H

#include <QWidget>

namespace Ui {
class LoginPage;
}

class LoginPage : public QWidget
{
    Q_OBJECT

public:
    explicit LoginPage(QWidget *parent = nullptr);
    ~LoginPage();

signals:
    // signal for main window to listen for
    void loginSuccessful();

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


};

#endif // LOGINPAGE_H
