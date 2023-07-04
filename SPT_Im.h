/* SPT_Im.h

 parsing and processing of SPT Im chunk data
 see SPT_types.h for description of SPT chunks
*/
#ifndef SPT_Im_h
#define SPT_Im_h

#include "SPT_types.h"
#include "SPT_common.h"

SPT_Im Im;

void SPT_Im_init()
{
  memset( &Im, 0, sizeof(Im) );
}

uint16_t SPT_Im_parse(uint8_t* c)
{
  uint16_t* w = (uint16_t*)c;     // unsigned twobyte value
  uint16_t numBytesChunk = w[1];  // skip bytes 'I','m'
  
  uint16_t ofs = 4; // skip bytes 'I','m',<numBytesChunk,##>
  memset( &Im, 0, numBytesChunk );
  memcpy( &Im, c+ofs, numBytesChunk );
  
  return numBytesChunk;
}

void SPT_Im_fwrite(FILE* file)
{
  if( NULL == file ) return;
  
  uint16_t numBytesChunk = sizeof(Im);
  fwrite("Im", 1, 2, file);
  fwrite(&numBytesChunk, 2, 1, file);
  fwrite(&Im, numBytesChunk, 1, file);
}

#endif

//end SPT_Im.h
