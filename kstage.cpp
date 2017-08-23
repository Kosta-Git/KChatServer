#include "kstage.h"
#include "ui_kstage.h"

KStage::KStage(QWidget *parent) :
    QWidget(parent)
{

    QVBoxLayout *vBox = new QVBoxLayout;

    m_status = new QLabel;

    m_disconnect = new QPushButton(tr("Drop the server"));
    connect(m_disconnect, SIGNAL(clicked(bool)), qApp, SLOT(quit()) );

    vBox->addWidget(m_status);
    vBox->addWidget(m_disconnect);

    setLayout(vBox);
    setWindowTitle("KChat Server Application");

    m_server = new QTcpServer(this);

    if(!m_server->listen(QHostAddress::AnyIPv4, 42666) )
        m_status->setText("Error while trying to listen. \nError log:" + m_server->errorString() );
    else{

        m_status->setText("Server listening on port:\t<b>"
                          + QString::number(m_server->serverPort())
                          + "</b><br>"
                          + "You can connect to the server via IPV4 or localhost");

        connect(m_server, SIGNAL(newConnection()), this, SLOT(newCo()) );

    }

    m_size = 0;

}

void KStage::newCo()
{

    QTcpSocket *newClient = m_server->nextPendingConnection();

    send( QString( QString("<i>New user connected</i>\t<b>") + QString("</b>") ));

    m_clients << newClient;

    connect(newClient, SIGNAL(readyRead()), this, SLOT(receivedData()) );

    connect(newClient, SIGNAL(disconnected()), this, SLOT(clientDown()) );

}

void KStage::receivedData()
{

    QTcpSocket *socket = qobject_cast<QTcpSocket*>(sender() );

    if(!socket)
        return;

    QDataStream data(socket);

    if(!m_size){

        if(socket->bytesAvailable() < (int)sizeof(quint16) )
            return;

        data >> m_size;

    }

    if(socket->bytesAvailable() < m_size )
        return;

    QString message;

    data >> message;

    send(message);

    m_size = 0;

}

void KStage::clientDown()
{

    QTcpSocket *socket = qobject_cast<QTcpSocket*>(sender());

    send(QString(QString("<i>Client dropped</i>\t<b>") + QString("</b>")));

    if(!socket)
        return;

    m_clients.removeOne(socket);

    socket->deleteLater();

}

void KStage::send(const QString message)
{

    QByteArray packet;

    QDataStream data(&packet, QIODevice::WriteOnly);

    data << (quint16) 0;

    data << message;

    data.device()->seek(0);

    data << (quint16) (packet.size() - sizeof(quint16));

    for(int x = 0; x < m_clients.size(); ++x){

        m_clients[x]->write(packet);

    }

}

KStage::~KStage()
{

    delete m_status;
    delete m_disconnect;
    delete m_server;

}
