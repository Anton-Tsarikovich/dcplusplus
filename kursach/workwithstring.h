#ifndef WORKWITHSTRING_H
#define WORKWITHSTRING_H
#include <QString>
#include <QDebug>
class WorkWithString
{

public:
    WorkWithString();
    static QString findCommand(QString &commandString);
    static QString detectCommand(QString &command, QString &commandString, int position);
    static QString validateMessage(QString &message);
    static QString lockToKey(QString lock);
    static char * dcmakekey(const char *lock);
    static int  reservedchar(unsigned char c);
    static QByteArray decodeList(QByteArray inputData);
    static QByteArray decodeBZList(QByteArray inputData);
    static quint32 get_bit(QByteArray data, quint32* cur_pos);
    static quint32 get_bits(QByteArray data, quint32* cur_pos, qint32 nb_bit);
};

#endif // WORKWITHSTRING_H
