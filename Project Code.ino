#include <Keypad.h>
#include <LiquidCrystal_I2C.h>
#include <SoftwareSerial.h>//gsm,gps
///////////////////////////////////////////////
#include <TinyGPS++.h>
TinyGPSPlus gps;
double latitude, longitude;
SoftwareSerial SIM800L(10, 11);

String response;
int lastStringLength = response.length();

String link;
//////////////////////////////////////////////////////
int trigPin = 36;
int echoPin = 38;
////////////////////////////

LiquidCrystal_I2C lcd(0x27,16,2);
int wn=0; //how many time password wrong
const byte r = 4; // Four rows
const byte c = 4; //  columns
// Define the Keymap
char h[r][c] = {
  {'1','2','3','A'},
  {'4','5','6','B'},
  {'7','8','9','C'},
  {'*','0','#','D'}
};

byte rp[r] = {9,8,7,6};// Connect keypad ROW0, ROW1, ROW2 and ROW3 to these Arduino pins.
byte cp[c] = { 5,4,3,2};// Connect keypad COL0, COL1 and COL2 to these Arduino pins.

int CV=0;// password correct hle lcd text dhore rakhar jonno.
int access=0; //authorize sothik hle access 0 hoye thakbe jeno spl key active holeo sms na jay.
int count=0;// vhole jaua chabir sms er jonno, driver na thakle time count kore sms pathabe.
long duration, distance;
int prmntoff=0;

//int D1=0,D2=0,D3=0,D4=0,D5=0;
int out1 =32,out2=34, x=0;
int in1=33,in2=35;
int ir=40;
int ldr=A1;//prvs42
int spl=44;
int splin=46;


int tm=0; //sms 1 time send,depending 3time wn pass

int val2=0;
int val=0; //after wrong of password sms will send for 1 time depending of value of val.
int valuetronic=1;// when valuetronic is 0 ultrasonic will work again and again. when distance <=10 cm valuetronic will 1 & ultrasonic will not work again.
int ULaccess=0;// after motor off by ultrasinc motor will not start again by vibrator sensor.
int wait=0;

char P[6] = {'4','5','6','7','8','9'};
char I[6];
Keypad p =Keypad(makeKeymap(h),rp,cp,r,c);
///////////////////////////////////////////////////////



void setup() {
  Serial.begin(9600);
  lcd.begin();
  Display();
  pinMode(12,OUTPUT);//12 for motor
  pinMode(out1,OUTPUT);//32 mega
  pinMode(out2,OUTPUT);//34 mega
  pinMode(48,OUTPUT);//extra pin mega
  pinMode(50,OUTPUT);//extra pin mega
  pinMode(ir,INPUT);
  pinMode(ldr,INPUT);//A1
  pinMode(in1,INPUT);//33 mega
  pinMode(in2,INPUT);//35 mega
  pinMode(spl,OUTPUT);//44
  pinMode(splin,INPUT);//46
  pinMode(A0,INPUT);
 
  

  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);
  ///////////////////////////////////////////////////
    Serial.println("GPS");
    SIM800L.begin(9600);  
    SIM800L.println("AT+CMGF=1");
    Serial.println("SIM800L started at 9600");
    delay(1000);
    Serial.println("Setup Complete! SIM800L is Ready!");
    SIM800L.println("AT+CNMI=2,2,0,0,0");
    ///////////////////////////////////////////////////////
}

