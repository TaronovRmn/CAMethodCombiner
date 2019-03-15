#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QMessageBox>

int freqMethod1(int);
int readNumberOfMethods();

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow) {
    ui->setupUi(this);

    int numberOfMethods = readNumberOfMethods();
    ui->numbersSpin->setMaximum(numberOfMethods);
}

MainWindow::~MainWindow() {
    delete ui;
}

void MainWindow::on_exit_triggered() {
    QApplication::quit();
}

void MainWindow::on_fm1StartButton_clicked() {
    int result = freqMethod1(ui->numbersSpin->value());
    if (result == 0) {
        QMessageBox::information(this, "Успешно", "Метод успешно отработал");
    } else {
        QMessageBox::information(this, "Error", "Ошибка");
    }

}


void MainWindow::on_author_triggered() {
    QMessageBox::information(this, "Автор", "Автор программы: <br> Таронов Роман, НГТУ, АВТФ, АБМ-17. 2019 г.");
}
