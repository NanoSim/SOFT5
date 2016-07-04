#pragma once
#include <QObject>
#include <collection.h>
class QProcess;

class Remark : public QObject
{
  Q_OBJECT
  
public:  
  explicit Remark(QObject *parent = nullptr);
  virtual ~Remark();
  void run(soft::Collection &collection);

public slots:
  void started();
  void readStdout();
  void readStderr();

signals:
  void finished();
  
private:
  QProcess *process;
};
