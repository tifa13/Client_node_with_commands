/*
project: smart parkng 
author: mostafa ahmed and ahmed sobhy 
PArking: sensor nodes 
acts as a client with commands not lib
*/
#include <Narcoleptic.h>
#include "SoftwareSerial.h"
// ssid must be 10 char
#define SSID        "tifa123456"
#define PASSWORD    "01005381961"
#define HOST_NAME   "192.168.4.1"
#define HOST_PORT   (8090)
SoftwareSerial myserial(7,8);
#define DEBUG true
String ID_node="0,";
// module pins 
uint8_t trigPin = 9;    
uint8_t echoPin = 10;
// flag to know if space empty ot not 
bool flag;
int prev_state=0;
void setup(void)
{
    Serial.begin(9600);
    myserial.begin(9600);
    pinMode(trigPin, OUTPUT);
    pinMode(echoPin, INPUT);
    Serial.print("setup begin\r\n");
    sendData("AT+RST\r\n",2000,DEBUG);
//check if module started correctly 
     sendData("AT\r\n",2000,DEBUG);
     
// set mode to sation ( client only ) 
    sendData("AT+CWMODE=1\r\n",1000,DEBUG);
 
// join access point 
    sendData("AT+CWJAP=\"tifa123456\",\"01005381961\"\r\n",10000,DEBUG);
    
    
    sendData("AT+CIPMUX=1\r\n",1000,DEBUG);
    myserial.flush();
    Serial.print("setup end\r\n");
}

void loop(void)
{
  
  int current_state=sense();
  // sleeping set buffer for sleeping mode
    uint8_t mode[1] = {0};
  // set recving buffer
    uint8_t buffer[128] = {0};
  // create TCP connection ( socket ) 
    if(prev_state!=current_state){
      send_data();
    }
    prev_state=current_state;
    //recv();
 //  Serial.println("I will sleep now zzzzz");
 //  Narcoleptic.delay(8000);
 //  Serial.println("ana s7et ba3d 10 sec");
 Narcoleptic.delay(30000);
// if(wifi.deepSleep(5000)){
//    Serial.println("entering deepsleep");
//  }
// else{
//    Serial.println("faield deepsleep");
//  }
//  Serial.println("ana s7eet yaaay :D");
   
    
}

void create_tcp_connection(){
  
  sendData("AT+CIPSTART=0,\"TCP\",\"192.168.4.1\",8090",20000,DEBUG);
}


//send sensed data to cluster head
void send_data(){
  // create TCP connection ( socket )
  create_tcp_connection();
  //set data sending mode
  sendData("AT+CIPMODE=1\r\n",1000,DEBUG);
  
  String hello = ID_node+String(sense());
  String send_command;
  delay(1000);
  send_command="AT+CIPSEND=0,";
  send_command+=hello.length();
  send_command+="\r\n";
  //sending command then data after it 
  sendData(send_command,2000,DEBUG);
  sendData(hello,1000,DEBUG);
  // close tcp connection (socket)
  sendData("AT+CIPCLOSE=0\r\n",3000,DEBUG);  
}


//Sending commands to node
String sendData(String command, const int timeout, boolean debug)
{
    String response = "";
    
    myserial.print(command); // send the read character to the myserial
    
    long int time = millis();
    
    while( (time+timeout) > millis())
    {/*
      while(myserial.available())
      {
        
        // The esp has data so display its output to the serial window 
        char c = myserial.read(); // read the next character.
        response+=c;
      }  */
    }
  /*  
    if(debug)
    {
      Serial.println(response);
    }
   */ 
   
    return response;
}


int sense(){
  long cm,duration;
  digitalWrite(trigPin, LOW);
  delayMicroseconds(5);
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);
  
  pinMode(echoPin, INPUT);
  duration = pulseIn(echoPin, HIGH);
   
  cm = (duration/2) / 29.1;
  
  //Serial.print(cm);
  //Serial.print("cm");
  //Serial.println();
  delay(50);
  //1 if free space 0 if occupied 
  int space;
  if(cm<=80){
    return(1);
  }else{
    return(0);
  }
  
}

void recv(){
  
    bool parsed= false;
    while(myserial.available())
    {
      // The esp has data so display its output to the serial window 

     char c = myserial.read(); // read the next character.
    
     if(parsed)
     {
     Serial.print(c);
     }
     if(c== 'm,')
       parsed=true;     
    } 
    

  }

