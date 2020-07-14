#include "myframe.h"


MyFrame::MyFrame(QWidget* parent)
    :QFrame(parent),firstConnect(false)
{
    textEditHistory=new QTextEdit(this);
    lineEditMessage=new QLineEdit(this);
    butConnect=new QPushButton("Подключиться",this);
    butSend=new QPushButton("Отправить",this);
    labelIp=new QLabel("Ip-адрес",this);
    labelPort=new QLabel("Номер порта",this);
    lineIp=new QLineEdit(this);
    linePort=new QLineEdit(this);
    labelName=new QLabel("Имя",this);
    lineEditName=new QLineEdit(this);
    butSend->setEnabled(false);
    textEditHistory->setReadOnly(true);

    butConnect->setSizePolicy(QSizePolicy::Minimum,QSizePolicy::Minimum);
    butSend->setSizePolicy(QSizePolicy::Minimum,QSizePolicy::Minimum);
    QString ipRange = "(?:[0-1]?[0-9]?[0-9]|2[0-4][0-9]|25[0-5])";


    QRegExp ipRegex ("^" + ipRange
                        + "\\." + ipRange
                        + "\\." + ipRange
                        + "\\." + ipRange + "$");
    QRegExpValidator *ipValidator = new QRegExpValidator(ipRegex, this);
    lineIp->setValidator(ipValidator);

    validatorInt=new QIntValidator(this);
    linePort->setValidator(validatorInt);


    //layout
    layoutGrid=new QGridLayout(this);
    layoutGrid->addWidget(labelIp,0,0);
    layoutGrid->addWidget(lineIp,0,1);
    layoutGrid->addWidget(labelPort,0,2);
    layoutGrid->addWidget(linePort,0,3);
    layoutGrid->addWidget(labelName,0,4);
    layoutGrid->addWidget(lineEditName,0,5);
    layoutGrid->addWidget(butConnect,0,6,5,1);
    layoutGrid->addWidget(lineEditMessage,2,0,1,6);
    layoutGrid->addWidget(textEditHistory,1,0,1,6);
    layoutGrid->addWidget(butSend,3,0,2,6);



    connect(butConnect,SIGNAL(clicked()),this,SLOT(slotConnected()));





    connect(butSend,SIGNAL(clicked()),this,SLOT(slotSendMessage()));


}

void MyFrame::slotSendMessage() // отправка сообщения
{


    QByteArray  arrBlock; // Выделяем Байтовый Массив
    QDataStream out(&arrBlock, QIODevice::WriteOnly);
    out << quint16(0) << QTime::currentTime() << lineEditMessage->text() << lineEditName->text();
    out.device()->seek(0);//Возвращает устройство ввода-вывода, установленное в данный момент,
    out << quint16(arrBlock.size() - sizeof(quint16));// записываем в поток

    mySocket->write(arrBlock);// Это перегруженная функция.
    //Записывает содержимое arrBlock на устройство. Возвращает количество байтов, которые были фактически записаны, или -1, если произошла ошибка.
    lineEditMessage->setText("");// чистим lineEdit
}

void MyFrame::slotError(QAbstractSocket::SocketError error)
{
    QString strError =
            "Error: " + (error == QAbstractSocket::HostNotFoundError ?
                             "Адрес узла не найден" :
                             error == QAbstractSocket::RemoteHostClosedError ?
                                 "Удалённый узел закрыл соединение" :
                                 error == QAbstractSocket::ConnectionRefusedError ?
                                     "Невозможно связаться с прокси-сервером, поскольку соединение к этому серверу было запрещено" :
                                     QString(mySocket->errorString())
                                     );
    strHistory.append(strError+"\n");
    textEditHistory->append(strError);
}
void MyFrame::slotReadyRead() // читаем что пришло от сервера
{
    if(firstConnect==false){
        if(mySocket->state()==QAbstractSocket::ConnectedState){
            strHistory.push_back("Подключился "+QDateTime::currentDateTime().toString()+"\n");
            textEditHistory->append("Подключился "+QDateTime::currentDateTime().toString());
            lineIp->setEnabled(false);
            linePort->setEnabled(false);
            lineEditName->setEnabled(false);
            butSend->setEnabled(true);
            firstConnect=true;
        }
    }

    QDataStream in(mySocket);// Создаем поток и помещяем сокет
        in.setVersion(QDataStream::Qt_5_3);//Устанавливает номер версии формата сериализации данных v, значение перечисления Version.

        for (;;) { // делаем бесконеный цикл
            if (!m_nNextBlockSize) { // если размер блока пустой то заходим
                if (mySocket->bytesAvailable() < sizeof(quint16)) {// Возвращаем количество расшифрованных байтов, которые сразу доступны для чтения и сравниваем с 16 бит
                    break;    // Если меньше выходим из цикла
                }
                in >> m_nNextBlockSize; /// если больше или равно попадаем сюда и Читаем 16-разрядное целое число со знаком из потока в in и возвращает ссылку на поток.
            }

            if (mySocket->bytesAvailable() < m_nNextBlockSize) {// Возвращаем количество расшифрованных байтов, которые сразу доступны для чтения и сравниваем с размером
                break; // если размер сокета меньше этого размера, выходим.
            }

            QTime   time; // Cоздаем объект времени
            QString str; // строка
            in >> time >> str;// записываем в поток время и строку
            textEditHistory->setTextColor(Qt::red);
            textEditHistory->append(str);
            m_nNextBlockSize = 0; // Обнуляем размер
        }
}

void MyFrame::slotConnected() // Слот коннекта
{
    mySocket=new QTcpSocket(this);
    port=quint16(linePort->text().toUInt());
    mySocket->connectToHost(lineIp->text(),port);

    ///////
    /// connect
    ///
    ///
    ///////
    ///
    ///
    ///////
    const QHostAddress &localhost = QHostAddress(QHostAddress::LocalHost);
    for (const QHostAddress &address: QNetworkInterface::allAddresses()) {
        if (address.protocol() == QAbstractSocket::IPv4Protocol && address != localhost)
               textEditHistory->append("my line ---> " + localhost.toString());
    }


    connect(mySocket,SIGNAL(connected()),this,SLOT(slotReadyRead()));
    connect(mySocket, SIGNAL(readyRead()), SLOT(slotReadyRead()));
    connect(mySocket, SIGNAL(error(QAbstractSocket::SocketError)),
            this,SLOT(slotError(QAbstractSocket::SocketError)));



}


