#include "paint.h"
#include "ui_paint.h"

Paint::Paint(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Paint)
{
    ui->setupUi(this);
    setWindowTitle("Сериализация");
}



Paint::~Paint()
{
    delete ui;
}

void Paint::paintEvent(QPaintEvent *e)
{
    setStyleSheet("background-color: yellow");

    QPainter painter(this);

    painter.setBrush(QBrush(Qt::red, Qt::SolidPattern));

    QRect rec(250, 10, 250, 50);

    painter.drawRect(rec);
    painter.drawText(rec, Qt::AlignCenter, "Path: " + allFiles[0]);
    painter.setBrush(QBrush(Qt::green, Qt::SolidPattern));
    int x = 10;
    int y = 100;
    for (int i = 2; i < allFiles.size(); i++)
    {
        QRect rec(x, y, 250, 25);
        painter.drawRect(rec);
        painter.drawText(rec, Qt::AlignCenter, allFiles[i]);

        if ((i - 1) % 5 == 0)
        {
            x = 10;
            y += 75;
        } else {
            x += 300;
        }
    }
}
