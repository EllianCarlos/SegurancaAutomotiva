#include <CapacitiveSensor.h>
#include <GPRS_Shield_Arduino.h>
#include <EEPROMex.h>
#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include <mcp_can.h>
#include <SPI.h>

#define PIN_TX    10
#define PIN_RX    11
#define BAUDRATE  19200
#define mensagem  "Uma emergencia ocorreu!"
#define TEMPOmin1mao 15
#define TEMPOmed1mao 10
#define TEMPOmax1mao 6
#define TEMPOmin0mao 5
#define TEMPOmed0mao 3
#define TEMPOmax0mao 2
#define CAN0_INT 2   
#define limite 1500
#define pin_alerta 35
#define pin_botao  34

CapacitiveSensor capSensor1 = CapacitiveSensor(27,26);
CapacitiveSensor capSensor2 = CapacitiveSensor(22,23);
LiquidCrystal_I2C lcd(0x3F, 16, 2);
GPRS gprs(PIN_TX,PIN_RX,BAUDRATE);
MCP_CAN CAN0(53);

bool update_StringEEPROM(int Addr, String input){
  char charbuff [15];
  input.toCharArray(charbuff,15);
  return EEPROM.writeBlock<char>(Addr,charbuff,15);
}
String read_StringEEPROM(int Addr){
  String outputEEPROM;
  char output[] = " ";
  EEPROM.readBlock<char>(Addr, output, 15); 
  outputEEPROM = String (output);
  return outputEEPROM;
}
int media_velocidade[] = {};
int media_temperatura[] = {};
int volante_porcentagem1[] = {};
int volante_porcentagem2[] = {};
int tempo_1mao = 0;
int tempo_2mao = 0;
long unsigned ultimoTempo1mao = 0;
long unsigned ultimoTempo0mao = 0;
long unsigned tempo_motor_ligado = 0;
long unsigned tempo_can = 0;
long unsigned contador_can = 0;
long unsigned contador_geral = 0;
long unsigned tempo_volante = 0;
long unsigned contador_geral2 = 0;
long unsigned contador_volante0 = 0;
long unsigned contador_volante1 = 0;
long unsigned contador_volante2 = 0;
long unsigned contador_volante3 = 0;
int contador1 = 0;
int contador2 = 0;
int contador3 = 0;
int contador4 = 0;
int porcentagem_0mao = 0;
int porcentagem_1mao = 0;
int porcentagem_2mao = 0;
bool alarme_volante = false;
char gprsBuffer[120];
char numero_de_emergencia[] = "+5524999757713";
String novo_numero;
bool estado_botao = 0;
bool estado_botao_anterior = 0;
bool botaoAtivado = 0;
long unsigned tempoBotao = 0;
bool faseEmergencia = false;
long unsigned int rxId;
unsigned char len = 0;
unsigned char rxBuf[8];
char msgString[128];
bool alarme_cinto = false; 
bool airbag = 0;
bool airbag_anterior = 0;
bool estado_airbag = 0;
bool cinto = 0;
bool motor = 0;
bool motor_anterior = 0;
long unsignedcontador_can;
long unsigned soma_temperatura_motor = 0;
int temperatura_motor = 0;
long unsigned soma_velocidade = 0;
int velocidade = 0;
long unsigned ultimoTempocan = 0;
long unsigned inicializacao_tempo = 0;
int iniciado = 0;
int contador = 0;
bool mao0_volante = false;
bool mao1_volante = false;
bool mao2_volante = false;
char dados_can_final [7];//[0] = velocidade; [1] = airbag; [2] = temp motor; [3] = cinto; [4] = motor;[5,6,7] = porcentagem do volante;

void setup() {
  lcd.begin();
  lcd.noDisplay();
  lcd.noBacklight();
  
  pinMode(pin_alerta, OUTPUT);
  pinMode(pin_botao, INPUT);
  
  if(!gprs.init()) {
      }
  if(!gprs.isNetworkRegistered()) {
      } 
      
  coletar_numero();

  Serial3.begin(115200,SERIAL_8N1);

  if(CAN0.begin(MCP_ANY, CAN_125KBPS, MCP_8MHZ) == CAN_OK)
 
  CAN0.setMode(MCP_NORMAL);
  pinMode(CAN0_INT, INPUT);

}

