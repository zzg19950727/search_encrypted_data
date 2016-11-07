#ifndef DB_H
#define DB_H
#include "keymanager.h"
#include "db_manager.h"
#include <QStringList>
#include <QString>
#include <vector>
#include <map>
#include <string>
using namespace::std;

#define late1 "select text.str from text where text.id in(\n\
        select i1.row_id from key_info i1, key_word w1 \n\
                    where i1.id = w1.id and w1.key_hash = %1"

#define late2 "    \n and i%1.pos +1= any(\n\
        select i%2.pos from key_info i%2, key_word w%2 \n\
                where i%2.id = w%2.id and w%2.key_hash = %3 \n\
                and i1.row_id = i%2.row_id"

#define late3 "    \n and exists(\n\
                select i%2.row_id from key_info i%2, key_word w%2 \n\
                        where i%2.id = w%2.id and w%2.key_hash = %3 \n\
                        and i1.row_id = i%2.row_id"

        #define plate1 "select text.str from text where text.id in(\n\
                select w1.row_id from key_word w1 \n\
                            where w1.key_hash = %1"

        #define plate2 "    \n and w%1.pos +1= any(\n\
                select w%2.pos from key_word w%2 \n\
                        where w%2.key_hash = %3 \n\
                        and w1.row_id = w%2.row_id"

        #define plate3 "    \n and exists(\n\
                        select w%2.row_id from key_word w%2 \n\
                                where w%2.key_hash = %3 \n\
                                and w1.row_id = w%2.row_id"
#define BREAKS "[, .;.，。 ]"
#define isSEND(id) (id>=0)

#define MAX_SEND_NUM 10000

#define isFULL(list) (list.size()>=MAX_SEND_NUM)


struct key_info
{
    QString key_hash;
    //int key_id;
    int row_id;
    int pos;
};
typedef struct key_info KEY_INFO;

struct Key
{
    QString key;
    int type;
};
typedef struct Key KEY;

bool store_text(const char* path, SID sid);

void search_text(QString key_word, QString sid, QStringList& text_list);

int send_text(const char* text, int len, SID sid);

int send_key_word(const char* key, int len);

void send_key_info(vector<KEY_INFO>& info_list);

void search_text_by_SQL(vector<KEY>& key_list, QStringList& text_list);

void search_one_level_text(QString key_word, QString sid, QStringList& text_list);

QString create_SQL(vector<KEY>& key_list);

#endif // DB_H
