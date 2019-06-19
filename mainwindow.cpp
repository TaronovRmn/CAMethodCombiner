#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QMessageBox>

//prototypes




MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow) {
    ui->setupUi(this);

    connect(ui->tablets, SIGNAL(currentChanged(int)), this, SLOT(tabSelected()));
    ui->tablets->setCurrentIndex(0);
    selectFm1();
}

MainWindow::~MainWindow() {
    delete ui;
}

void MainWindow::tabSelected() {
    switch (ui->tablets->currentIndex()) {
    case 0 : {
        selectFm1();
        break;
    }
    case 1: {
        selectFm2();
        break;
    }
    case 2: {
        selectSyn();
        break;
    }
    case 3: {
        selectDic();
        break;
    }

    }

}

void MainWindow::on_exit_triggered() {
    QApplication::quit();
}

void MainWindow::on_fm1StartButton_clicked() {
    ui->progressBar->show();
    int result = fr1::freqMethod1(ui->namesList->currentRow()+1, ui->progressBar);
    if (result == 0) {
        QMessageBox::information(this, "Успешно", "Метод успешно отработал");
    } else {
        QMessageBox::critical(this, "Error", "Ошибка");
    }
    ui->progressBar->hide();
    selectFm1();

}

void MainWindow::initList(QListWidget * list) {
    list->clear();
    QStringList methods = fr1::readMethods();
    if (methods.length() != 0) {

        list->addItems(methods);
        list->setCurrentRow(0);

    } else {


    }
}


void MainWindow::on_author_triggered() {
    QMessageBox::information(this, "Автор", "Автор программы: <br> Таронов Роман, НГТУ, АВТФ, АБМ-17. 2019 г.");
}

void MainWindow::selectFm1() {
    ui->progressBar->hide();


    initList(ui->namesList);
    ui->fm1tab->setEnabled(printTable(ui->fm1table, 1));
}

void MainWindow::selectFm2() {
    ui->progressBar->hide();


    initList(ui->namesList_2);
    ui->fm2tab->setEnabled(printTable(ui->fm2table, 2));
}

void MainWindow::selectSyn() {
    ui->progressBar->hide();

    if (ui->enuUsing->isChecked()) ui->ourGroup_3->setEnabled(false);
    else ui->ourGroup_3->setEnabled(true);
    initList(ui->synList);
    bool files = printTable(ui->synTable, 3);

    QString s;
    bool enuf = (readENU(s) != 0);
    ui->enuUsing->setEnabled(!enuf);

    ui->syntab->setEnabled(files);
}
void MainWindow::selectDic() {
    ui->progressBar->hide();
    initList(ui->dicList);
    bool files = printTable(ui->dicTable, 4);

    ui->dictab->setEnabled(files);
}
bool MainWindow::printTable(QTableWidget * table, int n) {
    int errors = 0;
    QStringList headers;
    headers<< "Файл"
           << "Кол-во текстов"
           << "Кол-во методов";
    table->setColumnCount(3);
    table->setShowGrid(true);
    table->setSelectionBehavior(QAbstractItemView::SelectRows);
    table->setHorizontalHeaderLabels(headers);
    table->horizontalHeader()->setStretchLastSection(true);
    switch (n) {
    case 1: {
        int nT,nM;
        QString name;
        table->setRowCount(1);
        int result = readOUR(nT, nM, name);
        if (result == 0) {
            table->setItem(0,0,new QTableWidgetItem(name));
            table->setItem(0,1,new QTableWidgetItem(QString::number(nT)));
            table->setItem(0,2,new QTableWidgetItem(QString::number(nM)));
        } else {
            table->setItem(0,0,new QTableWidgetItem(name));
            table->setItem(0,1,new QTableWidgetItem("не найден!"));
            errors++;
        }

        break;
    }
    case 2: {
        int nT,nM;
        QString name;
        table->setRowCount(4);
        int result = readOUR(nT, nM, name);
        if (result == 0) {
            table->setItem(0,0,new QTableWidgetItem(name));
            table->setItem(0,1,new QTableWidgetItem(QString::number(nT)));
            table->setItem(0,2,new QTableWidgetItem(QString::number(nM)));
        } else {
            table->setItem(0,0,new QTableWidgetItem(name));
            table->setItem(0,1,new QTableWidgetItem("не найден!"));
            errors++;
        }
        result = readSAR(nT, nM, name);
        if (result == 0) {
            table->setItem(1,0,new QTableWidgetItem(name));
            table->setItem(1,1,new QTableWidgetItem(QString::number(nT)));
            table->setItem(1,2,new QTableWidgetItem(QString::number(nM)));
        } else {
            table->setItem(1,0,new QTableWidgetItem(name));
            table->setItem(1,1,new QTableWidgetItem("не найден!"));
            errors++;
        }
        result = readSTE(nM, name);
        if (result == 0) {
            table->setItem(2,0,new QTableWidgetItem(name));
            table->setItem(2,2,new QTableWidgetItem(QString::number(nM)));
        } else {
            table->setItem(2,0,new QTableWidgetItem(name));
            table->setItem(2,1,new QTableWidgetItem("не найден!"));
            errors++;
        }
        result = readENQ(name);
        if (result == 0) {
            table->setItem(3,0,new QTableWidgetItem(name));
            table->setItem(3,1,new QTableWidgetItem("найден"));
        } else {
            table->setItem(3,0,new QTableWidgetItem(name));
            table->setItem(3,1,new QTableWidgetItem("не найден!"));
            errors++;
        }

        break;
    }
    case 3: {
        int nT,nM;
        QString name;
        table->setRowCount(2);
        int result = readOUR(nT, nM, name);
        if (result == 0) {
            table->setItem(0,0,new QTableWidgetItem(name));
            table->setItem(0,1,new QTableWidgetItem(QString::number(nT)));
            table->setItem(0,2,new QTableWidgetItem(QString::number(nM)));
        } else {
            table->setItem(0,0,new QTableWidgetItem(name));
            table->setItem(0,1,new QTableWidgetItem("не найден!"));
            errors++;
        }
        result = readENU(name);
        if (result == 0) {
            table->setItem(1,0,new QTableWidgetItem(name));
            table->setItem(1,1,new QTableWidgetItem("найден"));
        } else {
            table->setItem(1,0,new QTableWidgetItem(name));
            table->setItem(1,1,new QTableWidgetItem("не найден!"));
        }


        break;

    }
    case 4: {
        int nT,nM;
        QString name;
        table->setRowCount(4);
        int result = readOUR(nT, nM, name);
        if (result == 0) {
            table->setItem(0,0,new QTableWidgetItem(name));
            table->setItem(0,1,new QTableWidgetItem(QString::number(nT)));
            table->setItem(0,2,new QTableWidgetItem(QString::number(nM)));
        } else {
            table->setItem(0,0,new QTableWidgetItem(name));
            table->setItem(0,1,new QTableWidgetItem("не найден!"));
            errors++;
        }
        result = readENU(name);
        if (result == 0) {
            table->setItem(1,0,new QTableWidgetItem(name));
            table->setItem(1,1,new QTableWidgetItem("найден"));
        } else {
            table->setItem(1,0,new QTableWidgetItem(name));
            table->setItem(1,1,new QTableWidgetItem("не найден!"));
            errors++;
        }
        result = readDIC(name);
        if (result == 0) {
            table->setItem(2,0,new QTableWidgetItem(name));
            table->setItem(2,1,new QTableWidgetItem("найден"));
        } else {
            table->setItem(2,0,new QTableWidgetItem(name));
            table->setItem(2,1,new QTableWidgetItem("не найден!"));
            errors++;
        }
        result = readINV(name);
        if (result == 0) {
            table->setItem(3,0,new QTableWidgetItem(name));
            table->setItem(3,1,new QTableWidgetItem("найден"));
        } else {
            table->setItem(3,0,new QTableWidgetItem(name));
            table->setItem(3,1,new QTableWidgetItem("не найден!"));
            errors++;
        }


        break;

    }
    }
    if (errors > 0) ui->statusBar->showMessage("Некоторые файлы не найдены!");
    else ui->statusBar->showMessage("");
    return (errors == 0);
}

