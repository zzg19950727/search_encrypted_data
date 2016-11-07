#ifndef KEYMANAGER_H
#define KEYMANAGER_H
#include <QStringList>
#include <QString>

#define KLEN            16
#define OK              -1
#define ID_NOT_EXISTS   0
#define ID_EXISTS       1
#define ADD_SID_ERR     2

typedef QString SID;

typedef int KID;

void init_SID();

int add_SID(SID pid, SID id);

unsigned char* get_EN_key_by_sid(SID id);

unsigned char* get_EN_key_by_kid(KID id);

unsigned char* get_hash_key_by_sid(SID id);

unsigned char* get_hash_key_by_kid(KID id);

void get_all_SID(QStringList& SID_list);

void get_lower_sid(SID sid, QStringList& SID_list);


bool have_error();

bool sid_exists(SID id);

QString random_key();

KID get_kid_by_sid(SID id);

KID get_next_kid(KID id);

bool kid_exists(KID id);


#endif // KEYMANAGER_H
