#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTableWidget>
#include <QListWidget>
#include <QProgressBar>
#include <fstream>

namespace Ui {
class MainWindow;
}
namespace fr1 {
void copy_file(std::ifstream &f, std::ofstream &t);
int freqMethod1(int, QProgressBar *);
QStringList readMethods();
}
namespace fr2 {
int freqMethod2(QProgressBar *);
}

int syntacticMin(bool, int, QProgressBar *);
int dicMin(QProgressBar * );
int dicCont(QProgressBar * );

int readOUR(int &, int &, QString &);
int readSAR (int &, int &, QString &);
int readSTE (int &, QString &);
int readENQ (QString &);
int readENU (QString &);
int readDIC (QString &);
int readINV (QString &);

class MainWindow : public QMainWindow {
    Q_OBJECT

  public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

  private slots:
    void on_exit_triggered();

    void on_fm1StartButton_clicked();
    void tabSelected();

    void on_author_triggered();
    void initList(QListWidget * list);
    void selectFm1();
    void selectFm2();
    void selectSyn();
    void selectDic();
    bool printTable(QTableWidget * table, int n);

    void on_fm2StartButton_clicked();

    void on_synStartButton_clicked();

    void on_enuUsing_stateChanged(int arg1);

    void on_dicStartButton_clicked();

    void on_contUsing_stateChanged(int arg1);

    void on_about_triggered();

  private:
    Ui::MainWindow *ui;
};

#endif // MAINWINDOW_H
