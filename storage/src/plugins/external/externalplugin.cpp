#include <QTextStream>
#include "externalplugin.h"

#include "extstrategy.h"
#include "soft.h"

ExternalStrategyPlugin :: ~ExternalStrategyPlugin()
{}

void ExternalStrategyPlugin :: registerStreategy()
{
  auto isOk = soft::registerStorage("external", soft::ExtStrategy::create);
  if (!isOk) {
    QTextStream(stderr) << "Cannot register the external storage strategy" << endl;
  }
}
