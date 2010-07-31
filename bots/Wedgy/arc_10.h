#define ENABLE_BIT_DEFINITIONS
#include <avr/io.h>
#define CPUCLK = 8000000
#define BAUD = 19200

// ADC Channels are just numbers passed to GetADC()

enum {
    left_sharp = 0,
    left_floor = 1,
    center_floor = 2,
    right_floor = 3,
    right_sharp = 7,
    motor_forward = 0,
    motor_reverse = 1
};

// Port B

#define prog_led 4      // Output
#define option_a 6      // Input
#define option_b 7      // Input

// Port C

#define right_motor_dir 4       // Output
#define right_motor_pwm Pwm1a
#define right_encoder_b 5       // Input
#define right_servo_out 5       // Output
#define left_motor_dir 3        // Input
#define left_motor_pwm Pwm1b
#define left_encoder_b 2        // Input
#define left_servo_out 2        // Output
#define SDA            1        // I2C
#define SCL            0       // I2C

// Port D

#define right_encoder_a 2       // Input INT0
#define left_encoder_a 3        // Input INT1

// Library routines
//
// These routines pass parameters by value since a typical situation is to
// pass constants (e.g. 1/2 speed forward, servo position X) otherwise a global
// would have to be declared to hold the value and then the routine called.

void right_servo(unsigned);
void left_servo(unsigned);
void set_left_motor_pwm(int);
void set_right_motor_pwm(int);
void initialize_pwm(void);
int getadc(char);
void waitms(int);
