/* SPT_parse.h

read (and parse) SPT data from global File_data
 
TODO - we might add SPT_parse() func here to
interpret and parse raw values particularly
for "Il" chunks as the meaning of values for
modulation, note, velo, gate, probability, etc
are context dependent.
*/
#ifndef SPT_parse_h
#define SPT_parse_h

#include "SPT_types.h"

#include "fileops.h"
#include "SPT_t_P.h"
#include "SPT_Tp.h"
#include "SPT_Sa.h"
#include "SPT_Pm.h"
#include "SPT_Ec.h"
#include "SPT_Im.h"
#include "SPT_Il.h"
#include "SPT_Ar.h"

/**
SPT_read() ingests bytes from File_data and
calls SPT_read_...() funcs
 reading global structs tP Tp Sa Pm Ec Im
 and reading global arrays IlGs IlAs IlMs IlPs
 and reading global arrays IlGVData IlAVData
 
before calling SPT_read() you must populate the
global vars "File_size" and "File_data"
*/
void SPT_read()
{
  SPT_tP_init();
  SPT_Tp_init();
  SPT_Sa_init();
  SPT_Pm_init();
  SPT_Ec_init();
  SPT_Im_init();
  SPT_Il_init();
  SPT_Ar_init();

  uint16_t i=0;
  while(i<File_size)
  {
    uint8_t* c = File_data + i;
  
    if( c[0] == 't' && c[1] == 'P' ) i += SPT_tP_read(c);
  
    if( c[0] == 'T' && c[1] == 'p' ) i += SPT_Tp_read(c);
    if( c[0] == 'S' && c[1] == 'a' ) i += SPT_Sa_read(c);
    if( c[0] == 'P' && c[1] == 'm' ) i += SPT_Pm_read(c);
    if( c[0] == 'E' && c[1] == 'c' ) i += SPT_Ec_read(c);
    if( c[0] == 'I' && c[1] == 'm' ) i += SPT_Im_read(c);
    
    if( c[0] == 'I' && c[1] == 'l' ) i += SPT_Il_read(c);
    if( c[0] == 'A' && c[1] == 'r' ) i += SPT_Ar_read(c);
    else i++;
  }
}

#endif /* SPT_parse.h */
