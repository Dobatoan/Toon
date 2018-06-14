#include <SoftwareSerial.h>
#include <TinyGPS.h>


SoftwareSerial Serial_GPS(2,3);
TinyGPS gps;

SoftwareSerial Serial_Sim(4,5);
char msg;  
char call;
float flat=0, flon=0;
unsigned long age;


void SendMessage()
{
  
  Serial_Sim.println("AT+CMGF=1");    //Sets the GSM Module in Text Mode
  delay(1000);  // Delay of 1000 milli seconds or 1 second
  Serial_Sim.println("AT+CMGS=\"+84964871225\"\r"); // Replace x with mobile number
  delay(1000);
  Serial_Sim.println("Flat = "+String(flat,6)+"Flon = "+String(flon,6));// The SMS text you want to send
  delay(100);
   Serial_Sim.println((char)26);// ASCII code of CTRL+Z
  delay(2000);

  
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
  Serial_Sim.println("ATD+84964871225;"); // ATDxxxxxxxxxx; -- watch out here for semicolon at the end!!
  Serial.println("Calling  "); // print response over serial port
  delay(1000);
}


void HangupCall()
{
  Serial_Sim.println("ATH");
  Serial.println("Hangup Call");
  delay(1000);
}

void Make_GPS()
{
  gps.f_get_position(&flat, &flon, &age);  
}
void setup()
{
  Serial_Sim.begin(115200);   // Setting the baud rate of GSM Module  
  Serial_GPS.begin(9600);
  Serial.begin(9600);    // Setting the baud rate of Serial Monitor (Arduino)
}

void loop()
{ 
  gps.f_get_position(&flat, &flon, &age);  
  Serial.print(flat);
  Serial.print("\t");
  Serial.println(flon);
  MakeCall();
  SendMessage();
  delay(4000);
 if (Serial_Sim.available()>0)
 Serial.write(Serial_Sim.read());
}





