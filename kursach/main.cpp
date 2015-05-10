#include <QtGui/QApplication>
#include "client.h"
#include <testserver.h>
int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
  //  TestServer server(2323);
   // server.show();
  //m_pTcpSocket->connectToHost("dc.h4lt.net", 411);
    Client *w;
    w = new Client("4local.ru",411);
    w->show();
    
    return a.exec();
}
//&#36Key 123&#124&#36ValidateNick lol&#124

//F1qaPNq

//$Lock EXTENDEDPROTOCOL_verlihub Pk=version0.9.8e-r2|
//<VerliHub> This hub is running version 0.9.8e-r2 (Monday Jul 20 2009) of VerliHub (RunTime: 8weeks 2days  / Current user count: 12)|
