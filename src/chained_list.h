#ifndef _CHAINED_LIST_H_
# define _CHAINED_LIST_H_


typedef struct ChainedList ChainedList;
struct ChainedList
{
  struct ChainedList *next;
  char *file_name;
  struct stat *stats;
};

#endif /* _CHAINED_LIST_H_ */