void loop() {
  latitude = 23.787907 ;
    longitude = 90.331578;
    link = "www.google.com/maps/place/" + String(latitude, 6) + "," + String(longitude, 6) ;
    Serial.println(link);
       motor();
       vibration();
       
////////////////////////////GSM/////////GSM/////////////////////
  if (SIM800L.available()>0){
      response = SIM800L.readStringUntil('\n');
    }
     

  if(lastStringLength != response.length()){
      GPS();
      //Perintah ON
      if(response.indexOf("ON") == 4)//Only ON word is use for General But GPS ON for Permanent
      {
          Serial.println("sms");
           digitalWrite(out1,HIGH);
           digitalWrite(out2,HIGH);
           digitalWrite(spl,LOW);
           access=0;
           tm=0;
           count=0;
           wn=0;
           val2=0;
           prmntoff=0;
         
      }

    else if(response.indexOf("OFF") == 4)//Only OFF wor is use for General But GPS OFF for Permanent 
     {
           Serial.println("sms");
           digitalWrite(out1,LOW);
           digitalWrite(out2,LOW);
           digitalWrite(spl,LOW);
           tm=0;
           access=1;

      }

      
else if(response.indexOf("LL") == 4) //LL(Location)
         {  
          Serial.println("sms");
          SIM800L.println("AT+CMGF=1");    //Sets the GSM Module in Text Mode
          delay(1000);  // Delay of 1000 milli seconds or 1 second
          SIM800L.println("AT+CMGS=\"+8801871468217\"\r"); // Replace x with mobile number
          delay(1000);
          SIM800L.println("link is:");// The SMS text you want to send
          delay(100);
          SIM800L.println(link);// The SMS text you want to send
          delay(100);
          SIM800L.println((char)26);// ASCII code of CTRL+Z
          delay(1000);
          
          }

   else if(response.indexOf("PP") == 4)// PP(Priority Password)
         {
            Serial.println("Keypad Priority");
           tm=0;
           count=0;
           wn=0;
           prmntoff=0;
          }       
              
     }
////////////////////////////GSM///////////////////GSM//////////////////////

/////////////////////////////////////////////////////////////////////////////ppppppppppppppppppppppp//////////////////////////////////////////

      Serial.println(wn);
      Serial.print("splin ");
      Serial.println(digitalRead(splin));
      Serial.print(distance);
      Serial.println("CM");

      


   
 if(digitalRead(splin)==1 && access==1) //if special key on & access=1 ,then sms will send
        {    
                   for(int i=0;i<=5;i++)
                   {
                      
                      digitalWrite(trigPin,HIGH);
                      delayMicroseconds(1000);
                      digitalWrite(trigPin, LOW);
                      duration=pulseIn(echoPin, HIGH);
                      distance =(duration/2)/29.1;
                      Serial.print(distance);
                      Serial.println("CM");
                      delay(10);
                }     
                      if(response.indexOf("STOP") == 4)//Only OFF wor is use for General But GPS OFF for Permanent 
                     {
                           Serial.println("sms");
                           digitalWrite(out1,LOW);
                           digitalWrite(out2,LOW);
                           digitalWrite(spl,LOW);
                           tm=0;
                           access=1;
                           prmntoff=1;
                          
                         
                          SIM800L.println("AT+CMGF=1");    //Sets the GSM Module in Text Mode
                          delay(1000);  // Delay of 1000 milli seconds or 1 second
                          SIM800L.println("AT+CMGS=\"+8801871468217\"\r"); // Replace the number with mobile number
                          delay(1000);
                          SIM800L.println("Car Permanently off.");
                          delay(100);
                          SIM800L.println(link);// The SMS text you want to send
                          delay(100);
                          SIM800L.println((char)26);// ASCII code of CTRL+Z
                          delay(1000);
                        
                
                      }

     
     
            
               else if( distance<=25 && distance>=0)//Only OFF wor is use for General But GPS OFF for Permanent 
                 {
                   
                  
                       Serial.println("sms");
                           digitalWrite(out1,LOW);
                           digitalWrite(out2,LOW);
                           digitalWrite(spl,LOW);
                           tm=0;
                           access=1;
                           prmntoff=1;
                          
                         
                          SIM800L.println("AT+CMGF=1");    //Sets the GSM Module in Text Mode
                          delay(1000);  // Delay of 1000 milli seconds or 1 second
                          SIM800L.println("AT+CMGS=\"+8801871468217\"\r"); // Replace the number with mobile number
                          delay(1000);
                          SIM800L.println("Car Permanently off by Ultrasonic.");
                          delay(100);
                          SIM800L.println(link);// The SMS text you want to send
                          delay(100);
                          SIM800L.println((char)26);// ASCII code of CTRL+Z
                          delay(1000);
            
                  }

                  
                  
              else
              {     
                if(prmntoff==0)
                {
                  digitalWrite(spl,HIGH);
                }
                  if(val2==0)
                 {
                  SIM800L.println("AT+CMGF=1");    //Sets the GSM Module in Text Mode
                  delay(1000);  // Delay of 1000 milli seconds or 1 second
                  SIM800L.println("AT+CMGS=\"+8801871468217\"\r"); // Replace the number with mobile number
                  delay(1000);
                  SIM800L.println("Someone is stealing your car without password.");
                  delay(100);
                  SIM800L.println(link);// The SMS text you want to send
                  delay(100);
                  SIM800L.println((char)26);// ASCII code of CTRL+Z
                  delay(1000);
                 }
                 val2++; 
               }
    }


//    if(digitalRead(splin)==1 && digitalRead(ir)==1 && analogRead(ldr)>=1000 && prmntoff==0 )
//   {
//     lcd.setCursor(0,0);
//     lcd.print(" Access Denied ");
//     lcd.setCursor(0,1);
//     lcd.print("                ");
//   }

//if(digitalRead(splin)==0 && access==1) //if special key on & access=1 ,then sms will send
//    {    
//          digitalWrite(out1,LOW);
//          digitalWrite(out2,LOW);
//    }
////////////////////////////////////////////wn (wrong count start)////////////////////////////////////////////////////////////////////

  if(wn>=3 && prmntoff==0)
  {
     
     Serial.println("more time pass wrong");
     lcd.setCursor(0,0);
     lcd.print(" Access Denied ");
     lcd.setCursor(0,1);
     lcd.print("                ");
     digitalWrite(spl,HIGH);
     
    if(tm==0)
    {
          
          SIM800L.println("AT+CMGF=1");    //Sets the GSM Module in Text Mode
          delay(1000);  // Delay of 1000 milli seconds or 1 second
          SIM800L.println("AT+CMGS=\"+8801871468217\"\r"); // Replace the number with mobile number
          delay(1000);
          SIM800L.println("Someone is trying to start your car");
          delay(100);
          SIM800L.println(link);// The SMS text you want to send
          delay(100);
          SIM800L.println((char)26);// ASCII code of CTRL+Z
          delay(1000);
          tm++;
    }

        
  }


/////////////////////////////////////////////////wn (wrong count)///////////////////////////enddddddddddddddddddddddddddddddddd

  if(digitalRead(ir)==1 && analogRead(ldr)>=1000 && wn<=2)
{
  CV=0;
  x=0;
  access=1;
  digitalWrite(out1,LOW);
  digitalWrite(out2,LOW);
  if(prmntoff==0)
  {
  digitalWrite(spl,HIGH);
  }
  lcd.setCursor(0,0);
  lcd.print("   Lock Mode    ");
  lcd.setCursor(0,1);
  lcd.print("                ");
}
 else if(digitalRead(ir)==0 && analogRead(ldr)>=1000 && wn<=2)
  {
  CV=0;
  x=0;
  access=1;
  count=0;
  digitalWrite(out1,LOW);
  digitalWrite(out2,LOW);
  if(prmntoff==0)
  {
  digitalWrite(spl,HIGH);
  }
  lcd.setCursor(0,0);
  lcd.print("Enter Ignition");
  lcd.setCursor(0,1);
  lcd.print("Key             ");
  }
  else if(digitalRead(ir)==1 && analogRead(ldr)<=999 && wn<=2)
  {
      Serial.print("count ");
      Serial.println(count);
      if(CV==1)
      {
        lcd.setCursor(14,0);
        lcd.print(count/25);//40sec
        
      }
        if(CV==0 && count<=999)
      {
  lcd.setCursor(0,0);
  lcd.print("   No Driver    ");
  lcd.setCursor(0,1);
  lcd.print("                ");
        
      }    
      
if(count>=1004)
{
  lcd.setCursor(0,0);
  lcd.print("   Lock Mode    ");
  lcd.setCursor(0,1);
  lcd.print("                ");
}
      
    if(count>=1000 && count<=1003)
    {

  CV=0;
  x=0;
  access=1;
  digitalWrite(out1,LOW);
  digitalWrite(out2,LOW);
  if(prmntoff==0)
  {
  digitalWrite(spl,HIGH);
  }
  lcd.setCursor(0,0);
  lcd.print("    No Driver    ");
  lcd.setCursor(0,1);
  lcd.print("                  ");
          SIM800L.println("AT+CMGF=1");    //Sets the GSM Module in Text Mode
          delay(1000);  // Delay of 1000 milli seconds or 1 second
          SIM800L.println("AT+CMGS=\"+8801871468217\"\r"); // Replace the number with mobile number
          delay(1000);
          SIM800L.println("Sir,The key exists in the car");
          delay(100);
          SIM800L.println((char)26);// ASCII code of CTRL+Z
          delay(1000);
    }
    count++;
  }

 else if(digitalRead(ir)==0 && digitalRead(ldr)<=999 && wn<=2)
  {
    count=0;
          if(CV==1)
      {
        lcd.setCursor(14,0);
        lcd.print( count);
        
      }
  if(CV==0 && wn==0)
  {
 
  lcd.setCursor(0,0);
  lcd.print("Enter Pass Key");
  lcd.setCursor(0,1);
  lcd.print("Code Numb:");
  }
  if(CV==0 && wn==1)
  {
 
  lcd.setCursor(0,0);
  lcd.print("Your 2nd Chance");
  lcd.setCursor(0,1);
  lcd.print("Code Numb:");
  }
  if(CV==0 && wn==2)
  {
 
  lcd.setCursor(0,0);
  lcd.print("The Last Attempt");
  lcd.setCursor(0,1);
  lcd.print("Code Numb:");
  }
  
  
///////////////////////////////////////////////////////////////////
char k = p.getKey();
if(k){
  I[x] =k;
  x++;
  lcd.setCursor(x+9,1);
  lcd.print("*");
if(k=='A')
  {
  for(int i=10;i<=15;i++)
  {  
lcd.setCursor((i),1);
lcd.print(" ");
  }
x=0;
  }
  
  if (x==6){
    Check();
  }
  if(x>=7){
    x=0;
    CV=0;
    access=1;
  digitalWrite(out1,LOW);
  digitalWrite(out2,LOW);
  if(prmntoff==0);
  {
  digitalWrite(spl,HIGH);
  }
    Display();
  }
 }
 
  }
 ////////////////////////////////////////////////////////////////////////////////////ppppppppppppppppppppppppppppp///////////////////////////////////

  delay(10);
}



