/* -*- C -*-  (not really, but good for syntax highlighting) */

/*
 * C code
 */    
%{
#define CK_ALLOC(alloc) if (!(alloc)) do {	 \
    PyErr_SetString(PyExc_MemoryError, # alloc); \
    goto fail;					 \
  } while (0)
  
  /* Mirror the opaque definition in softc-entity.c */
  typedef struct _softpy_entity_t {

    /* Header common to all softc-entity_t, see softc-entity.c */
    struct softc_entity_vtable_ *vtable_;
    const char *id;

    PyObject *this;  // Reference to python pointer object to this entity

    /* References to Python callables */
    PyObject *get_meta_name;
    PyObject *get_meta_version;
    PyObject *get_meta_namespace;
    PyObject *get_dimensions;
    PyObject *get_dimension_size;
    PyObject *store;
    PyObject *load;

    /* Entity data */
    const char *name;
    const char *version;
    const char *namespace;
    size_t ndims;
    const char **dims;
    size_t *dim_sizes;
  } softpy_entity_t;

  

  /* Help function setting self->ndims and self->dims.  Returns
   * non-zero on error. */
  static int softpy_set_dimensions(softpy_entity_t *self, PyObject *seq)
  {
    int i, n;
    PyObject *label=NULL;
    assert(PySequence_Check(seq));

    for (i=0; i<self->ndims; i++) free((char *)self->dims[i]);
    self->ndims = 0;

    n = PySequence_Length(seq);
    CK_ALLOC(self->dims = realloc(self->dims, n * sizeof(char *)));

    for (self->ndims=0; self->ndims < n; self->ndims++) {
      if (!(label = PySequence_GetItem(seq, self->ndims))) goto fail;
      if (!PyString_Check(label)) {
	PyErr_SetString(PyExc_TypeError,"dimension labels must be strings");
	goto fail;
      }
      CK_ALLOC(self->dims[self->ndims] = strdup(PyString_AsString(label)));
      Py_DECREF(label);
      label = NULL;
    }
    assert(self->ndims == n);
    return 0;
  fail:
    if (label) Py_DECREF(label);
    return 1;
  }

  /* Help function setting self->ndims and self->dim_sizes.  Returns
   * non-zero on error. */
  static int softpy_set_dimension_size(softpy_entity_t *self, PyObject *seq)
  {
    int i, n;
    PyObject *item=NULL;

    assert(PySequence_Check(seq));
    n = PySequence_Length(seq);
    if ((!self->get_dimensions) && self->ndims != n) {
	PyErr_SetString(PyExc_TypeError,
			"length of sequence must match number of dimensions");
	goto fail;
    }
    CK_ALLOC(self->dim_sizes = realloc(self->dim_sizes, n * sizeof(char *)));
    for (i=0; i<n; i++) {
      if (!(item = PySequence_GetItem(seq, i))) goto fail;
      if (!PyInt_Check(item)) {
	PyErr_SetString(PyExc_TypeError,"dimension sizes must be ints");
	goto fail;
      }
      self->dim_sizes[i] = PyInt_AsLong(item);
    }
    return 0;
  fail:
    if (item) Py_DECREF(item);
    return 1;
  }



  //const char *softpy_get_meta_name(const softc_entity_t *self) {
  //  return self->meta_name;
  //}
  static const char *softpy_get_meta_name() {
    return "XXX";
  }

  static const char *softpy_get_meta_version() {
    return "XXX";
  }

  static const char *softpy_get_meta_namespace() {
    return "XXX";
  }


  /* Returns a pointer to a array of pointers to dimension labels.  On
   * return `size` is set to the length of the array. The returned
   * array should not be free'ed. 
   * On error NULL is returned. */
  static const char **softpy_get_dimensions(const softc_entity_t *ptr, 
					    size_t *size) 
  {
    softpy_entity_t *self = (softpy_entity_t *)ptr;
    PyObject *args=NULL, *seq=NULL;

    if (self->get_dimensions) {
      assert(PyCallable_Check(self->get_dimensions));
      
      if (!(args = Py_BuildValue("(O)", self->this))) goto fail;
      assert(PyTuple_Check(args));
      
      if (!(seq = PyObject_CallObject(self->get_dimensions, args))) goto fail;
      if (!PySequence_Check(seq)) {
	PyErr_SetString(PyExc_TypeError, "callable must return a sequence "
			"of dimension labels");
	goto fail;
      }
      softpy_set_dimensions(self, seq);
      Py_DECREF(seq);
      Py_DECREF(args);
    }

    *size = self->ndims;
    return self->dims;

 fail:
    if (seq) Py_DECREF(seq);
    if (args) Py_DECREF(args);
    return NULL;
  }


  /* Returns the size of dimension `label` or -1 on error. */
  static int softpy_get_dimension_size(const softc_entity_t *ptr, 
				       const char *label) 
  {
    int i, size;
    const softpy_entity_t *self = (const softpy_entity_t *)ptr;
    PyObject *args=NULL, *pysize=NULL;
    
    if (self->get_dimension_size) {
      assert(PyCallable_Check(self->get_dimension_size));

      if (!(args = Py_BuildValue("Os", self->this, label))) goto fail;
      assert(PyTuple_Check(args));

      if (!(pysize = PyObject_CallObject(self->get_dimension_size, args))) 
	goto fail;
      if (!PyInt_Check(pysize)) {
	PyErr_SetString(PyExc_TypeError, "callable must return an int");
	goto fail;
      }
      size = PyInt_AsLong(pysize);
      Py_DECREF(pysize);
      Py_DECREF(args);
      return size;
    } else {
      size_t ndims;
      const char **dims = softpy_get_dimensions(ptr, &ndims);

      assert(self->dim_sizes);
      for (i=0; i<ndims; i++)
	if (strcmp(dims[i], label) == 0) return self->dim_sizes[i];
      return -1;
    }

  fail:
    if (pysize) Py_DECREF(pysize);
    if (args) Py_DECREF(args);
    return -1;
  }
  

  static void softpy_store_or_load(const softc_entity_t *ptr,
				   softc_datamodel_t *model, int storing) 
  {
    const softpy_entity_t *self = (const softpy_entity_t *)ptr;
    PyObject *args=NULL, *pymodel=NULL, *retval=NULL;

    if ((pymodel = SWIG_NewPointerObj(SWIG_as_voidptr(model),
				      SWIGTYPE_p_softc_datamodel_t, 0))) 
      goto fail;
    if (!(args = Py_BuildValue("OO", self->this, pymodel))) goto fail;
    assert(PyTuple_Check(args));

    if (storing) {
      if (!(retval = (PyObject_CallObject(self->store, args)))) goto fail;
    } else {
      if (!(retval = (PyObject_CallObject(self->load, args)))) goto fail;
    }
    
  fail:
    if (retval) Py_DECREF(retval);
    if (pymodel) Py_DECREF(pymodel);
    if (args) Py_DECREF(args);
    if (retval) Py_DECREF(pymodel);
  }


  static void softpy_store(const softc_entity_t *ptr, softc_datamodel_t *model)
  {
    const softpy_entity_t *self = (const softpy_entity_t *)ptr;
    assert(self->store);
    if (!PyCallable_Check(self->store)) {
      PyErr_SetString(PyExc_TypeError,"`store` must be callable");
      return;
    }
    softpy_store_or_load(ptr, model, 1);
  }

  static void softpy_load(softc_entity_t *ptr, const softc_datamodel_t *model)
  {
    const softpy_entity_t *self = (const softpy_entity_t *)ptr;
    assert(self->load);
    if (!PyCallable_Check(self->load)) {
      PyErr_SetString(PyExc_TypeError,"`load` must be callable");
      return;
    }
    softpy_store_or_load(ptr, (softc_datamodel_t *)model, 0);
  }


  softc_entity_t *new_softc_entity_t(PyObject *meta_name,
				     PyObject *meta_version,
				     PyObject *meta_namespace,
				     PyObject *dimensions, 
				     PyObject *dimension_size, 
				     PyObject *store, 
				     PyObject *load,
				     const char *id) 
  {
    softpy_entity_t *self=NULL;
    CK_ALLOC(self = calloc(1, sizeof(softpy_entity_t)));
    CK_ALLOC(self->vtable_ = calloc(1, sizeof(softc_entity_vtable)));

    self->vtable_->get_meta_name = softpy_get_meta_name;
    self->vtable_->get_meta_version = softpy_get_meta_version;
    self->vtable_->get_meta_namespace = softpy_get_meta_namespace;
    self->vtable_->get_dimensions = softpy_get_dimensions;
    self->vtable_->get_dimension_size = softpy_get_dimension_size;
    self->vtable_->store = softpy_store;
    self->vtable_->load = softpy_load;

    self->id = (id) ? id : softc_uuidgen();

    self->this = SWIG_NewPointerObj(SWIG_as_voidptr(self), 
    				    SWIGTYPE_p_softc_entity_t, 0);
    
    if (PyCallable_Check(meta_name)) {
      self->get_meta_name = meta_name;
      Py_INCREF(meta_name);
    } else {
      CK_ALLOC(self->name = strdup(PyString_AsString(meta_name)));
    }

    if (PyCallable_Check(meta_version)) {
      self->get_meta_version = meta_version;
      Py_INCREF(meta_version);
    } else {
      CK_ALLOC(self->version = strdup(PyString_AsString(meta_version)));
    }

    if (PyCallable_Check(meta_namespace)) {
      self->get_meta_namespace = meta_namespace;
      Py_INCREF(meta_namespace);
    } else {
      CK_ALLOC(self->namespace = strdup(PyString_AsString(meta_namespace)));
    }

    if (PyCallable_Check(dimensions)) {
      self->get_dimensions = dimensions;
      Py_INCREF(dimensions);
    } else if (PySequence_Check(dimensions)) {
      if (softpy_set_dimensions(self, dimensions)) goto fail;
    } else {
      PyErr_SetString(PyExc_TypeError, 
		      "`dimensions` must be a callable or sequence");
      goto fail;
    }

    if (PyCallable_Check(dimension_size)) {
      self->get_dimension_size = dimension_size;
      Py_INCREF(dimension_size);
    } else if (PySequence_Check(dimension_size)) {
      if (softpy_set_dimension_size(self, dimension_size)) goto fail;
    } else {
      PyErr_SetString(PyExc_TypeError, 
		      "`dimension_size` must be a callable or sequence");
      goto fail;
    }

    if (PyCallable_Check(store)) {
      self->store = store;
      Py_INCREF(store);
    } else {
      PyErr_SetString(PyExc_TypeError, "`store` must be callable");
      goto fail;
    }

    if (PyCallable_Check(load)) {
      self->load = load;
      Py_INCREF(load);
    } else {
      PyErr_SetString(PyExc_TypeError, "`load` must be callable");
      goto fail;
    }

    return (softc_entity_t *)self;

  fail:
    if (self) {
      if (self->vtable_) free(self->vtable_);
      free(self);
    }
    return NULL;
  }


  static void delete_softc_entity_t(softc_entity_t *ptr) {
    softpy_entity_t *self = (softpy_entity_t *)ptr;

    if (self->get_meta_name) Py_DECREF(self->get_meta_name);
    if (self->get_meta_version) Py_DECREF(self->get_meta_version);
    if (self->get_meta_namespace) Py_DECREF(self->get_meta_namespace);
    if (self->get_dimensions) Py_DECREF(self->get_dimensions);
    if (self->get_dimension_size) Py_DECREF(self->get_dimension_size);
    Py_DECREF(self->store);
    Py_DECREF(self->load);

    free(self->vtable_);
    free((char *)self->id);
    Py_DECREF(self->this);  

    if (self->name) free((char *)self->name);
    if (self->version) free((char *)self->version);
    if (self->namespace) free((char *)self->namespace);
    if (self->dims) {
      int i;
      for (i=0; i<self->ndims; i++) free((char *)self->dims[i]);
      free(self->dims);
    }
    if (self->dim_sizes) free(self->dim_sizes);

    free(self);
  }

  static const char *softc_entity_t_id_get(softc_entity_t *self) {
    return ((softpy_entity_t *)self)->id;
  }

  static const char *softc_entity_t_name_get(softc_entity_t *self) {
    //return softc_entity_get_meta_name(self);
    return ((softpy_entity_t *)self)->name;
  }

  static const char *softc_entity_t_version_get(softc_entity_t *self) {
    //return softc_entity_get_meta_version(self);
    return ((softpy_entity_t *)self)->version;
  }

  static const char *softc_entity_t_namespace_get(softc_entity_t *self) {
    //return softc_entity_get_meta_namespace(self);
    return ((softpy_entity_t *)self)->namespace;
  }

  // Wrapper for softc_entity_get_dimensions() that returns a NULL-terminates
  // string list.
  const char **entity_get_dimensions(const softc_entity_t *ptr)
  {
    int i;
    size_t size;
    const char **s = softc_entity_get_dimensions(ptr, &size);
    const char **new = malloc((size + 1) * sizeof(char *));
    for (i=0; i<size; i++) new[i] = strdup(s[i]);
    new[size] = NULL;
    return new;
  }

%}


