#ifndef KSTAGE_H
#define KSTAGE_H

#include <QWidget>
#include <QtNetwork>
#include <QString>
#include <QLabel>
#include <QVBoxLayout>
#include <QPushButton>

namespace Ui {
class KStage;
}

class KStage : public QWidget
{
    Q_OBJECT

private:
    QLabel *m_status;
    QPushButton *m_disconnect;
    QTcpServer *m_server;
    QList<QTcpSocket*> m_clients;
    quint16 m_size;

public slots:
    void newCo();
    void receivedData();
    void clientDown();

public:
    explicit KStage(QWidget *parent = 0);
    void send(const QString);
    ~KStage();

};

#endif // KSTAGE_H
