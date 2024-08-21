#include "find_ui.h"
#include "ui_find_ui.h"
#include<QSqlDatabase>
#include<QSqlQuery>
#include<QMessageBox>
#include"logon.h"
bool ok1,ok2,ok3=false;
Find_ui::Find_ui(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::Find_ui)
{
    ui->setupUi(this);
    ui->icon1->setVisible(false);//隐藏对钩图标，只在信息检查正确时显示
    ui->icon2->setVisible(false);
    ui->icon3->setVisible(false);
    ui->lineEdit->setEnabled(0);//密码和密保输入框初始不可编辑
    ui->lineEdit_2->setEnabled(0);
    ui->lineEdit_3->setEnabled(0);
    ui->lineEdit_2->setEchoMode(QLineEdit::Password);//设置密码显示样式
    ui->lineEdit_3->setEchoMode(QLineEdit::Password);
    QSqlDatabase db=QSqlDatabase::addDatabase("QMYSQL");//连接到已创建数据库
    db.setHostName("127.0.0.1");
    db.setPort(3306);
    db.setDatabaseName("itcase");
    db.setUserName("root");
    db.setPassword("");
    db.open();
    QSqlQuery *q=new QSqlQuery("itcase");//指定数据库
    connect(ui->lineEdit_4,QLineEdit::textChanged,this,[=](QString n){//检查用户名输入框，实时查询用户表中是否有已创建用户，查询到则恢复输入框编辑
    q->exec(QString("select question from user where name='%1'").arg(n));
    if(q->next()){//查询到信息
        ok1=true;//第一个状态bool值，代表用户存在
    ui->icon1->setVisible(true);//显示信息正确图标
    ui->label_5->setText(q->value(0).toString());//显示密保问题
    ui->lineEdit->setEnabled(1);//恢复密码和密保以及新密码输入框
    ui->lineEdit_2->setEnabled(1);
    ui->lineEdit_3->setEnabled(1);
}
    else{//查询不到信息时，重置图标和输入框初始状态
    ui->icon1->setVisible(false);
    ui->lineEdit->setEnabled(0);
    ui->lineEdit_2->setEnabled(0);
    ui->lineEdit_3->setEnabled(0);
    ok1=false;
}
    });
    connect(ui->lineEdit,QLineEdit::textChanged,this,[=](QString answer){//实时检测密保答案输入是否正确
        q->exec(QString("select answer from user where name='%1'").arg(ui->lineEdit_4->text()));
if(q->next()&&answer==q->value(0).toString()){//匹配答案
ui->icon2->setVisible(true);//显示正确图标
    ok2=true;//第二个状态bool值，代表密保密码正确
}
else//答案不正确
    {
    ok2=false;
    ui->icon2->setVisible(false);//重置图标
}
});
    connect(ui->lineEdit_3,QLineEdit::textChanged,this,[=](QString pasw){//检查两次输入密码是否一致
        if(pasw==ui->lineEdit_2->text()){//密码一致
            ui->icon3->setVisible(true);//显示图标
            ok3=true;//第三个状态bool值，代表两次密码是否一致
        }
        else{
            ui->icon3->setVisible(false);
            ok3=false;
        }
    });
}

Find_ui::~Find_ui()
{
    delete ui;
}

void Find_ui::on_pushButton_clicked()//确定修改按钮
{
    if(ok1&&ok2&&ok3){//三个检测全部通过
        QMessageBox *mes=new QMessageBox();
        QSqlQuery *query=new QSqlQuery("itcase");
        query->exec(QString("updata from user set password='%1' where name='%2'").arg(ui->lineEdit_2->text()).arg(ui->lineEdit_4->text()));//修改密码
        mes->setText("修改成功");//提示
        mes->exec();
        logon *l=new logon();//跳转到登录界面
        this->close();
        l->show();
    }
    else if(!ok1){//
         QMessageBox *mes=new QMessageBox();
        mes->setText("用户名错误!");
        mes->exec();
    }
    else if(!ok2){
        QMessageBox *mes=new QMessageBox();
        mes->setText("密保答案错误!");
        mes->exec();
    }
    else{
        QMessageBox *mes=new QMessageBox();
        mes->setText("两次密码输入不一致!");
        mes->exec();
    }

}

