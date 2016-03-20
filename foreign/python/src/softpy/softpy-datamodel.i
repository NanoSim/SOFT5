/* -*- C -*-  (not really, but good for syntax highlighting) */



/* Redefine ARGOUTVIEWM_ARRAY1 to avoid appending output reults */
%typemap(argout,
         fragment="NumPy_Backward_Compatibility,NumPy_Utilities")
  (DATA_TYPE** ARGOUTVIEWM_ARRAY1, DIM_TYPE* DIM1)
{
  npy_intp dims[1] = { *$2 };
  PyObject* obj = PyArray_SimpleNewFromData(1, dims, DATA_TYPECODE, (void*)(*$1));
  PyArrayObject* array = (PyArrayObject*) obj;

  if (!array) SWIG_fail;

%#ifdef SWIGPY_USE_CAPSULE
    PyObject* cap = PyCapsule_New((void*)(*$1), SWIGPY_CAPSULE_NAME, free_cap);
%#else
    PyObject* cap = PyCObject_FromVoidPtr((void*)(*$1), free);
%#endif

%#if NPY_API_VERSION < 0x00000007
  PyArray_BASE(array) = cap;
%#else
  PyArray_SetBaseObject(array,cap);
%#endif
   
  //$result = SWIG_Python_AppendOutput($result,obj);
  $result = obj;
}


/* Typemaps for input array_double_2d */
%typemap(in,numinputs=1) (double **IN_ARRAY2D, size_t DIM1, size_t DIM2)
  (PyArrayObject *arr=NULL, int is_new_object)
{
  // typemap(in,numinputs=1) (double **IN_ARRAY2D, size_t DIM1, size_t DIM2)
  int i;
  if (!(arr = obj_to_array_allow_conversion($input, NPY_DOUBLE,
					    &is_new_object))) SWIG_fail;
  if (array_numdims(arr) != 2)
    SWIG_exception_fail(SWIG_TypeError, "requires 2d array");
  $2 = array_size(arr, 0);
  $3 = array_size(arr, 1);
  if (!($1 = malloc($2 * sizeof(double *))))
    SWIG_exception_fail(SWIG_MemoryError, "allocation failure");
  for (i=0; i<$2; i++) $1[i] = (double *)((char *)array_data(arr) + i * array_stride(arr, 0));

  printf("*** dims:    %2lu, %lu\n", $2, $3);
  printf("*** strides: %2lu, %lu\n", array_stride(arr, 0), array_stride(arr, 1));
  for (i=0; i<$2; i++) {
    int j;
    for (j=0; j<$3; j++) {
      printf("   %d,%d ->%-4.1f (%ld)\n", i, j, $1[i][j], (char *)(&$1[i][j]) - (char *)array_data(arr));
    }
  }
  
}
%typemap(freearg) (double **IN_ARRAY2D, size_t DIM1, size_t DIM2)
{
  // typemap(freearg) (double **IN_ARRAY2D, size_t DIM1, size_t DIM2)
  if ($1) free($1);
  if (arr$argnum && is_new_object$argnum) Py_DECREF(arr$argnum);
}


