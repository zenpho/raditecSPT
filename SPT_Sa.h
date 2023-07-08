/* SPT_Sa.h

 parsing and processing of SPT Sa chunk data
 see SPT_types.h for description of SPT chunks
*/
#ifndef SPT_Sa_h
#define SPT_Sa_h

#include "SPT_types.h"
#include "SPT_common.h"

SPT_Sa Sa;

// NB: UPPERCASE TANGO, lowecase papa
void SPT_Sa_init()
{
  memset( &Sa, 0, sizeof(Sa) );
}

uint16_t SPT_Sa_read(uint8_t* c)
{
  uint16_t* w = (uint16_t*)c;    // unsigned twobyte value, consistently observed to be 834 bytes
  uint16_t numBytesChunk = w[1]; // skip bytes 'S','a'
  
  uint16_t ofs = 4; // skip bytes 'S','a',<numBytesChunk,##>
  memset( &Sa, 0, numBytesChunk );
  memcpy( &Sa, c+ofs, numBytesChunk );
  
  return numBytesChunk;
}

void SPT_Sa_fwrite(FILE* file)
{
  if( NULL == file ) return;
  
  uint16_t numBytesChunk = sizeof(Sa);
  fwrite("Sa", 1, 2, file);
  fwrite(&numBytesChunk, 2, 1, file);
  fwrite(&Sa, numBytesChunk, 1, file);
}

#endif

//end SPT_Sa.h
