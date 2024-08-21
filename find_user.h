#ifndef FIND_USER_H
#define FIND_USER_H
#include<QLabel>
#include<QMouseEvent>
class find_user:public QLabel//提升QLabel，重写点击事件处理函数，实现找回密码界面跳转
{
    Q_OBJECT
public:
    explicit find_user(QWidget *parent=0);
protected:
    void mouseReleaseEvent(QMouseEvent *ev)override;
signals:
    void finduser();
};

#endif // FIND_USER_H
