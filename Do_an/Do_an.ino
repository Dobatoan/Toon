#include <SoftwareSerial.h>
#include <TinyGPS.h>
#define   ACCEL_XOUT_H    0x3B
#define   ACCEL_XOUT_L    0x3C
#define   ACCEL_YOUT_H    0x3D
#define   ACCEL_YOUT_L    0x3E
#define   ACCEL_ZOUT_H    0x3F
#define   ACCEL_ZOUT_L    0x40
#define   TEMP_OUT_H      0x41
#define   TEMP_OUT_L      0x42
#define   GYRO_XOUT_H     0x43
#define   GYRO_XOUT_L     0x44
#define   GYRO_YOUT_H     0x45
#define   GYRO_YOUT_L     0x46
#define   GYRO_ZOUT_H     0x47
#define   GYRO_ZOUT_L     0x48
#define   ADDR_MPU6050     0x68
#include <Wire.h>
void mpu6050()
{
  Wire.begin();
  delay(10);
  Wire.beginTransmission(ADDR_MPU6050);
  Wire.write(0x1b);
  Wire.write(0b11000);
  Wire.endTransmission();


  Wire.beginTransmission(ADDR_MPU6050);
  Wire.write(0x6b);
  Wire.write(0x01);
  Wire.endTransmission();

}
int16_t getData_mpu6050(int8_t diachi)
{

  Wire.beginTransmission(ADDR_MPU6050);
  Wire.write(diachi);
  Wire.endTransmission();
  Wire.requestFrom(ADDR_MPU6050, 2);
  return (Wire.read() << 8) | Wire.read();
}

int16_t get_accx()
{
  return getData_mpu6050(ACCEL_XOUT_H);
}
int16_t get_accy()
{
  return getData_mpu6050(ACCEL_YOUT_H);
}
int16_t get_accz()
{
  return getData_mpu6050(ACCEL_ZOUT_H);
}
int16_t get_gyrox()
{
  return getData_mpu6050(GYRO_XOUT_H );
}
int16_t get_gyroy()
{
  return getData_mpu6050(GYRO_YOUT_H );
}

int16_t get_gyroz()
{
  return getData_mpu6050(GYRO_ZOUT_H );
}



SoftwareSerial Serial_GPS(7,8);
TinyGPS gps;

SoftwareSerial Serial_Sim(6,5);
char msg;  
char call;
float flat=0, flon=0;
unsigned long age;


void SendMessage()
{ 
  Serial_Sim.listen();
  
  if(flat != 1000 && flon != 1000)
  {//MakeCall();
  delay(2000); 
   Serial_Sim.println("AT+CMGF=1");    //Sets the GSM Module in Text Mode
  delay(1000);  // Delay of 1000 milli seconds or 1 second
  Serial_Sim.println("AT+CMGS=\"+84964871225\"\r"); // Replace x with mobile number
  delay(1000);
  Serial.println("http://google.com.vn/maps/place/"+String(flat,6)+","+String(flon,6));// The SMS text you want to send
  Serial_Sim.println("http://google.com.vn/maps/place/"+String(flat,6)+","+String(flon,6));// The SMS text you want to send
  delay(100);
  Serial_Sim.println((char)26);// ASCII code of CTRL+Z
  delay(1000);  
  }

  Serial_Sim.end();
}

void ReceiveMessage()
{
  Serial_Sim.println("AT+CNMI=2,2,0,0,0"); // AT Command to recieve a live SMS
  delay(1000);
  if (Serial_Sim.available()>0)
  {
    msg=Serial_Sim.read();
    Serial.print(msg);
  }
}

void MakeCall()
{
  Serial_Sim.listen();
  delay(2000);
  Serial_Sim.println("ATD+84964871225;"); // ATDxxxxxxxxxx; -- watch out here for semicolon at the end!!
  Serial.println("Calling  "); // print response over serial port
  delay(10000);
  Serial_Sim.end();
}


void HangupCall()
{
  Serial_Sim.println("ATH");
  Serial.println("Hangup Call");
  delay(1000);
}
static void print_float(float val, float invalid, int len, int prec)
{
  if (val == invalid)
  {
    while (len-- > 1)
      Serial.print('*');
    Serial.print(' ');
  }
  else
  {
    Serial.print(val, prec);
    int vi = abs((int)val);
    int flen = prec + (val < 0.0 ? 2 : 1); // . and -
    flen += vi >= 1000 ? 4 : vi >= 100 ? 3 : vi >= 10 ? 2 : 1;
    for (int i=flen; i<len; ++i)
      Serial.print(' ');
  }
  smartdelay(0);
}
static void smartdelay(unsigned long ms)
{
  unsigned long start = millis();
  do 
  {
    while (Serial_GPS.available())
      gps.encode(Serial_GPS.read());
  } while (millis() - start < ms);
}

void Make_GPS()
{
  gps.f_get_position(&flat, &flon, &age);  
}
void setup()
{ Serial.begin(9600);    // Setting the baud rate of Serial Monitor (Arduino)
    // Setting the baud rate of GSM Module  
  Serial_GPS.begin(9600);
  Serial_Sim.begin(115200);
  //mpu6050();
}

void Get_GPS()
{
  Serial_GPS.listen();
  do{
  gps.f_get_position(&flat, &flon, &age); 
 // Serial.print(flat);
 // Serial.println(flon);
  print_float(flat, TinyGPS::GPS_INVALID_F_ANGLE, 10, 6);
  print_float(flon, TinyGPS::GPS_INVALID_F_ANGLE, 11, 6);
  Serial.println();
  }while(flat==1000); 
 smartdelay(5000);
 Serial_GPS.end();
}
float tmp;
float tmp_old = -5;

int  Check()
{
  tmp = map(get_accx(), -16384,16384,-90,90);
 
  if(tmp> tmp_old+20 || tmp < tmp_old -20)
  {  Serial.println(tmp);
    tmp_old = tmp;
    return 1;
  }
 return 0;  
}
void loop()
{ 
// if(Check() == 1)
{
  //Get_GPS();  

 //Serial.println("http://google.com.vn/maps/place/"+String(flat,6)+","+String(flon,6));//
 delay(1000);
  MakeCall();
 //SendMessage();
}

}





