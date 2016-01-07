#ifndef SIM_PRIVATE_H_DEF
#define SIM_PRIVATE_H_DEF

#include "sim-context.h"

struct _sim_application_t
{
  sim_context_t *ctx;
  void (*user_init)        (sim_context_t*, void*);
  void (*user_compute)     (sim_context_t*, void*);
  void (*user_cleanup)     (sim_context_t*, void*);
  void (*user_begin_iter)  (sim_context_t*, void*);
  void (*user_end_iter)    (sim_context_t*, void*);
  void (*user_time_stepper)(sim_context_t*, void (*)(sim_context_t*, void*), void (*)(sim_context_t*, void*), void (*)(sim_context_t*, void*), void*);
};

#endif
