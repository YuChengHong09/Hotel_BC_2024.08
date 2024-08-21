#ifndef LOGON_H
#define LOGON_H
#include <QWidget>

namespace Ui {
class logon;
}

class logon : public QWidget//登录界面
{
    Q_OBJECT

public:
    explicit logon(QWidget *parent = nullptr);
    ~logon();

private slots:


    void on_pushButton_clicked();

    void on_pushButton_3_clicked();

private:
    Ui::logon *ui;
    bool check_repeat(QString str);
    void paintEvent(QPaintEvent *e);


};

#endif // LOGON_H
