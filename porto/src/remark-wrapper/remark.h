#pragma once
#include <QObject>
class QProcess;

class Remark : public QObject
{
  Q_OBJECT
  
public:  
  explicit Remark(QObject *parent = nullptr);
  virtual ~Remark();
  void run();

public slots:
  void started();
  void readStdout();
  void readStderr();

signals:
  void finished();
  
private:
  QProcess *process;
};
