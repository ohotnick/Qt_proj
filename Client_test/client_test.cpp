#include "client_test.h"
#include "ui_client_test.h"
#include <QtAlgorithms>
#include <QtGui>

int globNumRead = 0;
int globNumSend = 0;

Client_test::Client_test(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::Client_test)
{
    ui->setupUi(this);

    m_pTcpSocket = new QTcpSocket(this);

    connect(ui->pushButton, SIGNAL(clicked()),
            this,        SLOT(slotBildMyClient())
           );
    connect(ui->pushButton_2, SIGNAL(clicked()),
            this,        SLOT(slotAddHexData())
           );
    connect(m_pTcpSocket, SIGNAL(connected()), SLOT(slotConnected()));
    connect(m_pTcpSocket, SIGNAL(readyRead()), SLOT(slotReadyRead()));
    connect(m_pTcpSocket, SIGNAL(error(QAbstractSocket::SocketError)),
            this,         SLOT(slotError(QAbstractSocket::SocketError))
           );
    connect(ui->pushButton_3, SIGNAL(clicked()), SLOT(slotSendToServer()));
    ui->statusBar->showMessage("V1.2 GV");

}

//------------------------------------------------
int BildHexDecData(QLineEdit *pth,QByteArray &pth2, int convert_format) // convert_format = 0 (HEX)
{
    QByteArray  arrBlock;
    arrBlock.append(pth->text());

    int i  = 0;
    int j  = 0;
    int k  = 0;
    int k2 = 0;
    int count = 0;

    if( convert_format == 0)
      {
        while(1)
          {
            if(arrBlock[i] <= ' ')
              {qDebug()<<"Last port = NULL"<<i;
                if(count==1)
                {
                    pth2.append(k2);
                }
                return k;                         //в HEX формате
              }
            j = arrBlock[i];
            qDebug()<<"Port №"<<i<<":"<<j;

            if((j >= 48)&&(j <= 57))
              {
                k = k << 4;
                k = k + (j-48);
                k2 = k2 << 4;
                k2 = k2 + (j-48);
                count = count + 1;
              }
            else if((j >= 65)&&(j <= 70))
                {
                  k = k << 4;
                  k = k + (j-55);
                  k2 = k2 << 4;
                  k2 = k2 + (j-55);
                  count = count + 1;
                }
            else if((j >= 97)&&(j <= 102))
                {
                  k = k << 4;
                  k = k + (j-87);
                  k2 = k2 << 4;
                  k2 = k2 + (j-87);
                  count = count + 1;
                }
             if(count == 2)
             {
                 pth2.append(k2);
                 k2 = 0;
                 count = 0;
             }

            qDebug()<<"Return int:"<<k;  // в формате int
            i = i + 1;
          }
         }
     else
       {
        k = arrBlock.toInt();
        return k;                                //в INT формате
        }

}
//------------------------------------------------
void Client_test::slotAddHexData()
{
  QByteArray  arrBlock;
  QByteArray  arrBlock2;
  int data_pash = 0;
  data_pash     = BildHexDecData(ui->lineEdit_2,arrBlock2 , 0);
  qDebug()<<"Return data_pash:"<<data_pash;
  arrBlock.append(ui->lineEdit_3->text());
  arrBlock.append(arrBlock2);
  qDebug()<<"Return arrBlock2:"<<arrBlock2;
  ui->lineEdit_3->setText(arrBlock);

}
//------------------------------------------------
void Client_test::slotBildMyClient()
{
    quint16 port = 40001;
    QByteArray  arrBlock2;

    port = BildHexDecData(ui->lineEdit,arrBlock2, 1);
    qDebug()<<"Return port:"<<port;

    if(ui->pushButton->isChecked() == true)
    {
       ui->pushButton->setText("Отключить");
       ui->pushButton_3->setEnabled(1);


       m_pTcpSocket->connectToHost(ui->lineEdit_4->text(),port );

       globNumRead = 0;
       globNumSend = 0;

    }
    else {
       ui->pushButton->setText("Подключить");
       m_pTcpSocket->disconnectFromHost();
       ui->pushButton_3->setEnabled(0);
    }
}
//------------------------------------------------

Client_test::~Client_test()
{
    delete ui;
}

//-----------------------------------------------------

void Client_test::slotConnected()
{
    ui->textEdit->setTextColor(Qt::black);
    ui->textEdit->append("Received the connected() signal");
}

// ----------------------------------------------------------------------
void Client_test::slotError(QAbstractSocket::SocketError err)
{
    QString strError =
        "Error: " + (err == QAbstractSocket::HostNotFoundError ?
                     "The host was not found." :
                     err == QAbstractSocket::RemoteHostClosedError ?
                     "The remote host is closed." :
                     err == QAbstractSocket::ConnectionRefusedError ?
                     "The connection was refused." :
                     QString(m_pTcpSocket->errorString())
                    );
    ui->textEdit->setTextColor(Qt::black);
    ui->textEdit->append(strError);
}
// ----------------------------------------------------------------------
void Client_test::slotReadyRead()
{
    qDebug()<<"Start read";

    QDataStream in(m_pTcpSocket);
    in.setVersion(QDataStream::Qt_5_3);
     qDebug()<<"Read 2"<<m_pTcpSocket->bytesAvailable();

     QString str;
     QString number;
     QByteArray array;
     while (1)
     {
       qDebug()<<m_pTcpSocket->bytesAvailable();
       qDebug()<<array;
       if(m_pTcpSocket->bytesAvailable() <= 0)
           {
            break;

            }
       array.append(m_pTcpSocket->readAll());

     }
     number.setNum(globNumRead);
     globNumRead = globNumRead + 1;

     str.append(array);
     qDebug()<<array;
     ui->textEdit->setTextColor(Qt::darkCyan);
     ui->textEdit->append("Read:(" +number+ ")\nHEX:"+ array.toHex()+ "\n" +"Char:\n"+ str);
     ui->textEdit->moveCursor(QTextCursor::End);



}
// ----------------------------------------------------------------------
void Client_test::slotSendToServer()
{
    QByteArray  arrBlock;
    QString number;
    number.setNum(globNumSend);
    globNumSend = globNumSend + 1;

    arrBlock.append(ui->lineEdit_3->text());
    qDebug()<<arrBlock<<arrBlock.size();
    m_pTcpSocket->write(arrBlock);
    ui->textEdit->setTextColor(Qt::blue);
    ui->textEdit->append("Send:("+number+")" + arrBlock);
}