/* Typemaps for input array_double_3d */
%typemap(in,numinputs=1) (double ***IN_ARRAY3D, size_t DIM1, size_t DIM2, size_t DIM3)
  (PyArrayObject *arr=NULL, double **ptr=NULL, int is_new_object)
{
  // typemap(in,numinputs=1) (double ***IN_ARRAY3D, size_t DIM1, size_t DIM2, size_t DIM3)
  int i, j;
  if (!(arr = obj_to_array_allow_conversion($input, NPY_DOUBLE,
					    &is_new_object))) return NULL;
  if (array_numdims(arr) != 3)
    SWIG_exception_fail(SWIG_TypeError, "requires 3d array");
  $2 = array_size(arr, 0);
  $3 = array_size(arr, 1);
  $4 = array_size(arr, 2);
  if (!($1 = malloc($2 * sizeof(double *))))
    SWIG_exception_fail(SWIG_MemoryError, "error allocating $1");
  if (!(ptr = malloc($2 * $3 * sizeof(double *))))
    SWIG_exception_fail(SWIG_MemoryError, "error allocating ptr$argnum");

  printf("*** strides: %lu, %lu, %lu\n", array_stride(arr, 0),
	 array_stride(arr, 1), array_stride(arr, 2));

  for (i=0; i<$2; i++) {
    for (j=0; j<$3; j++)
      ptr[i*$2+j] = (double *)((char *)array_data(arr) +
			       i*array_stride(arr, 0) + j*array_stride(arr, 1));
    $1[i] = &ptr[i*$2];
  }


  for (i=0; i<$2; i++) {
    for (j=0; j<$3; j++) {
      int k;
      for (k=0; k<$4; k++) {
	printf("   %d,%d,%d -> %g\n", i, j, k, $1[i][j][k]);
      }
    }
  }
  
}
%typemap(freearg) (double ***IN_ARRAY3D, size_t DIM1, size_t DIM2, size_t DIM3)
{
  // typemap(freearg) (double ***IN_ARRAY3D, size_t DIM1, size_t DIM2, size_t DIM3)
  if ($1) free($1);
  if (ptr$argnum) free(ptr$argnum);
  if (arr$argnum && is_new_object$argnum) Py_DECREF(arr$argnum);
}


/* Typemap for argout array_double_2d */
%typemap(in,numinputs=0) (double ***value, size_t *size_i, size_t *size_j)
  (double **v, size_t ni, size_t nj)
{
  $1 = &v;
  $2 = &ni;
  $3 = &nj;
}
%typemap(argout) (double ***value, size_t *size_i, size_t *size_j)
{
  int i, j;
  npy_intp dims[] = {*$2, *$3};
  PyObject *arr = PyArray_SimpleNew(2, dims, NPY_DOUBLE);
  for (i=0; i<*$2; i++) {
    for (j=0; j<*$3; j++) {
      double v = (*$1)[i][j];
      *((double *)((char *)array_data(arr) +
		   i*array_stride(arr, 0) + j*array_stride(arr, 1))) = v;
    }
  }
  $result = arr;
}


// Create numpy typemaps
%numpy_typemaps(unsigned char, NPY_UBYTE,  size_t)
%numpy_typemaps(int32_t,       NPY_INT32,  size_t)
%numpy_typemaps(double,        NPY_DOUBLE, size_t)

// Wrap softc_datamodel_append_*()
%apply (char       **STRING_LIST, size_t LEN)                            {(const char    **value, size_t n_elements)};
%apply (unsigned char *IN_ARRAY1, size_t DIM1)                           {(unsigned char  *value, size_t length)};
%apply (int32_t       *IN_ARRAY1, size_t DIM1)                           {(const int32_t  *value, size_t size)};
%apply (double        *IN_ARRAY1, size_t DIM1)                           {(const double   *value, size_t size)};
%apply (double       **IN_ARRAY2D, size_t DIM1, size_t DIM2)              {(const double  **value, size_t size_i, size_t size_j)};
%apply (double      ***IN_ARRAY3D, size_t DIM1, size_t DIM2, size_t DIM3) {(const double ***value, size_t size_i, size_t size_j, size_t size_k)};
bool softc_datamodel_append_string         (softc_datamodel_t *model, const char *key, const char *value);
bool softc_datamodel_append_int8           (softc_datamodel_t *model, const char *key, int8_t value);
bool softc_datamodel_append_uint8          (softc_datamodel_t *model, const char *key, uint8_t value);
bool softc_datamodel_append_int16          (softc_datamodel_t *model, const char *key, int16_t value);
bool softc_datamodel_append_uint16         (softc_datamodel_t *model, const char *key, uint16_t value);
bool softc_datamodel_append_int32          (softc_datamodel_t *model, const char *key, int32_t value);
bool softc_datamodel_append_uint32         (softc_datamodel_t *model, const char *key, uint32_t value);
bool softc_datamodel_append_int64          (softc_datamodel_t *model, const char *key, int64_t value);
bool softc_datamodel_append_uint64         (softc_datamodel_t *model, const char *key, uint64_t value);
bool softc_datamodel_append_float          (softc_datamodel_t *model, const char *key, float value);
bool softc_datamodel_append_double         (softc_datamodel_t *model, const char *key, double value);
bool softc_datamodel_append_bool           (softc_datamodel_t *model, const char *key, bool value);
bool softc_datamodel_append_blob           (softc_datamodel_t *model, const char *key, unsigned char  *value, size_t length);
bool softc_datamodel_append_string_list    (softc_datamodel_t *model, const char *key, const char **value, size_t n_elements);
bool softc_datamodel_append_array_int32    (softc_datamodel_t *model, const char *key, const int32_t  *value, size_t size);
bool softc_datamodel_append_array_double   (softc_datamodel_t *model, const char *key, const double   *value, size_t size);
bool softc_datamodel_append_array_double_2d(softc_datamodel_t *model, const char *key, const double  **value, size_t size_i, size_t size_j);
bool softc_datamodel_append_array_double_3d(softc_datamodel_t *model, const char *key, const double ***value, size_t size_i, size_t size_j, size_t size_k);
%clear (char         **value, size_t n_elements);
%clear (unsigned char *value, size_t length);
%clear (int32_t       *value, size_t size);
%clear (double        *value, size_t size);
%clear (double       **value, size_t size_i, size_t size_j);
%clear (double      ***value, size_t size_i, size_t size_j, size_t size_k);


