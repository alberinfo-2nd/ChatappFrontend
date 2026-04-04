#include "loginpage.h"
#include "ui_loginpage.h"
#include <QMessageBox>

LoginPage::LoginPage(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::LoginPage)
{
    ui->setupUi(this);

    // Set up default view for login page
    updateLoginView();

    // connects toggle button for switching between user and admin
    connect(ui->adminButton, &QPushButton::clicked, this, &LoginPage::toggleLoginMode);
    // connects login buttin to slot for handling logins
    connect(ui->loginButton, &QPushButton::clicked, this, &LoginPage::handleLogin);

    // style sheet for LoginPage
    this->setStyleSheet(R"(

        QWidget#loginCard {
            border: 2px solid black;
            background-color: #8ecae6;
            padding: 10px 12px;
            border-radius: 14px;
        }
    )");
}

// Destructor
LoginPage::~LoginPage()
{
    delete ui;
}

// Updates UI based on current mode(either admin or user)
void LoginPage::updateLoginView() {
    const bool isAdmin{ m_mode == LoginMode::Admin };

    if (isAdmin) {
        ui->adminButton->setText("Back to user login");
        ui->passwordLabel->show();
        ui->passwordLineEdit->show();
    } else {
        ui->adminButton->setText("Admin login");
        ui->passwordLabel->hide();
        ui->passwordLineEdit->clear();
        ui->passwordLineEdit->hide();
    }
}

// slot function for changing mode between user and admin
void LoginPage::toggleLoginMode() {
    if (m_mode == LoginMode::User) {
        m_mode = LoginMode::Admin;
    } else {
        m_mode = LoginMode::User;
    }

    updateLoginView();
}

// slot function for handling login requests (emits loginSuccessful signal)
void LoginPage::handleLogin() {

    const QString username{ ui->usernameLineEdit->text().trimmed() };

    if (username.isEmpty() || username.size() < 5) {
        QMessageBox::warning(this, "Login", "Username must be atleast 5 characters");
        return;
    } else if (username.contains(' ')) {
        QMessageBox::warning(this, "Login", "Username can't contain spaces");
        return;
    }
    if (m_mode == LoginMode::Admin) {

    } else {
        emit loginSuccessful();
    }

}

