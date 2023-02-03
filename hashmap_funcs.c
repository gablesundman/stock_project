// hashmap_funcs.c: utility functions for operating on hash maps. Most
// functions are used in hash_main.c which provides an application to
// work with the functions.


#include <stdio.h>
#include <stdlib.h>
#include "hashmap.h"
#include "string.h"

// PROVIDED: Compute a simple hash code for the given character
// string. The code is "computed" by casting the first 8 characters of
// the string to a long and returning it. The empty string has hash
// code 0. If the string is a single character, this will be the ASCII
// code for the character (e.g. "A" hashes to 65).  Longer strings
// will have numbers which are the integer interpretation of up to
// their first 8 bytes.  ADVANTAGE: constant time to compute the hash
// code. DISADVANTAGE: poor distribution for long strings; all strings
// with same first 8 chars hash to the same location.
long hashcode(char key[]){
  union {
    char str[8];
    long num;
  } strnum;
  strnum.num = 0;

  for(int i=0; i<8; i++){
    if(key[i] == '\0'){
      break;
    }
    strnum.str[i] = key[i];
  }
  return strnum.num;
}

void hashmap_init(hashmap_t *hm, int table_size){
// initializes table size and count, and creates memory space for the table
// and fills it with nulls
hm->table_size = table_size;
hm->item_count = 0;
hm->table = malloc(table_size*sizeof(hashnode_t));
for (int i = 0; i < table_size; i++){
  hm->table[i] = NULL;
}
return;
}
// Initialize the hash map 'hm' to have given size and item_count
// 0. Ensures that the 'table' field is initialized to an array of
// size 'table_size' and filled with NULLs. 


int hashmap_put(hashmap_t *hm, char key[], char val[]){
  // finds hashcode
  long insert_pos = hashcode(key)%hm->table_size;
  // if the key is already in the hashmap, the while loop will find the key
  // and replace its current val with the parameter
  if(hashmap_get(hm,key)!=NULL){
    hashnode_t *curr_node = hm->table[insert_pos];
    while(curr_node!=NULL){
      if(strcmp(curr_node->key,key)==0){
        strcpy(curr_node->val,val);
        return 0;
      }
      curr_node = curr_node->next;
    }
  }
  else{
    hm->item_count ++;
    // if the first node at this index in the table is NULL, creates a new
    // node with given key and val to become the first node
    if(hm->table[insert_pos]==NULL){
      hashnode_t *new_node = malloc(sizeof(hashnode_t));
      strcpy(new_node->key, key);
      strcpy(new_node->val, val);
      new_node->next = NULL;
      hm->table[insert_pos] = new_node;
      return 1;
    }
    // creates a new node with key and val from the parameters and makes it
    // the last node in the linked list
    else{
      hashnode_t *curr_node = hm->table[insert_pos];
      hashnode_t *new_node = malloc(sizeof(hashnode_t));
      strcpy(new_node->key, key);
      strcpy(new_node->val, val);
      new_node->next = NULL;
      while(curr_node->next!=NULL){
        curr_node = curr_node->next;
      }
      curr_node->next = new_node;
      return 1;
    }
  }
  return -1;
}
// Adds given key/val to the hash map. 'hashcode(key) modulo
// table_size' is used to calculate the position to insert the
// key/val.  Searches the entire list at the insertion location for
// the given key. If key is not present, a new node is added. If key
// is already present, the current value is altered in place to the
// given value "val" (no duplicate keys are every introduced).  If new
// nodes are added, increments field "item_count".  Makes use of
// standard string.h functions like strcmp() to compare strings and
// strcpy() to copy strings. Lists in the hash map are arbitrarily
// ordered (not sorted); new items are always appended to the end of
// the list.  Returns 1 if a new node is added (new key) and 0 if an
// existing key has its value modified.

