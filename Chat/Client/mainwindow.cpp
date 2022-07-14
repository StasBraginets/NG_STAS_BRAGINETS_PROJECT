#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    ui->p_chat->setReadOnly(true);

    m_socket = new QTcpSocket();
    setStyle();

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

void MainWindow::setStyle()
{
    ui->b_connect->setStyleSheet("color: yellow; background: green;"
                                 "border-style: outset; border-width: 2px;"
                                 "border-radius: 10px; border-color: beige;"
                                 "font: bold 14px; min-width: 10em; padding: 6px");

    ui->p_chat->setStyleSheet("color: yellow; border-image: url(:/img/Images/original.png)");

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
        data[i] = data[i] + ui->sb_encryptor->text().toInt();
    }
    qDebug() << data;
    m_socket->write("<m>" + data);
    ui->p_message->clear();
}

void MainWindow::dataReceived()
{
    QString message = m_socket->readAll();
    qDebug() << message;

    if (message.indexOf("<n>") != -1)   // For nicknames
    {
        if (nicknames.size() == 0)      // If there is no nicknames at all
        {
            message.remove(0, 3);
            QStringList n_block = message.split("<n>");

            for (short i = 0; i < n_block.size(); i++)
            {
                if (n_block[i] == "")
                {
                    continue;
                }
                nicknames.push_back(n_block[i]);
            }
            for (short i = 0; i < nicknames.size(); i++)
            {
                QListWidgetItem* nickname = new QListWidgetItem(nicknames[i]);
                ui->lw_nicknames->addItem(nickname);
            }
        }
        else if (nicknames.size() > 0)  // If there is nicknames
        {
            message.replace("<n>", "");
            for (short i = 0; i < nicknames.size(); i++)
            {
                if (message.indexOf(nicknames[i]) != -1)
                {
                    message.replace(nicknames[i], "");
                }
            }
            nicknames.push_back(message);

            QListWidgetItem* nickname = new QListWidgetItem(message);
            ui->lw_nicknames->addItem(nickname);
        }
    }
    else if (message.indexOf("<m>") != -1)  // For usuall messages
    {
        message.remove(0, 3);
        ui->p_chat->setPlainText(ui->p_chat->toPlainText() + message + "\n"); // New text
    }
    else if (message.indexOf("<dc>") != -1)     // Delete the nickname from List Widget
    {
        message.replace("<dc>", "");
        for (short i = 0; i < nicknames.size(); i++)
        {
            if (nicknames[i] == message)
            {
                ui->lw_nicknames->takeItem(i);
                nicknames.erase(nicknames.begin()+i);
            }
        }
    }
}

