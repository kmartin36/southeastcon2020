/*
 * ArenaControl.h
 *
 * I encourage everyone to read through the code and help me identify any
 *    issues. This will make the arena electronics better for both me,
 *    your team, and all the other teams.
 *
 * Any person (or team) who identifies any issues will have their name (or
 *    names) listed in the collaboration section below.
 *
 * Note that only the first person to find each issue is listed.
 *
 * Trivial question - the first person (or team) who can answer this will also
 *    get their name/names listed in the honors section below.
 *    "What digit sequence occurs at position 18,900,827 and why is it important?"
 *    (the correct answer was submitted by Ben Wiegand [BigBenMOG] of the Bob
 *    Jones University team. The answer is that the sequence 0-3-1-4-2-0-2-0 is
 *    found starting at that position, which corresponds to the date of the
 *    Pi Day challenge during SoutheastCon 2020 (03/14/2020).
 *
 * COLLABORATORS / HONORS (for helping identify new issues in the software):
 *    YOUR NAME COULD BE HERE FOR EVERYONE TO SEE!
 *
 * Luc Lagarde USM - found a typo in my header comments (2029 instead of 2020)
 * Ammar Ratnani - issue with first incorrect digit not counted (also reported 
 *       by dskaggs (Dylan) of WKU`
 * Luc Lagarde USM - suggested adding PROGMEM to pi (needed once #digits 
 *       was bumped to 10k) - issue reported by mdixon2
 * dskaggs (Dylan of WKU - discovered I dropped PROGMEM on my lastest update
 * John Barnes UK - suggested using a larger value than int for the score, 
 *       and found a bug where I used a global name instead of the local param
 * Paul MacDougal - code review/analysis/test jig, resulting in several bug
 *                  fixes, and suggested dropping DEBOUNCE_DELAY to 24ms
 * 
 * VERSIONS:
 *    1.0 - initial release
 *    1.+ - bug fixes, bump digits to 10k, more bug fixes
 *    2.0 - add PROGMEM back, add VERSION # to track what is loaded on the board
 *    2.1 - change score to long, fix piDgit param
 *    2.2 - Paul MacDougal's code suggestions - thanx!
 *    3.0 - add support for LCD controller, switch to uS timing
 */

#define HELLO   "SoutheastCon 2020 Hardware Arena Control"
#define VERSION "Version 3.0, released March 1, 2020"

#define MATCH_RUNTIME   (180L*1000L)  // 3 minutes (in milliseconds)

// DEBOUNCE_DELAY is 1ms shorter than 25ms to include the processing
//     time for the debounce (less than 1ms, but this makes sure that
//     25ms down and 25ms up works!
#define DEBOUNCE_DELAY   (24500ul) // debounce time in usec
#define FLASH_INTERVAL   25        // ms time to flash LEDs on wrong push

// PIN_OFFSET is the first pin used for this project. Every even pin
//    is an button, followed by the matching LED on the odd pin
//
//      ------ Pin LED/Button mapping -------
//      ID       0  1  2  3  4  5  6  7  8  9
//      LED     26 28 30 32 34 36 38 40 42 44 
//      BUTTON  27 29 31 33 35 37 39 41 43 45
//
// The other side of the LED and button are both tied to ground
//    This allows a single bus wire to tie 2x wires of each of
//    of the 10x buttons together to one connection - ground.
#define NUM_BUTTONS   10    // 10 digits - zero through nine
#define PIN_OFFSET    26    // Mega2560 first pin used
#define LED_PIN(n)    (PIN_OFFSET + (2*n))  // LEDs on the even pins
#define BUTTON_PIN(n) (LED_PIN(n) + 1)      // buttons on the odd pins

struct {
   int buttonState;                 // the current reading from the input pin
   int lastButtonState;             // the previous reading from the input pin
   uint32_t lastDebounceTime;       // the last time the output pin was toggled
} buttonState[NUM_BUTTONS];


uint32_t startTimestamp = 0;

uint32_t flashTimeout;              // if set, the time to turn off the LEDs

boolean inSequence = true;          // if true, we are still sequencing correctly
int numSequenced = 0;               // total number of digits scored correctly
int extraNotSequenced = 0;          // total number of other digit presses

int piDigitPosn = 0;                // current position in the Pi sequence