%typemap(in,numinputs=0) char **value (char *temp)        { $1 = &temp; };
%typemap(argout)         char **value                     { $result = PyString_FromString(*$1); };

%typemap(in,numinputs=0) int8_t *value (int8_t temp)      { $1 = &temp; };
%typemap(argout)         int8_t *value                    { $result = PyInt_FromLong(*$1); };

%typemap(in,numinputs=0) uint8_t *value (uint8_t temp)    { $1 = &temp; };
%typemap(argout)         uint8_t *value                   { $result = PyInt_FromLong(*$1); };

%typemap(in,numinputs=0) int16_t *value (int16_t temp)    { $1 = &temp; };
%typemap(argout)         int16_t *value                   { $result = PyInt_FromLong(*$1); };

%typemap(in,numinputs=0) uint16_t *value (uint16_t temp)  { $1 = &temp; };
%typemap(argout)         uint16_t *value                  { $result = PyInt_FromLong(*$1); };

%typemap(in,numinputs=0) int32_t *value (int32_t temp)    { $1 = &temp; };
%typemap(argout)         int32_t *value                   { $result = PyInt_FromLong(*$1); };

%typemap(in,numinputs=0) uint32_t *value (uint32_t temp)  { $1 = &temp; };
%typemap(argout)         uint32_t *value                  { $result = PyInt_FromLong(*$1); };

%typemap(in,numinputs=0) int64_t *value (int64_t temp)    { $1 = &temp; };
%typemap(argout)         int64_t *value                   { $result = PyInt_FromLong(*$1); };

%typemap(in,numinputs=0) uint64_t *value (uint64_t temp)  { $1 = &temp; };
%typemap(argout)         uint64_t *value                  { $result = PyLong_FromUnsignedLong(*$1); };

%typemap(in,numinputs=0) float *value (float temp)        { $1 = &temp; };
%typemap(argout)         float *value                     { $result = PyFloat_FromDouble(*$1); };

%typemap(in,numinputs=0) double *value (double temp)      { $1 = &temp; };
%typemap(argout)         double *value                    { $result = PyFloat_FromDouble(*$1); };

%typemap(in,numinputs=0) bool *value (bool temp)          { $1 = &temp; };
%typemap(argout)         bool *value                      { $result = PyBool_FromLong(*$1); };


