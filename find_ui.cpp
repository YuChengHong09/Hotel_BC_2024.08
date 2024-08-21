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
ui->icon1->setVisible(false);
    ui->icon2->setVisible(false);
ui->icon3->setVisible(false);
    ui->lineEdit->setEnabled(0);
    ui->lineEdit_2->setEnabled(0);
    ui->lineEdit_3->setEnabled(0);
    ui->lineEdit_2->setEchoMode(QLineEdit::Password);
    ui->lineEdit_3->setEchoMode(QLineEdit::Password);
    QSqlDatabase db=QSqlDatabase::addDatabase("QMYSQL");
    db.setHostName("127.0.0.1");
    db.setPort(3306);
    db.setDatabaseName("itcase");
    db.setUserName("root");
    db.setPassword("");
    db.open();
    QSqlQuery *q=new QSqlQuery("itcase");
    connect(ui->lineEdit_4,QLineEdit::textChanged,this,[=](QString n){
    q->exec(QString("select question from user where name='%1'").arg(n));
    if(q->next()){
        ok1=true;
    ui->icon1->setVisible(true);
    ui->label_5->setText(q->value(0).toString());
    ui->lineEdit->setEnabled(1);
    ui->lineEdit_2->setEnabled(1);
    ui->lineEdit_3->setEnabled(1);
}
    else{
    ui->icon1->setVisible(false);
    ok1=false;
}
    });
    connect(ui->lineEdit,QLineEdit::textChanged,this,[=](QString answer){
        q->exec(QString("select answer from user where name='%1'").arg(ui->lineEdit_4->text()));
if(q->next()&&answer==q->value(0).toString()){
ui->icon2->setVisible(true);
    ok2=true;
}
else
    {
    ok2=false;
    ui->icon2->setVisible(false);
}
});
    connect(ui->lineEdit_3,QLineEdit::textChanged,this,[=](QString pasw){
        if(pasw==ui->lineEdit_2->text()){
            ui->icon3->setVisible(true);
            ok3=true;
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

void Find_ui::on_pushButton_clicked()
{
    if(ok1&&ok2&&ok3){
        QMessageBox *mes=new QMessageBox();
        QSqlQuery *query=new QSqlQuery("itcase");
        query->exec(QString("updata from user set password='%1' where name='%2'").arg(ui->lineEdit_2->text()).arg(ui->lineEdit_4->text()));
        mes->setText("修改成功");
        mes->exec();
        logon *l=new logon();
        this->close();
        l->show();
    }
    else if(!ok1){
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

