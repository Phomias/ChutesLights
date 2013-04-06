/*
  Chutes Sketch:
  A sketch to light up chutes.
*/

// The G35 Arduino library for GE Color Effects G-35 holiday lights.
#include <G35String.h>
//////// G35 SETUP //////////
// How many lights are plugged in...
#define LIGHT_COUNT (43)
// ...and where they are plugged in on our Arduino.
#define G35_PIN (6)
G35String lights(G35_PIN, LIGHT_COUNT);
/////////////////////////////

// Throughout the sketch I have comments prefaced with 'Sketch Size'
// and these comments will tell you something about the sketch size
// before and after a change was made. It should help you figure out
// how big certain data and code are in memory.

// Sketch Size: For reference BasicExample was 5,184 bytes.

typedef struct Blight {
  // logical_ == logical light index number, i.e. what we like to think about as humans (fork is 1-16 etc)
  char logical_index;
          // b[xyz]_ == bike-relative coordinate
  short bx; // +bx == port   -bx == starboard    -10 to 10
  short by; // +by == up     -by == down           0 == ground    
  short bz; // +bz == fore   -bz == aft            0 == rear contact patch
} Blight;

// i[xy] == coordinates in the left_side.xcf image in pixels
// ix+ == fore     ix- == aft
// iy+ == down     iy- == up
const short iy_kGroundLevel = 660;
const short ix_kFrontTireContactPatch = 800;
const short ix_kRearTireContactPatch = 174;
const short iy_kTopOfFork = 178;
const short ix_kTopOfFork = 630;

#define BY_FROM_IY(iy) (iy_kGroundLevel - (iy))
short by_from_iy( short iy );
short by_from_iy( short iy ) {
  return  BY_FROM_IY(iy);
}

#define BZ_FROM_IX(ix) ((ix) - ix_kRearTireContactPatch)
short bz_from_ix( int ix );
short bz_from_ix( int ix ){
  return BZ_FROM_IX(ix);
}

// Takes a kLeft, kRight coordinate and converts it to bx style coordinate.
#define BX_LEFT    10
#define BX_CENTER   0
#define BX_RIGHT  -10

// Indicator
Blight chutes_indicator = { 17,  BX_CENTER, BY_FROM_IY(215), BZ_FROM_IX(604) };

const char kNumLEDsOnDisplay = 38;
// Blights in struct:     39     38      4          3          2           1
// Binary sketch size: 8,956         6,106 {78} 6,028 {84} 5,944 {186} 5,758
// Macrod sketch size:        6,070  5,832 // Good, each light takes 7 bytes -> what it should (i.e. two shorts and a char).
// This struct holds an array of blight structs which represent info 
// like logical index, coordinates, color, and brightness.
struct Blight chutes_lights[kNumLEDsOnDisplay] = {
   /*logical_index  
          bx                      by               bz */
   // Fork
   {  1,  BX_LEFT  , BY_FROM_IY(179), BZ_FROM_IX(632) }
  ,{  2,  BX_LEFT  , BY_FROM_IY(263), BZ_FROM_IX(678) }
  ,{  3,  BX_LEFT  , BY_FROM_IY(341), BZ_FROM_IX(723) }
  ,{  4,  BX_LEFT  , BY_FROM_IY(425), BZ_FROM_IX(760) }
  ,{  5,  BX_LEFT  , BY_FROM_IY(422), BZ_FROM_IX(765) }
  ,{  6,  BX_LEFT  , BY_FROM_IY(345), BZ_FROM_IX(718) }
  ,{  7,  BX_LEFT  , BY_FROM_IY(260), BZ_FROM_IX(680) }
  ,{  8,  BX_LEFT  , BY_FROM_IY(178), BZ_FROM_IX(636) }
  ,{  9,  BX_RIGHT , BY_FROM_IY(178), BZ_FROM_IX(636) }
  ,{ 10,  BX_RIGHT , BY_FROM_IY(260), BZ_FROM_IX(680) }
  ,{ 11,  BX_RIGHT , BY_FROM_IY(345), BZ_FROM_IX(718) }
  ,{ 12,  BX_RIGHT , BY_FROM_IY(422), BZ_FROM_IX(765) }
  ,{ 13,  BX_RIGHT , BY_FROM_IY(425), BZ_FROM_IX(760) }
  ,{ 14,  BX_RIGHT , BY_FROM_IY(341), BZ_FROM_IX(723) }
  ,{ 15,  BX_RIGHT , BY_FROM_IY(263), BZ_FROM_IX(678) }
  ,{ 16,  BX_RIGHT , BY_FROM_IY(179), BZ_FROM_IX(632) }
  // Down tube
  ,{ 18,  BX_RIGHT , BY_FROM_IY(250), BZ_FROM_IX(608) }
  ,{ 19,  BX_RIGHT , BY_FROM_IY(327), BZ_FROM_IX(559) }
  ,{ 20,  BX_LEFT  , BY_FROM_IY(327), BZ_FROM_IX(559) }
  ,{ 21,  BX_LEFT  , BY_FROM_IY(250), BZ_FROM_IX(608) }
  ,{ 22,  BX_CENTER, BY_FROM_IY(260), BZ_FROM_IX(609) }
  ,{ 23,  BX_CENTER, BY_FROM_IY(338), BZ_FROM_IX(563) }
  // Bottom backet to rear hub left
  ,{ 24,  BX_LEFT  , BY_FROM_IY(354), BZ_FROM_IX(472) }
  ,{ 25,  BX_LEFT  , BY_FROM_IY(367), BZ_FROM_IX(389) }
  ,{ 26,  BX_LEFT  , BY_FROM_IY(407), BZ_FROM_IX(305) }
  ,{ 27,  BX_LEFT  , BY_FROM_IY(458), BZ_FROM_IX(225) }
  ,{ 28,  BX_LEFT  , BY_FROM_IY(542), BZ_FROM_IX(170) }
  // Rack
  ,{ 29,  BX_LEFT  , BY_FROM_IY(467), BZ_FROM_IX(143) }
  ,{ 30,  BX_CENTER, BY_FROM_IY(411), BZ_FROM_IX(108) }
  ,{ 31,  BX_RIGHT , BY_FROM_IY(467), BZ_FROM_IX(143) }
  // Rear hub to bottom bracket Right
  ,{ 32,  BX_RIGHT , BY_FROM_IY(542), BZ_FROM_IX(170) }
  ,{ 33,  BX_RIGHT , BY_FROM_IY(458), BZ_FROM_IX(225) }
  ,{ 34,  BX_RIGHT , BY_FROM_IY(407), BZ_FROM_IX(305) }
  ,{ 35,  BX_RIGHT , BY_FROM_IY(367), BZ_FROM_IX(389) }
  ,{ 36,  BX_RIGHT , BY_FROM_IY(354), BZ_FROM_IX(472) }
  // Upper rear triangle
  ,{ 37,  BX_CENTER, BY_FROM_IY(318), BZ_FROM_IX(455) }
  ,{ 38,  BX_CENTER, BY_FROM_IY(259), BZ_FROM_IX(399) }
  ,{ 39,  BX_CENTER, BY_FROM_IY(332), BZ_FROM_IX(343) }
};

