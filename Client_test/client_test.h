#ifndef CLIENT_TEST_H
#define CLIENT_TEST_H

#include <QMainWindow>
#include <QWidget>
#include <QTcpSocket>


namespace Ui {
class Client_test;
}

class Client_test : public QMainWindow
{
    Q_OBJECT

public:
    explicit Client_test(QWidget *parent = nullptr);
    ~Client_test();

private:
    Ui::Client_test *ui;
    QTcpSocket*     m_pTcpSocket;
    quint16         m_nNextBlockSize;

private slots:
    void slotBildMyClient   (                         );
    void slotAddHexData     (                         );
    void slotReadyRead   (                            );
    void slotError       (QAbstractSocket::SocketError);
    void slotSendToServer(                            );
    void slotConnected   (                            );
};



#endif // CLIENT_TEST_H
