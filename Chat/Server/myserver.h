#ifndef MYSERVER_H
#define MYSERVER_H
#include <QObject>
#include <QTcpServer>
#include <QTcpSocket>
#include <QDebug>
#include <QByteArray>
#include <QDataStream>
#include <QTime>

class MyServer:public QTcpServer
{
    Q_OBJECT
private:
    QTcpServer *server; // указатель на сервер
    quint16  m_nNextBlockSize;
    QMap<int,QTcpSocket *> SClients;
    int connectSize;
    void sendToClient(QTcpSocket *pSocket, const QString &str);
    int tempDes;
public:
    MyServer(quint16 port); // Конструктор

public slots:
        void slotNewConnection();
        void slotReadClient();
        void slotDeleteClient();
};
#endif // MYSERVER_H
