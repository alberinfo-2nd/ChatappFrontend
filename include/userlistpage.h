#ifndef USERLISTPAGE_H
#define USERLISTPAGE_H

#include <QWidget>

namespace Ui {
class UserListPage;
}

class UserListPage : public QWidget
{
    Q_OBJECT

public:
    explicit UserListPage(QWidget *parent = nullptr);
    ~UserListPage();

private:
    Ui::UserListPage *ui;
};

#endif // USERLISTPAGE_H
