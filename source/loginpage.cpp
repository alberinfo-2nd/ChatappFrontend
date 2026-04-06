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

    // set place holder for username line edit
    ui->usernameLineEdit->setPlaceholderText("Username");

    // set place holder and mode for password line edit
    ui->passwordLineEdit->setPlaceholderText("Password");
    ui->passwordLineEdit->setEchoMode(QLineEdit::Password);

    // make link styled admin button hand cursor on hover
    ui->adminButton->setCursor(Qt::PointingHandCursor);

    // set background stlying attribute for login page;
    this->setAttribute(Qt::WA_StyledBackground, true);


    // style sheet for LoginPage
    this->setStyleSheet(R"(

        #LoginPage {
            background-color: #54566a;
        }

        QWidget#loginCard {
            background-color: white;
            padding: 20px 16px;
            border-radius: 14px;
            border: 1px solid rgba(0, 0, 0, 100);
        }

        #loginTitle {
            font-size: 30pt;
            font-weight: bold;
            margin-bottom: 18px;
            color: black;
        }

        #pageTitle {
            font-size: 36pt;
            font-weight: bold;
            margin-bottom: 30px;
        }

        .QPushButton, .QLineEdit {
            font-size: 16pt;
            color: black;
        }

        #loginButton:hover {
            background-color: #aed683;
        }

        #loginButton {
            background-color: #bacba8;
            padding: 6px 10px;
            color: white;
        }

        .QLineEdit {
            border: 1px solid rgba(0, 0, 0, 100);
            border-radius: 6px;
            background-color: white;
            padding: 6px 10px;
        }

        #adminButton {
            color: #bacba8;
            background: transparent;
            border: none;
            padding 0;
            margin: 0;
            text-decoration: underline;
        }

        #adminButton:hover {
            color: #aed683;
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
        ui->passwordLineEdit->show();
    } else {
        ui->adminButton->setText("Admin login");
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

