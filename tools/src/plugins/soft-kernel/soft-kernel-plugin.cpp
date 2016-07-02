#include <QtCore>
#include <QtScript>
#include "qentity.h"
#include "qstorage.h"
#include "register.hh"
#include "soft-kernel-plugin.h"


Q_DECLARE_METATYPE (QEntity*)
Q_DECLARE_METATYPE (QStorage*)

SoftKernelPlugin :: ~SoftKernelPlugin()
{}

static QScriptValue createSoftStorage (QScriptContext *ctx, QScriptEngine *engine)
{
  if (ctx->argumentCount() < 2) {
    ctx->throwError("Syntax error");
    return engine->nullValue();
  }
  QString const driver  = ctx->argument(0).toString();
  QString const uri     = ctx->argument(1).toString();
  QString const options = (ctx->argumentCount() >= 3 ? ctx->argument(2).toString() : "");
  auto storage = new QStorage(driver,uri,options);
  return engine->newQObject(storage, QScriptEngine::ScriptOwnership);
}

static QScriptValue createSoftEntity (QScriptContext *ctx, QScriptEngine *engine)
{
  QEntity *entity = nullptr;
  if (ctx->argumentCount() == 0) {
    entity = new QEntity();
  } else if(ctx->argumentCount() == 1) {
    entity = new QEntity(ctx->argument(0).toString());
  } else {
    ctx->throwError("Syntax error");
    return engine->nullValue();
  }
  return engine->newQObject(entity, QScriptEngine::ScriptOwnership);    
}

void SoftKernelPlugin :: registerPlugin (QScriptEngine *engine)
{
  Q_CHECK_PTR (engine);
  auto globalObj = engine->globalObject();
  auto soft = engine->newObject();
  globalObj.setProperty("soft", soft);
  globalObj.setProperty("porto", soft);

  registerConstructor<QStorage> (engine, soft, "Storage", createSoftStorage);
  registerConstructor<QEntity> (engine, soft, "Entity", createSoftEntity);
}

