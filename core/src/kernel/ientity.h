#ifndef IENTITY_H_DEF
#define IENTITY_H_DEF

#include "softns.h"
#include <string>

SOFT_BEGIN_NAMESPACE

class IDataModel;
class IEntity
{
public:
   IEntity();
   explicit IEntity (const char *id);
   virtual  ~IEntity() = 0;

   virtual void        save(IDataModel *)                      = 0;
   virtual const char* metaType()                        const = 0;
   virtual std::string id()                              const;
   static  IEntity*    create (const char *uuid);

   static const char *staticMetaType;
private:
   class Private;
   Private *d;
};

SOFT_END_NAMESPACE

#endif // IENTITY_H_DEF
