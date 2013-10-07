open_project my_adder -reset
set_top my_adder

add_file ./my_adder.cpp
add_file ./ap_interfaces.h
add_file -tb ./my_adder.cpp
 
open_solution "solution1" -reset
add_library  { xilinx/zynq/zynq xilinx/zynq/zynq_fpv6 }

create_clock -period 10
# set_clock_uncertainty 1.25
 
elaborate
autosyn
autoimpl -export -setup -rtl vhdl