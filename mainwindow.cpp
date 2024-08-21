#include "mainwindow.h"
#include "./ui_mainwindow.h"
#include<QSqlDatabase>
#include<QSqlQuery>
#include<QDebug>
#include<QLabel>
#include<QMessageBox>
#include<QButtonGroup>
#include<QMouseEvent>
#include<QMenu>
QString user_type="";
MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    db = QSqlDatabase::addDatabase("QMYSQL");
    db.setHostName("127.0.0.1");  //连接本地主机
    db.setPort(3306);
    db.setDatabaseName("itcase");
    db.setUserName("root");
    db.setPassword("");
     db.open();
    q=new QSqlQuery("itcase");

     q->exec("select price from room_status where type='单人房'");
    q->next();
     auto price_one_one_bed=q->value("price").toInt();
    q->exec("select price from room_status where type='双人房'");
    q->next();
    auto price_one_two_bed=q->value("price").toInt();
    q->exec("select price from room_status where type='豪华大床房'");
    q->next();
    auto price_one_best_room=q->value("price").toInt();
    ui->price_one_bed->setRange(0,2000);
    ui->price_two_bed->setRange(0,2000);
    ui->price_bestroom->setRange(0,2000);
   ui->price_one_bed->setValue(price_one_one_bed);
   ui->price_two_bed->setValue(price_one_two_bed);
   ui->price_bestroom->setValue(price_one_best_room);
   if(user_type=="员工"){//权限为员工。禁止编辑房间价格
ui->change_button->setEnabled(false);//改变房价按钮变灰
ui->price_one_bed->setEnabled(false);//spin box无法被编辑
ui->price_two_bed->setEnabled(false);
ui->price_bestroom->setEnabled(false);
   }

ui->stackedWidget->setCurrentIndex(0);
    ui->dateTimeEdit->setDateTime(QDateTime::currentDateTime().addSecs(600));//预约时间默认当前时间+10分钟
ui->dateTimeEdit_3->setDateTime(QDateTime::currentDateTime());//登记时间默认
    ui->dateTimeEdit_4->setDateTime(QDateTime::currentDateTime().addDays(1));//离店时间默认一天
ui->dateTimeEdit_5->setDateTime(QDateTime::currentDateTime());//右上角显示当前时间
ui->comboBox->addItem("");//预约界面房间类型下拉框
    ui->comboBox->addItem("单人房");
ui->comboBox->addItem("双人房");
    ui->comboBox->addItem("豪华大床房");
ui->comboBox->setCurrentIndex(0);//

    connect(ui->lineEdit_5,QLineEdit::textChanged,this,[=](QString room){//检测输入房间名，自动生成房间类型
    q->exec(QString("select type from room_status where room_id='%1'").arg(room));//，通过房间名搜索房间的类型
    q->next();
    QString room_type=q->value(0).toString();
    if(room_type=="单人房")//下拉框自动设置为相应类型
    ui->comboBox->setCurrentIndex(1);
    else if(room_type=="双人房")
    ui->comboBox->setCurrentIndex(2);
    else if(room_type=="豪华大床房")
    ui->comboBox->setCurrentIndex(3);
    else
    ui->comboBox->setCurrentIndex(0);

});
ui->comboBox_2->addItem("");//登录页面房间类型下拉框
ui->comboBox_2->addItem("单人房");
ui->comboBox_2->addItem("双人房");
ui->comboBox_2->addItem("豪华大床房");
ui->comboBox_2->setCurrentIndex(0);
connect(ui->lineEdit_8,QLineEdit::textChanged,this,[=](QString room){//检测输入房间名，自动生成房间类型
    q->exec(QString("select type from room_status where room_id='%1'").arg(room));//搜索房间的类型
    q->next();
    QString room_type=q->value(0).toString();
    if(room_type=="单人房")
        ui->comboBox_2->setCurrentIndex(1);
    else if(room_type=="双人房")
        ui->comboBox_2->setCurrentIndex(2);
    else if(room_type=="豪华大床房")
        ui->comboBox_2->setCurrentIndex(3);
    else
        ui->comboBox_2->setCurrentIndex(0);

});
ui->dateTimeEdit->setCalendarPopup(true);//时间选择加入日历
ui->dateTimeEdit_3->setCalendarPopup(true);
ui->dateTimeEdit_4->setCalendarPopup(true);
QButtonGroup *RadioBttonGroup=new QButtonGroup(this);//性别单选按钮
RadioBttonGroup->addButton(ui->radioButton,0);
RadioBttonGroup->addButton(ui->radioButton_2,1);
tablemodel=new QSqlTableModel(this);//入住表数据库模型
tablemodel->setTable("check_in");//绑定入住数据表
tablemodel->select();//查询表中所有信息
tablemodel->setEditStrategy(QSqlTableModel::OnManualSubmit);//所有对模型的改变都会缓冲，通过submitall来提交
QStringList TableHead;//表头
TableHead<<"姓名"<<"性别"<<"身份证号"<<"手机号"<<"入住房间"<<"房间消费"<<"入住时间"<<"预计退房时间";
for(int i=0;i<TableHead.size();i++)
tablemodel->setHeaderData(i,Qt::Horizontal,TableHead[i]);//在数据模型中加入表头
ui->tableView->setModel(tablemodel);//应用模型到视图表
ui->tableView->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);//列宽随内容自动扩展
ui->tableView->installEventFilter(this);//添加事件监听器
}

