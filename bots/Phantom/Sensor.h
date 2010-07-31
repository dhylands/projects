/****************************************************************************
*
*  Sensor.h
*
*  externs for senosr values
*
****************************************************************************/

#if !defined( SENSOR_H )
#define SENSOR_H

#define  HUNT_STATE_SPIN_LEFT    1
#define  HUNT_STATE_SPIN_RIGHT   2
#define  HUNT_STATE_GO_STRAIGHT  3

extern   uns8  gLineLO;
extern   uns8  gLineLI;
extern   uns8  gLineRO;
extern   uns8  gLineRI;

extern   uns16 gLineAvg;
extern   uns8  gLineThresh;

extern   uns16 gEyeL;
extern   uns16 gEyeR;

extern   uns16 gEyeLSide;
extern   uns16 gEyeRSide;

extern   uns8  gPortD;

#define  EYE_DETECTED_LEFT          0x01
#define  EYE_DETECTED_RIGHT         0x02
#define  EYE_DETECTED_LEFT_SIDE     0x04
#define  EYE_DETECTED_RIGHT_SIDE    0x08

extern   uns8  gEyeState;
extern   uns8  gPrevEyeState;

extern   uns8  gHuntDir;
extern   uns16 gHuntCounter;

#define  HUNT_SELECT_LEFT  0
#define  HUNT_SELECT_RIGHT 1

#define  PGM_SELECT_LINE_FOLLOW  0
#define  PGM_SELECT_SUMO         1

void InitSensors( void );
void ReadSensors( void );

void InitLineThresh( void );
int LineDetectedLeft( void );
int LineDetectedRight( void );

int IsBlackButtonPressed( void );
int IsDirectionSwitchLeft( void );
int IsRunStopRun( void );
int IsRunStopStop( void );


#endif   // SENSOR_H

