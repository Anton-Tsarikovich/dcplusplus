#include "client.h"
#include <QApplication>
#include "readsettings.h"
int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    ReadSettings aa;
    aa.readFromFile();
    //Client *w;
    //w = new Client("dc.h4lt.net", 411);
    //w->show();
    //m_pTcpSocket->connectToHost("dc.h4lt.net", 411);

   // dchub://hub.tankafett.biz:8888
    //dchub://dc-hub.tk
    return a.exec();
}
