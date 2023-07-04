/* SPT_common.h

 functions used to parse and process SPT data
 see SPT_types.h for description of SPT chunks
*/
#ifndef SPT_common_h
#define SPT_common_h

#include "SPT_types.h"
#include <string.h>

// find 2-byte chunkid 'needle' in data 'haystack'
uint8_t* SPT_chunkStart(uint8_t* data, uint16_t dataSize, const char* id)
{
    if( !data || !dataSize ) return 0;
  
    uint16_t remain = dataSize;
    while( remain-- )
    {
        if( strncmp((char*)data, id, 2) == 0 ) return data;
        data++;
    }
  
    return data;
}


#endif

// end SPT_common.h
