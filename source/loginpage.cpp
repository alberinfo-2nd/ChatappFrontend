#include "loginpage.h"
#include "ui_loginpage.h"
#include <QMessageBox>
#include <BackendClient.h>
#include <sodium.h>

LoginPage::LoginPage(QWidget *parent, BackendClient *backendClient)
    : QWidget(parent)
    , ui(new Ui::LoginPage)
    , m_backendClient{backendClient}
{
    ui->setupUi(this);

    // Set up default view for login page
    updateLoginView();

    // Connects toggle button for switching between user and admin
    connect(ui->adminButton, &QPushButton::clicked, this, &LoginPage::toggleLoginMode);
    // Connects login buttin to slot for handling logins
    connect(ui->loginButton, &QPushButton::clicked, this, &LoginPage::handleLogin);

    // Set place holder for username line edit
    ui->usernameLineEdit->setPlaceholderText("Username");

    // Set place holder and mode for password line edit
    ui->passwordLineEdit->setPlaceholderText("Password");
    ui->passwordLineEdit->setEchoMode(QLineEdit::Password);

    // Make link styled admin button hand cursor on hover
    ui->adminButton->setCursor(Qt::PointingHandCursor);
    ui->loginButton->setCursor(Qt::PointingHandCursor);

    // Set background stlying attribute for login page;
    this->setAttribute(Qt::WA_StyledBackground, true);

    // Style sheet for LoginPage
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
            font-size: 28pt;
            font-weight: bold;
            color: black;
        }

        #descriptiveLabel {
            font-size: 14pt;
            color: rgba(0, 0, 0, 100);
        }

        #pageTitle {
            font-size: 40pt;
            font-weight: bold;
            margin-bottom: 80px;
            color: #7f906c;
        }

        .QPushButton, .QLineEdit {
            font-size: 16pt;
            color: black;
        }

        #loginButton:hover {
            background-color: #bacba8;
        }

        #loginButton {
            background-color: #7f906c;
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
            color: #7f906c;
            background: transparent;
            border: none;
            padding 0;
            margin: 0;
            text-decoration: underline;
        }

        #adminButton:hover {
            color: #bacba8;
        }
    )");
}

// Destructor
LoginPage::~LoginPage()
{
    delete ui;
}

// Updates UI based on current mode(either admin or user)
void LoginPage::updateLoginView()
{
    const bool isAdmin{m_mode == LoginMode::Admin};

    if (isAdmin)
    {
        ui->adminButton->setText("Back to user login");
        ui->descriptiveLabel->setText("Enter username and password");
        ui->passwordLineEdit->show();
    }
    else
    {
        ui->adminButton->setText("Admin login");
        ui->descriptiveLabel->setText("Create a username");
        ui->passwordLineEdit->clear();
        ui->passwordLineEdit->hide();
    }
}

// Slot function for changing mode between user and admin
void LoginPage::toggleLoginMode()
{
    if (m_mode == LoginMode::User)
    {
        m_mode = LoginMode::Admin;
    }
    else
    {
        m_mode = LoginMode::User;
    }

    updateLoginView();
}

// Slot function for handling login requests (emits loginSuccessful signal)
void LoginPage::handleLogin()
{
    const bool isAdmin{m_mode == LoginMode::Admin};
    const QString username{ui->usernameLineEdit->text().trimmed()};
    const QString password{ui->passwordLineEdit->text().trimmed()};

    // Input Validation
    if (username.isEmpty() || username.size() < 5)
    {
        QMessageBox::warning(this, "Login", "Username must be atleast 5 characters");
        return;
    }
    else if (username.contains(' '))
    {
        QMessageBox::warning(this, "Login", "Username can't contain spaces");
        return;
    }

    if (m_mode == LoginMode::Admin)
    {
        if (password.isEmpty())
        {
            QMessageBox::warning(this, "Warning", "Password can't empty");
            return;
        }
        else if (password.contains(' '))
        {
            QMessageBox::warning(this, "Warning", "Password can't contain spaces");
            return;
        }
    }

    QByteArray publicKey;
    QByteArray privateKey;

    publicKey.resize(crypto_box_PUBLICKEYBYTES);
    privateKey.resize(crypto_box_SECRETKEYBYTES);

    crypto_box_keypair(
        reinterpret_cast<unsigned char *>(publicKey.data()),
        reinterpret_cast<unsigned char *>(privateKey.data()));

    std::string authorizationToken = m_backendClient->sendLogin(username.toStdString(), publicKey.toBase64().toStdString(), password.toStdString());

    const QString message = QString::fromStdString(authorizationToken);

    // Error handling for various backend failure responses
    if (message == "Request failed" || message == "User is already logged in!" || message == "Login failed" || message == "Administrator not found!" || message == "Invalid password!" || message == "Username belongs to an admin!")
    {
        QMessageBox::warning(this, "Warning", message);
        return;
    }
    emit loginSuccessful(username, QString::fromStdString(publicKey.toBase64().toStdString()), privateKey, QString::fromStdString(authorizationToken), isAdmin);
}
