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
};

#endif // WORKWITHSTRING_H