%apply (char            ***OUT_STRING_LIST, size_t *LEN)  {(char         ***value, size_t *n_elements)};
%apply (unsigned char **ARGOUTVIEWM_ARRAY1, size_t *DIM1) {(unsigned char **value, size_t *size)};
%apply (int32_t       **ARGOUTVIEWM_ARRAY1, size_t *DIM1) {(int32_t       **value, size_t *size)};
%apply (double        **ARGOUTVIEWM_ARRAY1, size_t *DIM1) {(double        **value, size_t *size)};
//%apply (double        **ARGOUTVIEWM_ARRAY2, size_t *DIM1, size_t *DIM2) {(double       ***value, size_t *size_i, size_t *size_j)};
//%apply (double        **ARGOUTVIEWM_ARRAY3, size_t *DIM1, size_t *DIM2, size_t *DIM3) {(double        ****value, size_t *size_i, size_t *size_j, size_t *size_k)};
bool softc_datamodel_get_string          (const softc_datamodel_t *model, const char *key, char **value);
bool softc_datamodel_get_int8            (const softc_datamodel_t *model, const char *key, int8_t *value);
bool softc_datamodel_get_uint8           (const softc_datamodel_t *model, const char *key, uint8_t *value);
bool softc_datamodel_get_int16           (const softc_datamodel_t *model, const char *key, int16_t *value);
bool softc_datamodel_get_uint16          (const softc_datamodel_t *model, const char *key, uint16_t *value);
bool softc_datamodel_get_int32           (const softc_datamodel_t *model, const char *key, int32_t *value);
bool softc_datamodel_get_uint32          (const softc_datamodel_t *model, const char *key, uint32_t *value);
bool softc_datamodel_get_int64           (const softc_datamodel_t *model, const char *key, int64_t *value);
bool softc_datamodel_get_uint64          (const softc_datamodel_t *model, const char *key, uint64_t *value);
bool softc_datamodel_get_float           (const softc_datamodel_t *model, const char *key, float *value);
bool softc_datamodel_get_double          (const softc_datamodel_t *model, const char *key, double *value);
bool softc_datamodel_get_bool            (const softc_datamodel_t *model, const char *key, bool *value);
bool softc_datamodel_get_blob            (const softc_datamodel_t *model, const char *key, unsigned char **value, size_t *length);
bool softc_datamodel_get_string_list     (const softc_datamodel_t *model, const char *key, char ***value, size_t *n_elements);
bool softc_datamodel_get_array_int32     (const softc_datamodel_t *model, const char *key, int32_t **value, size_t *size);
bool softc_datamodel_get_array_double    (const softc_datamodel_t *model, const char *key, double **value, size_t *size);
bool softc_datamodel_get_array_double_2d (const softc_datamodel_t *model, const char *key, double ***value, size_t *size_i, size_t *size_j);
//bool softc_datamodel_get_array_double_3d (const softc_datamodel_t *model, const char *key, double ****value, size_t *size_i, size_t *size_j, size_t *size_k);
%clear int8_t *value;
%clear uint8_t *value;
%clear int16_t *value;
%clear uint16_t *value;
%clear int32_t *value;
%clear uint32_t *value;
%clear int64_t *value;
%clear uint64_t *value;
%clear float *value;
%clear double *value;
%clear bool *value;
%clear (char ***value, size_t *n_elements);
%clear (unsigned char **value, size_t *length);
%clear (int32_t **value, size_t *size);
%clear (double **value, size_t *size);
//%clear (double ***value, size_t *size_i, size_t *size_j);
//%clear (double ****value, size_t *size_i, size_t *size_j, size_t *size_k);



bool softc_datamodel_set_id              (softc_datamodel_t* model, const char *id);
bool softc_datamodel_set_meta_name       (softc_datamodel_t* model, const char *meta_name);
bool softc_datamodel_set_meta_version    (softc_datamodel_t* model, const char *meta_version);
bool softc_datamodel_set_meta_namespace  (softc_datamodel_t* model, const char *meta_namespace);
//bool softc_datamodel_set_meta_description(softc_datamodel_t* model, const char *meta_description);
const char * softc_datamodel_get_id              (const softc_datamodel_t* model);
const char * softc_datamodel_get_meta_name       (const softc_datamodel_t* model);
const char * softc_datamodel_get_meta_version    (const softc_datamodel_t* model);
const char * softc_datamodel_get_meta_namespace  (const softc_datamodel_t* model);
//const char * softc_datamodel_get_meta_description(const softc_datamodel_t* model);
