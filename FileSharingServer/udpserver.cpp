#include "udpserver.h"
#include <QDebug>

UdpServer::UdpServer(QWidget* wgt) : QWidget(wgt)
{
    portIn = 5000;
    portOut = 5001;

    m_udpInSock = new QUdpSocket(this);
    m_udpOutSock = new QUdpSocket(this);

    bindPortIn(portIn);

    connect(m_udpInSock, &QUdpSocket::readyRead, this, &UdpServer::slotProcessDatagram);
}

void UdpServer::bindPortIn(int port)
{
    portIn = port;
    m_udpInSock->bind(QHostAddress::LocalHost, portIn);
}

void UdpServer::bindPortOut(int port)
{
    portOut = port;
    m_udpOutSock->bind(QHostAddress::LocalHost, portOut);
}

void UdpServer::slotProcessDatagram()
{
    QString data;
    QByteArray baDatagram;
    do
    {
        baDatagram.resize(m_udpInSock->pendingDatagramSize());
        m_udpInSock->readDatagram(baDatagram.data(), baDatagram.size());
    }
    while(m_udpInSock->hasPendingDatagrams());

    QDateTime dateTime;
    QDataStream in(&baDatagram, QIODevice::ReadOnly);
    in.setVersion(QDataStream::Qt_5_15);
    in >> dateTime >> data;
    QString str = "Received: " + dateTime.toString() + " " + data;
    emit showData(str);
    slotSendDatagram(data);
}

void UdpServer::slotSendDatagram(QString data)
{
    QByteArray baDatagram;
    QDataStream out(&baDatagram, QIODevice::WriteOnly);
    out.setVersion(QDataStream::Qt_5_15);
    QDateTime dt = QDateTime::currentDateTime();
    QString str = "Send: " + dt.toString() + " " + data;
    out << dt << data;

    bindPortOut(portOut);
    m_udpOutSock->writeDatagram(baDatagram, QHostAddress::LocalHost, portOut);
    m_udpOutSock->disconnectFromHost();

    emit showData(str);
}
