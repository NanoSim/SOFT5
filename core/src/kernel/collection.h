#ifndef COLLECTION_H_DEF
#define COLLECTION_H_DEF

#include <string>
#include <list>
#include "softns.h"
#include "ientity.h"

SOFT_BEGIN_NAMESPACE

class Collection;

class RelationTriplet
{
public:
  public:
  RelationTriplet(std::string const &s,
		  std::string const &p,
		  std::string const &o);

  std::string subject() const;
  std::string predicate() const;
  std::string object() const;
private:
  friend class Collection;
  class Private;
  Private *d;    
};

typedef struct DimMap_ DimMap;
typedef struct Dim_ Dim;
typedef struct EntityRef_ EntityRef;

class Collection : public IEntity
{
public:
  SOFT_ENTITY_METADATA("Collection", "http://emmc.eu/TR/metadata-entity", "1.0-SNAPSHOT-4")
  
  Collection();
  explicit Collection(std::string const &id);
  explicit Collection(const IEntity *ptr);
  virtual ~Collection();

  std::string name() const;
  std::string version() const;

  static IEntity* create (std::string const &uuid = std::string());
  void setName(std::string const &name);
  void setVersion(std::string const &version);
  void registerEntity(std::string const &label, IEntity const *entity);		      
  void addEntity(std::string const &label,
		 std::string const &name,
		 std::string const &version,
		 std::string const &ns,
		 std::string const &uuid);

  void addDim(std::string const &label,
	      std::string const &description = std::string());
  void connect(std::string const &subject,
		   std::string const &predicate,
		   std::string const &object);
  
  void addRelation(std::string const &subject,
		   std::string const &predicate,
		   std::string const &object);
  void addDimMap(std::string const &label,
		 std::string const &entityDim,
		 std::string const &collectionDim);
  
  int numEntities() const;
  int numDims() const;
  int numRelations() const;
  int numDimMaps() const;

  IEntity const *findInstance(std::string const &label) const;
  std::list<RelationTriplet> findRelations(std::string const &subject) const;

  virtual void save (IDataModel *) const override;
  virtual void load (IDataModel const *) override;

 protected:
  std::list<DimMap> dimMapList() const;
  std::list<RelationTriplet> relationList() const;
  std::list<Dim> dimList() const;
  std::list<EntityRef> entityList() const;

private:
  class Private;
  Private *d;
};

SOFT_END_NAMESPACE


#endif 
