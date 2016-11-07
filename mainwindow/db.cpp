#include "sm4EncDec.h"
#include "MyFriso.h"
#include "StrHex.h"
#include "sm3.h"
#include "db.h"

#include <QSqlQuery>
#include <QSqlError>
#include <QDebug>
#include <QFile>
#include <QTime>

bool store_text(const char *path, SID sid)
{
    //QString table = get_free_table();

    map<string,int> word_map;
    int num = 0;

    QFile fp(path);
    if( !fp.open(QIODevice::ReadOnly | QIODevice::Text) )
        return false;

    QByteArray array = fp.readAll();
    char* text = array.data();
    int len = array.length();

    int row_id = send_text(text,len, sid);
    if(!isSEND(row_id))
        return false;

    unsigned char* key = get_hash_key_by_sid(sid);
    if( !key )
        return false;

    int pos = 1;
    KEY_INFO info;
    info.row_id = row_id;
    vector<KEY_INFO> info_list;

    set_text(text);
    friso_token_t token = next_token();
    while(token)
    {
        map<string, int>::iterator iter = word_map.find(token->word);
        if( iter != word_map.end() )
        {
                token = next_token();
                continue;
        }
        else
            word_map[token->word] = num++;

        {
            unsigned char out[32];
            sm3_hmac(key, 16, (unsigned char*)token->word, token->length, out);
            char* tmp = hex2str(out, 32);

            info.key_hash = QString(tmp);
            //info.key_id = key_id;
            info.pos = pos++;
            info_list.push_back(info);
            free(tmp);
        }
        if( isFULL(info_list) )
        {
            send_key_info(info_list);
            info_list.clear();
        }
        token = next_token();
    }
    send_key_info(info_list);
    info_list.clear();
    fp.close();
    return true;
}

int send_text(const char* text, int len, SID sid)
{
    if( !text || len==0 )
        return -1;

    unsigned char* key = get_EN_key_by_sid(sid);
    if( !key )
        return -1;

    KID kid = get_kid_by_sid(sid);

    int id = -1;
    QSqlQuery query;

    int outLen = 0;
    unsigned char* encrypt_text = sm4_encrypt((unsigned char*)text, len, &outLen, key);
    char* data = hex2str(encrypt_text, outLen);
    QString str = data;

    query.exec(QString("insert into text(str,kid) values('%1',%2);").arg(str).arg(kid));
    query.exec(QString("select id from text where str='%1';").arg(str));
    while(query.next())
    {
          id = query.value(0).toInt();
    }

    free(encrypt_text);
    free(data);
    qDebug()<<query.lastError();
    return id;
}

int send_key_word(const char* key, int len)
{
    if( !key || len==0 )
        return -1;

    int id = -1;
    QSqlQuery query;

    unsigned char out[32];
    sm3((unsigned char*)key, len, out);
    char* tmp = hex2str(out, 32);
    QString key_word = tmp;

    query.exec(QString("select find_key('%1');").arg(key_word));
    while(query.next())
    {
          id = query.value(0).toInt();
    }

    free(tmp);
    return id;
}

void send_key_info(vector<KEY_INFO>& info_list)
{
    if(info_list.size() < 1)
        return;
    QSqlQuery query;
    QString sql = "insert into key_word values";
    unsigned int i=0;
    for(; i<info_list.size()-1; i++)
    {
        sql += QString("('%1',%2,%3),")
                //.arg(info_list[i].key_id)
                .arg(info_list[i].key_hash)
                .arg(info_list[i].row_id)
                .arg(info_list[i].pos);
    }
    sql += QString("('%1',%2,%3)")
            //.arg(info_list[i].key_id)
            .arg(info_list[i].key_hash)
            .arg(info_list[i].row_id)
            .arg(info_list[i].pos);
    query.exec(sql);
}

QString create_SQL(vector<KEY> &key_list)
{
    unsigned int size = key_list.size();
    if(size < 1)
        return QString("");

    QString sql = QString(plate1).arg("'"+key_list[0].key+"'");
    for(unsigned int i=1; i<size; i++)
    {
        if(key_list[i].type == -1)
            sql += QString(plate3).arg(i+1).arg("'"+key_list[i].key+"'");
        else
            sql += QString(plate3).arg(i+1).arg("'"+key_list[i].key+"'");
    }
    for(unsigned int i=1; i<size; i++)
    {
        sql += ")";
    }
    sql += ");";
    return sql;
}

void search_text_by_SQL(vector<KEY> &key_list, QStringList &text_list)
{
    if(key_list.empty())
        return;
    //qDebug()<<"key:"<<key_list.size();
    QString sql = create_SQL(key_list);
    //qDebug()<<sql;
    QSqlQuery query;
    query.exec(sql);

    while(query.next())
    {
            QString str = query.value(0).toString();
            text_list.push_back(str);
    }
}

int get_key_type(QList<int>& pos_list, int offset, int& ptr)
{
    if( ptr >= pos_list.size() )
        return -1;

    if(offset/3 < pos_list[ptr])
        return 1;

    ptr++;
    return -1;
}

void search_text(QString key_word, QString sid, QStringList &text_list)
{
    text_list.clear();
    QTime t1 = QTime::currentTime();

    QStringList sid_list;
    get_lower_sid(sid, sid_list);
    for(int i=0; i<sid_list.size(); i++)
        search_one_level_text(key_word, sid_list[i], text_list);

    QTime t2 = QTime::currentTime();
    qDebug()<<"search:"<<t1.msecsTo(t2)<<"ms";
}

void search_one_level_text(QString key_word, QString sid, QStringList& text_list)
{
    unsigned char* key = get_hash_key_by_sid(sid);
    if( !key )
        return;

    QList<int> pos_list;
    int pos = 0;
    while( pos != -1 ){
        pos_list.push_back(pos);
        pos = key_word.indexOf(QRegExp(BREAKS),pos+1);}

    string str = key_word.toStdString();
    char* data = new char[str.length()+1];
    strcpy(data, str.c_str());data[str.length()]='\0';
    if( !data )
        return;
    set_text(data);

    int ptr = 0;
    vector<KEY>key_list;
    friso_token_t token = next_token();
    while(token)
    {
        int len = token->length;
        char* word = token->word;
        unsigned char out[32];
        sm3_hmac(key, 16, (unsigned char*)word, len, out);
        char* tmp = hex2str(out, 32);

        KEY key;
        key.key = QString(tmp);
        key.type = get_key_type(pos_list, token->offset, ptr);

        key_list.push_back(key);

        free(tmp);
        token = next_token();
    }

    QStringList encrypt_text_list;

    search_text_by_SQL(key_list, encrypt_text_list);

    unsigned char* EN_key = get_EN_key_by_sid(sid);
    if( !EN_key )
        return;

    for(int i=0; i<encrypt_text_list.size(); i++)
    {
        char* in = (char*)encrypt_text_list[i].toStdString().c_str();
        int inLen = encrypt_text_list[i].length();
        unsigned char* tm = str2hex(in, inLen);

        int outLen;
        char* dat = (char*)sm4_decrypt(tm, inLen/2, &outLen, EN_key);
        if( !dat )
            continue;
        dat[outLen] = 0;
        QString text = dat;
        text_list.push_back(text);

        free(tm);
        free(dat);
    }
    free(data);
}
