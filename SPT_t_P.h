/* SPT_t_P.h

 parsing and processing of SPT tP chunk data
 NB: spelling here lowecase tango, UPPERCASE PAPA
 see also SPT_Tp.h

 see SPT_types.h for description of SPT chunks
*/
#ifndef SPT_t_P_h
#define SPT_t_P_h

#include "SPT_types.h"
#include "SPT_common.h"

// NB: lowercase tango, UPPERCASE PAPA
SPT_tP tP;

void SPT_tP_init()
{
  memset( &tP, 0, sizeof(tP) );
}

uint16_t SPT_tP_parse(uint8_t* c)
{
  uint16_t numBytesChunk = sizeof(tP);
  uint16_t ofs = 2; // skip bytes 't','P'
  memset( &tP, 0, numBytesChunk );
  memcpy( &tP, c+ofs, numBytesChunk );
  
  return numBytesChunk;
}

void SPT_tP_fwrite(FILE* file)
{
  if( NULL == file ) return;
  
  uint16_t numBytesChunk = sizeof(tP); // not written to file
  fwrite("tP", 1, 2, file);
  fwrite(&tP, numBytesChunk, 1, file);
}

#endif

// end SPT_t_P.h
