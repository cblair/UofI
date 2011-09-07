/* array.h
 */

    //  Prototypes
int ReadArray( ifstream &fIn, int A[], const int nMax );
void WriteArray( ofstream &fOut, const int A[], const int nMax );

void InitializeArray( int A[], const int nMax, const int iVal );

int MinArray( const int A[], const int nMax );

int MaxArray(const int A[], const int nMax);

int MedianArray(const int A[], const int nMax);

int StandardDeviatonArray(const int A[], const int nMax);

int SearchArray( const int A[], const int nMax, const int keyVal );
