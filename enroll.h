#ifndef ENROLL_H
#define ENROLL_H

#include <QWidget>

namespace Ui {
class enroll;
}

class enroll : public QWidget//注册界面
{
    Q_OBJECT

public:
    explicit enroll(QWidget *parent = nullptr);
    ~enroll();

private slots:
    void on_pushButton_2_clicked();

    void on_pushButton_clicked();

private:
    Ui::enroll *ui;
    bool check_repeat(QString str);
};

#endif // ENROLL_H
