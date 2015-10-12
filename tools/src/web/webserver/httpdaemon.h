#pragma once
#include <QTcpServer>

class HttpDaemon : public QTcpServer
{
  Q_OBJECT

public:
  HttpDaemon(QString const &configScript, QObject *parent = 0);
  virtual ~HttpDaemon();
  bool run();
  
protected:
  void incomingConnection (qintptr socket) override;

private slots:
  void readClient();
  void discardClient();
    
private:
  void handleToken(QStringList const &token, QTcpSocket *socket);

  class Private;
  Private *d;
};
