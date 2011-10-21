#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#define false 0
#define true 1
#define bool int
#define DEBUG_PUSH 0x1
#define DEBUG_TABLE 0x2
#define DEBUG_LOOKUP 0x4
#define DEBUG_ALL 0xffffffff

typedef struct {
    char *name;
    char *scope;
    int depth;
    void *ptr;
} SymTabEntry;
    

// 
// Class SymTab
// 
// A general simple stack of symbol tables that maps 
// a char * to a void *.  Provides a user definable
// print routine for the objects stored in the symbol table.
// The print rouinte is defined when the constructor is called.
// 
// debug flags setable by the debug method:
//   DEBUG_TABLE - announce entry to a scope and prints the symbol
//       table on exit from a scope.
//   DEBUG_PUSH - print everything that is pushed on the stack (uses
//       the print routine for printing the ptr value (treeNode *?)
// these flags are bit masks and so can be ored together to turn
// on multiple affects.  For example debug(DEBUG_TABLE | DEBUG_PUSH) would
// turn on both the DEBUG_PUSH and DEBUG_TABLE flags.
//
//  The four most important operations are insert, lookup, enter, leave.
//

	
void initSymTab(void (* elemPrint)(void *));  // the constructor creates and sets the print routine
void freeSymTab();                           // destructor
void debug(int newDebugValue);       // sets the debug flags
void print();                        // prints the entire stack
bool insert(char *sym, void *ptr);   // inserts a new ptr associated with symbol sym 
                                     // returns false if already defined
void *lookup(char *sym);             // returns the ptr associated with sym
                                     // returns NULL if symbol not found
SymTabEntry *lookupSymTabEntry(char *sym);  // returns pointer to SymTabEntry associated with sym
                                             // returns NULL if symbol not found
// scope functions
void enter(char *funcname);          // enter a function named funcname
bool leave();                        // leave that function
int numEntries();                    // number of entries (more for debugging)
int depth();                         // depth of scopes on stack (useful in later assignment)
