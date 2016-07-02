# Example of using softshell to query a mongodb database


	$ softshell
	SOFT v5.1.5 (GNU LESSER GENERAL PUBLIC LICENSE (v 2.1, February 1999))

	For help, type :help


	> var client = new MongoClient("mongodb://localhost");
	> var collection = client.collection("porto", "demo");
	> var cursor = collection.find({"uuid":"{36f217eb-bdfc-4a52-88c3-861ef213b8f7}"});
	> var data = cursor.next();
	> data.asString()
  		{ "_id" : { "$oid" : "5775a163945f3f774e434432" }, "name" : "", "version" : "", "namespace" : "", "uuid" : "{36f217eb-bdfc-4a52-88c3-861ef213b8f7}", "properties" : { "filename" : "remarc.in", "suffix" : "in", "size" : 628, "data" : { "$type" : "00", "$binary" : "NjAwLiAgICNUX21pbiAoSykKNTAwMC4gICNUX21heCAoSykKMTAuICAgI0RlbHRhX1QgKEspCjAuMiAgICAjZXN0aW1hdGVkIHVuY2VydGFpbnR5IGluIGJhcnJpZXIgaGVpZ2h0IChlbGVjdHJvbiB2b2x0cykKMC4xICAgICNlc3RpbWF0ZWQgdW5jZXJ0YWludHkgaW4gYWRzb3JwdGlvbiBlbmVyZ3kgKGVsZWN0cm9uIHZvbHRzKSAKLjUgICAgI0dhcyBwcmVzc3VyZSAoYXRtKQpZICAgICAgI1Nob3VsZCBmcmVxdWVuY2llcyBiZSB1c2VkIHRvIGNhbGN1bGF0ZSByYXRlIGNvbnN0YW50cz8gKFkgb3IgTikKWSAgICAgICNTaG91bGQgZnJlcXVlbmNpZXMgYmUgdXNlZCBmb3IgY2xlYW4gc3VyZmFjZT8gKFkgb3IgTikKUSAgICAgICNRdWFudHVtIChRKSBvciBjbGFzc2ljYWwgKEMpIHZpYnJhdGlvbmFsIHBhcnRpdGlvbiBmdW5jdGlvbnMgZm9yIHJhdGUgY29uc3RhbnQgY2FsY3VhdGlvbnM/CjEuMEUxMyAjUHJlZmFjdG9yIGZvciByYXRlIGNvbnN0YW50IGNhbGN1bGF0aW9ucyAocy0xKSBpZiBmcmVxdWVuY2llcyBhcmUgbm90IHVzZWQKMS4wRTEzICNQcmVmYWN0b3IgZm9yIGRlc29ycHRpb24gcmF0ZSBjb25zdGFudCBjYWxjdWxhdGlvbnMgKHMtMSkgaWYgZnJlcXVlbmNpZXMgYXJlIG5vdCB1c2VkCg==" } }, "dimensions" : {  } }
	> 
