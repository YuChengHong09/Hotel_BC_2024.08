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
QString  MainWindow::user_type="";
logon::logon(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::logon)
{
    ui->setupUi(this);
    QSqlDatabase db = QSqlDatabase::addDatabase("QMYSQL");
    db.setHostName("127.0.0.1");  //连接本地主机
    db.setPort(3306);
    db.setDatabaseName("itcase");
    db.setUserName("root");
    db.setPassword("");
     db.open();
ui->lineEdit->setMaxLength(20);
ui->lineEdit_2->setMaxLength(18);
ui->lineEdit_2->setEchoMode(QLineEdit::Password);
ui->comboBox->addItem("员工(仅可登记，无修改权限)");
ui->comboBox->addItem("经理(可登记，有修改权限)");
ui->comboBox->setCurrentIndex(0);
connect(ui->label_6,find_user::finduser,this,[this]{
    Find_ui *f=new Find_ui();
    this->close();
    f->show();
});
}

logon::~logon()
{
    delete ui;
}

void logon::on_pushButton_2_clicked()
{
    this->close();

MainWindow *main_ui=new MainWindow();
        main_ui->show();
}


void logon::on_pushButton_clicked()
{


    QSqlQuery *query= new QSqlQuery("itcase");
    QString b=QString("select *from user where name='%1'").arg(ui->lineEdit->text());
    query->exec(b);
    query->next();
    if((query->value(1).toString()==ui->lineEdit_2->text())&&(query->value(2).toString()==ui->comboBox->currentText().mid(0,2))){
        MainWindow::user_type=query->value(2).toString();//将身份信息传递到操作系统页面
        QMessageBox *mesbox=new  QMessageBox(this);
        mesbox->setText("登录成功！");
        mesbox->exec();
        this->close();
MainWindow *main_ui=new MainWindow();
        main_ui->show();
    }
    else{
        QMessageBox *errormessage=new QMessageBox(this);
    errormessage->setText("登录失败，请检查信息是否正确");
    errormessage->exec();
    }
}


void logon::on_pushButton_3_clicked()
{

    enroll *enroll_UI=new enroll();
    this->close();
    enroll_UI->show();
}
void logon::paintEvent(QPaintEvent *e)
{
    QStyleOption opt;
    opt.initFrom(this);
    QPainter p(this);
    style()->drawPrimitive(QStyle::PE_Widget, &opt, &p, this);
}


