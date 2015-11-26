/*  -*- Mode: C -*- */

%define DOCSTRING
"Python bindings to SOFT5."
%enddef

%module(docstring=DOCSTRING) entity



%{
 /* Includes the header in the wrapper code */
#include "ientity.h"

#if defined(SOFT_NAMESPACE)
  using SOFT_NAMESPACE::IEntity;
  using SOFT_NAMESPACE::IDataModel;
#endif
%}
  
//%include "ientity.h"

class IEntity
{
public:
  IEntity();
  explicit IEntity (std::string const &id);
  explicit IEntity (IEntity const *other);
  virtual  ~IEntity() = 0;

  virtual void        save(IDataModel *) const                = 0;
  virtual void        load(IDataModel const *)                = 0;
  static  IEntity*    create (const std::string &uuid);

  virtual std::string id()                              const;    
  virtual const char* metaType()                        const = 0;
  virtual const char* metaName()                        const = 0;
  virtual const char* metaNamespace()                   const = 0;
  virtual const char* metaVersion()                     const = 0;

private:
  class Private;
  Private *d;
};
   
