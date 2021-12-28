#include "qsocketthread.h"

QSocketThread::QSocketThread(int descriptor, QObject *parent) :
    QThread(parent), socketDescriptor(descriptor)
{

}

QSocketThread::~QSocketThread() {
    delete socket;
}

void QSocketThread::run()
{
    socket = new QTcpSocket();
    socket->setSocketDescriptor(socketDescriptor);

    connect(socket, SIGNAL(readyRead()), this, SLOT(onReadyRead()), Qt::DirectConnection);          //так как подключение сигнала со слотом происходит внутри
    connect(socket, SIGNAL(disconnected()), this, SLOT(onDisconnected()), Qt::DirectConnection);    //потока, необходимо передать Qt::DirectConnection

    exec();    //запускается цикл обработки событий
}

void QSocketThread::onReadyRead()
{
    qDebug() << socket->readAll();                                           //в qdebug выводим то, что нам прислали

    QString response = "HTTP/1.1 200 OK\r\n\r\n%1";                          //написание ответа согласно протоколу HTTP; в %1 пишется заполнение контента на следующей строке
    socket->write(response.arg(QTime::currentTime().toString()).toUtf8());   //с помощью response.arg() вместо %1 можно добавить какую-то строку

    socket->disconnectFromHost();
}

void QSocketThread::onDisconnected()
{
    socket->close();   //закрывается сокет
    quit();            //выходим из цикла обработки событий
}


