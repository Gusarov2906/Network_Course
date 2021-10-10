#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    udpClient = new UdpClient(this);
    connect(udpClient, &UdpClient::showData, this, &MainWindow::showData);
}

void MainWindow::showData(QString data)
{
    ui->textEdit->append(data);
}

MainWindow::~MainWindow()
{
    delete ui;
}


void MainWindow::on_sendButton_clicked()
{
    udpClient->slotSendDatagram(ui->lineEdit->text());
}

void MainWindow::on_connectButton_clicked()
{
    udpClient->bindPortOut(ui->portLineEdit->text().toInt());
}

