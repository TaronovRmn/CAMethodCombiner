#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTableWidget>
#include <QListWidget>

namespace Ui {
class MainWindow;
}

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
    bool printTable(QTableWidget * table, int n);

    void on_fm2StartButton_clicked();

    void on_synStartButton_clicked();

    void on_enuUsing_stateChanged(int arg1);

  private:
    Ui::MainWindow *ui;
};

#endif // MAINWINDOW_H
