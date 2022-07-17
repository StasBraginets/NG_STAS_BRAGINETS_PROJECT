#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTcpSocket>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void connectToServer();
    void connected();
    void sendMessage();
    void dataReceived();

    //void on_actionSad_emoji_triggered();

    void on_actionQuite_triggered();

private:
    Ui::MainWindow *ui;
    QTcpSocket* m_socket;
    QVector <QString> nicknames;
};
#endif // MAINWINDOW_H
