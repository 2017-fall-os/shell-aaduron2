/* 
 * Description: This source file parses the string inputted by the user
 *
 * Author: Duron, Andrew
 * Date: December 10, 2017
 * Availability: github repository
 *
 *
 * Based on
 * ---------
 * Title: strcopy.c
 * Author: Freudenthal, Eric
 * Availability: github repository
 *
 */

#include <stdio.h>
#include <stdlib.h>

int noTokens;

char ** mytoc(char *str, char delim) {

  char *pStr, *copy, *pCopy;
  size_t len;
  int tokNum = 0, bytes = 0, index = 0, start = 0;

  for (pStr = str; *pStr; pStr++)
    len = pStr - str + 1;

  copy = pCopy = (char *)malloc(len);
  for (pStr = str; *pStr; pStr++)
    *(pCopy++) = *pStr;

  pCopy = 0;

  noTokens = 0;
  
  /* Traverse through the string and count the no. of tokens */
  while(1) {
    if (copy[index] != delim) {
      if (copy[index] == '\0') {
	noTokens++;
	break;
      }
      bytes++;
      index++;
    } else {
      if (bytes != 0) { 
	bytes = 0;
	noTokens++;
      }
      index++;
    }
  }

  /* Allocate memory for token vector */
  char **tokenVec = (char **)calloc(noTokens, sizeof(char*));

  index = 0;
  int cStart = 0;  //Start of string
  
  /* Parse through the characters for each word */
  while (copy[index] != '\0') {
    if (copy[index] != delim) {
      cStart = index;
      while (copy[index] != delim && copy[index] != '\0') {
	index++;
      }
      int length = index - cStart; 
      tokenVec[tokNum] = (char *)malloc(length+1);
      for (int i = 0; i < length; i++) {
	tokenVec[tokNum][i] = str[cStart+i];
      }
      tokenVec[tokNum][length] = '\0';
      tokNum++;
    }
    else {
      index++;
    }
  }
  
  if (noTokens >= 3)
    tokenVec[noTokens] = '\0';
  
  free(copy);

  return tokenVec;
}

void print_vector(char **vector) {
  int k = 0;
  while (vector[k] != '\0') {
      printf("%s\n", vector[k]);
      fflush(stdout);
      k++;
  }
}

void free_vector(char **vector) {
  int i = 0;
  for (int i = 0; i < noTokens - 1; i++) {
    free(vector[i]);
  }
}

int find_length(char **vector) {
    int k = 0;
    int size;
    while (vector[k] != '\0') {
        k++;
    }
    
    return k;
}

int compare_str(char *string, char *test) {
  int length = sizeof(test);
  int result = 0;
  if (length = sizeof(string)) {
    int i = 0;
    while (i < length) {
      if (string[i] == test[i]) {
        result = 1;
        i++;
      }
      else
	return 0;
    }
  }
  return result;
}

int search_str(char **vector, char *str) {
    int i = 0;
    int result = 0;
    while (vector[i] != '\0') {
        result = compare_str(vector[i], str);
        if (result)
            return result;
        i++;
    }
    
    return result;
}

int path_index(char **vector) {
    int i = 0;
    int result = 0;
    while (vector[i] != '\0') {
        result = compare_str(vector[i], "PATH");
        if (result)
            return result;
        i++;
    }
    
    return result;
}

char *concat(char *str, char *str2) {
  char *i, *j, *temp, *pStr;
  
  int length = sizeof(str);
  int length2 = sizeof(str);
  char* str3 = (char*)malloc((length+length2)*sizeof(char));
  temp = str3;
  
  for (i = str; *i != '\0'; i++, temp++) {
    *temp = *i;
  }
  for(j = str2; *j != '\0'; j++, temp++) {
     *temp = *j;
  }
  
  *temp = '\0';
  return temp;

    
}

