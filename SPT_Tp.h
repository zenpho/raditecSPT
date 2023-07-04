/* SPT_Tp.h

 parsing and processing of SPT Tp chunk data
 NB: spelling here UPPERCASE TANGO, lowecase papa
 see also SPT_t_P.h

 see SPT_types.h for description of SPT chunks
*/
#ifndef SPT_Tp_h
#define SPT_Tp_h

#include "SPT_types.h"
#include "SPT_common.h"

SPT_Tp Tp;

void SPT_Tp_init()
{
  memset( &Tp, 0, sizeof(Tp) );
}

uint16_t SPT_Tp_parse(uint8_t* c)
{
  uint16_t* w = (uint16_t*)c;     // unsigned twobyte value
  uint16_t numBytesChunk = w[1];  // skip bytes 'T','p'
  
  uint16_t ofs = 4; // skip bytes 'T','p',<numBytesChunk,##>
  memset( &Tp, 0, numBytesChunk );
  memcpy( &Tp, c+ofs, numBytesChunk );
  
  return numBytesChunk;
}

void SPT_Tp_fwrite(FILE* file)
{
  if( NULL == file ) return;
  
  uint16_t numBytesChunk = sizeof(Tp);
  fwrite("Tp", 1, 2, file);
  fwrite(&numBytesChunk, 2, 1, file);
  fwrite(&Tp, numBytesChunk, 1, file);
}

#endif

//end SPT_Tp.h
