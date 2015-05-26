#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QString>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    std::vector<QString> all_sites; // Вектор, в котором хранятся НЕ повторяющиеся ссылки
    QString startFolderPath;
    QString domenName;
    ~MainWindow();

private slots:
    QString readAllFile(const QString &filePath);
    void printFile(const QString &folderPath);
    void on_actionRead_dir_triggered();

private:
    Ui::MainWindow *ui;
};

#endif // MAINWINDOW_H