/*
 * SWIG declarations
 */    

%exception softc_entity_t {
  $action
  if (!result) SWIG_fail;
}

/* Keep softc_entity_t opaque, also to python */
typedef struct {
  %extend {
    softc_entity_t(PyObject *get_meta_name,
		   PyObject *get_meta_version,
		   PyObject *get_meta_namespace,
		   PyObject *get_dimensions, 
		   PyObject *get_dimension_size, 
		   PyObject *store, 
		   PyObject *load,
		   const char *id=NULL);
    ~softc_entity_t();

    %immutable;
    const char *id;
    const char *name;
    const char *version;
    const char *namespace;
    %mutable;
  }
} softc_entity_t;




//softc_entity_t *softc_entity_new(const char *uri);
const char  *softc_entity_get_id(const softc_entity_t *self);
const char  *softc_entity_get_meta_type(const softc_entity_t *self);
const char  *softc_entity_get_meta_name(const softc_entity_t *self);
const char  *softc_entity_get_meta_namespace(const softc_entity_t *self);
const char  *softc_entity_get_meta_version(const softc_entity_t *self);  
int          softc_entity_get_dimension_size(const softc_entity_t *self, 
					     const char *label);
void         softc_entity_store(const softc_entity_t *self, 
				softc_datamodel_t *model);
void         softc_entity_load(softc_entity_t *self, 
			       const softc_datamodel_t *model);

/* Special handeling of softc_entity_get_dimensions() */
const char **entity_get_dimensions(const softc_entity_t *self);
