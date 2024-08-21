#include "logon.h"
#include "ui_logon.h"
#include"mainwindow.h"
#include"enroll.h"
#include<QSqlDatabase>
#include<QSqlQuery>
#include<QMessageBox>
#include"find_user.h"
#include"find_ui.h"
#include<QStyleOption>
#include<QPainter>
QString  MainWindow::user_type;
logon::logon(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::logon)
{
    ui->setupUi(this);
    QSqlDatabase db = QSqlDatabase::addDatabase("QMYSQL");//创建数据库连接,连接已有数据库
    db.setHostName("127.0.0.1");  //连接本地主机
    db.setPort(3306);
    db.setDatabaseName("itcase");
    db.setUserName("root");
    db.setPassword("");
     db.open();
ui->lineEdit->setMaxLength(20);//设置可输入最大长度
ui->lineEdit_2->setMaxLength(18);
ui->lineEdit_2->setEchoMode(QLineEdit::Password);//设置密码显示样式
ui->comboBox->addItem("员工(仅可登记，无修改权限)");//添加下拉框内容
ui->comboBox->addItem("经理(可登记，有修改权限)");
ui->comboBox->setCurrentIndex(0);

}

logon::~logon()
{
    delete ui;
}



void logon::on_pushButton_clicked()//点击登录按钮，检查账号密码信息
{


    QSqlQuery *query= new QSqlQuery("itcase");//连接到数据库
    QString b=QString("select *from user where name='%1'").arg(ui->lineEdit->text());//检查用户表中是否有相关信息
    query->exec(b);
    query->next();
    if((query->value(1).toString()==ui->lineEdit_2->text())&&(query->value(2).toString()==ui->comboBox->currentText().mid(0,2))){//检查密码和权限
        MainWindow::user_type=query->value(2).toString();//将身份信息传递到酒店操作系统页面
        QMessageBox *mesbox=new  QMessageBox(this);
        mesbox->setText("登录成功！");
        mesbox->exec();
        this->close();
MainWindow *main_ui=new MainWindow();//跳转到操作界面
        main_ui->show();
    }
    else{//信息有误警告
        QMessageBox *errormessage=new QMessageBox(this);
    errormessage->setText("登录失败，请检查信息是否正确");
    errormessage->exec();
    }
}


void logon::on_pushButton_3_clicked()//跳转到注册界面
{

    enroll *enroll_UI=new enroll();
    this->close();
    enroll_UI->show();
}
void logon::paintEvent(QPaintEvent *e)//重写函数用以ui重绘
{
    QStyleOption opt;
    opt.initFrom(this);
    QPainter p(this);
    style()->drawPrimitive(QStyle::PE_Widget, &opt, &p, this);
}


