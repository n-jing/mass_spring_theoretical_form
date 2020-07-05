#! /bin/bash


maxima -b edge.mac

mer_exe=merge_gradient_and_hessiane_to_fortran
g++ merge_gradient_and_hessiane_to_fortran.cpp -o ${mer_exe} -DEDGE_GRADIENT_AND_HESSIAN
./${mer_exe} ./edge_energe.f90 ./edge_gradient.f90 ./edge_hessian.f90 6
