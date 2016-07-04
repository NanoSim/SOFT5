#include <Soft>
#include <QtCore>

#include <collection.h>
#include <storage.h>
#include <reference.hxx>
#include <file.hxx>
#include "remark.h"
int main(int argc, char **argv)
{
  ::soft::init(argc, argv); 
  if (argc <= 1) {
    QTextStream(stderr) << "usage: " << argv[0] << "<uuid>" << endl;
    return 1;
  }

  soft::Collection collection(argv[1]);
  soft::Storage storage("mongo2", "mongodb://localhost", "db=porto;coll=demo");

  soft::Reference reference;
  soft::File file;

  collection.setName("DFTPrep");
  collection.setVersion("0.1");
  collection.attachEntity("dftPath", &reference);
  collection.attachEntity("dftBoundayFile", &file);

  storage.load(&collection);

  Remark *r = new Remark();

  QObject::connect(r, SIGNAL(finished()), r, SLOT(deleteLater()));
  QObject::connect(r, SIGNAL(finished()), QCoreApplication::instance(), SLOT(quit()));

  r->run(collection);
  return QCoreApplication::instance()->exec();
}
