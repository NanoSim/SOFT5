@{
    var capi = require('soft.forge.capi');

    ENTITY = soft.model.name.toUpperCase();
    entity = soft.model.name.toLowerCase();
    Entity = entity.charAt(0).toUpperCase() + entity.slice(1);

    attributes = (function(){
	var attr = [];
	soft.model.properties.forEach(function (entry){
	    var o = {};
	    o.name = entry.name;
	    o.type = capi.type_to_c(entry.type);
	    o.rank = (entry.dims != undefined ? entry.dims.length : 0);
	    o.desc = (entry.description != undefined ? "/* " + entry.description + " */" : "");
	    o.dims = entry.dims;
	    attr.push(o);
	});
	return attr;
    })();

    dataModelStoreFunction = function(entry){
	var getFunction = "dataModel->append";
	switch (entry.type) {
        case 'double':
	    getFunction += "Double";
	    break;
        case 'int32_t':
	    getFunction += "Int32";
	    break;
	default:
	    throw("Unimplemented" + entry.type);
	    break;
        }
	return getFunction;
    };

    dataModelLoadFunction = function(entry){
	var getFunction = "dataModel->get";
	switch (entry.type) {
        case 'double':
	    getFunction += "Double";
	    break;
        case 'int32_t':
	    getFunction += "Int32";
	    break;
	default:
	    throw("Unimplemented type" + entry.type);
	    break;
        }
	return getFunction;
    }

    saveSection = (function(){
	getList = [];
	attributes.forEach( function (entry) {
	    var str = dataModelStoreFunction(entry);
	    str += " (\"" + entry.name + "\", " + entry.name + ");";
	    getList.push(str);
	});
	return getList;
    })();

    loadSection = (function(){
	getList = [];
	attributes.forEach( function (entry) {
	    var str = dataModelLoadFunction(entry);
	    str += " (\"" + entry.name + "\", " + entry.name + ");";
	    getList.push(str);
	});
	return getList;
    })();


    undefined;
}
#include <stdexcept>
#include <idatamodel.h>
#include "@{entity}.h"

SOFT_BEGIN_NAMESPACE
#define NOT_IMPLEMENTED throw std::runtime_error("Not implemented");

class @{Entity} :: Private
{
  friend class @{Entity};
  
};

@{Entity} :: @{Entity} ()
  : IEntity()
  , d(new @{Entity}::Private())
{}

@{Entity} :: @{Entity}(std::string const &id)
  : IEntity(id)
  , d(new @{Entity}::Private())
{}

@{Entity} :: @{Entity}(const IEntity *ptr)
  : IEntity(ptr)
  , d(new @{Entity}::Private())
{}

@{Entity} :: @{Entity}(IDataModel const *dm)
  : IEntity()
  , d(new @{Entity}::Private())
{
  load(dm);
}

@{Entity} :: ~@{Entity} ()
{
  delete d;
}

IEntity* @{Entity} :: create (std::string const &uuid)
{
  NOT_IMPLEMENTED	
  return nullptr;	
}

void @{Entity} :: save (IDataModel *dataModel) const
{
  dataModel->setId(id());

  @{saveSection.join("\n  ");}
}

void @{Entity} :: load (IDataModel const *dataModel)
{
  setId(dataModel->id());

  @{loadSection.join("\n  ");}
}

std::vector<std::string> @{Entity} :: dimensions() const
{
  NOT_IMPLEMENTED	
  return std::vector<std::string>();
}

SOFT_END_NAMESPACE
