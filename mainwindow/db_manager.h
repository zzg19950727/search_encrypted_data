#ifndef DB_MANAGER_H
#define DB_MANAGER_H
#include <QString>
#include <QStringList>
#include <vector>
#include <string>
#include <map>
using namespace::std;

#define table_plate "create table key_word%1(key_hash char(80), row_id int, pos int,\
                    primary key(key_hash, row_id, pos),\
                    foreign key(row_id) references text(id));"

QString get_free_table();

void init_table_state();

void get_all_index_table(QStringList& table_list);

void create_new_table();

void update_table_state();

void change_table_state(long state);

#endif // DB_MANAGER_H
