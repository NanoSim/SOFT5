#ifndef SIM_CONTEXT_H
#define SIM_CONTEXT_H

#include "sim-macros.h"

SIM_BEGIN_DECLS

typedef struct _sim_context_t sim_context_t;

sim_context_t *sim_context_create();
void           sim_context_free(sim_context_t *ctx);
void           sim_context_set_user_data(sim_context_t *ctx, void *);
void *         sim_context_get_user_data(sim_context_t *ctx);

SIM_END_DECLS

#endif 
