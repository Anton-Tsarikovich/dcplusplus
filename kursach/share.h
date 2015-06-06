#ifndef SHARE_H
#define SHARE_H

#include <QWidget>
#include <QDirModel>
#include <QString>

namespace Ui {
class Share;
}

class Share : public QWidget
{
    Q_OBJECT
    
public:
    explicit Share(QWidget *parent = 0);
    ~Share();
    
private:
    Ui::Share *ui;
    QDirModel* model;
private slots:
    void goShare();
};

#endif // SHARE_H
