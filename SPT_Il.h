/* SPT_Il.h

 parsing and processing of SPT Il chunk data
 NB: spelling here UPPERCASE INDIGO, lowercase lemur
 
 see SPT_types.h for description of SPT chunks
 
 efficiency and clarity of code in SPT_Il.h is not good!
 my excuse is that "this is a first draft of a hack tool
 to reverse engineer the SPT format" but that's a weak
 excuse. i'm not too proud of the clunkiness here :(
 
 probably the first priority is to clean up storage with
 void pointers to void* Ils and simply cast to one of
 struct SPT_Il_G or _A or _M or _P - avoiding the need
 for multiple arrays of each separate datatype... it
 would also be good to avoid the macro fwrite nonsense!
*/
#ifndef SPT_Il_h
#define SPT_Il_h

#include "SPT_types.h"

/** arrays here store pointers to config and vData structs

 Il chunks seem to have sub types 'G', 'A', 'M', 'P'.
 This is complicated further due to variable length data
 (called "vData" here) used only sometimes by 'G' and 'A'
*/
SPT_Il_G*        IlGs[32];
SPT_Il_A*        IlAs[32];
SPT_Il_M*        IlMs[32];
SPT_Il_P*        IlPs[32];

SPT_Il_vData*    IlGVData[32];
SPT_Il_vData*    IlAVData[32];

/** zero all pointers ready for SPT_Il_parse() */
void SPT_Il_init()
{
  for(int i=0; i<32; i++) IlGs[i] = 0;
  for(int i=0; i<32; i++) IlGVData[i] = 0;
  
  for(int i=0; i<32; i++) IlAs[i] = 0;
  for(int i=0; i<32; i++) IlAVData[i] = 0;
}

/** free any memory allocated by SPT_Il_parse() */
void SPT_Il_cleanup()
{
  for(int i=0; i<32; i++)
  {
    if( IlGs[i] ) free( IlGs[i] );
    if( IlAs[i] ) free( IlAs[i] );
    if( IlMs[i] ) free( IlMs[i] );
    if( IlPs[i] ) free( IlPs[i] );
    
    if( IlGVData[i] ) free( IlGVData[i] );
    if( IlAVData[i] ) free( IlAVData[i] );
  }
}

/** parse data and allocate memory, see also SPT_Il_cleanup() */
uint16_t SPT_Il_parse(uint8_t* c)
{
  uint16_t* w = (uint16_t*)c;    // skip bytes 'I','l'
  uint16_t numBytesChunk = w[1]; // unsigned twobyte value, num bytes for this chunk
  uint16_t numBytesStruc = w[2]; // unsigned twobyte value, struct size (including this word)
  uint16_t numBytesVData = w[3]; // unsigned twobyte value, vData size in bytes (may be zero)
  char     type          = c[8]; // 'G', 'A', 'M', 'P', see below
  uint8_t  counter       = c[9]; // zero based indexing for arrays

  uint16_t ofs = 4; // skip bytes 'I','l',<numBytesChunk,##>

  if( type == 'P' )
  {
    IlPs[counter] = calloc( 1, numBytesStruc );
    memcpy( IlPs[counter], c+ofs, numBytesStruc );
  }
  
  if( type == 'M' )
  {
    IlMs[counter] = calloc( 1, numBytesStruc );
    memcpy( IlMs[counter], c+ofs, numBytesStruc );
  }

  if( type == 'G' )
  {
    IlGs[counter] = calloc( 1, numBytesStruc );
    memcpy( IlGs[counter], c+ofs, numBytesStruc );
    
    if( numBytesVData )
    {
      ofs = numBytesStruc + 4; // skip bytes 'I','l',<numBytesChunk,##>,<struc,..>
      
      IlGVData[counter] = calloc( 1, numBytesVData );
      memcpy( IlGVData[counter], c+ofs, numBytesVData );
    }
  }

  if( type == 'A' )
  {
    IlAs[counter] = calloc( 1, numBytesStruc );
    memcpy( IlAs[counter], c+ofs, numBytesStruc );
    
    if( numBytesVData )
    {
      ofs = numBytesStruc + 4; // skip bytes 'I','l',<numBytesChunk,##>,<struc,..>
      
      IlAVData[counter] = calloc( 1, numBytesVData );
      memcpy( IlAVData[counter], c+ofs, numBytesVData );
    }
  }
  
  return numBytesChunk;
}

/** use SPT_IL_FWRITE() for 'M' 'P' subtypes without vdata */
#define SPT_IL_FWRITE(file, src) \
  for(int i=0; i<32; i++) \
  { \
    /*items may not be stored contiguously, skip nullptrs*/ \
    if( 0 == src[i] ) continue;  \
    \
    /*Il chunk size = vDataSize + strucSize*/ \
    uint16_t numBytesVData = src[i]->h.numBytesVData; \
    uint16_t numBytesStruc = src[i]->h.numBytesStruc; \
    uint16_t numBytesChunk = numBytesVData + numBytesStruc; \
    fwrite("Il", 1, 2, file); /*two chars*/ \
    fwrite(&numBytesChunk, 2, 1, file); /*one twobyte value*/ \
    \
    /*items may not be stored contiguously, must write one-at-a-time*/ \
    fwrite(src[i], sizeof(*src[i]), 1, file); \
  } \

/** use SPT_IL_FWRITE() for 'G' 'A' subtypes */
#define SPT_IL_FWRITE_WITH_VDATA(file, src, srcVData) \
  for(int i=0; i<32; i++) \
  { \
    /*items may not be stored contiguously, skip nullptrs*/ \
    if( 0 == src[i] ) continue;  \
    \
    /*Il chunk size = vDataSize + strucSize*/ \
    uint16_t numBytesVData = src[i]->h.numBytesVData; \
    uint16_t numBytesStruc = src[i]->h.numBytesStruc; \
    uint16_t numBytesChunk = numBytesVData + numBytesStruc; \
    fwrite("Il", 1, 2, file); /*two chars*/ \
    fwrite(&numBytesChunk, 2, 1, file); /*one twobyte value*/ \
    \
    /*items may not be stored contiguously, must write one-at-a-time*/ \
    fwrite(src[i], sizeof(*src[i]), 1, file); \
    \
    /*items may have vData, skip those without*/ \
    if( 0 == srcVData[i] ) continue; \
    if( 0 == numBytesVData ) continue; \
    \
    uint16_t vDataNumBytesEach = sizeof(*srcVData[i]); \
    uint16_t vDataNumItems = numBytesVData / vDataNumBytesEach; \
    if( vDataNumItems ) fwrite(srcVData[i], vDataNumBytesEach, vDataNumItems, file); \
  } \

#endif

// end SPT_Il.h
