#include <iostream>

using namespace std;

int main (int argc, char *argv[])
{
  if (argc != 5)
  {
    printf("usage: exe path_to_energy_file path_to_gradient_file path_to_hessian_file variable_number\n");
    return -1;
  }
  const char *path[] = {argv[1], argv[2], argv[3]};
  char *content[3] = {nullptr, nullptr, nullptr};
  const int var_num = atoi(argv[4]);
  for (int i = 0; i < 3; ++i)
  {
    FILE* f_in = fopen(path[i], "r");
    if (!f_in)
    {
      printf("[  \033[1;31merror\033[0m  ] l%d:error in file open.\n", __LINE__);
      return -1;
    }
    fseek(f_in, 0L, SEEK_END);
    long size = ftell(f_in);
    content[i] = new char[size];
    fseek(f_in, 0L, SEEK_SET);
    rewind(f_in);
    fread(content[i], sizeof(char), size, f_in);
    fclose(f_in);
  }

#ifdef EDGE_GRADIENT_AND_HESSIAN
  char fortran_function_name[] = "edge_gradient_and_hessian";
  char hg_format[] = "SUBROUTINE %s(x, K, l0, E, g, h)\n  REAL*8 x(%d)\n  REAL*8 K\n  REAL*8 l0\n  REAL*8 E\n  REAL*8 g(%d)\n  REAL*8 h(%d,%d)\n";
  FILE *f_out = fopen("edge_gradient_and_hessian.f90", "w");
#else
  char fortran_function_name[] = "ms_gradient_and_hessian";
  char hg_format[] = "SUBROUTINE %s(x, g, h)\n  REAL*8 x(%d)\n  REAL*8 E\n  REAL*8 g(%d)\n  REAL*8 h(%d,%d)\n";
  FILE *f_out = fopen("ms_gradient_and_hessian.f90", "w");
#endif

  fprintf(f_out, hg_format, fortran_function_name, var_num, var_num, var_num, var_num);
  fprintf(f_out, "E = %s", content[0]);
  fprintf(f_out, "%s", content[1]);
  fprintf(f_out, "%s", content[2]);
  fprintf(f_out, "END SUBROUTINE %s", fortran_function_name);
  fclose(f_out);

  delete[] content[0];
  delete[] content[1];
  delete[] content[2];
  return 0;
}

