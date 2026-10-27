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
void MyWidget::on_importButton_clicked(){
    QString fileName = QFileDialog::getOpenFileName(this, "匯入通訊錄", ".", "CSV 檔案 (*.csv);;文字檔 (*.txt)");
    if (fileName.isEmpty()) {
        return; // 使用者取消選擇
    }

    QFile inFile(fileName);
    if (!inFile.open(QFile::ReadOnly | QFile::Text)) {
        qDebug() << "Could not open file for read";
        return;
    }

    QTextStream in(&inFile);
    in.setEncoding(QStringConverter::Utf8);

    // 清空目前表格中的舊資料（若希望保留舊資料並 append，可註解掉此行）
    ui->contactBook->setRowCount(0);

    while (!in.atEnd()) {
        QString line = in.readLine().trimmed();
        if (line.isEmpty()) continue;

        // 依逗號拆分欄位
        QStringList fields = line.split(',');

        int row = ui->contactBook->rowCount();
        ui->contactBook->insertRow(row);

        // 填入 4 個欄位 (學號、班級、姓名、電話)
        for (int col = 0; col < 4 && col < fields.size(); ++col) {
            QString val = fields[col].trimmed();

            // 去除匯出時所加上 的 `="內容"` 格式或一般引號 `"`
            if (val.startsWith("=\"") && val.endsWith("\"")) {
                val = val.mid(2, val.length() - 3);
            } else if (val.startsWith("\"") && val.endsWith("\"")) {
                val = val.mid(1, val.length() - 2);
            }

            ui->contactBook->setItem(row, col, new QTableWidgetItem(val));
        }
    }

    inFile.close();
}

