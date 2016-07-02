#include <stdexcept>
#include <iostream>
#include "qentity.h"
#include "qstorage.h"

QStorage :: QStorage(QString const &driver,
	 QString const &uri,
	 QString const &options)
  : QObject()
  , storage(new soft::Storage(qPrintable(driver), qPrintable(uri), qPrintable(options)))
{
}

QStorage ::  ~QStorage()
{}

void QStorage :: save (QEntity const *entity)
{
  try {
    storage->save(entity->generic.data());
  } catch(const std::invalid_argument& ia) {
    std::cerr << "Invalid argument: " << ia.what() << "\n";
  } catch(const std::runtime_error& re) {
    std::cerr << "Runtime error: " << re.what() << "\n";
  }

}

void QStorage :: load (QEntity *entity)
{
  try {
    storage->load(entity->generic.data());
  } catch(const std::invalid_argument& ia) {
    std::cerr << "Invalid argument: " << ia.what() << "\n";
  } catch(const std::runtime_error& re) {
    std::cerr << "Runtime error: " << re.what() << "\n";
  }
}