char *hashmap_get(hashmap_t *hm, char key[]){
  // finds hashcode and then creates a node equal to the first node
  // at the found index of the table
  long insert_pos = hashcode(key)%hm->table_size;
  hashnode_t *curr_node = hm->table[insert_pos];
  // runs through the nodes and finds if the key is the same and then returns
  // the value
  while(curr_node != NULL){
      if(strcmp(key,curr_node->key)==0){
        return curr_node->val;
      }
      curr_node = curr_node->next;
  }
  // returns null if not found
  return NULL;
}
// Looks up value associated with given key in the hashmap. Uses
// hashcode() and field "table_size" to determine which index in table
// to search.  Iterates through the list at that index using strcmp()
// to check for matching key. If found, returns a pointer to the
// associated value.  Otherwise returns NULL to indicate no associated
// key is present.

void hashmap_free_table(hashmap_t *hm){
  // loops through each table index and finds if the initial node is NULL
  for(int i = 0; i < hm->table_size; i++){
    if(hm->table[i]!=NULL){
      // if the node is not already null, it is made null here
      hashnode_t *curr_node = hm->table[i];
      hashnode_t *temp;
      while(curr_node != NULL){
        temp = curr_node->next;
        free(curr_node);
        curr_node = temp;
      }
    }
  }
  // frees the table and resets item count and table size
  free(hm->table);
  hm->item_count = 0;
  hm->table_size = 0;
  return;
}
// De-allocates the hashmap's "table" field. Iterates through the
// "table" array and its lists de-allocating all nodes present
// there. Subsequently de-allocates the "table" field and sets all
// fields to 0 / NULL. Does NOT attempt to free 'hm' as it may be
// stack allocated.

void hashmap_show_structure(hashmap_t *hm){
  // prints basic values
  printf("item_count: %d\n", hm->item_count);
  printf("table_size: %d\n", hm->table_size);
  printf("load_factor: %.4f\n", ((double)hm->item_count)/(hm->table_size));
  // prints the key and val pairs in the hashmap, along with the hashcoded key
  for(int i = 0; i < hm->table_size; i++){
    printf("%3d : ",i);
    hashnode_t *curr_node = hm->table[i];
    while(curr_node != NULL){
      printf("{(%ld) %s : %s} ",hashcode(curr_node->key),curr_node->key,curr_node->val);
      curr_node = curr_node->next;
    }
    printf("\n");
  }
  return;
}
// Displays detailed structure of the hash map. Shows stats for the
// hash map as below including the load factor (item count divided
// by table_size) to 4 digits of accuracy.  Then shows each table
// array index ("bucket") on its own line with the linked list of
// key/value pairs on the same line. The hash code for keys is appears
// prior to each key.  EXAMPLE:
// 
// item_count: 6
// table_size: 5
// load_factor: 1.2000
//   0 : {(65) A : 1} 
//   1 : 
//   2 : {(122) z : 3} {(26212) df : 6} 
//   3 : {(98) b : 2} {(25443) cc : 5} 
//   4 : {(74) J : 4} 
//
// NOTES:
// - Uses format specifier "%3d : " to print the table indices
// - Shows the following information for each key/val pair
//   {(25443) cc : 5}
//     |      |    |
//     |      |    +-> value
//     |      +-> key
//     +-> hashcode("cc"), print using format "%ld" for 64-bit longs

void hashmap_write_items(hashmap_t *hm, FILE *out){
  // runs through each table index and prints key, val pairs down the screen
  for(int i = 0; i < hm->table_size; i++){
    hashnode_t *curr_node = hm->table[i];
    while(curr_node != NULL){
      fprintf(out,"%12s : %s\n",curr_node->key, curr_node->val);
      curr_node = curr_node->next;
    }
  }
  return;
}
// Outputs all elements of the hash table according to the order they
// appear in "table". The format is
// 
//       Peach : 3.75
//      Banana : 0.89
//  Clementine : 2.95
// DragonFruit : 10.65
//       Apple : 2.25
// 
// with each key/val on a separate line. The format specifier
//   "%12s : %s\n"
// 
// is used to achieve the correct spacing. Output is done to the file
// stream 'out' which is standard out for printing to the screen or an
// open file stream for writing to a file as in hashmap_save().

