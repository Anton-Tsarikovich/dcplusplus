#include "workwithstring.h"

WorkWithString::WorkWithString()
{
}
QString WorkWithString::findCommand(QString &commandString)
{
    int count = 0;
    QString temp;
    for(int i = 0; i < commandString.size(); i++)
    {
        if(commandString[i] == '$')
        {
            i++;
            while(commandString[i] != ' ')
            {
                temp[count] = commandString[i];
                i++;
                count++;
            }
            qDebug()<<temp;

            return (detectCommand(temp,commandString,i));

        }
    }
}

QString WorkWithString::detectCommand(QString &command, QString &commandString, int position)
{
    int count = 0;
    if(command == "Lock")
    {
        command.clear();
        for(int i = position+1; commandString[i] != ' '; i++)
        {
            command[count] = commandString[i];
            count++;
            if(count == 16)
            {
                if(command == "EXTENDEDPROTOCOL")
                {

                    command[count+1] = commandString[i+1];
                    if(command[count+1] == '_')
                        i++;
                    count = 0;
                    command.clear();
                }
            }
        }
        return(validateMessage(command));

    }
}
QString WorkWithString::validateMessage(QString &message)
{
    //lock
    int len = message.size();
      char *key = new char[len+1];
      char *lock = new char[len];
      lock = message.toAscii().data();
      int i;

      for(i = 1; i < len; ++i)
        key[i] = lock[i] ^ lock[i-1];
      key[0] = lock[0] ^ lock[len-1] ^ lock[len-2] ^ 5;

      for(i = 0; i < len; ++i)
        key[i] = ((key[i]<<4) & 0xF0) | ((key[i]>>4) & 0x0F);

      char *newkey = new char[len+100];
      char *newkey_p = newkey;
      for(i = 0; i < len; ++i)
      {
        switch(key[i])
        {
          case 0:
          case 5:
          case 36:
          case 96:
          case 124:
          case 126:
            sprintf(newkey_p, "/%%DCN%03d%%/", key[i]);
            newkey_p += 10;
            break;
          default:
            *newkey_p = key[i];
            ++newkey_p;
        }
      }
      *newkey_p = '\0';
      message.clear();
      message = newkey;
      return message;
}
