#include "udpclient.h"
#include <QDebug>

UdpClient::UdpClient(QWidget* wgt) : QWidget(wgt)
{
    portIn = 5001;
    portOut = 5000;

    m_udpInSock = new QUdpSocket(this);
    m_udpOutSock = new QUdpSocket(this);

    connect(m_udpInSock, &QUdpSocket::readyRead, this, &UdpClient::slotProcessDatagram);
}

void UdpClient::bindPortIn(int port)
{
    portIn = port;
    m_udpInSock->bind(QHostAddress::LocalHost, portIn);
}

void UdpClient::bindPortOut(int port)
{
    portOut = port;
    m_udpOutSock->bind(QHostAddress::LocalHost, portOut);
}

void UdpClient::slotProcessDatagram()
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
}

void UdpClient::slotSendDatagram(QString data)
{
    QByteArray baDatagram;
    QDataStream out(&baDatagram, QIODevice::WriteOnly);
    out.setVersion(QDataStream::Qt_5_15);
    QDateTime dt = QDateTime::currentDateTime();
    QString str = "Send: " + dt.toString()  + " " + data;
    out << dt << data;

    bindPortIn(portIn);
    m_udpOutSock->writeDatagram(baDatagram, QHostAddress::LocalHost, portOut);

    emit showData(str);
}
