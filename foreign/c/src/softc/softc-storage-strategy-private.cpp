#include <cstdlib>

#include "softc-datamodel.h"
#include "softc-datamodel-private.hpp"
#include "softc-storage-strategy-private.h"
#include "softc-storage-strategy-private.hpp"

softc_datamodel_t * softc_storage_strategy_private_get_datamodel(softc_storage_strategy_t *self)
{
  softc_datamodel_t *datamodel;
  datamodel = (softc_datamodel_t *)malloc(sizeof *datamodel);
  datamodel->ref = self->ref->dataModel();
}

void softc_storage_strategy_private_store(softc_storage_strategy_t *self,
					  const softc_datamodel_t *model)
{
  self->ref->store(model->ref);
}

void softc_storage_strategy_private_retrieve(softc_storage_strategy_t *self,
					     softc_datamodel_t *model)
{
  self->ref->retrieve(model->ref);
}