MainWindow::~MainWindow()
{
    delete ui;
}








void MainWindow::on_pushButton_clicked()//预约确定按钮
{
      QMessageBox *b=new QMessageBox(this);
    auto name=ui->lineEdit->text();
    auto id=ui->lineEdit_2->text();
    auto number=ui->lineEdit_3->text();
    auto datetime=ui->dateTimeEdit->dateTime();//预约时间
    auto room_id=ui->lineEdit_5->text();
    bool ok=datetime>=(ui->dateTimeEdit_5->dateTime());//预约的时间是否在当前时间之后
    QString date_time=datetime.toString("yyyy-MM-dd HH:mm:ss");//转换Qdatetime为Mysql可接受的格式
    q->exec(QString("select status from room_status where room_id ='%1'").arg(room_id));//查询房间状态
    if(q->next()){
    if(q->value(0).toString()=="可入住"&&ok){
    q->exec(QString("insert into reservation(name,card_id,phone_number,time,room_id) values ('%1','%2','%3','%4','%5')").arg(name).arg(id)
                .arg(number).arg(date_time).arg(room_id));
    q->exec(QString("update room_status set status='已预约' where room_id='%1'").arg(room_id));//修改房间状态为已预约

    b->setText("预约成功!");
        b->exec();
}
    else if(!ok){//预约时间不能在现在时间之前
        b->setText("预约时间在当前时间之前,请修改!");
    b->exec();
    }
else if(q->value(0).toString()=="已登记")//房间已登记
    {
    b->setText("预约失败,该房间有人入住!");
    b->exec();
    }
    else{//房间已被预约
        b->setText("预约失败，该房间已被预约!");
        b->exec();
    }
    }
    else
    {

        QMessageBox::critical(this,"错误","未找到相关房间");//输入房间号错误弹出错误
    }
}


void MainWindow::on_pushButton_3_clicked()//查询预约按钮
{
    QMessageBox *box=new QMessageBox(this);
    QString name=ui->lineEdit_4->text();
    q->exec(QString("select card_id,phone_number,time,room_id from reservation where name='%1'").arg(name));
    if(q->next()){//查询到预约信息
        ui->lineEdit_6->setText(q->value(0).toString());//显示预约的身份证号，手机号，房间号
        ui->lineEdit_7->setText(q->value(1).toString());
        auto datetime=q->value(2).toDateTime();
        bool ok_time=QDateTime::currentDateTime()>=datetime;//判断当前是否在预约时间内
        ui->lineEdit_8->setText(q->value(3).toString());
        if(ok_time){
            ui->dateTimeEdit_3->setDateTime(datetime);
            box->setText("查询到预约信息!");
            box->exec();
    }
        else//未到预约时间提示，并告知预约时间
        {
            auto reservation_time=q->value(2).toDateTime().toString("yyyy-MM-dd HH:mm:ss");
        box->setText(QString("客户预约时间未到，预约时间为:'%1'").arg(reservation_time));
        box->exec();
        }
}
    else{
        box->setText("未查询到相关预约信息");
    box->exec();
    }
}

