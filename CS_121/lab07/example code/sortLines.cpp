/* sortLines.cpp

   Sort lines of text using function pointers.

   Derived from the example in:
   "The C Programming Language", Second edition,
   Kernighan & Ritchie, Prentice-Hall, 1988

   Bruce M. Bolden
   March 31, 2005
 */

#include <iostream>
#include <stdlib.h>
#include <string.h>

using namespace std;

const int MAXLINES = 5000;
char *lines[MAXLINES];

   //  sorting
void qSort( void *lines[], int left, int right,
            int (*comp)(void *, void *) );
void swap( void *v[], int i, int j );
int numcmp( const char *, const char * );

   //  read/write lines
int readlines( char *lines[], int nLines );
void writelines( char *lines[], int nLines );
int getline( char s[], int lim );


int main( int argc, char *argv[] )
{
   int nLines;
   int numeric = 0;         /*  1 if numeric sort  */

   if( argc > 1 && strcmp(argv[1], "-n") == 0 )
      numeric = 1;

   if( (nLines = readlines( lines, MAXLINES) ) >= 0 )
   {
       qSort( (void **)lines, 0, nLines-1,
              (int (*)(void*,void*))(numeric ? numcmp : strcmp) );

       writelines( lines, nLines );
       return 0;
   }
   else
   {
       printf( "input too big to sort\n" );
       return 1;
   }
}

/*  qSort:  sort v[left]...v[right into increasing order  */
void qSort( void *v[], int left, int right,
            int (*comp)(void *, void *) )
{
    int i, last;

    if( left >= right )
       return;

    swap( v, left, (left + right)/2 );
    last = left;
    for( i = left+1 ; i <= right ; i++ )
       if( (*comp)(v[i], v[left]) < 0 )
           swap( v, ++last, i );

    swap( v, left, last );
    qSort( v, left,   last-1, comp );
    qSort( v, last+1, right,  comp );
}

/*  numcmp:  compare s1 and s2 numerically  */
int numcmp( const char *s1, const char* s2 )
{
   double v1, v2;
   v1 = atof( s1 );
   v2 = atof( s2 );

   if( v1 < v2 )
      return -1;
   else if( v1 > v2 )
      return 1;
   else
      return 0;
}

/*  swap: two items in an array  */
void swap( void *v[], int i, int j )
{
    void *temp;

    temp = v[i];
    v[i] = v[j];
    v[j] = temp;
}

/*  readlines:  read lines  */
int readlines( char *lines[], int maxLines )
{
    int len, nLines;
    const int MAXLEN = 1024;      /* max length of any input line  */
    char *p, line[MAXLEN];

    nLines = 0;
    while( (len = getline(line, MAXLEN)) > 0 )
    {
       if( nLines >= maxLines || (p = new char[len+1]) == NULL )
          return -1;
       else
       {
           line[len] = '\0';
           strcpy( p, line );
           lines[nLines++] = p;
       }
    }

    return nLines;
}

/*  writelines:  write lines  */
void writelines( char *lines[], int nLines )
{
   int i;

   for( i = 0 ; i < nLines ; i++ )
       cout << lines[i] << endl;
}

/*  getline:  get line into s, return length  */
int getline( char s[], int lim )
{
   int i = 0;
   cin.getline( s, lim );
   i = strlen( s );

   return i;
}
