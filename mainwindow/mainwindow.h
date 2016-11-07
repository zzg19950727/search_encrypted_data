#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlError>
#include <QSqlRecord>
#include <QFileDialog>
#include <QDebug>
#include <QTimer>
#include <QTime>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

    void init_UI();

private slots:
    void connect_mysql();

    void on_pushButton_clicked();

    void change_result(int);

    void flush_SID();

    void search(QString word, QString sid);

    void store_file(QStringList file_list, QString sid);

    void on_pushButton_store_clicked();

    void on_pushButton_flush_clicked();

    void on_pushButton_add_SID_clicked();

    void check_table_state();

Q_SIGNALS:
    void new_search(QString, QString);

    void new_file(QStringList, QString);

private:
    Ui::MainWindow *ui;
    QSqlDatabase db;
    QStringList text_list;
    QTimer timer;
};

#endif // MAINWINDOW_H
