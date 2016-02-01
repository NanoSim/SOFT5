#include <QtCore>
#include <softc/softc.h>
#include "sim.h"
#include "sim-private.h"
#include "sim-context.h"
#include "config.h"

namespace {
  using SIM_fn_prototype          = void(*)(sim_context_t*,
					    void*);
  using SIM_timestepper_prototype = void(*)(sim_context_t*,
					    SIM_fn_prototype,
					    SIM_fn_prototype,
					    SIM_fn_prototype,
					    void*);

  class SIMCommon
  {
  public:
    SIMCommon(){}
    ~SIMCommon(){}

    void loadPlugins() {      
      QDir pluginDir(PLUGIN_DIR);
      foreach (auto const &file, pluginDir.entryList (QDir::Files)) {
	if (QLibrary::isLibrary (file)) {
	  auto lib = QSharedPointer <QLibrary>(new QLibrary(pluginDir.absoluteFilePath(file)));
	  if (lib->resolve("SIM_init") != nullptr) {
	    libList.push_back(lib);	    
	  } else {
	    qDebug() << lib->errorString();
	  }
	}
      }     
    }

    int count() const {
      return libList.count();
    }

    void reg(_sim_application_t *app) const
    {
      qDebug() << "#Registering simulation: " << libList.count();
      foreach (auto lib, libList) {
	SIM_fn_prototype initFunction                 = (SIM_fn_prototype) lib->resolve("SIM_init");
	SIM_fn_prototype computeFunction              = (SIM_fn_prototype) lib->resolve("SIM_compute");
	SIM_timestepper_prototype timeStepperFunction = (SIM_timestepper_prototype) lib->resolve("SIM_time_stepper");
	SIM_fn_prototype cleanupFunction              = (SIM_fn_prototype) lib->resolve("SIM_cleanup");

	if (initFunction && computeFunction && timeStepperFunction && cleanupFunction) {
	  sim_register_init(app, initFunction);
	  sim_register_compute(app, computeFunction);
	  sim_register_time_stepper(app, timeStepperFunction);
	  sim_register_cleanup(app, cleanupFunction);
	} else {
	  qDebug() << "Unable to run example";
	}
      }
    }
    
  private:
    QList<QSharedPointer <QLibrary> > libList;
  };
  
  static softc_t *softapp = nullptr;  
  static SIMCommon *simCommon = nullptr;
}

sim_application_t* sim_init(int *argc, char **argv)
{ 
  if (softapp == nullptr) {
    softapp = softc_init(*argc, argv);
    sim_application_t * ret;
    ret = new sim_application_t;
    ret->ctx = sim_context_create();
    ret->user_init = NULL;
    ret->user_compute = NULL;
    ret->user_time_stepper = NULL;
    ret->user_cleanup = NULL;
    ret->user_begin_iter = NULL;
    ret->user_end_iter = NULL;
    ret->user_finalize = NULL;

    return ret;
  }

  return NULL;
}

void sim_load (sim_application_t* handle)
{
  simCommon = new SIMCommon();
  simCommon->loadPlugins();
  simCommon->reg(handle);
}


void sim_run (sim_application_t* handle, void *user_data)
{
  if (handle == NULL) {
    fprintf(stderr, "sim_init (int*, char**) needs to be called first\n");
    return;
  }
  
  if (handle->user_init != NULL) {
    (*handle->user_init)(handle->ctx, user_data);
  } else {
    fprintf(stderr, "warning - user_init function is not defined\n");
  }
  
  if (handle->user_time_stepper != NULL && handle->user_compute != NULL) {
    (*handle->user_time_stepper)(handle->ctx, handle->user_compute, handle->user_begin_iter, handle->user_end_iter, user_data);
  } else if (handle->user_compute != NULL) {
    (*handle->user_compute)(handle->ctx, user_data);
  } else {
    fprintf(stderr, "warning - user_compute function is not defined\n");
  }

  if (handle->user_finalize != NULL) {
    (*handle->user_finalize)(handle->ctx, user_data);
  } else {
    fprintf(stderr, "warning - user_finalize function is not defined\n");
  }
  
  if (handle->user_cleanup != NULL) {
    (*handle->user_cleanup)(handle->ctx, user_data);  
  } else {
    fprintf(stderr, "warning - user_cleanup function is not defined\n");
  }
}

void sim_register_init    (sim_application_t* handle, SIM_fn_prototype fn)
{
  handle->user_init = fn;
}

void sim_register_compute (sim_application_t* handle, SIM_fn_prototype fn)
{
  handle->user_compute = fn;
}

void sim_register_time_stepper (sim_application_t* handle, SIM_timestepper_prototype fn)
{
  handle->user_time_stepper = fn;
}

void sim_register_cleanup(sim_application_t* handle, SIM_fn_prototype fn)
{
  handle->user_cleanup = fn;
}

void sim_register_begin_iter(sim_application_t* handle, SIM_fn_prototype fn)
{
  handle->user_begin_iter = fn;
}

void sim_register_end_iter(sim_application_t* handle, SIM_fn_prototype fn)
{
  handle->user_end_iter = fn;
}

void sim_register_finalize(sim_application_t* handle, SIM_fn_prototype fn)
{
  handle->user_finalize = fn;
}