void loop() {
    can();
    botao();
 if(!motor){
  sendToEsp();
  gsm();
  }
 
  if(airbag && !airbag_anterior){
    estado_airbag = true;
  digitalWrite(pin_alerta, LOW);
    lcd.setCursor(0,0);
    lcd.print("AIRBAG DISPARADO");
    lcd.setCursor(0,1);
    lcd.print("   EMERGENCIA   ");
    ligacao_emergencia();
    sms_emergencia();
    sendToEsp();
    lcd.setCursor(0,0);
    lcd.print("AIRBAG DISPARADO");
    lcd.setCursor(0,1);
    lcd.print("  SMS ENVIADO!  ");
    }
    
  airbag_anterior = airbag;
  
  if(!motor && motor_anterior){
      estado_airbag = false;
      digitalWrite(pin_alerta,LOW);
      tratar_dados_can();
      sendToEsp();
      if(((millis()/1000)-tempo_motor_ligado) >= 300){
      finalizacao();
      delay(3000);
      }
      lcd.noDisplay();
      lcd.noBacklight();
    }
    
  if(motor && !motor_anterior){
     estado_airbag = false;
     iniciado = 0; 
     tempo_motor_ligado = (millis()/1000);
     tempo_can = millis()/1000;
     tempo_volante = millis()/1000;
     inicializacao();
    }
    
  motor_anterior = motor;
  
  if(iniciado >= 1 && motor){
    if(iniciado == 1){
    delay(3000);
  lcd.setCursor(0,0);
  lcd.print("                ");
  lcd.setCursor(0,1);
  lcd.print("                ");
    }
   if(!estado_airbag && !faseEmergencia){
    volante();
    tela();
    iniciado=2;
     }
    }
   if(botaoAtivado > 0){
    while(botaoAtivado){
       botao();
     }
   }
 }

 void finalizacao(){
    lcd.setCursor(3,0);
    lcd.print(" km/h");
    lcd.setCursor(0,0);
    lcd.print("   ");
    if(media_velocidade[contador_geral-1] > 99){
    lcd.setCursor(0,0);
    lcd.print(media_velocidade[contador_geral-1]);
    }
    if(media_velocidade[contador_geral-1] <= 99 && media_velocidade[contador_geral-1] > 9){
    lcd.setCursor(1,0);
    lcd.print(media_velocidade[contador_geral-1]);
    }
    if(media_velocidade[contador_geral-1] <= 9){
    lcd.setCursor(2,0);
    lcd.print(media_velocidade[contador_geral-1]);
    }
    
    lcd.setCursor(3,1);
    lcd.print(" C   ");
    lcd.setCursor(0,1);
    lcd.print("   ");
     if( media_temperatura[contador_geral-1] > 99){
    lcd.setCursor(0,1);
    lcd.print( media_temperatura[contador_geral-1]);
    }
    if(media_temperatura[contador_geral-1] <= 99 &&  media_temperatura[contador_geral-1] > 9 ){
    lcd.setCursor(1,1);
    lcd.print( media_temperatura[contador_geral-1]);
    }
    if(media_temperatura[contador_geral-1] <= 9){
    lcd.setCursor(2,1);
    lcd.print( media_temperatura[contador_geral-1]);
    }

    lcd.setCursor(8,0);
    lcd.print("        ");
    lcd.setCursor(8,0);
    lcd.print(" 2M:");
    lcd.setCursor(12,0);
    lcd.write(volante_porcentagem2[contador_geral2-1]);
    lcd.setCursor(15,0);
    lcd.print("s");

    lcd.setCursor(8,1);
    lcd.print("        ");
    lcd.setCursor(8,1);
    lcd.print(" 1M:");
    lcd.setCursor(12,1);
    lcd.write(volante_porcentagem1[contador_geral2-1]);
    lcd.setCursor(15,1);
    lcd.print("s");
 }
   
 void inicializacao(){
  can();
  if(motor == 1){
  lcd.backlight();
  lcd.display();
  lcd.setCursor(0,0);
  lcd.print("CENTRAL INICIADA");
  lcd.setCursor(0,1);
  lcd.print(" IFRJ_campusVR  ");
  iniciado = 1;
  }
 }

 void botao(){
   estado_botao = digitalRead(pin_botao);
   if(estado_botao && !estado_botao_anterior){
    tempoBotao = millis();
    botaoAtivado = true;
   }
   if(!estado_botao && estado_botao_anterior){
    if((millis() - tempoBotao) >= 4000 && botaoAtivado){
      if(botaoAtivado){
    faseEmergencia = false;
    estado_airbag = false;
    tempoBotao = millis();
    botaoAtivado = false;
      }  
    }
    else if((millis() - tempoBotao) < 4000 && botaoAtivado){
      if(botaoAtivado){
    alarme_volante = false;
    alarme_cinto = false;
    digitalWrite(pin_alerta,LOW);
    tela();
    faseEmergencia = true;
    ligacao_emergencia();
    sms_emergencia();   
    tempoBotao = millis();
    botaoAtivado = false;  
     }
    }      
  }    
    estado_botao_anterior = estado_botao;
   } 

 void ligacao_emergencia(){
  if(faseEmergencia||estado_airbag){
    if(gprs.callUp(numero_de_emergencia)){
      while(gprs.callUp(numero_de_emergencia));
      }
   else{
      tela();
      ligacao_emergencia();
   }
  }
 }

  
