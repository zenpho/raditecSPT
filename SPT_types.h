/* SPT_types.h

 I am not the author of the SPT fileformat. I am writing
 this to decode and encode pattern data. I want to move
 data between the Groove and Line Sequencers and to
 import and export Standard Midi Format (SMF) files.

 SPT data files appear to contain blocks/chunks of data
 each chunk has an id (2 chars) e.g. "Tp" "Sa" "Il"
 
 Most chunk ids start with a capital letter, a deviation
 being the SPT file header, which always begins "tP"
 with a lowercase 't' usually followed by a two byte
 sequence 0x1B 0x00 which may be a file format indicator.

 SPT data files indicate number of 'bytes in this chunk'
 immediately following the chunk id... These values are
 used (in this software) for fread() and fwrite() calls
 not stored in the struct member variables.
 
 Some exceptions exist. See "Il" comments particularly.
 
 Most chunk sizes seem fixed, e.g. "Tp" 48 bytes with
 sizes stored usually two-bytes little endian. The "Tp"
 chunk header on disk appears as 0x54 0x70 0x30 0x00.

 At time of writing, most chunk structures are a mystery!
*/
#ifndef SPT_types_h
#define SPT_types_h

#include <stdint.h>

typedef struct {
  uint16_t mystery;                 // ? version number ?
} SPT_tP;

typedef struct {
  char name[12];                    // 12 bytes incl. null terminator
  uint8_t lenbar;                   // 24 = Len:1/8.  192 = Len:8/8.
  uint8_t numbars;                  // for patn retrig
  
  uint8_t mysteryData[34];          // ?
} SPT_Tp;

typedef struct {
  uint8_t mysteryData[834];         // ?
                                    // consistently observed to be 834 bytes
} SPT_Sa;

typedef struct {
  uint8_t mysteryData[256];         // ?
                                    // consistently observed to be 256 bytes
} SPT_Im;

typedef struct {
  uint8_t mysteryData[128];         // ?
                                    // consistently observed to be 128 bytes
} SPT_Pm;

typedef struct {
  uint8_t mysteryData[40];          // ?
                                    // consistently observed to be 40 bytes
                                    // often 20 bytes repeated
                                    // 0x30 0x80 0x18 0x80 0x01 0x64 0xff [0x00..]-13bytes
} SPT_Ec;

/** header for all Il chunks

 Several subtypes of Il chunks seem to exist
 'G' = Groove, drum parts seem to be 162 bytes.
 'A' = Analog, automation seem to be 36 bytes.
 'M' = ?midi, mute? seem to be 34 bytes.
 'P' = ?part, play? seem to be 164 bytes.
 
 This is complicated further due to variable length data
 (called "vData" here) which may be stored following the
 "main" structure data.
 
 For this reason the SPT_Il_M, _P, _G, _A structs below
 all begin with this common SPT_Il_head structure.
 
 An array of SPT_Il_vData structs will follow, if the
 numBytesVData valus is nonzero.
*/
typedef struct
{
  uint16_t numBytesStruc;           // num bytes in structure
  uint16_t numBytesVData;           // num bytes of variable data

  char     type;                    // A = analog, automation
                                    // G = groove, drum parts
                                    // M = ?midi, mute?
                                    // P = ?part, play?
  uint8_t  counter;
  char     name[8];                 // 8 bytes incl. null termination
} SPT_Il_head;

/** config data for Il type M = ?midi, mute? */
typedef struct
{
  SPT_Il_head h;
  uint8_t  mysteryData[20];         // ?
} SPT_Il_M;

/** config data for Il type P = ?part, play? */
typedef struct
{
  SPT_Il_head h;
  uint8_t  mysteryData[150];        // ?
} SPT_Il_P;

/** config data for Il type G = Groove, drum parts */
typedef struct
{
  SPT_Il_head h;
  
  // ?? = CONT.   ?? = ONCE.  ?? = RPT.
  // ?? = VOI.    ?? = TRG.
  
  uint8_t  linelen;                 // steps on this line
  uint8_t  lineres;                 // tick-type per step
                                    // 6=16, 7=12th, 8=8th, 9=, 10=
  
  uint8_t  mystery1[140];           // ? groove voice params ?
  
  uint8_t  pitch;                   // midi note num
  uint8_t  editplus;                // edit +XX
  uint8_t  editvelo;                // edit velocity
  uint8_t  lenbar;                  // 24 = Len:1/8. 192 = Len:8/8.
  uint8_t  numbars;                 // for patn retrig
  
  uint8_t  mystery2;                // ?
  
  // NB typically variable data follows these values - see numBytesVData
} SPT_Il_G;

/** variable data for Il  */
typedef struct
{
  uint16_t type;
  uint16_t pos;
} SPT_Il_vData;

/** config data for Il type A = Analog, automation */
typedef struct
{
  SPT_Il_head h;

  uint8_t  target;                  // target type for this line
                                    //   DSyn > Play > Note
                                    //   0x00=Kick ... 0x0a=Perc3,
                                    //   0x0c=Dsyn1 ... 0x0e=Dsyn2,
                                    //   0x90=Ext MIDI ch1 ... 0x9f=Ext MIDI ch16
  
  uint8_t  glide;                   // portamento glide rate
  uint8_t  pitch;                   // trigger midi note num
  uint8_t  len;                     // trigger gate duration
  uint8_t  transposing;             // if set, pitches are transposing
  uint8_t  grooving;                // if set, timing is grooving
  
  uint8_t  mysteryData[16];         // ?
  
  // NB typically variable-length data follows these values - see numBytesVData
} SPT_Il_A;

typedef struct {
  uint8_t mysteryData[14];          // ?
                                    // consistently observed to be 14 bytes
} SPT_Ar;

#endif

//end SPT_types.h
