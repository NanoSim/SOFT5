#include "csvstrategyplugin.h"
#include <QCoreApplication>
#include <QTextStream>

#include "soft.h"
#include "csvstrategy.h"

CsvStrategyPlugin :: ~CsvStrategyPlugin()
{}

void CsvStrategyPlugin :: registerStrategy()
{
   auto isOk = soft::registerStorage("csv", soft::CsvStrategy::create);
   if (!isOk) {
      QTextStream(stderr) << "Cannot register the csv storage strategy" << endl;
   }
}