// gindex is the indexing used by the g35 library
// gindex =>  logical index
// 0      =>  50
// 49     =>  1
// Sketch Size: taking in and returning an int: 6070
//                                  ... a char: 6060
//            : As a macro : still 6060! I guess a function call takes about the same space as a function call.
char gindex_from_logical_index( char logical_index );
char gindex_from_logical_index( char logical_index ) {
  return LIGHT_COUNT - logical_index;
}
#define GINDEX_FROM_LOGICAL_INDEX(logical_index) (LIGHT_COUNT - (logical_index))


// TODO: This function sets the indicator too. :/
void MakeChutesAndLaddersOneColor( color_t color );
void MakeChutesAndLaddersOneColor( color_t color ) {
  lights.fill_color(
    gindex_from_logical_index(kNumLEDsOnDisplay + 1),
    kNumLEDsOnDisplay,
    G35::MAX_INTENSITY,
    color
  );
}

void setup() {
  lights.enumerate();
  lights.fill_color(0, LIGHT_COUNT, G35::MAX_INTENSITY, COLOR_BLACK);
}

// Sketch size: before with floats and ints: 6060
//            : after with mostly shorts and chars: 6032
void FillFromFrontToBack( int ms_transition, color_t color );
void FillFromFrontToBack( int ms_transition, color_t color ) {

  // Find farthest forward and farthest aft lights.
  short bz_farthest_forward = chutes_lights[0].bz;
  short bz_farthest_aft = chutes_lights[0].bz;
  
  // Sketch size: this loop takes 80 bytes. I.e. It's 0.5% or 1/200th of the program.
  //              For comparison, The rest of our code takes about 800 bytes.
  for( char i = 0; i < sizeof(chutes_lights)/sizeof(chutes_lights[0]); i++) {
    if ( chutes_lights[i].bz > bz_farthest_forward ) 
      bz_farthest_forward = chutes_lights[i].bz;
    if ( chutes_lights[i].bz < bz_farthest_aft ) 
      bz_farthest_aft = chutes_lights[i].bz;
  }
  
  short bz_span = bz_farthest_forward - bz_farthest_aft;
  
  short ms_step = 10;
  for ( short ms = 0; ms <= ms_transition; ms += ms_step, delay(ms_step) ) {
    float progress = 1.0 * ms / ms_transition;
    for ( char i = 0; i < sizeof(chutes_lights)/sizeof(chutes_lights[0]); i++ ) {
      if ( (1.0 * bz_farthest_forward - chutes_lights[i].bz) / bz_span <= progress ) {
        lights.set_color_if_in_range(
          gindex_from_logical_index(chutes_lights[i].logical_index),
          G35::MAX_INTENSITY,
          color
        );
      }
    }
  }
}

void loop() {
  //MakeChutesAndLaddersOneColor( COLOR_RED );
  //delay(1000);
  //MakeChutesAndLaddersOneColor( COLOR_BLUE );
  //delay(1000);
  FillFromFrontToBack( 500, COLOR_RED );
  FillFromFrontToBack( 500, COLOR_BLUE );
  FillFromFrontToBack( 500, COLOR_ORANGE );
  FillFromFrontToBack( 500, COLOR_GREEN );
}

