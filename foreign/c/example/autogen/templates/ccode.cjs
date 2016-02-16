@{
    function type_to_c(metaType) {
	switch(metaType){
	case 'int8':
	    return "int8_t";
	case 'uint8':
	    return "uint8_t";
	case 'int16':
	    return "int16_t";
	case 'uint16':
	    return "uint16_t";
	case 'int':
	case 'integer':
    	case 'int32':
	    return "int32_t";
	case 'uint32':
	    return "uint32_t";
    	case 'int64':
	    return "int64_t";
	case 'uint64':
	    return "uint64_t";
	case 'float':
	    return "float";
	case 'double':
	    return "double";
	case 'bool':
	case 'boolean':
	    return "bool";
	default:
	    return "undefined";
	};
    };
    
    function dims_to_ptr(rank) {
	var ret = "";
	for (var i = 0; i < rank; ++i) {
	    ret += "*";
	}
	return ret;
    };
    
    constructorArgs = (function(){
	var args = ["const char *id"];	
	if (soft.model.dimensions != undefined) {   
	    soft.model.dimensions.forEach(function (d){		
		args.push("size_t " + d.name);		
	    });
	}
	return args;
    })();

    attributes = (function(){
	var attr = [];
	soft.model.properties.forEach(function (entry){
	    var o = {};
	    o.name = entry.name;
	    o.type = type_to_c(entry.type);
	    o.rank = (entry.dims != undefined ? entry.dims.length : 0);
	    o.desc = (entry.description != undefined ? "/* " + entry.description + " */" : "");
	    o.dims = entry.dims;
	    attr.push(o);
	});
	return attr;
    })();

    dataModelAppendFunction = function(entry) {
	var appendFunction = "softc_datamodel_append_";
	switch (entry.type) {
	case 'float':
	case 'double':
	    if (entry.rank > 0) { appendFunction += "array_"; }
	    appendFunction += entry.type;
	    if (entry.rank > 1) {
		appendFunction += "_" + entry.rank + "d";
	    }
	    break;
	case 'int32_t': /* TODO: remove underscore and make general */
	    if (entry.rank > 0) { appendFunction += "array_"; }
	    appendFunction += 'int32';
	    if (entry.rank > 1) {
		appendFunction += "_" + entry.rank + "d";
	    }
	    break;
	default:
	    appendFunction += "undefined";
	};
	return appendFunction;
    };
    
    setDataBlock = (function(){
	setList = [];
	attributes.forEach(function (entry) {
	    var str = dataModelAppendFunction(entry);
	    str += " (data_model, \"" + entry.name + "\", (const " + entry.type + dims_to_ptr(entry.rank) + ")self->props." + entry.name;
	    if (entry.rank > 0) {
		str += ", " + entry.dims.join(",");
	    }
	    str += ");";
	    setList.push(str);
	});
	return setList;
    })();
    
    paren = function(list) {
	var plist = [];
	list.forEach(function(entry){
	    plist.push("("+entry+")");
	});
	return plist;
    };

    attrInitList = (function(){
	var as = [];
	attributes.forEach(function(entry){
	    if (entry.rank == 0) {
		as.push("0");
	    } else {
		as.push("malloc(" + paren(entry.dims).join('*') + "*sizeof(" + entry.type + "))");
	    }
	});
	return as;
    })();

    newAttrInitList = (function(){
	var as = [];
	attributes.forEach(function(entry){
	    if (entry.rank == 0) {
		as.push("0");
	    } else {
		as.push("softc_allocatable_allocatev(" + entry.rank + ", " + paren(entry.dims).join(",") + ")");
//              as.push("malloc(" + paren(entry.dims).join('*') + "*sizeof(" + entry.type + "))");
	    }
	});
	return as;
    })();
	
    attrDeclList = (function(){
	var as = [];
	attributes.forEach(function(entry){
	    as.push(entry.type + " " + dims_to_ptr(entry.rank) + entry.name + ";" + entry.desc);
	});
	return as;
    })();

    dimsList = (function(){
	var ds = [];
	if (soft.model.dimensions != undefined) {
	    soft.model.dimensions.forEach(function(entry){
		ds.push(entry.name);
	    });
	}
	return ds;	
    })();
    
    dimsDeclList = (function(){
	var ds = [];
	if (soft.model.dimensions != undefined) {
	    soft.model.dimensions.forEach(function(entry){
		ds.push("size_t " + entry.name + ";");
	    });
	}
	return ds;
    })();

    dimsSelfDeclList = (function(){
	var ds = [];
	if (soft.model.dimensions != undefined) {
	    soft.model.dimensions.forEach(function(entry){
		ds.push("size_t " + entry.name + " = self->dims." + entry.name + ";");
	    });
	}
	return ds;
    })();

    entity = soft.model.name.toLowerCase();
    ENTITY = soft.model.name.toUpperCase();
    
    undefined;
}/* This file is autgenerated. Do not edit! */


#include <string.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <assert.h>

#include <softc/softc.h>
#include <softc/softc-entity.h>
#include <softc/softc-datamodel.h>
#include <softc/softc-allocatable.h>    

#include "@{entity}.h"

static const char @{ENTITY}_META_TYPE[] = "@soft.model.name:@soft.model.version:@soft.model.namespace";
static const char @{ENTITY}_META_NAME[] = "@soft.model.name";
static const char @{ENTITY}_META_VERSION[] = "@soft.model.version";
static const char @{ENTITY}_META_NAMESPACE[] = "@soft.model.namespace";

struct _@{entity}_dimensions_s
{
  @{dimsDeclList.join("\n  ");}
};

struct _@{entity}_properties_s
{
  @{attrDeclList.join("\n  ");}
};

struct _@{entity}_s {
  const struct softc_entity_vtable_ *vtable_;
  const char *id;
  @{entity}_dimensions_s dims;
  @{entity}_properties_s props;
};

static void store (const softc_entity_t *ptr, softc_datamodel_t *data_model)
{
    const @{entity}_s *self = (const @{entity}_s*)ptr;
    @{dimsSelfDeclList.join("\n    ")};
    @{setDataBlock.join("\n    ");}
}

static void load (softc_entity_t *ptr, const softc_datamodel_t *data_model)
{}

static const char ** get_dimensions(const softc_entity_t *ptr, size_t *size)
{}

static int get_dimension_size(const softc_entity_t *ptr, const char *label)
{}

static const char * get_meta_type()
{
  return @{ENTITY}_META_TYPE;
}

static const char * get_meta_name()
{
  return @{ENTITY}_META_NAME;
}

static const char * get_meta_namespace()
{
  return @{ENTITY}_META_NAMESPACE;
}

static const char * get_meta_version()
{
  return @{ENTITY}_META_VERSION;
}

@{entity}_s *@{entity}_create(@{constructorArgs.join(", ")})
{
  SOFTC_ENTITY_VTABLE(@{entity});
  @{entity}_s *self;
  self = malloc (sizeof *self);
  *self = (@{entity}_s) {SOFTC_ENTITY_VTABLE_NAME(@{entity}),
    softc_uuidgen(),
    (@{entity}_dimensions_s)
    {@{dimsList.join(',')}},
    (@{entity}_properties_s)
    {@{newAttrInitList.join(',\n    ')}
    }
  };
  return self;
}

void @{entity}_free(@{entity}_s *self)
{
  free (self);
}

@{entity}_properties_s *const @{entity}_props (@{entity}_s *self)
{
  return &self->props;
}

@{entity}_dimensions_s *const @{entity}_dims (@{entity}_s *self)
{
  return &self->dims;
}
