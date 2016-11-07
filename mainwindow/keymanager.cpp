#include "keymanager.h"
#include "StrHex.h"
#include <QSqlQuery>
#include <QSqlError>
#include <QVariant>
#include <QList>
#include <time.h>

void init_SID()
{
    QStringList SID_list;
    get_all_SID(SID_list);
    if( !SID_list.empty() )
        return;

    QSqlQuery query;
    QString sql = QString("insert into SID values('%1',%2)").arg("highest").arg(1);
    query.exec(sql);

    sql = QString("insert into SID values('%1',%2)").arg("lowest").arg(100);
    query.exec(sql);

    QString EN_key = random_key();
    QString hash_key = random_key();
    sql = QString("insert into MY_KEY values(%1,'%2','%3')").arg(1).arg(EN_key).arg(hash_key);
    query.exec(sql);

    srand( time(NULL) );
    EN_key = random_key();
    hash_key = random_key();
    sql = QString("insert into MY_KEY values(%1,'%2','%3')").arg(100).arg(EN_key).arg(hash_key);
    query.exec(sql);
}

int add_SID(SID pid, SID id)
{
    if( !sid_exists(pid) )
        return ID_NOT_EXISTS;

    if( sid_exists(id) )
        return ID_EXISTS;

    KID p_kid = get_kid_by_sid(pid);
    KID n_kid = get_next_kid(p_kid);
    if( n_kid == ID_NOT_EXISTS )
        n_kid = 2*p_kid;

    int c_kid = (p_kid + n_kid)/2;
    if( kid_exists(c_kid) )
        return ADD_SID_ERR;

    QSqlQuery query;
    QString sql = QString("insert into SID values('%1',%2)").arg(id).arg(c_kid);
    query.exec(sql);

    if( have_error() )
        return ADD_SID_ERR;

    QString EN_key = random_key();
    QString hash_key = random_key();
    sql = QString("insert into MY_KEY values(%1,'%2','%3')").arg(c_kid).arg(EN_key).arg(hash_key);
    query.exec(sql);

    if( have_error() )
    {
        sql = QString("delete from SID where sid='%1' and kid=%2").arg(id).arg(c_kid);
        query.exec(sql);
        return ADD_SID_ERR;
    }
    else
        return OK;
}

unsigned char* get_EN_key_by_sid(SID id)
{
    KID kid = get_kid_by_sid(id);
    if( kid == ID_NOT_EXISTS )
        return NULL;
    else
        return get_EN_key_by_kid(kid);
}

unsigned char* get_EN_key_by_kid(KID id)
{
    QSqlQuery query;
    QString sql = QString("select EN_key from MY_KEY where kid=%1").arg(id);
    query.exec(sql);
    while(query.next())
    {
        QString key = query.value(0).toString();
        return str2hex(key.toUtf8().data(), key.length());
    }
    return NULL;
}

unsigned char* get_hash_key_by_sid(SID id)
{
    KID kid = get_kid_by_sid(id);
    if( kid == ID_NOT_EXISTS )
        return NULL;
    else
        get_hash_key_by_kid(kid);
}

unsigned char* get_hash_key_by_kid(KID id)
{
    QSqlQuery query;
    QString sql = QString("select hash_key from MY_KEY where kid=%1").arg(id);
    query.exec(sql);
    while(query.next())
    {
        QString key = query.value(0).toString();
        return str2hex(key.toUtf8().data(), key.length());
    }
    return NULL;
}

void get_all_SID(QStringList& SID_list)
{
    QSqlQuery query;
    QString sql = QString("select sid from SID order by kid asc");
    query.exec(sql);
    while(query.next())
        SID_list.push_back(query.value(0).toString());
}

void get_lower_sid(SID sid, QStringList& SID_list)
{
    KID kid = get_kid_by_sid(sid);
    if( kid == ID_NOT_EXISTS )
        return;

    QSqlQuery query;
    QString sql = QString("select sid from SID where kid >= %1").arg(kid);
    query.exec(sql);
    while(query.next())
        SID_list.push_back(query.value(0).toString());
}

bool have_error()
{
    QSqlQuery query;
    QSqlError error = query.lastError();
    return error.isValid();
}

bool sid_exists(SID id)
{
    QSqlQuery query;
    QString sql = QString("select sid from SID where sid='%1'").arg(id);
    query.exec(sql);
    while(query.next())
        return true;
    return false;
}

QString random_key()
{
    unsigned char key[KLEN];
    for(int i=0; i<KLEN; i++)
    {
        key[i] = rand()%128;
    }

    char* tmp = hex2str((unsigned char*)key, KLEN);

    QString str = tmp;
    free(tmp);
    return str;
}

KID get_kid_by_sid(SID id)
{
    QSqlQuery query;
    QString sql = QString("select kid from SID where sid='%1'").arg(id);
    query.exec(sql);
    while(query.next())
        return query.value(0).toInt();

    return ID_NOT_EXISTS;
}

KID get_next_kid(KID id)
{
    QSqlQuery query;
    QString sql = QString("select kid from SID where kid>%1 order by kid asc").arg(id);
    query.exec(sql);
    while(query.next())
        return query.value(0).toInt();

    return ID_NOT_EXISTS;
}

bool kid_exists(KID id)
{
    QSqlQuery query;
    QString sql = QString("select kid from SID where kid=%1").arg(id);
    query.exec(sql);
    while(query.next())
        return true;
    return false;
}
