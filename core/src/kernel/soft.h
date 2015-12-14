#ifndef SOFT_H_DEF
#define SOFT_H_DEF
#include "config.h"
#include "softns.h"

#include <string>
#include <list>
#include <memory>

SOFT_BEGIN_NAMESPACE

class StorageStrategy;

void                              init(int &argc, char *argv[]);
std::string                       applicationDirPath();
std::string                       uuidGen(); /* TODO: Move this to a utils header */
std::list<std::string>            arguments();
int                               storageDriverCount();
std::list<std::string>            registeredStorageDrivers();
bool                              registerStorage(const char *name, StorageStrategy*(*createFunc)(const char*, const char *options));
std::shared_ptr<StorageStrategy>  create(const char *name, const char *uri, const char *options);
StorageStrategy*                  createStrategy(const char *name, const char *uri, const char *options);

SOFT_END_NAMESPACE

#endif // SOFT_H_DEF
