#include "myserver.h"

MyServer::MyServer(quint16 port):connectSize(0)
{

    server = new QTcpServer(this);
    if (!server->listen(QHostAddress::Any, port))
    {
        qDebug()<<"Ошибка работы";
        return;
    }
    qDebug()<<"Сервер готов к работе!";
    connect(server, SIGNAL(newConnection()),//Этот сигнал испускается каждый раз, когда доступно новое соединение.
            this,SLOT(slotNewConnection()));
}


void MyServer::slotNewConnection()
{
    QTcpSocket* pClientSocket = server->nextPendingConnection(); // 2 // Сокет создается как дочерний элемент сервера .И удаляетсься вместе серевером .

    int des=pClientSocket->socketDescriptor();
    SClients[des]=pClientSocket;
    ++connectSize;

    qDebug()<<des;
    tempDes=des;
    connect(SClients[des], SIGNAL(readyRead()),  // Если прочитал в консоле
            this,SLOT(slotReadClient())
            );


    connect(SClients[des], SIGNAL(disconnected()),
            this, SLOT(slotDeleteClient())  // если не произошол разрыв соединения то удаляем сокет
            );
    sendToClient(SClients[des], "Вы в чате !!"); // Отправляем письмо клиенту 4
}
void MyServer::sendToClient(QTcpSocket *pSocket, const QString &str) // 4п
{
    QByteArray arrBlock;
    QDataStream out(&arrBlock,QIODevice::WriteOnly);
    out << quint16(0) << QTime::currentTime() << str;
    out.device()->seek(0);
    out <<quint16(arrBlock.size()-sizeof(quint16));
    pSocket->write(arrBlock);
}
void MyServer::slotReadClient()
{
    QTcpSocket* pClientSocket = (QTcpSocket*)sender();
    QDataStream in(pClientSocket);
    for (;;) {
        if (!m_nNextBlockSize) {
            if (pClientSocket->bytesAvailable() < sizeof(quint16)) {
                break;
            }
            in >> m_nNextBlockSize;
        }

        if (pClientSocket->bytesAvailable() < m_nNextBlockSize) {
            break;
        }
        QTime   time;
        QString str;
        QString name;
        in >> time >> str >> name;

        QString strMessage =
                time.toString() + " " + "Client has sended - " + str + " " + "( " + name + " )";
        qDebug()<<strMessage;

        m_nNextBlockSize = 0;

        foreach (QTcpSocket* sock, SClients){
            sendToClient(sock,
                         str+ " " +"( " + name + " )"
                         );
        }
    }
}

void MyServer::slotDeleteClient()
{
    QMap<int, QTcpSocket*>::iterator it;
    for (it = SClients.begin(); it != SClients.end(); ++it){
        if(it.key()==tempDes){
            SClients.erase(it);
        }
    }
    qDebug()<<tempDes<<" "<<"delete";
    SClients.remove(tempDes);
    foreach (QTcpSocket* sock, SClients){
        sendToClient(sock,"Disconnect client");
    }
    --connectSize;

}

