#include "json.h"

int softc_plugin_identify( char* name, int maxlen ) {
  // TODO
}

int softc_plugin_capabilities() {
  return SOFTC_CAPABILITY_READ | SOFTC_CAPABILITY_WRITE;
}

int softc_plugin_load( softc_datamodel_t* datamodel, const char* uri, const char* options ) {
  // TODO
}

int softc_plugin_save( const softc_datamodel_t* datamodel, const char* uri, const char* options) {
  // TODO
}

