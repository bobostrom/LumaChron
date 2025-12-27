#ifndef MYCONSTANTS
#define MYCONSTANTS
//#include <map>

enum Colors {RED=0,GREEN=1,BLUE=2};

enum commands : char{
        // GETBRIGHTNESS = 'g',
        SETBRIGHTNESS = 'b',
        RTC_UPDATE = 'u',
        COUNTDOWN = 'd',
        CLEARCOLORCRON = 'l',
        LOADCOLORCRON = 'm',
        TOGGLEANTIFLICKER = 'f',
        START = 's',
        STOP = 't',
        LAPPAUSE = 'p',
        RESET = 'r',
        CLOCKMODE = 'c',
        STOPWATCHMODE = 'w',
        ACK = 0x06,
        NACK = 0x15,
        EMPTY = 0x00
    };



static const uint8_t NUMPIXELSPERSEGMENT = 7;
static const uint8_t NUMSEGMENTSPERDIGIT = 7;
// static const uint8_t NUMDIGITSPERDISPLAY = 1;
static const uint8_t NUMDIGITSPERDISPLAY = 6;
static const uint8_t NUMCOLONSPERDIGIT = 1;
static const uint8_t NUMPIXELSPERCOLON = 4;
static const uint8_t NUMPIXELSPERDECIMAL = 2;
static const uint8_t NUMDECIMALSPERDIGIT = 1;
static const uint8_t NUMPIXELSPERDIGIT = ((NUMPIXELSPERSEGMENT*NUMSEGMENTSPERDIGIT)+(NUMPIXELSPERCOLON*NUMCOLONSPERDIGIT)+(NUMPIXELSPERDECIMAL*NUMDECIMALSPERDIGIT));
static const uint16_t NUMPIXELSPERDISPLAY = (NUMDIGITSPERDISPLAY*NUMPIXELSPERDIGIT);
static const uint colonStart = 0;
static const uint colonEnd = colonStart+NUMPIXELSPERCOLON-1;
static const uint decimalPtStart = colonEnd+1;
static const uint decimalPtEnd = decimalPtStart + NUMPIXELSPERDECIMAL-1;
static const uint segmentsStart = decimalPtEnd+1;
static const uint segmentsEnd = segmentsStart+NUMDIGITSPERDISPLAY*NUMSEGMENTSPERDIGIT*NUMPIXELSPERSEGMENT-1;

static const uint SUCCESS = 1;





#endif
