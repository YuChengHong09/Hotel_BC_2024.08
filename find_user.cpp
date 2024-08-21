#include "find_user.h"
#include<QMouseEvent>
#include"find_ui.h"
find_user::find_user(QWidget *parent)
    :QLabel(parent) {

}
void find_user::mouseReleaseEvent(QMouseEvent *ev){//点击跳转到找回密码界面
    Find_ui *f=new Find_ui();
    f->show();
}
