// main function for a linked list manager, student version

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "hashmap.h"

int main(int argc, char *argv[]){
  int echo = 0;                                // controls echoing, 0: echo off, 1: echo on
  if(argc > 1 && strcmp("-echo",argv[1])==0) { // turn echoing on via -echo command line option
    echo=1;
  }

 printf("Hashmap Main\n");
  printf("Commands:\n");
  printf("  hashcode <key>   : prints out the numeric hash code for the given key (does not change the hash map)\n");
  printf("  put <key> <val>  : inserts the given key/val into the hash map, overwrites existing values if present\n");
  printf("  get <key>        : prints the value associated with the given key or NOT FOUND\n");
  printf("  print            : shows contents of the hashmap ordered by how they appear in the table\n");
  printf("  structure        : prints detailed structure of the hash map\n");
  printf("  clear            : reinitializes hash map to be empty with default size\n");
  printf("  save <file>      : writes the contents of the hash map the given file\n");
  printf("  load <file>      : clears the current hash map and loads the one in the given file\n");
  printf("  next_prime <int> : if <int> is prime, prints it, otherwise finds the next prime and prints it\n");
  printf("  expand           : expands memory size of hashmap to reduce its load factor\n");
  printf("  quit             : exit the program\n");
  
  char cmd[128];
  hashmap_t thehash;
  int success;
  hashmap_init(&thehash, HASHMAP_DEFAULT_TABLE_SIZE);

  while(1){
    printf("HM> ");                 // print prompt
    success = fscanf(stdin,"%s",cmd); // read a command
    if(success==EOF){                 // check for end of input
      printf("\n");                   // found end of input
      break;                          // break from loop
    }

    if( strcmp("quit", cmd)==0 ){     // check for exit command
      if(echo){
        printf("quit\n");
      }
      break;                          // break from loop
    }

    else if( strcmp("hashcode", cmd)==0 ){ // hashcode
      fscanf(stdin,"%s",cmd);         
      if(echo){
        printf("hashcode %s\n",cmd);
      }
      printf("%ld\n", hashcode(cmd));
    }

    else if( strcmp("get", cmd)==0 ){     // get command
      fscanf(stdin,"%s",cmd);          // read a string in to be the key
      if(echo){
        printf("get %s\n",cmd);
      }
      if(hashmap_get(&thehash,cmd)==NULL){
        printf("NOT FOUND\n");
      }
      else{
        printf("FOUND: %s\n",hashmap_get(&thehash,cmd));
      }
    }

    else if( strcmp("clear", cmd)==0 ){   // clear command
      if(echo){
        printf("clear\n");
      }
      hashmap_free_table(&thehash);
      hashmap_init(&thehash,HASHMAP_DEFAULT_TABLE_SIZE);
    }

    else if( strcmp("print", cmd)==0 ){   // print command
      if(echo){
        printf("print\n");
      }
      hashmap_write_items(&thehash, stdout);
    }

    else if( strcmp("next_prime", cmd)==0 ){   // next_prime command
      int temp = 0;
      fscanf(stdin,"%d",&temp);  
      if(echo){
        printf("next_prime %d\n",temp);
      }
      printf("%d\n",next_prime(temp));
    }

    else if( strcmp("put", cmd)==0 ){  // put command
      char val[128];
      fscanf(stdin,"%s %s",cmd,val);
      if(echo){
        printf("put %s %s\n",cmd,val);
      }
      int outcome = hashmap_put(&thehash,cmd,val);
      if(outcome==0){
        printf("Overwriting previous key/val\n");
      }
    }

    else if( strcmp("structure", cmd)==0 ){  // structure command
      if(echo){
        printf("structure\n");
      }
      hashmap_show_structure(&thehash);
    }

    else if( strcmp("save", cmd)==0 ){  // save command
    fscanf(stdin,"%s",cmd);  
      if(echo){
        printf("save %s\n",cmd);
      }
      hashmap_save(&thehash,cmd);
    }

    else if( strcmp("load", cmd)==0 ){  // load command
    fscanf(stdin,"%s",cmd);  
      if(echo){
        printf("load %s\n",cmd);
      }
      hashmap_load(&thehash,cmd);
    }

    else{                                 // unknown command
      if(echo){
        printf("%s\n",cmd);
      }
      printf("unknown command %s\n",cmd);
    } 
  }  

  hashmap_free_table(&thehash);
  return 0;
}
