#ifndef SELECTFRIENDSDIALOG_H
#define SELECTFRIENDSDIALOG_H

#include <QDialog>
#include <QString>

namespace Ui {
class SelectFriendsDialog;
}

class SelectFriendsDialog : public QDialog
{
    Q_OBJECT

public:
    explicit SelectFriendsDialog(QWidget *parent = 0);
    ~SelectFriendsDialog();

private slots:
    void on_pushButton_clicked();

private:
    Ui::SelectFriendsDialog *ui;

signals:
    void exportFriendsIP(QString friendIP);
};

#endif // SELECTFRIENDSDIALOG_H
