#ifndef MYFRAME_H
#define MYFRAME_H

#include <QFrame>
#include <QPushButton>
#include <QTextEdit>
#include <QLabel>
#include <QGridLayout>
#include <QHBoxLayout>
#include <QStringList>
#include <QLineEdit>
#include <QIntValidator>
#include <QTcpSocket>
#include <QByteArray>
#include <QDateTime>
#include <QDebug>
#include <QHostAddress>
#include <QNetworkInterface>

class MyFrame : public QFrame
{
    Q_OBJECT
private:
    //UI
    QTextEdit* textEditHistory;
    QLineEdit* lineEditMessage;
    QPushButton* butConnect;
    QPushButton* butSend;
    QLabel* labelName;
    QLineEdit* lineEditName;
    QLabel* labelPort;
    QLabel* labelIp;
    QString strHistory;
    QGridLayout* layoutGrid;
    QHBoxLayout* layoutH;
    QLineEdit* lineIp;
    QLineEdit* linePort;
    QIntValidator* validatorInt;

    //socket
    QByteArray byteArray;
    QTcpSocket* mySocket;
    quint16 port;
    quint16 m_nNextBlockSize;
    QString name;
    bool firstConnect;
public:
    MyFrame(QWidget* parent);
private slots:
    void slotReadyRead();
    void slotSendMessage();
    void slotError(QAbstractSocket::SocketError);
    void slotConnected();
};

#endif // MYFRAME_H