void hashmap_save(hashmap_t *hm, char *filename){
  // opens file and prints table size and item count
  FILE *file_handle = fopen(filename,"w");
  fprintf(file_handle,"%d %d\n",hm->table_size,hm->item_count);
  // can use hashmap_write_items to simply print the key/val pairs
  hashmap_write_items(hm, file_handle);
  fclose(file_handle);
  return;
}
// Writes the given hash map to the given 'filename' so that it can be
// loaded later.  Opens the file and writes its 'table_size' and
// 'item_count' to the file. Then uses the hashmap_write_items()
// function to output all items in the hash map into the file.
// EXAMPLE FILE:
// 
// 5 6
//            A : 2
//            Z : 2
//            B : 3
//            R : 6
//           TI : 89
//            T : 7
// 
// First two numbers are the 'table_size' and 'item_count' field and
// remaining text are key/val pairs.

int hashmap_load(hashmap_t *hm, char *filename){
  // opens the file and then checks if null
  FILE *file_handle = fopen(filename, "r");
  if(file_handle == NULL){
    printf("Could not open file '%s'\n", filename);
    return 0;
  }
  else{
    // frees the table
    hashmap_free_table(hm);
    // reads in the table size and a count variable
    fscanf(file_handle,"%d",&hm->table_size);
    int real_count = 0;
    fscanf(file_handle,"%d\n",&real_count);
    // runs through each line and inputs the key and val, putting them in the
    // hashmap via put
    for(int i = 0; i <= hm->table_size; i++){
      char temp1[128],temp2[128];
      fscanf(file_handle,"%s", temp1);
      fscanf(file_handle,"%*s");
      fscanf(file_handle,"%s", temp2);
      hashmap_put(hm,temp1,temp2);
      // sets the item count to be the real count from earlier
      hm->item_count = real_count;
    }
  }
  fclose(file_handle);
  return -1;
}
// Loads a hash map file created with hashmap_save(). If the file
// cannot be opened, prints the message
// 
// ERROR: could not open file 'somefile.hm'
//
// and returns 0 without changing anything. Otherwise clears out the
// current hash map 'hm', initializes a new one based on the size
// present in the file, and adds all elements to the hash map. Returns
// 1 on successful loading. This function does no error checking of
// the contents of the file so if they are corrupted, it may cause an
// application to crash or loop infinitely.

int next_prime(int num){
  // initializes count
  int count;
  // runs through and checks if the number is divisible by anything, then
  // returning the number if there were no instances of divisibility
  for(int curr = num; ;curr++){
    for(int i = 2; i < num/2; i++){
      if(num%i==0){
        count ++;
      }
    }
    if(count==0){
      return curr;
    }
    else{
      count = 0;
    }
  }
  return -1;
}
// If 'num' is a prime number, returns 'num'. Otherwise, returns the
// first prime that is larger than 'num'. Uses a simple algorithm to
// calculate primeness: check if any number between 2 and (num/2)
// divide num. If none do, it is prime. If not, tries next odd number
// above num. Loops this approach until a prime number is located and
// returns this. Used to ensure that hash table_size stays prime which
// theoretically distributes elements better among the array indices
// of the table.

void hashmap_expand(hashmap_t *hm){
  return;
}
// Allocates a new, larger area of memory for the "table" field and
// re-adds all items currently in the hash table to it. The size of
// the new table is next_prime(2*table_size+1) which keeps the size
// prime.  After allocating the new table, all entries are initialized
// to NULL then the old table is iterated through and all items are
// added to the new table according to their hash code. The memory for
// the old table is de-allocated and the new table assigned to the
// hashmap fields "table" and "table_size".  This function increases
// "table_size" while keeping "item_count" the same thereby reducing
// the load of the hash table. Ensures that all memory associated with
// the old table is free()'d (linked nodes and array). Cleverly makes
// use of existing functions like hashmap_init(), hashmap_put(),
// and hashmap_free_table() to avoid re-writing algorithms
// implemented in those functions.