//////////////////////////////ppppppppppppffffffffffffffffffffpppppppppppppppppppppppfffffffffffffffffffffffpppppppppppppppppffffffffffffff
int Check(){
  delay(1000);
  if(I[0] == P[0] && I[1] == P[1] && I[2] == P[2] && I[3] == P[3] && I[4] == P[4] && I[5] == P[5])
  {
    Correct();
    wn=0;
    val2=0;
  }
  else
  {
    Incorrect();
    wn++;
  }
}

void User()
{
}

void Display()
{
  access=1;
  digitalWrite(out1,LOW);
  digitalWrite(out2,LOW);
  if(prmntoff==0)
  {
  digitalWrite(spl,HIGH);
  }
  lcd.setCursor(0,0);
  lcd.print("   Lock Mode     ");
  lcd.setCursor(0,1);
  lcd.print("                ");
  
}



void Correct()
{
  CV++;   //for stying this massage on lcd
  access=0;
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print("Password Correct");
  digitalWrite(out1,HIGH);
  digitalWrite(out2,HIGH);
  digitalWrite(spl,LOW);
  delay(2000);
  val=1;
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print("Key System ON");
  lcd.setCursor(0,1);
  lcd.print("Press any key...");
  
}

void Incorrect()
{
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print("Password Wrong");
  delay(2000);
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print("Password Wrong");
  lcd.setCursor(0,1);
  lcd.print("Try Again");
}
///////////////////////////////////ppppppppppppppppppppppppfffffffffffffffffffffffffffffffffffffpppppppppppppppppppppppppppppppppffff

