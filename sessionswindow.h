#ifndef SESSIONSWINDOW_H
#define SESSIONSWINDOW_H

#include <QMainWindow>
#include <QString>

namespace Ui {
class SessionsWindow;
}

class SessionsWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit SessionsWindow(QWidget *parent = 0);
    ~SessionsWindow();

public slots:
    void addNewSession(QStringList friendSplited);

private:
    Ui::SessionsWindow *ui;
};

#endif // SESSIONSWINDOW_H
