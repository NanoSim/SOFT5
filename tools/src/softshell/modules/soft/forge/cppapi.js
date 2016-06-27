(function (cppapi){
    cppapi.type_to_cpp = function(metaType, rank) {
	var rank = typeof rank !== 'undefined' ? rank : 0;
	switch(metaType){
	case 'int8':
	    return "StdInt8";
	case 'uint8':
	    return "StdUInt8";
	case 'int16':
	    return "StdInt16";
	case 'uint16':
	    return "StdUInt16";
	case 'int':
	case 'integer':
    	case 'int32':
	    return "StdInt";
	case 'uint32':
	    return "StdUInt";
    	case 'int64':
	    return "StdInt64";
	case 'uint64':
	    return "StdUInt64";
	case 'float':
	    return "StdFloat";
	case 'double':
	    return "StdDouble";
	case 'bool':
	case 'boolean':
	    return "StdBool";
	case 'blob':
	    return "StdBlob";
	case 'string':
	    return "StdString";
	default:
	    return "undefined";
	};
    };
    return cppapi;
})(exports);
