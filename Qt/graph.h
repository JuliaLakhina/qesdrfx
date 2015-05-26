#ifndef GRAPH_H
#define GRAPH_H

#include <QDialog>
#include <QtGui>
#include <QDialog>
#include <QtCore>

namespace Ui {
class Graph;
}

class Graph : public QDialog
{
    Q_OBJECT

public:
    explicit Graph(QWidget *parent = 0);
    std::vector<std::vector<QString>> files; // Вектор, в котором хранятся все ссылки
    ~Graph();

private:
    Ui::Graph *ui;
    void paintEvent(QPaintEvent *paint);
};

#endif // GRAPH_H