void MainWindow::on_pushButton_4_clicked()//点击预约按钮转移到第一页
{
    ui->stackedWidget->setCurrentIndex(0);
}


void MainWindow::on_pushButton_6_clicked()//点击登记按钮转移到第二页
{
    ui->stackedWidget->setCurrentIndex(1);
}


void MainWindow::on_pushButton_2_clicked()//登记确认按钮
{
    QMessageBox *b1=new QMessageBox(this);
    auto name=ui->lineEdit_4->text();
    QString gender;
    if(ui->radioButton->isChecked())
         gender="男";
    else
         gender="女";
    auto card_id=ui->lineEdit_6->text();
    auto phone_number=ui->lineEdit_7->text();
    auto room_id=ui->lineEdit_8->text();
    auto begin_time=ui->dateTimeEdit_3->dateTime().toString("yyyy-MM-dd HH:mm:ss");
    auto end_time=ui->dateTimeEdit_4->dateTime().toString("yyyy-MM-dd HH:mm:ss");
    q->exec(QString("select status,price from room_status where room_id='%1'").arg(room_id));
    if(q->next()&&q->value(0).toString()=="可入住"){
        auto price=q->value(1).toInt();
        int day=ui->dateTimeEdit_3->dateTime().daysTo(ui->dateTimeEdit_4->dateTime());//计算入住天数
        price*=day;//计算房费

    q->exec(QString("insert into check_in(name,gender,card_id,phone_number,room_id,consume,begin_time,end_time) value('%1','%2','%3','%4','%5','%6','%7','%8')")
                    .arg(name).arg(gender).arg(card_id).arg(phone_number).arg(room_id).arg(price).arg(begin_time).arg(end_time));//将登记信息录入数据库
        q->exec(QString("delete from reservation where name='%1'").arg(name));//登记入住成功后删除预约记录
        q->exec(QString("update room_status set status='已登记' where room_id='%1'").arg(room_id));//修改房间状态为已登记
    b1->setText("登记成功!");
    b1->exec();
    }
    else if(q->value(0).toString()=="已登记")
    {
        b1->setText("房间有人入住，请选择空闲房间办理登记!");
        b1->exec();
    }
    else if(q->value(0).toString()=="已预约"){
        b1->setText("房间已被预约，请选择空闲房间办理登记!");
        b1->exec();
    }
    else
    {
        b1->setText("房间信息有误，请检查房号是否属于本酒店!");
        b1->exec();
    }


}


void MainWindow::on_pushButton_5_clicked()//重置按钮
{
    ui->lineEdit->clear();
    ui->lineEdit_2->clear();
     ui->lineEdit_3->clear();
    ui->dateTimeEdit_3->setDateTime(QDateTime::currentDateTime());
     ui->lineEdit_5->clear();
}


void MainWindow::on_pushButton_8_clicked()//入住信息
{
    ui->stackedWidget->setCurrentIndex(2);
}


bool MainWindow::eventFilter(QObject *watched, QEvent *event){//重写事件过滤器
    if(watched==ui->tableView&&event->type()==QEvent::ContextMenu){

            QMenu *menu=new QMenu(this);
            QAction *action1=new QAction("删除数据",this);
            menu->addAction(action1);
            QAction * action= menu->exec(QCursor::pos());//弹出菜单
            if(action->text().compare("删除数据") == 0){
                tablemodel->removeRow(ui->tableView->currentIndex().row());//删除数据行
            QMessageBox *box=new QMessageBox(this);
         auto ok= box->warning(this,"提示","是否退房",QMessageBox::Yes,QMessageBox::No);//警告
            if(ok==QMessageBox::Yes){
             int indx=ui->tableView->currentIndex().row();
                auto id=tablemodel->data(tablemodel->index(indx,4)).toString();//根据选中行的房间id来恢复退房后房间状态
             q->exec(QString("update room_status set status='可入住' where room_id='%1'").arg(id));//恢复为可入住
                auto money=tablemodel->data(tablemodel->index(indx,5)).toString();//读出所有消费
               tablemodel->submitAll();//提交事务
                QMessageBox *box_1=new QMessageBox(this);
               box_1->information(this,"退房成功",QString("共消费'%1'元,请到前台结清费用").arg(money));
            }
            else
                tablemodel->revertAll();//撤回
            }
    }


}