void MainWindow::on_fm2StartButton_clicked() {
    ui->progressBar->show();
    int result = fr2::freqMethod2(ui->progressBar);
    if (result == 0) {
        QMessageBox::information(this, "Успешно", "Метод успешно отработал");
    } else {
        QMessageBox::critical(this, "Ошибка", "Количество методов в файлах не совпадает!");
    }
    ui->progressBar->hide();
    selectFm2();

}

void MainWindow::on_synStartButton_clicked() {
    ui->progressBar->show();

    int result = syntacticMin(ui->enuUsing->isChecked(), ui->synList->currentRow()+1, ui->progressBar);
    if (result == 0) {
        QMessageBox::information(this, "Успешно", "Метод успешно отработал");
    } else {
        QMessageBox::critical(this, "Ошибка", "Ошибка!");
    }
    ui->progressBar->hide();
    selectSyn();

}

void MainWindow::on_enuUsing_stateChanged(int arg1) {
    selectSyn();
}

void MainWindow::on_dicStartButton_clicked() {
    ui->progressBar->show();
    int result;
    if (ui->contUsing->isChecked()) {
        result = dicCont(ui->progressBar);
    } else {
        result = dicMin(ui->progressBar);
    }
    if (result == 0) {
        QMessageBox::information(this, "Успешно", "Метод успешно отработал");
    } else {
        QMessageBox::critical(this, "Ошибка", "Ошибка!");
    }
    ui->progressBar->hide();
    selectDic();

}

void MainWindow::on_contUsing_stateChanged(int arg1) {
    selectDic();

}

void MainWindow::on_about_triggered() {
    QMessageBox::information(this, "О программе", "<p align=justify> Данная программы предназначена для нахождения минимального решения методов "
                             "криптоанализа для конкретного текста, среди имеющихся решений. <br> "
                             "Реализовано 4 метода: 2 частотных, синтаксический и словарный. <br>"
                             "При выборе каждого метода в левой колонке указывается список существующих в файле ouR.txt методов и так же возможен "
                             "выбор метода из этого списка в качестве опорного, "
                             "а в правой колонке список обязательных входящих файлов для конкретного метода. Также для словарного метода "
                             "есть дополнительная опция исправления по непротиворечивому контексту, а для синтаксического есть опция "
                             "использования эталона из отдельного файла enU. <br></p> Список обозначений методов: <br>"
                             "[SYN] - синтаксический метод <br>"
                             "[FR1] - первый частотный метод <br>"
                             "[FR2] - второй частотный метод <br>"
                             "[DIN] - простой словарный метод <br>"
                             "[DIC] - словарный метод с исправлением по контексту<br>");
}
