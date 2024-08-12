#include "time.h"
#include "sntp.h"
#include <WiFi.h>

time_t inittime=1723154010;

time_t curtime;
timeval curtime2;
//time(&curtime);
int usertime=0;
bool evalv1=0;
bool evalv2=0;
bool evalv3=0;

void confOutput(){ 
  evalv1=0;
  evalv2=0;
  evalv3=0;
  if(Alpha.enable){
    if((curtime2.tv_sec+usertime)%(86400*Alpha.interval)>Alpha.starttm&&(curtime2.tv_sec+usertime)%(86400*Alpha.interval)<Alpha.starttm + Alpha.duration){
      if(Alpha.valvbool[0]=='1'){evalv1=1;}
      if(Alpha.valvbool[1]=='1'){evalv2=1;}
      if(Alpha.valvbool[2]=='1'){evalv3=1;}
      }
    }
//PREPEI NA ALLAZEI TO OBJECT
  
  digitalWrite(26,evalv1);
  digitalWrite(33,evalv2);
  digitalWrite(25,evalv3);
  if(evalv1){Serial.println("valve 1");}
  if(evalv2){Serial.println("valve 2");}
  if(evalv3){Serial.println("valve 3");}
  if(!evalv1&&!evalv2&&!evalv3){Serial.println("all off");}
}




String calend(int rsec){
  String day="";
  switch ((rsec/86400)%7+1){
    case 1: day="Monday"; break;
    case 2: day="Tuesday"; break;
    case 3: day="Wednesday"; break;
    case 4: day="Thursday"; break;
    case 5: day="Friday"; break;
    case 6: day="Saturday"; break;
    case 7: day="Sunday"; break;
  }
  day= day+"  "+(rsec%86400)/36000+(rsec%86400/3600)%10+":"+rsec%3600/600+(rsec%3600/60)%10+":"+rsec%60/10+rsec%60%10; 
  return day;
}




void day_set(String input){
  int day=0;
  int cday=0;
   if(input[1]=='u'){if(input[0]=='t'){ day=1;} else{day=6;}}
   if(input[1]=='e'){day=2;}
   if(input[1]=='h'){day=3;}
   if(input[1]=='r'){day=4;}
   if(input[1]=='a'){day=5;}
  String Cday=calend(curtime2.tv_sec+usertime);
   if(Cday[1]=='u'){if(Cday[0]=='t'){ cday=1;} else{cday=6;}}
   if(Cday[1]=='e'){cday=2;}
   if(Cday[1]=='h'){cday=3;}
   if(Cday[1]=='r'){cday=4;}
   if(Cday[1]=='a'){cday=5;}
   if(cday>day){day=day+7;}
   usertime= usertime%604800+(day-cday)*86400;
 // 86400
}


class simple_prog{
  public:
  int interval=1;
  int duration=10;
  int starttm=0;
  bool enable=0;
  String label;
  String valvbool="111";
};


/**************************************************************
|                                                             |
**************************************************************/

void setup() {
  inittime=inittime+10800; //GMT +0300
  pinMode(26,OUTPUT);
  pinMode(33,OUTPUT);
  pinMode(25,OUTPUT);
  confOutput();
  Serial.begin(115200);
  delay(1000);
  Serial.println("setup OK");
  
  simple_prog Alpha;
  Alpha.duration= 120;
  Alpha.valvbool="100";
  Alpha.label="Ntomates";
  Alpha.enable=1;
  Alpha.starttm=8*3600;
  

}

/**************************************************************
|                                                             |
**************************************************************/

void loop() {

  gettimeofday(&curtime2,NULL);
  curtime=inittime+curtime2.tv_sec;
//Serial.println(ctime(&curtime));
  Serial.println(calend(curtime2.tv_sec+usertime));
  Serial.println(curtime2.tv_sec);

//Serial.println(gettimeofday(&curtime2,NULL));
 confOutput();

 /*************************
  if(evalv1){
    evalv1=0;
    evalv2=1;
    delay(200);
    digitalWrite(26,evalv1);
    }

  else
  if(evalv2){
    evalv2=0;
    evalv3=1;
    delay(200);
    digitalWrite(33,evalv2);
    }
  else
  if(evalv3){
    evalv3=0;
    //delay(200);
    digitalWrite(25,evalv3);
    }
  else
  if(!evalv1&&!evalv2&&!evalv3){
    delay(500);
    evalv1=1;
  }
delay(500);
*******************************/
}
