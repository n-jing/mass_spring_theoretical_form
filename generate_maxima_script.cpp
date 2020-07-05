#include <iostream>
#include <unordered_set>
#include <map>
#include <Eigen/Core>


#include "key_comparison.h"
#include "read_model.h"

using FLOAT_TYPE = double;
using namespace std;
using namespace Jing;
using namespace Eigen;

int main (int argc, char *argv[])
{
  if (argc != 3)
  {
    printf("usage: exe obj_model_path element_number. \n");
    return -1;
  }
  const char *path = argv[1];
  const double stiffness = atof(argv[2]);
  bool file_exist = false;
  Matrix<FLOAT_TYPE, -1, -1> verts;
  Matrix<size_t, -1, -1> cells;

  if (string(path).find(".vtk") != string::npos)
    tie(file_exist, verts, cells) = ReadTetHex<4, FLOAT_TYPE, size_t>(path);
  else if (string(path).find(".obj") != string::npos)
    tie(file_exist, verts, cells) = ReadObj<FLOAT_TYPE, size_t>(path);
  else
  {
    printf("[  \033[1;31merror\033[0m  ] error in unsupported file type\n");
    return -1;
  }

  EXITIF(!file_exist, "model read");
  FILE* ms_maxima = fopen("ms.mac", "w");
  if (!ms_maxima)
  {
    printf("[  \033[1;31merror\033[0m  ] error in file open\n0\n");
    return -1;
  }
  fprintf(ms_maxima, "load(\"f90\")$\nload (\"vect\")$\nE : ");
  unordered_set<array<int, 2>, UnorderedKeyHash<array<int, 2>>, UnorderedKeyEqual<array<int, 2>>> edge_set;
  const int f_num = cells.rows();
  const int vert_num = verts.rows();
  for (int f = 0; f < f_num; ++f)
  {
    for (int e = 0; e < 3; ++e)
    {
      array<int, 2> edge = {cells(f, e), cells(f, (e+1)%3)};
      if (edge_set.count(edge))
        continue;
      edge_set.insert(edge);
      FLOAT_TYPE l0 = (verts.row(edge[0]) - verts.row(edge[1])).norm();
      fprintf(ms_maxima, "+1/2*%lf*(", stiffness / l0);
      fprintf(ms_maxima, "sqrt(");
      for (int a = 1; a <= 3; ++a)
        fprintf(ms_maxima, "+(x(%d)-x(%d))^2", 3*edge[0]+a, 3*edge[1]+a);
      fprintf(ms_maxima, ")-%lf)^2", l0);
    }
  }

  fprintf(ms_maxima, "$\nH:hessian(E, [");
  for (int v = 1; v < 3*vert_num; ++v)
  {
    fprintf(ms_maxima, "x(%d),", v);
  }
  fprintf(ms_maxima, "x(%d)])$\n", 3*vert_num);
  for (int v = 1; v <= 3*vert_num; ++v)
  {
    fprintf(ms_maxima, "pg[%d]:diff(E, x(%d))$\n", v, v);
  }
  fprintf(ms_maxima, "g:listarray(pg)$\n");
  fprintf(ms_maxima, "with_stdout(\"%s\", f90(H))$\n", "ms_hessian.f90");
  fprintf(ms_maxima, "with_stdout(\"%s\", f90(g))$\n", "ms_gradient.f90");
  fclose(ms_maxima);
  printf("[   \033[1;37mlog\033[0m   ] variable number:%d\n", 3*vert_num);

  printf("%d\n", 3*vert_num);
  return 0;
}
