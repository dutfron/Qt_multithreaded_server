#include "qsimpleserver.h"

QSimpleServer::QSimpleServer(QObject *parent) : QTcpServer (parent)
{
   if(listen(QHostAddress::Any, 80)) {
       qDebug() << "Listening...";
   }
   else {
       qDebug() << "Error while starting: " << errorString();
   }
}

void QSimpleServer::incomingConnection(qintptr handle)                //приходит новый запрос
{
    QSocketThread*thread = new QSocketThread(handle);                 //создается поток
    connect(thread, SIGNAL(finished()), thread, SLOT(deleteLater())); //соединяется сигнал finished с удалением потока
    thread->start();                                                  //запускается поток
}

