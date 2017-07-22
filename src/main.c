#include "main.h"

void clear_mem(char *str, int size) {
  int i = 0;

  while (i < size)
    str[i++] = '\0';
}

int my_strlen(char *str) {
  int i = 0;

  while (str[i] != '\0'){
    ++i;
  }

  return i;
}

void join_strings(char *src, char *dest, char joiner) {
  int i = 0;
  int j = my_strlen(dest);


  if (joiner != '\0')
    dest[j++] = joiner;
  while (i < my_strlen(src)) {
    dest[j++] = src[i++];
  }
}

char *concat_path(char *path, char *file_name) {
  int i = 0;
  int j = 0;
  int result_size = my_strlen(path) + my_strlen(file_name) + 2;
  char *res = malloc(result_size * sizeof (char));
  int length = my_strlen(file_name);

  clear_mem(res, result_size);

  while (i < my_strlen(path)) {
    res[i] = path[i];
    i++;
  }
  res[i++] = '/';

  while (j < length){
    res[i++] = file_name[j++];
  }

  return res;
}

void set_stats(ChainedList *list, char *file_name) {
  struct stat *sts = malloc(sizeof (struct stat));
  char *path = file_name;

  if (path == NULL)
    path = list->file_name;

  stat(path, sts);
  list->stats = sts;
}

char *to_rights(int nb) {
  int i = 0;
  int j = 0;
  int nbmod2 = 0;
  char *str = malloc(30 * sizeof (char));
  char *res = malloc(30 * sizeof (char));

  clear_mem(str, 30);
  clear_mem(res, 30);
  if (nb == 0)
    str[0] = '0';
  else
    while (nb != 0) {
      nbmod2 = nb % 2;
      str[i++] = nbmod2 + '0';
      nb = nb / 2;
    }
  if (my_strlen(str) == 15)
    res[0] = 'd';
  else
    res[0] = '-';
  i = 8;
  j = 1;
  while (i > 0) {
    if (str[i] == '1')
      res[j] = 'r';
    else
      res[j] = '-';

    if (str[i - 1] == '1')
      res[j + 1] = 'w';
    else
      res[j + 1] = '-';

    if (str[i - 2] == '1')
      res[j + 2] = 'x';
    else
      res[j + 2] = '-';

    j = j + 3;
    i = i - 3;
  }
  free(str);

  return res;
}

void my_strcpy(char *src, char *dst) {
  int i = my_strlen(src);
  dst[i] = '\0';
  while (i > 0){
    i--;
    dst[i] = src[i];
  }
}

char *fetch_options(int argc, char **argv) {
  int i = 1;
  int k = 0;
  char *res = malloc(256 * sizeof (char));
  char *params = NULL;

  clear_mem(res, 256);
  while (i < argc) {
    params = argv[i++];
    if (params[0] == '-') {
      params++;
      while (params[0] != '\0')
        res[k++] = *params++;
    }
  }

  return res;
}

int included_option(char *options, char option) {
  int i = 0;

  while (options[i] != '\0')
    if (options[i++] == option)
      return 1;

  return 0;
}

int has_a_normal_arg(int argc, char **argv) {
  int i = 1;

  while (i < argc){
    if (argv[i][0] != '-')
      return 1;
    ++i;
  }

  return 0;
}

void init_with_dot(ChainedList *list) {
  list->file_name = malloc(2 * sizeof(char));
  list->file_name[0] = '.';
  list->file_name[1] = '\0';
  list->next = NULL;
  set_stats(list, NULL);
}

