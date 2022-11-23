/*
 * Include the provided hash table library.
 */
#include "hashtable.h"

/*
 * Include the header file.
 */
#include "philspel.h"

/*
 * Standard IO and file routines.
 */
#include <stdio.h>

/*
 * General utility routines (including malloc()).
 */
#include <stdlib.h>

/*
 * Character utility routines.
 */
#include <ctype.h>

/*
 * String utility routines.
 */
#include <string.h>

/*
 * This hash table stores the dictionary.
 */
HashTable *dictionary;

void printHash();
/*
 * The MAIN routine.  You can safely print debugging information
 * to standard error (stderr) as shown and it will be ignored in 
 * the grading process.
 */
int main(int argc, char **argv) {
  if (argc != 2) {
    fprintf(stderr, "Specify a dictionary\n");
    return 0;
  }
  /*
   * Allocate a hash table to store the dictionary.
   */
  fprintf(stderr, "Creating hashtable\n");
  dictionary = createHashTable(2048, &stringHash, &stringEquals);

  fprintf(stderr, "Loading dictionary %s\n", argv[1]);
  readDictionary(argv[1]);
  fprintf(stderr, "Dictionary loaded\n");
  //printHash();  // The print function add for the test purpose.

  fprintf(stderr, "Processing stdin\n");
  processInput();
  /*
   * The MAIN function in C should always return 0 as a way of telling
   * whatever program invoked this that everything went OK.
   */
  return 0;
}

/*
 * This should hash a string to a bucket index.  Void *s can be safely cast
 * to a char * (null terminated string) and is already done for you here 
 * for convenience.
 */
unsigned int stringHash(void *s) {
  unsigned int res = 0;
  char *string = (char *)s;
  while (*string != '\0') {
      res += (unsigned int) (*string);
      string++;
  }
  return res % dictionary->size;
}

/*
 * This should return a nonzero value if the two strings are identical 
 * (case sensitive comparison) and 0 otherwise.
 */
int stringEquals(void *s1, void *s2) {
  char *string1 = (char *)s1;
  char *string2 = (char *)s2;
  while (*string1 != '\0' && *string2 != '\0') {
      if (*string1 != *string2) {
          return 0;
      } else {
          string1++;
          string2++;
      }
  }
  if (*string1 == *string2) {
      return 1;
  }
  return 0;
}

/*
 * This function should read in every word from the dictionary and
 * store it in the hash table.  You should first open the file specified,
 * then read the words one at a time and insert them into the dictionary.
 * Once the file is read in completely, return.  You will need to allocate
 * (using malloc()) space for each word.  As described in the spec, you
 * can initially assume that no word is longer than 60 characters.  However,
 * for the final 20% of your grade, you cannot assumed that words have a bounded
 * length.  You CANNOT assume that the specified file exists.  If the file does
 * NOT exist, you should print some message to standard error and call exit(1)
 * to cleanly exit the program.
 *
 * Since the format is one word at a time, with new lines in between,
 * you can safely use fscanf() to read in the strings until you want to handle
 * arbitrarily long dictionary chacaters.
 */
void readDictionary(char *dictName) {
    FILE *fp;
    fp = fopen(dictName, "r");
    if (!fp) {
        fprintf(stderr, "The file doesn't exist!");
        exit(1);
    }
    for(;;) {
        char *entry = (char *) malloc (sizeof(char) * 60);
        if (fscanf(fp, "%[^\n] ", entry) == EOF) {
            break;
        }
        insertData(dictionary, entry, entry);
    }
    fclose(fp);
}

/*
 * This should process standard input (stdin) and copy it to standard
 * output (stdout) as specified in the spec (e.g., if a standard 
 * dictionary was used and the string "this is a taest of  this-proGram" 
 * was given to stdin, the output to stdout should be 
 * "this is a teast [sic] of  this-proGram").  All words should be checked
 * against the dictionary as they are input, then with all but the first
 * letter converted to lowercase, and finally with all letters converted
 * to lowercase.  Only if all 3 cases are not in the dictionary should it
 * be reported as not found by appending " [sic]" after the error.
 *
 * Since we care about preserving whitespace and pass through all non alphabet
 * characters untouched, scanf() is probably insufficent (since it only considers
 * whitespace as breaking strings), meaning you will probably have
 * to get characters from stdin one at a time.
 *
 * Do note that even under the initial assumption that no word is longer than 60
 * characters, you may still encounter strings of non-alphabetic characters (e.g.,
 * numbers and punctuation) which are longer than 60 characters. Again, for the 
 * final 20% of your grade, you cannot assume words have a bounded length.
 */
void processInput() {
    int ch;
    int temp = '\0';
    while ((ch=getchar()) != EOF) {
        if (temp != '\0') {
            putchar(temp);
            temp = '\0';
        }
        if (ch < 65 || (ch > 90 && ch < 97) || ch > 122) putchar(ch);
        else {
            char word[60], x;
            int i = 1;
            word[0] = ch;
            for (;;) {
                x = getchar();
                if (x < 65 || (x > 90 && x < 97) || x > 122) {
                    word[i] = '\0';
                    char word2[60], word3[60];
                    word2[0] = tolower(word[0]);
                    word3[0] = word[0];
                    for (int k = 1; k < i; k++) {
                        word2[k] = tolower(word[k]);
                        word3[k] = tolower(word[k]);
                    }
                    word2[i] = '\0';
                    word3[i] = '\0';
                    if (!findData(dictionary, word) && !findData(dictionary, word2) 
                            && !findData(dictionary, word3)) {
                        printf("%s [sic]", word);
                    } else {
                        printf("%s", word);
                    }
                    temp = (int) x;
                    break;
                }
                word[i] = x;
                i++;
            }
        }
    }
}
    
void printHash() {
    struct HashBucket **data = dictionary->data;
    int size = dictionary->size;
    for (int i = 0; i < size; i++) {
        struct HashBucket *link = data[i];
        while (link != NULL) {
            fprintf(stderr, " %s ", (char *) (link->data));
            link = link->next;
        }
    }
}