void sms_emergencia(){
  if(faseEmergencia||estado_airbag){
    if(gprs.sendSMS(numero_de_emergencia,mensagem)){
    }
 else{
    tela();
    sms_emergencia();
   }
  }
 }

void tela(){
  if(!faseEmergencia && !estado_airbag){
  if(!alarme_volante){
    if(mao1_volante){
      lcd.setCursor(8,0);
      lcd.print(" 1 MAO! ");
      }
    if(mao2_volante){
      lcd.setCursor(8,0);
      lcd.print(" 2 MAOS!");
      }
    if(mao0_volante){
      lcd.setCursor(8,0);
      lcd.print(" 0 MAOS!");
      } 
    }
  if(!alarme_cinto){
    if(cinto){
      lcd.setCursor(8,1);
      lcd.print("CINTO OK");
      }
    }
  
    lcd.setCursor(3,0);
    lcd.print(" km/h");
    lcd.setCursor(0,0);
    lcd.print("   ");
    if(velocidade > 99){
    lcd.setCursor(0,0);
    lcd.print(velocidade);
    }
    if(velocidade <= 99 && velocidade > 9){
    lcd.setCursor(1,0);
    lcd.print(velocidade);
    }
    if(velocidade <= 9){
    lcd.setCursor(2,0);
    lcd.print(velocidade);
    }
    
    lcd.setCursor(3,1);
    lcd.print(" C   ");
     if(temperatura_motor > 99){
    lcd.setCursor(0,1);
    lcd.print(temperatura_motor);
    }
    if(temperatura_motor <= 99 && temperatura_motor > 9 ){
    lcd.setCursor(1,1);
    lcd.print(temperatura_motor);
    }
    if(temperatura_motor <= 9){
    lcd.setCursor(2,1);
    lcd.print(temperatura_motor);
    }
    if(alarme_volante){
    lcd.setCursor(8,0);
    lcd.print("VOLANTE!");
    lcd.setCursor(8,0);
    lcd.print("        ");
    }
    if(alarme_cinto){
    lcd.setCursor(8,1);
    lcd.print("  CINTO!");
    lcd.setCursor(8,1);
    lcd.print("        ");
    }
   }
 }
      
void can () {
   if(!digitalRead(CAN0_INT)){
    CAN0.readMsgBuf(&rxId, &len, rxBuf);
     can_msg();
  }
}

