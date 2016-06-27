#ifndef IENTITY_H_DEF
#define IENTITY_H_DEF

#include "softns.h"
#include <string>
#include <vector>

SOFT_BEGIN_NAMESPACE

class IDataModel;

#define SOFT_ENTITY_METADATA(name,ns,ver)	\
  virtual const char * metaType() const override {return #name;} \
  virtual const char * metaName() const override {return #name;} \
  virtual const char * metaNamespace() const override {return #ns;} \
  virtual const char * metaVersion() const override {return #ver;}

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
  virtual void setId(std::string const &id);
  virtual const char* metaType()                        const = 0;
  virtual const char* metaName()                        const = 0;
  virtual const char* metaNamespace()                   const = 0;
  virtual const char* metaVersion()                     const = 0;
  virtual std::vector<std::string> dimensions()         const = 0;

  virtual int getDimensionSize(std::string const &dim) const;

private:
  class Private;
  Private *d;
};

SOFT_END_NAMESPACE

#endif // IENTITY_H_DEF
