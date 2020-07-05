#ifndef ERROR_DEAL_JJ_H
#define ERROR_DEAL_JJ_H


#include <cstdio>

#ifdef EXITIF
#error
#endif

#ifdef IFERR
#error
#endif

namespace Jing
{

//! \brief exit if op return non zero;
#define EXITIF(OP, M)                                                   \
  {                                                                     \
    if (OP)                                                             \
    {                                                                   \
      printf("[  \033[1;31merror\033[0m  ] error in %s, exit. %s\n", M, #OP); \
      exit(1);                                                          \
    }                                                                   \
  }

//! \brief return/exit if __VA_ARGS__ return non zero;
#define IFERROR(OP, ...)                                                \
  {                                                                     \
    if (__VA_ARGS__)                                                    \
    {                                                                   \
      printf("[ \033[1;35mwarning\033[0m ] warning in call function %s\n", #__VA_ARGS__); \
      OP;                                                               \
    }                                                                   \
  }
}


#endif // ERROR_DEAL_JJ_H
