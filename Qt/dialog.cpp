#include "dialog.h"
#include "ui_dialog.h"
#include <QPushButton>
#include <QMessageBox>
#include <QDir>
#include "paint.h"
#include <QTextStream>
#include <QRegExp>
#include <vector>

Dialog::Dialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Dialog)
{
    ui->setupUi(this);
    connect(ui->buttonBox->button(QDialogButtonBox::Ok), SIGNAL(clicked()), SLOT(okClicked()));
    connect(ui->buttonBox->button(QDialogButtonBox::Cancel), SIGNAL(clicked()), SLOT(close()));
    setWindowTitle("Рисование графа");

    connect(this, SIGNAL(filePath(QString)), this, SLOT(printFile(QString)));
}

void Dialog::okClicked()
{
    emit filePath(ui->lineEdit->text());
}


Dialog::~Dialog()
{
    delete ui;
}

void Dialog::printFile(const QString &folderPath) {
    QFile ourFile(folderPath);
    QString buffer;

    if(!ourFile.open(QFile::ReadOnly | QFile::Text))
    {
        // Error
    }
    QTextStream stream(&ourFile);

    while(!stream.atEnd()) {
        all_files.push_back(stream.readLine());
    }

    ourFile.flush();
    ourFile.close();

    Paint *window = new Paint(this);
    window->allFiles = this->all_files;
    window->show();
}
