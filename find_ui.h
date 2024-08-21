#ifndef FIND_UI_H
#define FIND_UI_H

#include <QWidget>

namespace Ui {
class Find_ui;
}

class Find_ui : public QWidget
{
    Q_OBJECT

public:
    explicit Find_ui(QWidget *parent = nullptr);
    ~Find_ui();

private slots:
    void on_pushButton_clicked();

private:
    Ui::Find_ui *ui;
};

#endif // FIND_UI_H
