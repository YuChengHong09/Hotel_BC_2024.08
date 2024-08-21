#include "find_user.h"
#include<QMouseEvent>

find_user::find_user(QWidget *parent)
    :QLabel(parent) {

}
void find_user::mouseReleaseEvent(QMouseEvent *ev){
    emit finduser();
}