void MainWindow::on_pushButton_11_clicked()//入住登记信息刷新按钮
{
    tablemodel->select();
}


void MainWindow::on_pushButton_12_clicked()//消费追加按钮
{
    auto room=ui->lineEdit_9->text();
    auto money=ui->lineEdit_10->text().toInt();
    q->exec(QString("select consume from check_in where room_id='%1'").arg(room));
    q->next();
    int price=q->value("consume").toInt()+money;
    q->exec(QString("update check_in set consume='%1'where room_id='%2'").arg(price).arg(room));
    QMessageBox *box=new QMessageBox(this);
    box->information(this,"提示","消费追加完成！");
}


void MainWindow::on_pushButton_7_clicked(){//转到消费页面
    ui->stackedWidget->setCurrentIndex(5);
}




void MainWindow::on_pushButton_9_clicked()//转到今日房况页面(绘制房间布局,并根据三种房间状态，改变样式表绘制颜色,实现房间状态可视化)
{
    ui->stackedWidget->setCurrentIndex(3);
    q->exec("select * from room_status");//查询所有房间状态
    while(q->next()){
        if(q->value("room_id").toString()=="101"){
            // if(q->value("status").toString()=="可入住")//如果可入住则显示绿色
                ui->room_101->setStyleSheet("background-color:green");
            if(q->value("status").toString()=="已预约")//已预约显示黄色
                ui->room_101->setStyleSheet("background-color:yellow");
            if(q->value("status").toString()=="已登记")//已登记显示红色
                ui->room_101->setStyleSheet("background-color:red");
        }
        if(q->value("room_id").toString()=="102"){
            if(q->value("status").toString()=="可入住")
                ui->room_102->setStyleSheet("background-color:green");
            if(q->value("status").toString()=="已预约")
                ui->room_102->setStyleSheet("background-color:yellow");
            if(q->value("status").toString()=="已登记")
                ui->room_102->setStyleSheet("background-color:red");
        }
        if(q->value("room_id").toString()=="103"){
            if(q->value("status").toString()=="可入住")
                ui->room_103->setStyleSheet("background-color:green");
            if(q->value("status").toString()=="已预约")
                ui->room_103->setStyleSheet("background-color:yellow");
            if(q->value("status").toString()=="已登记")
                ui->room_103->setStyleSheet("background-color:red");
        }
        if(q->value("room_id").toString()=="104"){
            if(q->value("status").toString()=="可入住")
                ui->room_104->setStyleSheet("background-color:green");
            if(q->value("status").toString()=="已预约")
                ui->room_104->setStyleSheet("background-color:yellow");
            if(q->value("status").toString()=="已登记")
                ui->room_104->setStyleSheet("background-color:red");
        }
        if(q->value("room_id").toString()=="105"){
            if(q->value("status").toString()=="可入住")
                ui->room_105->setStyleSheet("background-color:green");
            if(q->value("status").toString()=="已预约")
                ui->room_105->setStyleSheet("background-color:yellow");
            if(q->value("status").toString()=="已登记")
                ui->room_105->setStyleSheet("background-color:red");
        }
        if(q->value("room_id").toString()=="201"){
            if(q->value("status").toString()=="可入住")
                ui->room_201->setStyleSheet("background-color:green");
            if(q->value("status").toString()=="已预约")
                ui->room_201->setStyleSheet("background-color:yellow");
            if(q->value("status").toString()=="已登记")
                ui->room_201->setStyleSheet("background-color:red");
        }
        if(q->value("room_id").toString()=="202"){
            if(q->value("status").toString()=="可入住")
                ui->room_202->setStyleSheet("background-color:green");
            if(q->value("status").toString()=="已预约")
                ui->room_202->setStyleSheet("background-color:yellow");
            if(q->value("status").toString()=="已登记")
                ui->room_202->setStyleSheet("background-color:red");
        }
        if(q->value("room_id").toString()=="203"){
            if(q->value("status").toString()=="可入住")
                ui->room_203->setStyleSheet("background-color:green");
            if(q->value("status").toString()=="已预约")
                ui->room_203->setStyleSheet("background-color:yellow");
            if(q->value("status").toString()=="已登记")
                ui->room_203->setStyleSheet("background-color:red");
        }
        if(q->value("room_id").toString()=="204"){
            if(q->value("status").toString()=="可入住")
                ui->room_204->setStyleSheet("background-color:green");
            if(q->value("status").toString()=="已预约")
                ui->room_204->setStyleSheet("background-color:yellow");
            if(q->value("status").toString()=="已登记")
                ui->room_204->setStyleSheet("background-color:red");
        }
        if(q->value("room_id").toString()=="205"){
            if(q->value("status").toString()=="可入住")
                ui->room_205->setStyleSheet("background-color:green");
            if(q->value("status").toString()=="已预约")
                ui->room_205->setStyleSheet("background-color:yellow");
            if(q->value("status").toString()=="已登记")
                ui->room_205->setStyleSheet("background-color:red");
        }
        if(q->value("room_id").toString()=="301"){
            if(q->value("status").toString()=="可入住")
                ui->room_301->setStyleSheet("background-color:green");
            if(q->value("status").toString()=="已预约")
                ui->room_301->setStyleSheet("background-color:yellow");
            if(q->value("status").toString()=="已登记")
                ui->room_301->setStyleSheet("background-color:red");
        }
        if(q->value("room_id").toString()=="302"){
            if(q->value("status").toString()=="可入住")
                ui->room_302->setStyleSheet("background-color:green");
            if(q->value("status").toString()=="已预约")
                ui->room_302->setStyleSheet("background-color:yellow");
            if(q->value("status").toString()=="已登记")
                ui->room_302->setStyleSheet("background-color:red");
        }
        if(q->value("room_id").toString()=="303"){
            if(q->value("status").toString()=="可入住")
                ui->room_303->setStyleSheet("background-color:green");
            if(q->value("status").toString()=="已预约")
                ui->room_303->setStyleSheet("background-color:yellow");
            if(q->value("status").toString()=="已登记")
                ui->room_303->setStyleSheet("background-color:red");
        }
        if(q->value("room_id").toString()=="304"){
            if(q->value("status").toString()=="可入住")
                ui->room_304->setStyleSheet("background-color:green");
            if(q->value("status").toString()=="已预约")
                ui->room_304->setStyleSheet("background-color:yellow");
            if(q->value("status").toString()=="已登记")
                ui->room_304->setStyleSheet("background-color:red");
        }
        if(q->value("room_id").toString()=="305"){
            if(q->value("status").toString()=="可入住")
                ui->room_305->setStyleSheet("background-color:green");
            if(q->value("status").toString()=="已预约")
                ui->room_305->setStyleSheet("background-color:yellow");
            if(q->value("status").toString()=="已登记")
                ui->room_305->setStyleSheet("background-color:red");
        }
    }
}


void MainWindow::on_pushButton_10_clicked()//跳转到房间价格页面
{
    ui->stackedWidget->setCurrentIndex(4);
}


void MainWindow::on_pushButton_13_clicked()//改变房价按钮
{
    int price_one=ui->price_one_bed->value();//提取出spin box中的价格
    int price_two=ui->price_two_bed->value();
    int price_best=ui->price_bestroom->value();
    q->exec(QString("update room_status set price='%1' where type='单人房'").arg(price_one));//修改房间表中的价格
    q->exec(QString("update room_status set price='%1' where type='双人房'").arg(price_two));
    q->exec(QString("update room_status set price='%1' where type='豪华大床房'").arg(price_best));
    QMessageBox::information(this,"修改成功","房间价格修改成功");
}

