#ifndef PAINT_H
#define PAINT_H

#include <QDialog>
#include <QtGui>
#include <QDialog>
#include <QtCore>

namespace Ui {
class Paint;
}

class Paint : public QDialog
{
    Q_OBJECT

public:
    explicit Paint(QWidget *parent = 0);
    std::vector<QString> allFiles;// Вектор, в котором хранятся все ссылки
    ~Paint();

private:
    Ui::Paint *ui;
    void paintEvent(QPaintEvent *e);

};

#endif // PAINT_H
