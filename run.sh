#! /bin/bash

obj_path=./sparse_bunny_mesh_simp.vtk
stiffness=1000

gen_exe=generate_maxima_script
g++ generate_maxima_script.cpp -o ${gen_exe} -I/usr/local/include/eigen3
var_num=`./${gen_exe} ${obj_path} ${stiffness} | tail -n 1`

if [ "${var_num}" == "0" ]
then
    echo "no vertices"
    exit
fi

maxima -b ms.mac

mer_exe=merge_gradient_and_hessiane_to_fortran
g++ merge_gradient_and_hessiane_to_fortran.cpp -o ${mer_exe}
./${mer_exe} ./ms_gradient.f90 ./ms_hessian.f90 ${var_num}

