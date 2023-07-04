/* SPT_Pm.h

 parsing and processing of SPT Pm chunk data
 see SPT_types.h for description of SPT chunks
*/
#ifndef SPT_Pm_h
#define SPT_Pm_h

#include "SPT_types.h"
#include "SPT_common.h"

SPT_Pm Pm;

void SPT_Pm_init()
{
  memset( &Pm, 0, sizeof(Pm) );
}

uint16_t SPT_Pm_parse(uint8_t* c)
{
  uint16_t* w = (uint16_t*)c;     // unsigned twobyte value
  uint16_t numBytesChunk = w[1];  // skip bytes 'P','m'
  
  uint16_t ofs = 4; // skip bytes 'P','m',<numBytesChunk,##>
  memset( &Pm, 0, numBytesChunk );
  memcpy( &Pm, c+ofs, numBytesChunk );
  
  return numBytesChunk;
}

void SPT_Pm_fwrite(FILE* file)
{
  if( NULL == file ) return;
  
  uint16_t numBytesChunk = sizeof(Pm);
  fwrite("Pm", 1, 2, file);
  fwrite(&numBytesChunk, 2, 1, file);
  fwrite(&Pm, numBytesChunk, 1, file);
}


#endif

//end SPT_Pm.h
