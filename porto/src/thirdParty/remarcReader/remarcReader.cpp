#include <QtCore>
#include "remarcReader.h"

namespace IO
{

RemarcReader :: RemarcReader(std::string const remarcFile)
  : remarcFile(remarcFile)
{}

RemarcReader :: ~RemarcReader()
{}

void RemarcReader :: read()
{
  QString filename = QString::fromStdString(remarcFile);
  QFile file(filename);
  if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
    qWarning() << "Unable to read file " << filename << ": " << file.errorString();
    return;
  }

  QTextStream stream(&file);
  stream >> T_min;
  qDebug() << "T_min";
  
  
}

}
