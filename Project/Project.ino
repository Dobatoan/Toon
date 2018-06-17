#include <SoftwareSerial.h>
SoftwareSerial Serial_Sim(4,5);

#define Button_A 7
#define Button_B 8
#define Button_C 9
#define Button_D 10

char msg;  
char call;
int status_rf;
int count =0;

void SendMessage()
{ 
  delay(2000); 
   Serial_Sim.println("AT+CMGF=1");    //Sets the GSM Module in Text Mode
  delay(1000);  // Delay of 1000 milli seconds or 1 second
  Serial_Sim.println("AT+CMGS=\"+84964871225\"\r"); // Replace x with mobile number
  delay(1000);
  Serial_Sim.println("Warning! Nguoi than dang gap nguy hiem!");// The SMS text you want to send
  delay(100);
  Serial_Sim.println((char)26);// ASCII code of CTRL+Z
  delay(1000);  
}

void SendMessage_Reset()
{ 
  Serial.println("Report! Nguoi than da het nguy hiem!");
  delay(2000); 
  Serial_Sim.println("AT+CMGF=1");    //Sets the GSM Module in Text Mode
  delay(1000);  // Delay of 1000 milli seconds or 1 second
  Serial_Sim.println("AT+CMGS=\"+84964871225\"\r"); // Replace x with mobile number
  delay(1000);
  Serial_Sim.println("Report! Nguoi than da het nguy hiem!");// The SMS text you want to send
  delay(100);
  Serial_Sim.println((char)26);// ASCII code of CTRL+Z
  delay(1000);  
}

void MakeCall()
{
  delay(3000);
  Serial_Sim.println("ATD+84964871225;"); // ATDxxxxxxxxxx; -- watch out here for semicolon at the end!!
  Serial.println("Calling  "); // print response over serial port
  delay(10000);
  HangupCall();
}


void HangupCall()
{
  Serial_Sim.println("ATH");
  Serial.println("Hangup Call");
  delay(1000);
}

byte rf4ChannelReceiverSimple(byte port1,byte port2, byte port3, byte port4)
{
  int val =0;
  if (digitalRead(port1)==1)
  {
    delay(100);
    if(digitalRead(port1)==1)
      val =1; 
  }
    if (digitalRead(port2)==1)
  {
    delay(100);
    if(digitalRead(port2)==1)
      val =2; 
  }
    if (digitalRead(port3)==1)
  {
    delay(100);
    if(digitalRead(port3)==1)
      val =3; 
  }
     if (digitalRead(port4)==1)
  {
    delay(100);
    if(digitalRead(port4)==1)
      val =4; 
  };
  return val;
}

void setup() {
 pinMode(A4,OUTPUT);
 pinMode (Button_A,INPUT);
 pinMode (Button_B,INPUT);
 pinMode (Button_C,INPUT);
 pinMode (Button_D,INPUT);
 Serial_Sim.begin(115200);
 Serial.begin(9600);   
}

void loop() {
status_rf = rf4ChannelReceiverSimple(Button_A,Button_B,Button_C,Button_D);

  if(status_rf==1)
  { 
    Serial.println(status_rf);
    Serial.println("Goi dien thoai cho nguoi than");
     MakeCall();
   //  SendMessage();
    // Serial.println("gui tin nhan cho nguoi than");
      do{
      status_rf = rf4ChannelReceiverSimple(Button_A,Button_B,Button_C,Button_D);
      Serial.print(status_rf);
      digitalWrite(A4,1);
      delay(500);
      digitalWrite(A4,0);
      delay(500);    
      }while(status_rf==0);
  
  }
  if(status_rf == 2)
  { Serial.println(status_rf);
    delay(1000);
    status_rf = 0;
    count =0;
    Serial.println(status_rf);
    do{
        status_rf = rf4ChannelReceiverSimple(Button_A,Button_B,Button_C,Button_D);
        Serial.println(status_rf);
        digitalWrite(A4,1);
        delay(500);
        digitalWrite(A4,0);
        delay(500);
        count++;
        Serial.println(count);    
    }while(status_rf==0 && count <= 5 );
    Serial.println("Tat loa");
  }
   if(status_rf == 3)
  {
     Serial.println(status_rf);
     Serial.println("Tat loa");
     digitalWrite(A4,0);
     delay(500);
  }
   if(status_rf == 4)
  {
     Serial.println(status_rf);
     Serial.println("Tat loa va thong bao khong gap nguy hiem");
     digitalWrite(A4,0); // Tat loa
     MakeCall();
     SendMessage_Reset(); // Gui tin nhan thong bao khong gap nguy hiem
  }
}
