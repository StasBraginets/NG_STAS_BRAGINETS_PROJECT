#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    ui->p_chat->setReadOnly(true);

    m_socket = new QTcpSocket();

    connect(m_socket, &QTcpSocket::connected, this, &MainWindow::connected);
    connect(m_socket, &QTcpSocket::readyRead, this, &MainWindow::dataReceived);

    connect(ui->b_connect, &QPushButton::clicked, this, &MainWindow::connectToServer);
    connect(ui->b_send, &QPushButton::clicked, this, &MainWindow::sendMessage);
}

MainWindow::~MainWindow()
{
    delete ui;
    delete m_socket;
}

void MainWindow::connectToServer()
{
    m_socket->connectToHost(ui->l_ip->text(), ui->sb_port->value()); // Connect to the server if all configurations are correct
}

void MainWindow::connected()
{
    ui->tabWidget->setCurrentIndex(1);                          // First page of tabWidget
    m_socket->write("<n>" + ui->l_nickname->text().toUtf8());   // Send a nickname to server

}

void MainWindow::sendMessage()
{
    QByteArray data = ui->p_message->toPlainText().toUtf8(); // Return text editing in utf8
    if (data.indexOf("\n") != -1) // If the symbol was found
    {
        data.resize(data.length() - 1);
    }
    for (short i = 0; i < data.size(); ++i) // Encryptor
    {
        data[i] = data[i] + ui->l_encryptor->text().toInt();
    }
    qDebug() << data;
    m_socket->write("<m>" + data);
    ui->p_message->clear();
}

void MainWindow::dataReceived()
{
    QByteArray message = m_socket->readAll();

    if (message.indexOf("<n>") != -1)
    {
        message.remove(0, 3);
        nicknames.push_back(message);
        for (short i = 0; i < nicknames.size(); i++)
        {
            QListWidgetItem* nickname = new QListWidgetItem(nicknames[i]);
            ui->lw_nicknames->addItem(nickname);
        }
    }
    if (message.indexOf("<m>") != -1)
    {
        message.remove(0, 3);
        ui->p_chat->setPlainText(ui->p_chat->toPlainText() + message + "\n"); // New text
    }
}

