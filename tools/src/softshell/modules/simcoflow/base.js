'use strict';

SimCoFlow  = {};
Collection = {};
Domain     = {};

(function(base){
    Collection = function(id) {
	this._id = id;
    };

    Domain = function() {
	this._foo = 0;
    };
    
    SimCoFlow = function(args) {
	this._init = null;
	this._collection = new Collection('test');
	this._domain     = new Domain();
	print ("Hello");
    };

    SimCoFlow.prototype.registerInit = function (init) {
	this._init = init;
	print ("register init");
    };

    SimCoFlow.prototype.init = function () {
	this._init(this._collection, this._domain);
    };

    return base;
})(exports);
