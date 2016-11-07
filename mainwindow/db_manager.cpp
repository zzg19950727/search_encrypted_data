#include "db_manager.h"
#include <QSqlQuery>
#include <QSqlError>
#include <QVariant>

vector<long> table_state;
int cur_table_id;
int select_table;

QString get_free_table()
{
    int id = 1;
    long mini = table_state[0];
    for(int i=1; i<table_state.size(); i++)
    {
        if( table_state[i] < mini )
        {
            id = i;
            mini = table_state[i];
        }
    }
    select_table = id;
    return QString("key_word%1").arg(id);
}

void init_table_state()
{
    cur_table_id = 1;
    select_table = 1;

    QStringList table_list;
    get_all_index_table(table_list);

    for(int i=0; i<=table_list.size(); i++)
        table_state.push_back(0);
}

void get_all_index_table(QStringList& table_list)
{
    QSqlQuery query;
    QString sql = QString("show tables");
    query.exec(sql);
    while(query.next())
    {
        QString table = query.value(0).toString();
        if( table.contains("key_word") )
            table_list.push_back(table);
    }
}

void create_new_table()
{
    QStringList table_list;
    get_all_index_table(table_list);

    int table_id = table_list.size()+1;

    QSqlQuery query;
    QString sql = QString(table_plate).arg(table_id);
    query.exec(sql);
    table_state.push_back(0);
}

void update_table_state()
{
    QSqlQuery query;
    QString sql = QString("select count(*) from key_word%1").arg(cur_table_id);
    query.exec(sql);
    while(query.next())
    {
        long state = query.value(0).toLongLong();
        if( state > 5000000 )
            create_new_table();
        else if( state > 0 )
            table_state[cur_table_id] = state;
    }
    cur_table_id++;

    if( cur_table_id > table_state.size() )
        cur_table_id = 1;
}

void change_table_state(long state)
{
    table_state[select_table] += state;
}
