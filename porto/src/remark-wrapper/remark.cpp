#include <QtCore>
#include "remark.h"
#include <reference.hxx>
#include <file.hxx>
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

void Remark :: run(soft::Collection &collection)
{
  auto dftPath = dynamic_cast<const soft::Reference*>(collection.findInstance("dftPath"));
  auto dftBoundayFile = dynamic_cast<const soft::File*>(collection.findInstance("dftBoundayFile"));

  process->setWorkingDirectory(QCoreApplication::applicationDirPath());
  process->setProcessEnvironment(QProcessEnvironment::systemEnvironment());
  process->start(QString("./rmc-vasp.sh %1").arg(QString::fromStdString(dftPath->uri)));
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
