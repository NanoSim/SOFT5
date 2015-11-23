#include <QCoreApplication>
#include <QStringList>
#include <QVariant>
#include <QDir>
#include <QUuid>
#include <QLibrary>
#include <QPluginLoader>
#include <QScopedPointer>
#include <QTextStream>
#include <QProcessEnvironment>

#include "soft.h"
#include "storagefactory.h"
#include "storagestrategy.h"
#include "istrategyplugin.h"
//#include "defaultstrategy.h"

static const char *storagefactoryid = "ea1ae6168c404a31bcfdd59da91c1e85";
static const char *pluginDirectory = "/plugins";

SOFT_BEGIN_NAMESPACE

namespace {
   template <class T>
   static T* asPtr (QVariant v)
   {
      return (T*)v.value<void*>();
   }
   
   template <class T>
   static QVariant asQVariant(T *ptr)
   {
      return qVariantFromValue((void*)ptr);
   }
}

std::list<std::string> registeredStorageDrivers()
{
  if (QCoreApplication::instance() == nullptr) {
    return std::list<std::string>();
  }
  
  auto *factory = asPtr<StorageFactory>(qApp->property(storagefactoryid));
  std::list<std::string> retval;
  for (auto i = 0; i < factory->driverCount(); ++i) {
    retval.push_back(factory->driverName(i));
  }
  
  return retval;
}

/*
static bool registerDefaultStorage()
{
  return registerStorage("default", [](const char *uri) -> soft::StorageStrategy*
		  {
		    auto d = new soft::DefaultStrategy(uri);
		    return d;
		  });  
}
*/

static bool registerStoragePlugins()
{
  auto const softpath = QProcessEnvironment::systemEnvironment().value("SOFTBASE", SOFTBASE);  
  auto const pluginPath = softpath + "/storage"; // TODO: FIX THIS!
  QDir const pluginsDir(pluginPath);
  auto const files = pluginsDir.entryList (QDir::Files);
  foreach (auto const &file, files) {
    if (QLibrary::isLibrary (file)) {
      QScopedPointer<QPluginLoader> loader (new QPluginLoader (pluginsDir.absoluteFilePath(file)));
      auto isOk   = loader->load();
      if (isOk) {
	auto pluginPtr = qobject_cast<IStrategyPlugin*>(loader->instance());
	if (pluginPtr) {
	  pluginPtr->registerStrategy();
	}
	else {
	}
      }
      else {
	QTextStream(stderr) << loader->errorString() << endl;
      }
    }
  }
  return true;
}

static QCoreApplication* app = nullptr;
void init(int &argc, char *argv[])
{
  if (QCoreApplication::instance() == nullptr) {
    app = new QCoreApplication(argc, argv);
    auto storageFactory = new StorageFactory();
    app->setProperty(storagefactoryid, asQVariant(storageFactory));
//    registerDefaultStorage();
    registerStoragePlugins();
  }
}

bool registerStorage(const char *name, StorageStrategy*(*createFunc)(const char*, const char*))
{
  if (QCoreApplication::instance() == nullptr) 
    return false;

  auto *factory = asPtr<StorageFactory>(qApp->property(storagefactoryid));
  return factory->registerStrategy(name, createFunc);
}

std::shared_ptr<StorageStrategy> create(const char *name, const char *uri, const char *options)
{
  if (QCoreApplication::instance() == nullptr) 
    return std::shared_ptr<StorageStrategy>();
  
  auto *factory = asPtr<StorageFactory>(qApp->property(storagefactoryid));
  std::shared_ptr<StorageStrategy> retval (factory->create(name, uri, options));
  return retval;
}

StorageStrategy* createStrategy(const char *name, const char *uri, const char *options)
{
  if (QCoreApplication::instance() == nullptr) 
    return nullptr;
  
  auto *factory = asPtr<StorageFactory>(qApp->property(storagefactoryid));
  return factory->create(name, uri, options);
}

std::string applicationDirPath()
{
   return QCoreApplication::applicationDirPath().toStdString();
}

std::string uuidGen()
{
  return QUuid::createUuid().toString().mid(1,36).toStdString();
}

std::list<std::string> arguments()
{
   std::list<std::string> retval;
   foreach(auto argument, QCoreApplication::arguments()) {
      retval.push_back (argument.toStdString());
   }
   return retval;
}

SOFT_END_NAMESPACE
