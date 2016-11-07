#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "MyFriso.h"
#include "db.h"

#include <QTreeWidgetItem>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    init_UI();
    connect_mysql();
    init("friso.ini");
    init_SID();
    flush_SID();
    update_table_state();
    timer.start(1000*60*5);
}

MainWindow::~MainWindow()
{
    delete ui;
    free_friso();
}

void MainWindow::init_UI()
{
    ui->label_2->setMinimumWidth(80);
    ui->label_3->setMinimumWidth(80);
    ui->label_4->setMinimumWidth(80);
    ui->label_5->setMinimumWidth(80);
    ui->treeWidget->setHeaderLabel("SID(desc)");

    connect(this, SIGNAL(new_search(QString,QString)),\
            this, SLOT(search(QString,QString)));

    connect(this, SIGNAL(new_file(QStringList, QString)),\
            this, SLOT(store_file(QStringList, QString)));

    connect(ui->comboBox_result, SIGNAL(currentIndexChanged(int)),\
            this, SLOT(change_result(int)) );

    connect(&timer, SIGNAL(timeout()), \
            this, SLOT(check_table_state()));
}

void MainWindow::connect_mysql()
{
    db=QSqlDatabase::addDatabase("QMYSQL");
    db.setHostName("localhost");
    db.setDatabaseName("search_encrypt4");
    db.setUserName("root");
    db.setPassword("123456");
    if(!db.open())
    {
        qDebug()<<db.lastError();
    }

    QSqlQuery query;
    QString sql = QString("show tables");
    query.exec(sql);
    while(query.next())
        qDebug()<<query.value(0).toString();
}

void MainWindow::on_pushButton_clicked()
{
    QString text = ui->lineEdit->text();
    QString sid = ui->comboBox_USER->currentText();
    emit new_search(text, sid);
}

void MainWindow::change_result(int n)
{
    if( n >= text_list.size() || n < 0 )
        return;
    ui->textBrowser->clear();
    ui->textBrowser->append(text_list[n]);
}

void MainWindow::flush_SID()
{
    QStringList SID_list;
    get_all_SID(SID_list);

    ui->comboBox_USER->clear();
    ui->comboBox_pSID->clear();
    ui->comboBox_SID->clear();
    ui->treeWidget->clear();
    for(int i=0; i<SID_list.size(); i++)
    {
        QTreeWidgetItem* item = new QTreeWidgetItem();
        item->setText(0, SID_list[i]);
        ui->treeWidget->addTopLevelItem(item);
        ui->comboBox_USER->addItem( SID_list[i] );
        ui->comboBox_pSID->addItem( SID_list[i] );
        ui->comboBox_SID->addItem( SID_list[i] );
    }
}

void MainWindow::search(QString word, QString sid)
{
    ui->textBrowser->clear();

    text_list.clear();
    search_text(word, sid, text_list);
    ui->comboBox_result->clear();

    for(int i=0; i<text_list.size(); i++)
        ui->comboBox_result->addItem(QString::number(i));

    ui->textBrowser->append("finished!");
}

void MainWindow::store_file(QStringList file_list, QString sid)
{
    QTime t1 = QTime::currentTime();
    for(int i=0; i<file_list.size(); i++)
        store_text(file_list[i].toUtf8().data(), sid);
    QTime t2 = QTime::currentTime();
    qDebug()<<"store:"<<t1.msecsTo(t2)<<"ms";
}

void MainWindow::on_pushButton_store_clicked()
{
    QStringList file_list =
            QFileDialog::getOpenFileNames(this, "load files", ".", "*.txt");
    QString sid = ui->comboBox_SID->currentText();
    emit new_file(file_list,sid);
}

void MainWindow::on_pushButton_flush_clicked()
{
    flush_SID();
}

void MainWindow::on_pushButton_add_SID_clicked()
{
    QString sid = ui->lineEdit_new_SID->text();
    if( sid.length() == 0)
        return;

    QString psid = ui->comboBox_pSID->currentText();
    add_SID(psid, sid);

    flush_SID();
}

void MainWindow::check_table_state()
{
    update_table_state();
}