void volante(){
 long sensorValor1 = capSensor1.capacitiveSensor(30);
 long sensorValor2 = capSensor2.capacitiveSensor(30);

    contador_volante0++;

  if ((sensorValor1 > limite && sensorValor2 < limite) || (sensorValor1 < limite && sensorValor2 > limite)){
    if(contador1==0){
      ultimoTempo1mao = millis()/1000;
      contador1 = 1;
      contador2 = 0;
      contador3 = 0;
      mao1_volante = true;
      mao0_volante = false;
      mao2_volante = false;
    }
      
    if(((millis()/1000) - ultimoTempo1mao) >= TEMPOmin1mao && velocidade >= 5 && velocidade < 25){
       digitalWrite(pin_alerta,HIGH);
       alarme_volante = true;
       tela();
  
    }
    if(((millis()/1000) - ultimoTempo1mao) >= TEMPOmed1mao && velocidade >= 25 && velocidade < 50){
       digitalWrite(pin_alerta,HIGH);
       alarme_volante = true;
       tela();
    }
    if(((millis()/1000) - ultimoTempo1mao) >= TEMPOmax1mao && velocidade >= 50){
       digitalWrite(pin_alerta,HIGH);
       alarme_volante = true;
       tela();
    }
  }
  else if ((sensorValor1 < limite) && (sensorValor2 < limite)){   
    if(contador2==0){
      ultimoTempo0mao = millis()/1000;
      contador1 = 0;
      contador2 = 1;      
      contador3 = 0;
      mao1_volante = false;
      mao0_volante = true;
      mao2_volante = false;
    }
    if(((millis()/1000) - ultimoTempo0mao) >= TEMPOmin0mao && velocidade >= 5 && velocidade < 25){
       digitalWrite(pin_alerta,HIGH);
       alarme_volante = true;
       tela();
    }
    if(((millis()/1000) - ultimoTempo0mao) >= TEMPOmed0mao && velocidade >= 25 && velocidade < 50){
       digitalWrite(pin_alerta,HIGH);
       alarme_volante = true;
       tela();
    }
    if(((millis()/1000) - ultimoTempo0mao) >= TEMPOmax0mao && velocidade >= 50){
       digitalWrite(pin_alerta,HIGH);
       alarme_volante = true;
       tela(); 
     }
  }
  else if ((sensorValor1 > limite) && (sensorValor2 > limite))
  {  if(contador3==0){
      contador1 = 0;
      contador2 = 0;
      contador3 = 1;
      alarme_volante = false;
      if(alarme_cinto == false){
      digitalWrite(pin_alerta,LOW);
      }
      mao1_volante = false;
      mao0_volante = false;
      mao2_volante = true;
   }
  }
      if(mao0_volante){    
        contador_volante1++;
       }
       if(mao1_volante){
        contador_volante2++;
       }
       if(mao2_volante){
        contador_volante3++;
       }
       if(((millis()/1000)-tempo_volante)>=300){
        volante_porcentagem1[contador_geral2] = contador_volante2/contador_volante0;
        volante_porcentagem2[contador_geral2] = contador_volante3/contador_volante0;
        contador_geral2++;
        contador_volante1 = 0;
        contador_volante2 = 0;
        contador_volante3 = 0;
        contador_volante0 = 0;
        tempo_volante = millis()/1000;
        }
      
    }
    
 void can_msg(){
  if((rxId & 0x80000000) == 0x80000000){
    sprintf(msgString, "Extended ID: 0x%.8lX  DLC: %1d  Data:", (rxId & 0x1FFFFFFF), len);
    }
    else{
      sprintf(msgString, "Standard ID: 0x%.3lX       DLC: %1d  Data:", rxId, len);
    }
   if((rxId & 0x40000000) == 0x40000000){    // Determine if messagem is a remote request frame.
      sprintf(msgString, " REMOTE REQUEST FRAME");
    } 
    else {
      for(byte i = 0; i<len; i++){
        sprintf(msgString, " 0x%.2X", rxBuf[i]);
    }
    }
    coletar_dados_can();
    check_can();
 }

  void coletar_dados_can(){
             velocidade = rxBuf[0];
                 airbag = rxBuf[1];
      temperatura_motor = rxBuf[2];
                  cinto = rxBuf[3];
                  motor = rxBuf[4];
     if(motor){
      contador_can++;
      soma_velocidade = soma_velocidade + velocidade;
      soma_temperatura_motor = soma_temperatura_motor + temperatura_motor;
      }
    if(((millis()/1000) - tempo_can)>=300){
      media_velocidade[contador_geral] = (soma_velocidade/contador_can);
      media_temperatura[contador_geral] = (soma_temperatura_motor/contador_can);
      tempo_can = millis()/1000;
      contador_can=0;
      soma_velocidade = 0;
      soma_temperatura_motor = 0;
      contador_geral++;
  }
 }

  void tratar_dados_can(){
    dados_can_final[0] = tempo_motor_ligado;
    dados_can_final[1] = volante_porcentagem1[contador_geral2-1];
    dados_can_final[2] = volante_porcentagem2[contador_geral2-1];
    dados_can_final[3] = faseEmergencia;
    dados_can_final[4]=0;
    dados_can_final[5] = airbag;
    dados_can_final[6] = media_velocidade[contador_geral-1];
    dados_can_final[7] = media_temperatura[contador_geral-1];
   }

  void check_can(){
   if(cinto || !motor){
    alarme_cinto = false;
    if(alarme_volante == false){
    digitalWrite(pin_alerta,LOW);
    }
    tela();
    }
   if(!cinto && motor){
    digitalWrite(pin_alerta,HIGH);
    alarme_cinto = true;
    tela();
    }
  }

 void sendToEsp(){
  Serial3.write(-3);
  Serial3.write(dados_can_final[0]);
  Serial3.write(dados_can_final[1]);
  Serial3.write(dados_can_final[2]);
  Serial3.write(dados_can_final[3]);
  Serial3.write(dados_can_final[4]);
  Serial3.write(dados_can_final[5]);
  Serial3.write(dados_can_final[6]);
  Serial3.write(dados_can_final[7]);
  
}

 void gsm(){
  if(gprs.readable()) {
    Ler_dados_GSM();
    deletar_dados_GSM();
   }
  }
 void coletar_numero(){
  novo_numero = read_StringEEPROM(0);
  check_numero_anterior();
}

 void check_numero_anterior(){
  if(novo_numero.length() == 14){
    for ( int y = 0; y<14; y++){
      numero_de_emergencia[y] = novo_numero.charAt(y);
    }
  }
}

 void Ler_dados_GSM(){
  sim900_read_buffer(gprsBuffer,120,DEFAULT_TIMEOUT);
  check_dados();
}

 void deletar_dados_GSM(){
  sim900_clean_buffer(gprsBuffer,32);  
  novo_numero.remove(0,14);
}

 void check_dados(){
 bool check_1 = false;
 bool check_2 = false;
  deletar_numero_anterior();
  for(int x = 0; x<120; x++){
    if(gprsBuffer[x] == 'A'){
      check_1 = true;
    }  
    if (check_1){
      if (gprsBuffer[x+2] == '\n'){
        check_2 = true;
        x=120;
      }
      novo_numero += gprsBuffer[x+1];
    }
  }
  if(check_2){
    novo_numero.remove(14,1);
    check_novo_numero();
    check_1 = false;
    check_2 = false;
  }
}

 void deletar_numero_anterior(){
  novo_numero.remove(0,14);
}

 void check_novo_numero(){
  if(novo_numero.length() == 14){
    for ( int y = 0; y<14; y++){
      numero_de_emergencia[y] = novo_numero.charAt(y);
    }
    update_StringEEPROM(0,novo_numero);
    SMS_novo_numero();
  }
}

 void SMS_novo_numero(){
  if(gprs.sendSMS(numero_de_emergencia,numero_de_emergencia)){
  }
  else{
    can();
    SMS_novo_numero();
  }
}