ChainedList *fetch_args(int argc, char **argv) {
  int i = 1;
  ChainedList *head = NULL;
  ChainedList *args_list = malloc(sizeof (ChainedList));;
  int normal_arg = has_a_normal_arg(argc, argv);

  args_list->next = NULL;
  head = args_list;

  if (!normal_arg)
    init_with_dot(args_list);
  else {

    while (i < argc && argv[i][0] == '-')
      ++i;
    if (i < argc) {
      args_list->file_name = malloc(my_strlen(argv[i]) * sizeof(char));
      clear_mem(args_list->file_name, my_strlen(argv[i]));
      args_list->next = NULL;
      my_strcpy(argv[i], args_list->file_name);
      set_stats(args_list, argv[i]);
      ++i;
    }

    while (i < argc) {
      if (argv[i][0] != '-') {
        args_list->next = malloc(sizeof(ChainedList));
        args_list = args_list->next;
        args_list->file_name = malloc(my_strlen(argv[i]) * sizeof(char));
        clear_mem(args_list->file_name, my_strlen(argv[i]));
        args_list->next = NULL;
        my_strcpy(argv[i], args_list->file_name);
        set_stats(args_list, argv[i]);
      }
      ++i;
    }
  }

  return head;
}

int chained_list_count(ChainedList *args){
  int count = 0;

  while (args != NULL) {
    args = args->next;
    count++;
  }

  return count;
}

char *process_time(time_t updated_at) {
  char *str = malloc(2048 * sizeof (char));
  int year = 0;

  clear_mem(str, 2048);
  my_strcpy(ctime(&updated_at), str);

  // 6 months in seconds
  if (updated_at < time(NULL) - 15552000)
    year = 1;

  str[0] = str[8];
  str[1] = str[9];
  str[2] = ' ';
  str[3] = str[4] - 'A' + 'a';
  str[4] = str[5];
  str[5] = str[6];
  str[6] = ' ';
  if (year) {
    str[7] = ' ';
    str[8] = str[20];
    str[9] = str[21];
    str[10] = str[22];
    str[11] = str[23];
  } else {
    str[7] = str[11];
    str[8] = str[12];
    str[9] = str[13];
    str[10] = str[14];
    str[11] = str[15];
  }
  str[12] = '\0';
  return str;
}

void free_list(ChainedList *list) {
  ChainedList *next = NULL;

  while (list != NULL) {
    free(list->file_name);
    next = list->next;
    free(list);
    list = next;
  }
}


void print_list(ChainedList *list, char *options) {
  int length = 0;
  struct passwd *usr_pwd;
  struct group *grp_pwd;
  char *username = NULL;
  char *groupname = NULL;
  char *rights = NULL;
  char *updated_at = NULL;
  ChainedList *head = list;

  blkcnt_t total_size = 0;
  char *cur = malloc(256 * sizeof(char));
  char *res = malloc(40000 * sizeof(char));

  clear_mem(res, 40000);

  while (list != NULL && list->file_name != NULL) {
    length = my_strlen(list->file_name);
    if (included_option(options, 'l')) {
      usr_pwd = getpwuid(list->stats->st_uid);
      grp_pwd = getgrgid(list->stats->st_gid);
      username = usr_pwd->pw_name;
      groupname = grp_pwd->gr_name;
      rights = to_rights(list->stats->st_mode);
      updated_at = process_time(list->stats->st_mtime);
      total_size = total_size + list->stats->st_blocks;
      clear_mem(cur, 256);
      sprintf(cur, "%s  %i %s  %s\t%lld\t%s %s\n", rights, list->stats->st_nlink, username, groupname, list->stats->st_size, updated_at, list->file_name);
      join_strings(cur, res, '\0');
      free(rights);
    }
    else {
      write(STDOUT_FILENO, list->file_name, length);
      if (length >= 8)
        write(STDOUT_FILENO, "\t", 1);
      else
        write(STDOUT_FILENO, "\t\t", 2);
    }
    list = list->next;
  }
  if (included_option(options, 'l')) {
    printf("total %lli\n", total_size);
    printf("%s\n", res);
  }
  free(cur);
  free(res);
  list = head;
}

ChainedList *reverse_list(ChainedList *list) {
  ChainedList* prev   = NULL;
  ChainedList* current = list;
  ChainedList* next = NULL;

  while (current != NULL) {
      next = current->next;  
      current->next = prev;   
      prev = current;
      current = next;
    }

  return prev;
}

