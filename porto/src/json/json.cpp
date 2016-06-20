#include "json.h"

#include <string.h>
#include <algorithm>

#include <QJsonObject>
#include <QJsonArray>
#include <QJsonDocument>
#include <QFile>

int softc_plugin_identify( char* name, int maxlen ) {
  // TODO: Is this the name we want to return?
  const char plugin_name[] = "porto-json";

  // Copy no more than the length of the string above or the given maximum length
  const size_t len = std::min<size_t>(sizeof(plugin_name), maxlen);
  strncpy(name, plugin_name, len);

  // Ensure that the returned string is 0-terminated, no matter what length constraints it had
  name[len - 1] = '\0';

  // TODO: Return failure if the target string does not have sufficient space?
  return SOFTC_STATUS_OK;
}

int softc_plugin_capabilities() {
  return SOFTC_CAPABILITY_READ | SOFTC_CAPABILITY_WRITE;
}

int softc_plugin_load( softc_datamodel_t* datamodel, const char* uri, const char* options ) {
  // TODO: Return an actual datamodel with the proper structure, naming etc.
  
  QFile f(uri);
  if (!f.open(QIODevice::ReadOnly)) {
    return SOFTC_STATUS_FAILURE;
  }

  QByteArray ba = f.readAll();

  QJsonDocument doc = QJsonDocument::fromJson(ba);

  double dummy = doc.object()["amount"].toDouble();

  softc_datamodel_append_double(datamodel, "amount", dummy);

  return SOFTC_STATUS_OK;
}

int softc_plugin_save( const softc_datamodel_t* datamodel, const char* uri, const char* options) {
  
  // TODO: Some checks for id, version, name and namespace here?
  // softc_datamodel_get_id
  // softc_datamodel_get_meta_name
  // softc_datamodel_get_meta_version
  // softc_datamodel_get_meta_namespace
  
  double dummy;
  if (!softc_datamodel_get_double(datamodel, "amount", &dummy)) {
    return SOFTC_STATUS_FAILURE;
  }

  // TODO: Also make sure that, regardless of meta information, we check that the
  //       properties exist before we use them.

  QJsonObject root;
  root["recepie"] = "cheese cake";

  QJsonArray ingredients;
  ingredients.append("cheese");
  ingredients.append("flour");
  ingredients.append("sugar");

  root["ingredients"] = ingredients;
  
  root["amount"] = dummy;

  QJsonDocument doc(root);

  QFile f(uri);
  if (!f.open(QIODevice::WriteOnly)) {
    return SOFTC_STATUS_FAILURE;
  }

  // Can f.write fail? If so, return error code
  f.write(doc.toJson());

  return SOFTC_STATUS_OK;
}
 
