#ifndef MAINWINDOW_H
#define MAINWINDOW_H
#include<QSqlDatabase>
#include<QSqlQuery>
#include <QMainWindow>
#include<QSqlQueryModel>
#include<QSqlTableModel>
QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow//酒店操作主界面
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
    static QString user_type;//静态身份信息，用来判断在酒店系统中能否修改房间价格

private slots:
    void on_pushButton_clicked();//预约确定按钮

    void on_pushButton_3_clicked();//查询预约信息按钮

    void on_pushButton_4_clicked();//预约按钮，点击跳转到预约界面(操作界面中的子界面跳转使用Stacked Widget翻页实现跳转)

    void on_pushButton_6_clicked();//登记按钮，点击跳转到登记界面

    void on_pushButton_2_clicked();//登记确定按钮

    void on_pushButton_5_clicked();//预约重置按钮

    void on_pushButton_8_clicked();//入住信息按钮，点击跳转到入住信息按钮

    void on_pushButton_11_clicked();//入住信息刷新按钮

    void on_pushButton_12_clicked();//追加消费确认按钮

    void on_pushButton_7_clicked();//消费按钮，点击跳转到消费界面

    void on_pushButton_9_clicked();//今日房况按钮，点击跳转到今日房况界面

    void on_pushButton_10_clicked();//房间价格按钮，点击跳转到房间价格界面

    void on_pushButton_13_clicked();//修改房价按钮

private:
    Ui::MainWindow *ui;
    QSqlDatabase db;//数据库连接
    QSqlQuery *q=nullptr;
    QSqlTableModel *tablemodel=nullptr;//数据库查询模型
    void check_status();//检查房间状态并设置相应颜色


protected:
    bool eventFilter(QObject *watched, QEvent *event)override;//重写事件过滤器,实现右键显示菜单


};
#endif // MAINWINDOW_H
