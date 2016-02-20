module simple
  use iso_c_binding
  use softf_allocatable_module
  implicit none
  private

  type, bind(c) :: simple_vtable_
     type(c_funptr) :: store
     type(c_funptr) :: load
     type(c_funptr) :: get_dimensions
     type(c_funptr) :: get_dimension_size
     type(c_funptr) :: get_meta_type
     type(c_funptr) :: get_meta_name
     type(c_funptr) :: get_meta_namespace
     type(c_funptr) :: get_meta_version
  end type simple_vtable_
  
  type, bind(c) :: simple_dimensions_s
     integer(c_size_t) :: NI
     integer(c_size_t) :: NJ
  end type simple_dimensions_s

  type, bind(c) :: simple_properties_s
     real(c_double) :: scalar
     type(c_ptr) :: vec
     type(c_ptr) :: matrix
  end type simple_properties_s

  type, bind(c) :: simple_allocatable_s
     type(c_ptr) :: vec
     type(c_ptr) :: matrix
  end type simple_allocatable_s
  
  type, bind(c) :: simple_s
     type(c_ptr) :: vtable
     type(c_ptr) :: id
     type(simple_dimensions_s) :: dims
     type(simple_properties_s) :: props
     type(c_ptr) :: allocs
  end type simple_s

  type TSimpleDims
     integer(c_size_t) :: NI
     integer(c_size_t) :: NJ
  end type TSimpleDims
  
  type TSimple
     type(TSimpleDims)                       :: dimensions
     real(c_double)                          :: scalar
     real(c_double), dimension(:), pointer   :: vec
     real(c_double), dimension(:,:), pointer :: matrix     
  end type TSimple

  interface TSimple
     procedure constructor
  end interface TSimple

  public :: TSimple, TSimpleDims
contains

  function constructor(ref)
    use iso_c_binding
    
    interface
       type(c_ptr) function c_raw_data(alloc) bind(c, name='softc_allocatable_raw_data')
         import :: c_ptr
         type(c_ptr), value :: alloc
       end function c_raw_data
    end interface
    
    type(c_ptr), value :: ref
    type(TSimple) :: constructor
    type(simple_s), pointer :: self
    type(simple_allocatable_s), pointer :: allocs
    type(simple_properties_s), pointer :: props

    call c_f_pointer(ref, self)
    constructor%scalar = self%props%scalar
    constructor%dimensions%NI = self%dims%NI
    constructor%dimensions%NJ = self%dims%NJ

    call c_f_pointer(self%allocs, allocs)
    call c_f_pointer(c_raw_data(allocs%vec), constructor%vec, (/self%dims%NI/))
    call c_f_pointer(c_raw_data(allocs%matrix), constructor%matrix, (/self%dims%NI,(self%dims%NJ-1)/))
        
  end function constructor
    
  
end module simple
