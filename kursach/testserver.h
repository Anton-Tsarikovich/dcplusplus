#ifndef TESTSERVER_H
#define TESTSERVER_H

#include <QWidget>
#include <QTime>
#include <QTcpServer>
#include <QHostAddress>
#include <QMessageBox>
#include <QTextEdit>
#include <QLabel>
#include <QVBoxLayout>
#include <QTcpSocket>

class QTcpServer;
class QTextEdit;
class QTcpSocket;

class TestServer : public QWidget {
Q_OBJECT
private:
    QTcpServer* m_ptcpServer;
    QTextEdit*  m_ptxt;
    quint16     m_nNextBlockSize;

private:
    void sendToClient(QTcpSocket* pSocket, const QString& str);

public:
    TestServer(int nPort, QWidget* pwgt = 0);

public slots:
    virtual void slotNewConnection();
            void slotReadClient   ();
};
#endif // TESTSERVER_H
