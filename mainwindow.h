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

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
    static QString user_type;//静态身份信息，用来判断在酒店系统中能否修改房间价格

private slots:
    void on_pushButton_clicked();

    void on_pushButton_3_clicked();

    void on_pushButton_4_clicked();

    void on_pushButton_6_clicked();

    void on_pushButton_2_clicked();

    void on_pushButton_5_clicked();

    void on_pushButton_8_clicked();

    void on_pushButton_11_clicked();

    void on_pushButton_12_clicked();

    void on_pushButton_7_clicked();

    void on_pushButton_9_clicked();

    void on_pushButton_10_clicked();

    void on_pushButton_13_clicked();

private:
    Ui::MainWindow *ui;
    QSqlDatabase db;
    QSqlQuery *q=nullptr;
    QSqlTableModel *tablemodel=nullptr;
    void check_status();//检查房间状态并设置相应颜色


protected:
    bool eventFilter(QObject *watched, QEvent *event)override;//重写事件过滤器


};
#endif // MAINWINDOW_H
