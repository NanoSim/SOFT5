#include <cstring>
#include <iostream>
#include <QtCore>
#include <Soft>
#include <softc/softc-storage-plugin.h>
#include <softc/softc-datamodel-private.hpp>
#include <remarcReader.h>
#include "remarcsetup.hxx"

static char const defaultRemarcInFile[] = "remarc.in";

int softc_plugin_identify (char* name, int maxlen) {
  const char plugin_name[] = "remark-init";
  const size_t len = std::min<size_t>(sizeof(plugin_name), maxlen);
  strncpy(name, plugin_name, len);
  name[len - 1] = '\0';
  return SOFTC_STATUS_OK;
}

int softc_plugin_capabilities ()
{
  return SOFTC_CAPABILITY_READ|SOFTC_CAPABILITY_WRITE;
}

int softc_plugin_load (softc_datamodel_t* datamodel, char const *uri, char const* options)
{
  using namespace soft;
  QUrl const loc(uri);
  QUrlQuery const query(uri);
  QFileInfo const remarcInFile = (loc.path(), query.hasQueryItem("filename") ? query.queryItemValue("filename"): defaultRemarcInFile);
  IO::RemarcReader remarcReader(remarcInFile.absoluteFilePath().toStdString());
  remarcReader.read();
  soft::Remarcsetup setup;

  return SOFTC_STATUS_FAILURE;
}

int softc_plugin_save(softc_datamodel_t const* datamodel, const char* uri, const char* options)
{  
  return SOFTC_STATUS_FAILURE;
}
