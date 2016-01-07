#include <stdlib.h>
#include "sim-context.h"

struct _sim_context_t
{
  void *user_data;
};

sim_context_t *sim_context_create()
{
  sim_context_t *ctx;
  ctx = (sim_context_t*)malloc(sizeof (*ctx));
  ctx->user_data = NULL;
  return ctx;
}

void sim_context_free(sim_context_t *ctx)
{
  free (ctx);
}

void sim_context_set_user_data(sim_context_t *ctx, void *data)
{
  ctx->user_data = data;
}

void * sim_context_get_user_data(sim_context_t *ctx)
{
  return ctx->user_data;
}
