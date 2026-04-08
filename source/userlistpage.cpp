#include "userlistpage.h"
#include "ui_userlistpage.h"
#include <QLabel>
#include <QVBoxLayout>
#include <QPushButton>

UserListPage::UserListPage(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::UserListPage)
{
    ui->setupUi(this);
    this->setAttribute(Qt::WA_StyledBackground, true); // fixes background problem

    this->setStyleSheet(R"(
    #UserListPage {
        background-color: #54566a;
    }

    #OnlineUserLabel {
       color: white;
       font-size: 30pt;
       font-weight: bold;
       border-radius: 15px;
       padding: 8px 20px;
}

    #LeftScrollArea, #RightScrollArea,
    #LeftScrollWidget, #RightScrollWidget {
        background-color: #bacba8;
        border-radius: 20px;
        padding: 10px;
        margin: 4px;
    }

    #LeftScrollWidget QWidget, #RightScrollWidget QWidget {
        background-color: white;
        color: black;
        border-radius: 10px;
        padding: 5px 10px;
        margin: 2px;
        font-weight: 500;
        font-size: 18pt;
    }
    #pushButton{
        background-color: white;
        color: black;
        font-size: 14pt;
        padding: 5px 20px;
        min-width: 20px;
    }

)");

    // TEST
    // std::vector<std::string> users = requestActiveUsers(); - for when request file added
    std::vector<std::string> testUsers = {"Kolby", "Keila", "Albert", "James"};
    displayActiveUsers(testUsers);
}

UserListPage::~UserListPage()
{
    delete ui;
}

// Function to Display List
void UserListPage::displayActiveUsers(const std::vector<std::string> &users) {
    for(const auto& user : users){
        QString username = QString::fromStdString(user);
        addUserToList(username);
    }
}

// Funtion to Add User
void UserListPage::addUserToList(const QString &username){
    QLabel* newLabel = new QLabel(username);
    ui->verticalLayout_3->insertWidget(0, newLabel);
    activeUserLabels.insert(username, newLabel);
}

// Funtion to Remove Users
void UserListPage::removeActiveUser(const QString &username) {
    auto iterator = activeUserLabels.find(username);
    if (iterator == activeUserLabels.end()) {
        return;
    }

    QLabel* userLabel = iterator.value();
    userLabel->deleteLater();
    activeUserLabels.erase(iterator);
}

// Exit Button
void UserListPage::on_pushButton_clicked() {
    this->close();
}

