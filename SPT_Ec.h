/* SPT_Ec.h

 parsing and processing of SPT Ec chunk data
 see SPT_types.h for description of SPT chunks
*/
#ifndef SPT_Ec_h
#define SPT_Ec_h

#include "SPT_types.h"
#include "SPT_common.h"

SPT_Ec Ec;

void SPT_Ec_init()
{
  memset( &Ec, 0, sizeof(Ec) );
}

uint16_t SPT_Ec_read(uint8_t* c)
{
  uint16_t* w = (uint16_t*)c;     // unsigned twobyte value
  uint16_t numBytesChunk = w[1];  // skip bytes 'E','c'
  
  uint16_t ofs = 4; // skip bytes 'E','c',<numBytesChunk,##>
  memset( &Ec, 0, numBytesChunk );
  memcpy( &Ec, c+ofs, numBytesChunk );
  
  return numBytesChunk;
}

void SPT_Ec_fwrite(FILE* file)
{
  if( NULL == file ) return;
  
  uint16_t numBytesChunk = sizeof(Ec);
  fwrite("Ec", 1, 2, file);
  fwrite(&numBytesChunk, 2, 1, file);
  fwrite(&Ec, numBytesChunk, 1, file);
}


#endif

//end SPT_Ec.h