ChainedList *sort_list(ChainedList *list) {
  ChainedList *head = list;
  ChainedList *tmp = head;
  ChainedList *old = head;
  int i = 0;
  
  while (list->next != NULL) {
    while (list->file_name[i] == list->next->file_name[i] && i < my_strlen(list->file_name))
      ++i;
    if (i < my_strlen(list->file_name) && list->file_name[i] > list->next->file_name[i]) {
      if (list == head) {
        tmp = list->next;
        list->next = tmp->next;
        tmp->next = list;
        head = tmp;
        list = head;
        old = head;
      } else {
        tmp = list->next;
        list->next = tmp->next;
        old->next = tmp;
        tmp->next = list;
        list = head;
      }
    } else {
      old = list;
      list = list->next;
    }
    i = 0;
  }
  list = head;
  return head;
}


int streql(char *str1, char *str2) {
  int i = 0;

  if (my_strlen(str1) != my_strlen(str2))
    return 0;

  while (str1[i] == str2[i] && str1[i] != '\0')
    i++;

  if (i == my_strlen(str1))
    return 1;
  return 0;
}

void insert_next(ChainedList *list, char *full_path) {
  ChainedList *head = list;
  ChainedList *next = list->next;
  ChainedList *new = malloc(sizeof (ChainedList));
  new->file_name = malloc((my_strlen(full_path) + 1) * sizeof (char));
  clear_mem(new->file_name, my_strlen(full_path) + 1);
  my_strcpy(full_path, new->file_name);
  set_stats(new, NULL);
  list->next = new;
  new->next = next;
  list = head;
}

int is_a_folder(ChainedList *list) {
  char *rights = to_rights(list->stats->st_mode);

  if (!streql(list->file_name, ".") && !streql(list->file_name, "..") && rights[0] == 'd') {
    free(rights);
    return 1;
  }
  else {
    free(rights);
    return 0;
  }
}

ChainedList *list_dir(ChainedList *args, char *options) {
  DIR *dirp = NULL;
  struct dirent *dp = NULL;
  ChainedList *list = malloc(sizeof (ChainedList));
  ChainedList *old = list;
  ChainedList *head = list;
  char *full_path = NULL;

  list->next = NULL;
  list->file_name = NULL;

  dirp = opendir(args->file_name);
  if (dirp == NULL)
    return NULL;
  while ((dp = readdir(dirp)) != NULL)
    if (included_option(options, 'a') || dp->d_name[0] != '.') {
      list->file_name = malloc((dp->d_namlen + 1) * sizeof(char));
      clear_mem(list->file_name, dp->d_namlen + 1);
      my_strcpy(dp->d_name, list->file_name);
      full_path = concat_path(args->file_name, list->file_name);
      set_stats(list, full_path);
      if (included_option(options, 'R') && is_a_folder(list))
        insert_next(args, full_path);
      free(full_path);
      list->next = malloc(sizeof (ChainedList));
      old = list;
      list = list->next;
      list->next = NULL;
    }

  old->next = NULL;
  head = sort_list(head);
  if (included_option(options, 'r'))
    head = reverse_list(head);

  closedir(dirp);

  return head;
}

int main(int argc, char **argv) {
  char *options = NULL;
  ChainedList *args = NULL;
  ChainedList *directory_list = NULL;
  ChainedList *args_head = NULL;
  char *rights = NULL;

  options = fetch_options(argc, argv);
  args = fetch_args(argc, argv);
  args = sort_list(args);
  args_head = args;

  while (args != NULL) {
    directory_list = list_dir(args, options);
    rights = to_rights(args->stats->st_mode);

    if (rights[0] == 'd' && chained_list_count(args_head) > 1){
      write(STDOUT_FILENO, args->file_name, my_strlen(args->file_name));
      write(STDOUT_FILENO, ":\n", 2);
    }
    free(rights);
    print_list(directory_list, options);
    free_list(directory_list);
    if (!included_option(options, 'l')) {
      write(STDOUT_FILENO, "\n", 1);
      if (chained_list_count(args) > 1)
        write(STDOUT_FILENO, "\n", 1);
    }
    args = args->next;
  }

  free(options);
  free(args);

  return 0;
}
