#include "selectfriendsdialog.h"
#include "ui_selectfriendsdialog.h"

SelectFriendsDialog::SelectFriendsDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::SelectFriendsDialog)
{
    ui->setupUi(this);
}

SelectFriendsDialog::~SelectFriendsDialog()
{
    delete ui;
}

void SelectFriendsDialog::on_pushButton_clicked()
{
    emit exportFriendsIP(ui->lineEdit->text());
    close();
}
