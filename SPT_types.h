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

/** variable data for Il */
typedef struct
{
  uint16_t a;
  uint16_t b;
} SPT_Il_vData;

/** config data for Il type A = Analog, automation */
typedef struct
{
  SPT_Il_head h;

  uint8_t  linelen;                 // number of steps on this line
                                    // this only controls display/playback loop behaviour
                                    // the trigger data is stored in variable length "vData"

  uint8_t  mystery1[2];             // ?

  uint8_t  lineres;                 // ticks per step
                                    // 0=192th  1=96th   2=64th   3=48th
                                    // 4=32th   5=24th   6=16th   7=8th
                                    // 8=6th    9=4th    10=3th   11=2th
                                    // 13=1bar  14=2bar  15=3bar  16=4bar
                                    // 17=6bar  18=8bar  19=12bar
  
  uint8_t  targetPG[2];             // target <param><group>
                                    // some configs set nonzero additional "targetSD" bytes
                                    // 0xa3, 0x28 means ext midi (see targetSD)
                                    // 0xa4, 0x28 means dsyn (see targetSD)
  
                                    // these configs zero the additional "targetSD" bytes
                                    // 0xbf, 0x28 means asyn tgrp 1
                                    // 0xc0, 0x28 means asyn tgrp 2
                                    // 0xc1, 0x28 means asyn tgrp 3
                                    // 0xa4, 0x2a means asyn mm cutoff
                                    // 0x71, 0x2a means asyn lp cutoff exp
  
                                    // 0x76, 0x0e means filterbank bypass
                                    // 0x6e, 0x11 means filterbank band lpf
                                    // 0x96, 0x11 means filterbank band 1
                                    // 0xbe, 0x11 means filterbank band 2
                                    // 0xe6, 0x11 means filterbank band 3
                                    // 0x0e, 0x12 means filterbank band 4
                                    // 0x36, 0x12 means filterbank band 5
                                    // 0x5e, 0x12 means filterbank band 6
                                    // 0x86, 0x12 means filterbank band 7
                                    // 0x5e, 0x12 means filterbank band 8
                                    // 0xd6, 0x0d means filterbank band hpf
  
  uint8_t  glide;                   // portamento glide rate
  
  uint8_t  mystery2;                // ?
  
  uint8_t  editgate;                // edit gate length and type
                                    // 8th, 4th, 2th, 1, 2, 4, etc
                                    // DECAY, HARD, PULSE, SOFT, ATTACK

  uint8_t  editveldep;              // edit velocity, depth
                                    // 0 to 126 (when depth, displayed as -63 to +63)
  
  uint8_t  targetSD[2];             // some configurations leave these bytes zeroed
                                    // when "targetPG" set {0xa4, 0x28} this means "dsyn"
                                    // and "targetSD" set nonzero as below for "which dsyn"
                                    // {0x00, 0x00} means kick
                                    // {0x01, 0x00} means snare
                                    // {0x02, 0x00} means clhh
                                    // {0x03, 0x00} means ophh
                                    // {0x04, 0x00} means clap
                                    // {0x05, 0x00} means tomhi
                                    // {0x06, 0x00} means midhi
                                    // {0x07, 0x00} means lowhi
                                    // {0x08, 0x00} means perc1
                                    // {0x09, 0x00} means perc2
                                    // {0x0a, 0x00} means perc3
                                    // {0x0b, 0x00} seems UNUSED
                                    // {0x0c, 0x00} means dsyn1
                                    // {0x0d, 0x00} means dsyn1
                                    // {0x0e, 0x00} means dsyn3
                                    //
                                    // when "targetPG" set {0xa3, 0x28} this means "ext MIDI"
                                    // and "targetSD" set nonzero as below for "ext MIDI NOTE"
                                    // {0x90, 0x00} .. {0x9f, 0x00} means note ch1 .. ch16
                                    // i.e. first "targetSD" byte statusbyte for midi note on
                                    //      second "targetSD" byte zeroed
                                    //
                                    // when "targetPG" set {0xa3, 0x28} this means "ext MIDI"
                                    // and "targetSD" set nonzero as below for "ext MIDI CTL"
                                    // {0xb0, 0xNN} .. {0xbf, 0xNN} means cc control ch1 .. ch16
                                    // i.e. first "targetSD" byte statusbyte for midi ctl
                                    //      second "targetSD" byte NN for desired ctl number
                                    //      1 modwheel, 2 breath, etc
  
  uint8_t  editpitch;               // edit pitch
  
  uint8_t  mystery3;                // ?
  
  uint8_t  configtransp;            // config transposing, revpoint, grooving
  
  uint8_t  mystery4[12];            // ?
  
  // NB typically variable-length data follows these values - see numBytesVData
} SPT_Il_A;

typedef struct {
  uint8_t mysteryData[14];          // ?
                                    // consistently observed to be 14 bytes
} SPT_Ar;

#endif

//end SPT_types.h
