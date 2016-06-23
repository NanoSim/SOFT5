#ifndef COLLECTION_H_DEF
#define COLLECTION_H_DEF

#include <string>
#include <list>
#include "softns.h"
#include "ientity.h"
#include <memory>

SOFT_BEGIN_NAMESPACE

// TODO: Needed? Should be a part of tripletstore?
struct RelationTriplet {
};

class Collection : public IEntity
{
public:
  SOFT_ENTITY_METADATA("Collection", "http://emmc.eu/TR/metadata-entity", "1.0-SNAPSHOT-4")

  Collection();
  explicit Collection(std::string const &id);
  explicit Collection(const IEntity *ptr);
  explicit Collection(IDataModel const *dm);
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
  void attachEntity(std::string const &label, IEntity *entity);


  void addDim(std::string const &label,
              std::string const &description = std::string());
  // TODO: Not fully implemented.
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
  int numRelations() const;

  IEntity const *findInstance(std::string const &label) const;
  std::list<RelationTriplet> findRelations(std::string const &subject) const;

  virtual void save (IDataModel *) const override;
  virtual void load (IDataModel const *) override;

  virtual std::vector<std::string> dimensions() const override;

private:
  class Private;
  Private *d;
};

SOFT_END_NAMESPACE


#endif
