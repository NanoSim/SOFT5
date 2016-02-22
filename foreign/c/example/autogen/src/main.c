#include <stdio.h>
#include <stdlib.h>
#include <softc/softc-entity.h>
#include <softc/softc-allocatable.h>
#include <softc/softc-storage.h>
#include <sim/sim.h>
#include "user_ctx.h"
#include "simple.h"
#include "demo.h"

void user_init    (sim_handle_t *handle, void *user_data);
void user_finalize(sim_handle_t *handle, void *user_data);

int main(int argc, char **argv)
{
  sim_application_t *sim_app;

  /* Initialize sim */
  sim_app = sim_init(&argc, argv);

  /* Register user defined functions*/ 
  sim_register_init     (sim_app, user_init);
  sim_register_compute  (sim_app, demo_compute);
  sim_register_finalize (sim_app, user_finalize);

  sim_run(sim_app, NULL);
  return 0;
}

void user_init (sim_handle_t *handle, void *user_data)
{
  user_ctx_s *user_ctx = malloc(sizeof *user_ctx);
  sim_handle_set_user_context(handle, user_ctx);

  user_ctx->a = 42;
  user_ctx->simple = simple_create(10,10);
  double **matrix = simple_get_matrix(user_ctx->simple);
  matrix[0][0] = 3.14;
}

void user_finalize(sim_handle_t *handle, void *user_data)
{
  user_ctx_s *user_ctx = sim_handle_get_user_context(handle);
  free(user_ctx);
  sim_handle_free(handle);
}