///////////////////////GPS/////////////GPS////////////////////
void GPS(){
  if(Serial.available()) {
    gps.encode(Serial.read());
  }
  if(gps.location.isUpdated()) {
    latitude = 23.787907 ;
    longitude = 90.331578;
    link = "www.google.com/maps/place/" + String(latitude, 6) + "," + String(longitude, 6) ;
    Serial.println(link);
  
  }
}
//////////////////////gps//////////gps//////////////////


void motor()
{
  if(digitalRead(in1)==1 )
  {
    Serial.println("in1 is 1");
  }
  if(digitalRead(in2)==1 )
  {
    Serial.println("in2 is 1");
  }

  if(digitalRead(in1)==1 && digitalRead(in2)==1)
  {
    
    Serial.println("motor is on");
    digitalWrite(12,HIGH);
    digitalWrite(48,LOW);
    digitalWrite(50,LOW);
    
    
  }
  else
  { Serial.println("motor is off");
    digitalWrite(12,LOW);
    digitalWrite(48,HIGH);
    digitalWrite(50,HIGH);
  }
}

void vibration()
{
  if(digitalRead(A0)==0)
           {  

          SIM800L.println("AT+CMGF=1");    //Sets the GSM Module in Text Mode
          delay(1000);  // Delay of 1000 milli seconds or 1 second
          SIM800L.println("AT+CMGS=\"+8801871468217\"\r"); // Replace x with mobile number
          delay(1000);
          SIM800L.println("Someone is moving your car without key ");
          delay(100);
          SIM800L.println(link);// The SMS text you want to send
          delay(100);
          SIM800L.println((char)26);
          delay(1000);
   }
}
