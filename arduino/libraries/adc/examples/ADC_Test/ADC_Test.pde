#include <adc.h>

int ledPin =  13;    // LED connected to digital pin 13

void setup()
{
  ADC_Init( ADC_PRESCALAR_AUTO );
  pinMode(ledPin, OUTPUT);     
  Serial.begin(9600);
}

void loop()
{
  ADC_Sample_t  val;
  
  digitalWrite(ledPin, HIGH);   // set the LED on
  digitalWrite(ledPin, LOW);    // set the LED off
  digitalWrite(ledPin, HIGH);   // set the LED on
  val = ADC_Read( 0 );
  digitalWrite(ledPin, LOW);    // set the LED off

  Serial.print( "Channel 0 = " );
  Serial.print( (int)val );
  Serial.print( " " );
  Serial.print( (int)ADC_Read(1) );

  Serial.println( "");
  
  delay( 1000 );
}


