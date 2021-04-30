#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QtSql>
#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QMessageBox>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    m_db = QSqlDatabase::addDatabase("QSQLITE"); //соединение объекта базы данных
                                    // с СУБД
    m_db.setDatabaseName("myDB");  //определение имени базы данных
    query = new QSqlQuery(m_db); // создание объекта для запроса
    if(!m_db.open()) // проверка на ошибку при открытии или создании базы данных
        throw "can't open database";
    if(!m_db.tables().contains("Design")) // если в базе не существует таблица  Person,
    { //то создание таблицы  Person и заполнение данными
        query->clear(); // очистка запроса
  query->exec("CREATE TABLE Person(ID INTEGER PRIMARY KEY, Name VARCHAR, Year INTEGER);");  // исполнение запроса на добавление записи
        query->clear();
        query->exec("INSERT INTO Design (ID,Name,Style,Price) VALUES (1,picture,classic,15000);");
        query->clear();
        query->exec("INSERT INTO Design (ID,Name,Style,Price) VALUES (2,vasa,borocco,4000);");
        query->clear();
        query->exec("INSERT INTO Design (ID,Name,Style,Price) VALUES (3,picture,borocco,1000);");
        query->clear();
        query->exec("INSERT INTO Design (ID,Name,Style,Price) VALUES (4,bubs,classic,40000);");
    }
    model = new QSqlTableModel(this,m_db); // создание
                              // редактируемой модели базы данных
    model->setTable("Person"); // создание модели таблицы  Person

    model->select(); // заполнение модели данными
    model->setEditStrategy(QSqlTableModel::OnFieldChange); // выбор стратегии
                    //  сохранения изменений в базе данных
                    //- сохранение происходит при переходе к другому полю
    ui->tableView->setModel(model); // соединение модели
                    // и ее табличного представления в форме
}

MainWindow::~MainWindow()
{
    delete ui;
    delete query;
    delete model;
}
namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private slots:
    void on_pushButton_ADD_clicked();

private:
    Ui::MainWindow *ui;
    QSqlDatabase m_db; // объект базы данных
       QSqlQuery* query; // указатель на запрос
       QSqlTableModel* model; // указатель на таблицу данных в приложении

};

#endif // MAINWINDOW_H

void MainWindow::on_pushButton_ADD_clicked()
{
    if(ui->lineEdit_N->text().isEmpty()||ui->lineEdit_S->text().isEmpty()||ui->lineEdit_P->text().isEmpty()||ui->lineEdit_ID->text().isEmpty())
            return;

        QString id = ui->lineEdit_ID->text();
        QString name = ui->lineEdit_N->text();
        QString style = ui->lineEdit_S->text();
        QString price = ui->lineEdit_P->text();

        QString buf = tr("INSERT INTO Person (ID,Name,Style,Price) VALUES (")+id+tr(",'")+name+tr("',")+style+tr("',")+price+tr(");");
        query->clear();
        query->exec(buf);
        model->select();

}
