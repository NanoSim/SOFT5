#ifndef CSVSTRATEGYPLUGIN_H_DEF
#define CSVSTRATEGYPLUGIN_H_DEF

#include <QObject>
#include "istrategyplugin.h"

class CsvStrategyPlugin : public QObject
			  , public IStrategyPlugin
{
  Q_OBJECT
  Q_PLUGIN_METADATA(IID "org.sintef.soft/IStrategyPlugin/0.1")
  Q_INTERFACES(IStrategyPlugin)

public:
  virtual ~CsvStrategyPlugin();
  virtual void registerStrategy();
};

#endif // CSVSTRATEGYPLUGIN_H_DEF
