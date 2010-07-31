/****************************************************************************
*
*  Sensor.h
*
*  externs for senosr values
*
****************************************************************************/

#if !defined( SENSOR_H )
#define SENSOR_H

// ADC channel selection codes

#define EYE_R   0   // ADC0
#define LINE_R  1
#define LINE_C  2
#define LINE_L  3
#define ADC_4   4
#define ADC_5   5
#define ADC_6   6
#define EYE_L   7

#define  HUNT_STATE_SPIN_LEFT    1
#define  HUNT_STATE_SPIN_RIGHT   2
#define  HUNT_STATE_GO_STRAIGHT  3

extern   uns8  gLineL;
extern   uns8  gLineR;

extern   uns16 gLineAvg;
extern   uns8  gLineThresh;

extern   uns16 gEyeL;
extern   uns16 gEyeR;

#define  EYE_DETECTED_LEFT    0x01
#define  EYE_DETECTED_RIGHT   0x02

extern   uns8  gEyeState;
extern   uns8  gPrevEyeState;

extern   uns8  gHuntDir;
extern   uns16 gHuntCounter;

#define  HUNT_SELECT_LEFT  0
#define  HUNT_SELECT_RIGHT 1

#define  PGM_SELECT_LINE_FOLLOW  0
#define  PGM_SELECT_SUMO         1


void ReadSensors( void );

#endif   // SENSOR_H

