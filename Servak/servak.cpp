#include "servak.h"

servak::servak(QString ip, int port, QObject *parent)
    : QObject{parent}
{
    m_server = new QTcpServer();                // Allocate memory for the server
    m_server->listen(QHostAddress(ip), port);   // listen - our server will listen on a specific port
                                                // QHostAddress - will turn the ip string into a real ip

    if (m_server->isListening())        // If listen on
    {
        qDebug() << "Server started at: " <<
                    m_server->serverAddress() <<
                    m_server->serverPort();
    }
    else                                // It don`t linten on
    {
        qDebug() << "Server failed to start " <<
                    m_server->serverError();
    }

    connect(m_server, &QTcpServer::newConnection, this, &servak::newConnectionClient); // If there is a new connection, go to the function
    // If the signal and the slot have the same names, the program may not work correctly and conflict
}

void servak::newConnectionClient()        // New client
{
    QTcpSocket* client = m_server->nextPendingConnection(); // We define it, connect it to the server and allocate memory
    m_clients.push_back(client);                            // Add it to the vector
    qDebug() << "New client from: " << client->localAddress(); // Its address

    connect(client, &QTcpSocket::readyRead, this, &servak::readyRead);              // Connect all out functions to the client,
    connect(client, &QTcpSocket::disconnected, this, &servak::disconnectedClient);  // to the client, but not to the server
}

void servak::disconnectedClient()
{
    QTcpSocket* client = (QTcpSocket*)sender();         // Define, who is a sender
    m_clients.removeOne(client);                        // Delete it from vector

    qDebug() << "Disconnected the client from: "<<
                client->localAddress();

    disconnect(client, &QTcpSocket::readyRead, this, &servak::readyRead);               // Turn off it from our functions
    disconnect(client, &QTcpSocket::disconnected, this, &servak::disconnectedClient);
}

void servak::readyRead()                        // When there is something to read
{
    QTcpSocket* client = (QTcpSocket*)sender(); // Define the sender
    QByteArray data = client->readAll();        // We store the message as a string of bytes

    if (data.indexOf("<n>") == 0) // The first entry into the array. Returns a bool
    {
        QByteArray login = data.remove(0, 3);
        m_logins[client] = login;
    }
    if (data.indexOf("<m>") == 0)
    {
        QByteArray message = data.remove(0, 3);
        sendToAll(m_logins[client] + ": " + message);   // We send it to everyone on the server, in the general chat
    }                          
}

void servak::sendToAll(QByteArray message)
{
    for (QTcpSocket* buffer : m_clients)    // Iterate through the vector
    {
        buffer->write(message);             // Display all messages
    }
}
