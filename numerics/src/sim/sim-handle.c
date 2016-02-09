#include <stdlib.h>
#include "sim-handle.h"

struct _sim_handle_t
{
  void *user_context;
};

sim_handle_t *sim_handle_create()
{
  sim_handle_t *ctx;
  ctx = (sim_handle_t*)malloc(sizeof (*ctx));
  ctx->user_context = NULL;
  return ctx;
}

void sim_handle_free(sim_handle_t *ctx)
{
  free (ctx);
}

void sim_handle_set_user_context(sim_handle_t *ctx, void *data)
{
  ctx->user_context = data;
}

void * sim_handle_get_user_context(sim_handle_t *ctx)
{
  return ctx->user_context;
}
