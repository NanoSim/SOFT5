module demo
  use iso_c_binding
  use sim_handle_module
  use user_ctx_module
  use simple
  implicit none

contains
  subroutine demo_compute(handle, user_data) bind(c)
    use iso_c_binding
    implicit none
    type(c_ptr), value        :: handle
    type(c_ptr), value        :: user_data
    type(sim_handle), pointer :: hdl
    type(user_ctx), pointer   :: ctx
    type(TSimple)             :: simple
    !
    call c_f_pointer(handle, hdl)
    call c_f_pointer(hdl%user_context, ctx)
    print *, 'hello from demo-compute', ctx%a

    ! Setup entity 
    simple = TSimple(ctx%simple)
    print *, simple%matrix(1,1)
  end subroutine demo_compute
end module demo
