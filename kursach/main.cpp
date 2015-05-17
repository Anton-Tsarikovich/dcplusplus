#include <QtGui/QApplication>
#include "client.h"
#include <testserver.h>
int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    Client *w;
    w = new Client("dc.h4lt.net", 411);
    w->show();
    //m_pTcpSocket->connectToHost("dc.h4lt.net", 411);

   // dchub://hub.tankafett.biz:8888
    return a.exec();
}
