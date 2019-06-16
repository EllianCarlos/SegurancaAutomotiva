#include <WiFi.h>
#include <HTTPClient.h>
#include <Thread.h>
#include <HardwareSerial.h>
#include "time.h"

#define bytesOfCom 8

HardwareSerial MySerial(1);
 
const char* ssid = "tccteste";
const char* password =  "leitecondensado";
const char* url = "https://gsm-arduino.000webhostapp.com/add.php";
const String tags[8] = {"tempoAtivo","tempoUmaMaoVolante","tempoDuasMaoVolante","emerg","alerta","airbag","Velocidade","tempMotor"};
int sended = 0; // 0 para enviado 1 para pronto para envio -3 para sem internet
uint16_t count = 0;
bool reading=false;
String Post = "";
String buff[10];
unsigned long timers[10] = {0,0,0,0,0,0,0,0,0,0};
String dateTimes[10];
uint16_t countBuffer = 0;
const char* ntpServer = "pool.ntp.org";
const long  gmtOffset_sec = 3600;
const int   daylightOffset_sec = 3600;
bool conn =  false;

Thread threadSend = Thread();

void setup() {
  Serial.begin(115200);
  MySerial.begin(115200, SERIAL_8N1,16,17);
  delay(4000);   //Delay needed before calling the WiFi.begin
 
  WiFi.begin(ssid, password); 
  if(checkConn()) setConn();
  threadSend.onRun(post);
  threadSend.setInterval(12000);
}

void loop() {
  if(checkConn()) setConn();
  if(checkConn()
  if(threadSend.shouldRun() && !checkConn()){
    buffActualPost();
  }
  if(threadSend.shouldRun()) threadSend.run();
  getStr();
}

void post(){
 if(WiFi.status()== WL_CONNECTED){   //Check WiFi connection status
 
   HTTPClient http;   
 
   http.begin(url);  //Specify destination for HTTP request
   http.addHeader("Content-Type", "application/x-www-form-urlencoded");             //Specify content-type header
   Serial.print(Post);
   int httpResponseCode = http.POST(Post);   //Send the actual POST request
   if(httpResponseCode>0){
    String response = http.getString();                       //Get the response to the request
    Serial.println(httpResponseCode);   //Print return code
    Serial.println(response);           //Print request answer
    
   }else{
    Serial.print("Error on sending POST: ");
    Serial.println(httpResponseCode);
    buffActualPost();
   }
   http.end();  //Free resources
 }else{
    Serial.println("Error in WiFi connection");   
    buffActualPost();
 }
}

void postFromString(String s,String timeStr){
 if(WiFi.status()== WL_CONNECTED){   //Check WiFi connection status
 
   HTTPClient http;   
 
   http.begin(url);  //Specify destination for HTTP request
   http.addHeader("Content-Type", "application/x-www-form-urlencoded");             //Specify content-type header
   Serial.print(s);
   int httpResponseCode = http.POST(s+"&timeStampSend="+timeStr);   //Send the actual POST request
   if(httpResponseCode>0){
    String response = http.getString();                       //Get the response to the request
    Serial.println(httpResponseCode);   //Print return code
    Serial.println(response);           //Print request answer
    countBuffer--;
   }else{
    Serial.print("Error on sending POST: ");
    Serial.println(httpResponseCode);
   }
   http.end();  //Free resources
 }else{
    Serial.println("Error in WiFi connection");   
 }
}

void getStr(){
  if(MySerial.available()>0){
    int num = MySerial.read();
    Serial.println();
    Serial.print("Contador: ");
    Serial.println(count);
    Serial.print("String: ");
    Serial.println(Post);
    Serial.print("num: ");
    Serial.println(num);
    if(reading){
      Post += tags[count] + "=" + num;  
      if(count != 7){
        count++;
        Post += "&";
      } else {
        MySerial.flush();
        count = 0;
        reading = false;
      }
    }
    if(num == 2 && !reading){
      count = 0;
      Post = "";
      reading = true;
    }
  }
}

void buffActualPost(){
  if(countBuffer < 10){
    buff[countBuffer] = Post;
    timers[countBuffer] = (millis()/100);
    countBuffer++;
  } else {
    countBuffer = 0;
    buffActualPost();
  }
}

void postBuffs(){
  for(int w = 0; w < 10; w++){
    postFromString(buff[w],dateTimes[w]);
  }
}

String getTimersToString(){
  struct tm timeinfo;
  if(getLocalTime(&timeinfo)){
    for(int j = 0; j < 10; j++){
      if(timers[j] > 3600){
        dateTimes[j] = (&timeinfo, "%Y-%b-%d "+ String(String("%H").toInt()+ dividedIt(timers[j],3600) + ":" + String(String("%M").toInt()+ (dividedIt(timers[j] - 3600*dividedIt(timers[j],3600),60)) + ":" + String(String("%S").toInt()+ (timers[j]%60)))));
      } else if(timers[j] > 60){
        dateTimes[j] = (&timeinfo, "%Y-%b-%d %H:%M:" + String(String("%S").toInt()+ timers[j]));
      } else{
        dateTimes[j] = (&timeinfo, "%Y-%b-%d %H:%M:" + String(String("%S").toInt()+ timers[j]));  
      }
    }  
  } 
}

int dividedIt(int num, int den){
  return ((num - (num%den))/den);
}

void printLocalTime(){
  struct tm timeinfo;
  if(!getLocalTime(&timeinfo)){
    Serial.println("Failed to obtain time");
    return;
  }
  Serial.println(&timeinfo, "%A, %B %d %Y %H:%M:%S");
}

void setConn(){ 
  Serial.println("Connecting to WiFi and NTP server...");
  setNTP();
  checkConn();
  }

bool checkConn(){
  if(WiFi.status() == WL_CONNECTED) { //Check for the connection
    conn = true;
  } else{
    conn = false;
  }
  return conn;
}

void setNTP(){
  configTime(gmtOffset_sec, daylightOffset_sec, ntpServer);
  printLocalTime();
}

//TODO: maybe add a page doesEsp32HaveInternet where the response is just Yes;
