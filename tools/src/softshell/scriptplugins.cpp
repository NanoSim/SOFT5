#include <QtCore>
#include <QtScript>
#include <Soft>

#include "isoftplugin.h"
#include "scriptplugins.h"

class ScriptPlugins :: Private
{
   friend class ScriptPlugins;

   Private() {}
   
   QList<QPluginLoader*> plugins;
};

ScriptPlugins :: ScriptPlugins(soft::ScriptEngine const &e, QObject *parent)
   : QObject(parent)
   , d( new ::ScriptPlugins::Private() )
{
   auto engine = static_cast<QScriptEngine*>(e.ref());
   load(engine);
}

ScriptPlugins :: ~ScriptPlugins()
{
}

void ScriptPlugins :: load(QScriptEngine *engine)
{
  auto const softpath = QProcessEnvironment::systemEnvironment().value("SOFTBASE", SOFTBASE);
  QCoreApplication::addLibraryPath (softpath + "/modules");
  auto const pluginPath = softpath + "/plugins";
  QDir pluginsDir(pluginPath);
  auto files = pluginsDir.entryList(QDir::Files);
  foreach( auto const &file, files ) {
    if( QLibrary::isLibrary(file) ) {
      auto loader = new QPluginLoader(pluginsDir.absoluteFilePath(file), this);
      auto const ok = loader->load();
      if( ok ) {
	auto const jsonObj = loader->metaData();
	if (jsonObj.value("IID") == "org.sintef.soft/ISoftPlugin/0.1") {		
	  auto pluginPtr = qobject_cast<ISoftPlugin*>(loader->instance());
	  if( pluginPtr ) {
	    pluginPtr->registerPlugin(engine);
	    //	    QTextStream(stdout) << "[loaded: " << file << "]\n";
	  }
	  else {
	    QTextStream(stdout) << "failed to register: " << loader->errorString() << endl;
	  }
	}
      }
    }    
  }
}
