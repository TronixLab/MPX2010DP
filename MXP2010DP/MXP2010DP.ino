/***********************************************************************************************************
  MXP2010DP operating characteristic (Datasheet)
  Characteristic   |   Symmbol    |    Value
  Pressure Range         Pop          0-10 kPa
  Full-scale Span        Vfss         24-26 mV, 25 mV (typical)
  Offset                 Voff         -1 - 1 mV
  Sensitivity           ~V/~P         2.5 mV/kPa

  Transfer function equation:
  Vout = Voff + Sensitivity*Pc     Where: P calculated pressure over the operating pressure range

  Pressure ports Pressure (P1), Vacuum (P2) side:
  Notes:  The pressure sensor is designed to operate with positive differential pressure applied, P1 > P2.
          The Pressure (P1) side may be identified by
                                        Part number    |    Case Type     |   Pressure (P1) Side Identifier
                                        MPX2010DP             344C               Side with Part Marking

  Pinout:
  Note: Face with the part marking side
  1. GND (w/ the pin mark)
  2. +OUTPUT
  3. +SUPPLY
  4. -OUTPUT
******************************************************************************************************************
  ADS1115 Recommended Operating Conditions:
  Characteristic   |   Min    |    Max
  POWER SUPPLY         2V          5.5V
  FSR                ±0.256V      ±6.144V

  Device   |   Resolution    |    Max sampling rate   |   Input channel
  ADS1115        16-bit                860 SPS                2(4) differential(single-ended)

  Full-Scale Range (FSR) and LSB Size:
    FSR   |   LSB SIZE (PER BIT)
  ±6.144V         187.5 μV
  ±4.096V         125 μV
  ±2.048V         62.5 μV
  ±1.024V         31.25 μV
  ±0.512V         15.625 μV
  ±0.256V         7.8125 μV

  I2C Address Selection:
  ADDR PIN  |  CONNECTION SLAVE ADDRESS
  GND             1001000 (0x48)
  VDD             1001001 (0x49)
  SDA             1001010 (0x4A)
  SCL             1001011 (0x4B)
******************************************************************************************************************
  MXP2010DP with 16-bit ADC module (ADS1115) test connection:
      MXP2010DP   |   ADS1115   |   Arduino
         GND           GND            GND
         VDD         +SUPPLY          3.3V
       +OUTPUT          A0            --
       -OUTPUT          A1            --
          --           SCL            SCL
          --           SDA            SDA
          --           ADDR           GND
******************************************************************************************************************/

// MXP2010DP with 16-bit ADC module (ADS1115) test code
#include <Wire.h>
#include <Adafruit_ADS1015.h>

Adafruit_ADS1115 ads(0x48);   // ADC object at I2C address 0x48 for addr pin = GND
int16_t adc0_1, adc0, adc1;   // variables to hold ADC readings
float multiplier = 0.0625F;   // use 2x gain +/- 2.048V = 0.0625mV/step

float Vout = 0.00F;           // output voltage from pressure sensor
float Voff = 0.00F;           // offset voltage (-1 to 1V) can be use for calibration *see datasheet
float Sensitivity = 2.50F;    // sensitivity in milli-volts per pressure unit *see datasheet
float Pc = 0.00F;             // calculated pressure value

void setup() {
  ads.begin();
  Serial.begin(9600);
  Serial.println("Getting differential reading from AIN0 (P) - AIN1 (N)");
  Serial.println();
  Serial.println();
  delay(1000);
  ads.setGain(GAIN_TWO);
}

void loop() {
  adc0 = ads.readADC_SingleEnded(0);          // read single AIN0
  adc1 = ads.readADC_SingleEnded(1);          // read single AIN
  adc0_1 = ads.readADC_Differential_0_1();    // read differential AIN0 - AIN1

  // from transfer function equation: Vout = Voff + Sensitivity*Pc
  Vout = adc0_1 * multiplier;                 // output voltage in mV
  Pc = (Vout - Voff)/Sensitivity;             // calculated pressure in kPa
  
  Serial.print("A0: ");
  Serial.print(adc0 * multiplier); Serial.print("mV   ");
  Serial.print("A1: ");
  Serial.print(adc1 * multiplier); Serial.print("mV   ");
  Serial.print("A0-A1: ");
  Serial.print(adc0_1 * multiplier); Serial.print("mV   ");
  Serial.print("Pc: ");
  Serial.print(Pc); Serial.print("kPa   ");
  Serial.println();
}
