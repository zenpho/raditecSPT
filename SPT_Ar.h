/* SPT_Ar.h

 parsing and processing of SPT Ar chunk data
 see SPT_types.h for description of SPT chunks
*/
#ifndef SPT_Ar_h
#define SPT_Ar_h

#include "SPT_types.h"
#include <string.h>

SPT_Ar* Ars[32];
uint8_t ArsCount = 0;

/** zero all pointers ready for SPT_Ar_read() */
void SPT_Ar_init()
{
  for(int i=0; i<32; i++) Ars[i] = 0;
  ArsCount = 0;
}

/** free any memory allocated by SPT_Ar_read() */
void SPT_Ar_cleanup()
{
  for(int i=0; i<32; i++)
  {
    if( Ars[i] ) free( Ars[i] );
  }
  ArsCount = 0;
}

uint16_t SPT_Ar_read(uint8_t* c)
{
  uint16_t* w = (uint16_t*)c;     // unsigned twobyte value
  uint16_t numBytesChunk = w[1];  // skip bytes 'A','r'
  
  uint16_t ofs = 4; // skip bytes 'A','r',<numBytesChunk,##>
  Ars[ArsCount] = calloc( 1, numBytesChunk );
  memcpy( Ars[ArsCount], c+ofs, numBytesChunk );
  
  ArsCount++;                     // zero based index into Ars array

  return numBytesChunk;
}

void SPT_Ar_fwrite(FILE* file)
{
  for(int i=0; i<32; i++)
  {
    // items may not be stored contiguously, skip nullptrs
    if( 0 == Ars[i] ) continue;
   
    uint16_t numBytesChunk = sizeof(*Ars[i]);
    fwrite("Ar", 1, 2, file); // two chars
    fwrite(&numBytesChunk, 2, 1, file); // one twobyte value
    
    // items may not be stored contiguously, must write one-at-a-time
    fwrite(Ars[i], numBytesChunk, 1, file);
  }
}

#endif

// end SPT_Ar.h
