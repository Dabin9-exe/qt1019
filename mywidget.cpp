#include "mywidget.h"
#include "ui_mywidget.h"
#include <QFileDialog>
#include <QFile>
#include <QDebug>
#include <QStringConverter>

QString fileName = "C:/Users/user/Documents/qt1019/myfile.txt";

MyWidget::MyWidget(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::MyWidget)
{
    ui->setupUi(this);
    ui->contactBook->setColumnCount(4);
    QStringList colTotal;
    colTotal << "學號" << "班級" << "姓名" << "電話";
    ui->contactBook->setHorizontalHeaderLabels(colTotal);
}

MyWidget::~MyWidget()
{
    delete ui;
}

void MyWidget::on_insertButton_clicked(){
    QTableWidgetItem *col1, *col2, *col3, *col4;
    col1 = new QTableWidgetItem(ui->lineEdit_1->text());
    col2 = new QTableWidgetItem(ui->lineEdit_2->text());
    col3 = new QTableWidgetItem(ui->lineEdit_3->text());
    col4 = new QTableWidgetItem(ui->lineEdit_4->text());
    int row = ui->contactBook->rowCount();
    qDebug() << row;
    ui->contactBook->insertRow(row);
    ui->contactBook->setItem(row, 0, col1);
    ui->contactBook->setItem(row, 1, col2);
    ui->contactBook->setItem(row, 2, col3);
    ui->contactBook->setItem(row, 3, col4);
}

void cbWrite(QString Filename, QString str){
    QFile outFile(Filename);
    if(!outFile.open(QFile::WriteOnly | QFile::Text)){
        qDebug() << "Could not open file for write";
        return;
    }
    QTextStream out(&outFile);
    out.setEncoding(QStringConverter::Utf8);
    out.setGenerateByteOrderMark(true);
    out << str;
    outFile.flush();
    outFile.close();
}
void MyWidget::on_exportButton_clicked(){
    QString fileName;
    QString saveFile;
    fileName = QFileDialog:: getSaveFileName(this,"匯出通訊錄",".","csv(*.csv);;text(*.txt)");
    for(int i=0; i<ui->contactBook->rowCount(); i++){
        for(int j=0; j<ui->contactBook->columnCount(); j++){
            QTableWidgetItem *item = ui->contactBook->item(i, j);
            if (item != nullptr) {
                saveFile += "=\"" + item->text() + "\",";
            } else {
                saveFile += ",";
            }        }
        saveFile += "\n";
    }
    cbWrite(fileName, saveFile);
}

