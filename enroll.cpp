#include "enroll.h"
#include "ui_enroll.h"
#include"logon.h"
#include<QSqlDatabase>
#include<QSqlQuery>
#include<QMessageBox>
enroll::enroll(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::enroll)
{
    ui->setupUi(this);
    QSqlDatabase db=QSqlDatabase::addDatabase("QMYSQL");
   db.setHostName("127.0.0.1");
    db.setDatabaseName("itcase");
   db.setPort(3306);
    db.setUserName("root");
   db.setPassword("");
    db.open();

    ui->comboBox->addItem("员工(仅可查询，无修改权限)");
    ui->comboBox->addItem("经理(可查询，有修改权限)");
    ui->comboBox->setCurrentIndex(0);
}

enroll::~enroll()
{
    delete ui;
}

void enroll::on_pushButton_2_clicked()
{
    this->close();
    logon *logon_ui=new logon();
    logon_ui->show();

}


void enroll::on_pushButton_clicked()
{
    QSqlQuery *q=new QSqlQuery("itcase");
    if(ui->lineEdit->text()==""||ui->lineEdit_2->text()==""||ui->lineEdit_3->text()==""){//注册不得为空
        QMessageBox *nullwarning=new QMessageBox(this);
        nullwarning->setText("信息不得为空");
        nullwarning->exec();
    }
    else if(check_repeat(ui->lineEdit->text())){//已被注册警告

        QMessageBox *check=new QMessageBox(this);
        check->setText("该用户名已被注册");
        check->exec();

    }
    else//注册成功
    {
        auto name=ui->lineEdit->text();
        auto password=ui->lineEdit_2->text();
        auto power=ui->comboBox->currentText().mid(0,2);//截取有效字符串
        auto question=ui->lineEdit_3->text();
        auto answer=ui->lineEdit_4->text();
        q->exec(QString("insert into user(name,password,power,question,answer) value('%1','%2','%3','%4','%5')").arg(name).arg(password).arg(power).arg(question).arg(answer));
        QMessageBox *success=new QMessageBox(this);
        success->setText("注册成功");
        success->exec();
        this->close();
        logon *logon_ui=new logon();
        logon_ui->show();

    }
}
bool enroll::check_repeat(QString str){//检查注册时是否已被注册
    bool result=false;
    QSqlQuery *check=new QSqlQuery("itcase");
    check->exec("select name from user");
    while(check->next()){
        if(str==check->value(0).toString()){
            result=true;
            break;
        }
    }
    return result;
}
