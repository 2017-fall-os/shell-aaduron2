#ifndef MytocIncluded
#define MytocIncluded

char ** mytoc(char *str, char delim);
void print_vector(char **vector);
void free_vector(char **vector);
int find_length(char **vector);
int string_size(char *str);
int compare_str(char *str, char *test);
int search_str(char **vector, char *str);
int search_char(char **vector, char *character);
int path_index(char **vector);
char *concat(char *str, char *str2);

#endif
