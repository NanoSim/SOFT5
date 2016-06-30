#include <QtCore>
#include "remark.h"

Remark :: Remark(QObject *parent)
  : QObject(parent)
{
  process = new QProcess(this);
  connect(process, SIGNAL(started()), this, SLOT(started()));
  connect(process, SIGNAL(finished(int,QProcess::ExitStatus)), this, SIGNAL(finished()));
  connect(process, SIGNAL(readyReadStandardOutput()), this, SLOT(readStdout()));
  connect(process, SIGNAL(readyReadStandardError()), this, SLOT(readStderr()));

}

Remark :: ~Remark()
{}

void Remark :: run()
{
  process->setWorkingDirectory(QCoreApplication::applicationDirPath());
  process->setProcessEnvironment(QProcessEnvironment::systemEnvironment());
  process->start("./rmc-vasp.sh");
}

void Remark :: readStdout()
{
  QTextStream(stdout) << process->readAllStandardOutput();
}

void Remark :: readStderr()
{
  QTextStream(stdout) << process->readAllStandardError();
}

void Remark :: started()
{
  qDebug() << "started";
}
