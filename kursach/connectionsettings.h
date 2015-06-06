#ifndef CONNECTIONSETTINGS_H
#define CONNECTIONSETTINGS_H

#include <QWidget>
#include <QFile>
#include <QTextStream>
#include <QDebug>

namespace Ui {
class ConnectionSettings;
}

class ConnectionSettings : public QWidget
{
    Q_OBJECT
    
public:
    explicit ConnectionSettings(QWidget *parent = 0);
    ~ConnectionSettings();
    
private:
    Ui::ConnectionSettings *ui;
    QFile *connectionFile;
    QTextStream *textStream;

private slots:
    void slotCancel();
    void slotSave();

};

#endif // CONNECTIONSETTINGS_H
