#ifndef SERVAK_H
#define SERVAK_H

#include <QObject>
#include <QTcpServer>
#include <QTcpSocket>
#include <QDebug>

class servak : public QObject
{
    Q_OBJECT
public:
    explicit servak(QString ip, int port, QObject *parent = nullptr);

public slots:
    void newConnectionClient();
    void disconnectedClient();
    void readyRead();

    void sendToAll(QByteArray message);

private:
    QTcpServer* m_server;
    QVector<QTcpSocket*> m_clients;
    QMap<QTcpSocket*, QByteArray> m_logins; // QMap - vocabulary
};

#endif // SERVAK_H
