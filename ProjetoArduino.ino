//#include <WiFiEsp.h>
//#include <PubSubClient.h>

#include <dht.h>

#include <Servo.h>

/*
 * Q0383
 * AUTOR:   BrincandoComIdeias
 * LINK:    https://www.youtube.com/brincandocomideias ; https://cursodearduino.net/
 * SKETCH:  Exemplo DHT22
 * DATA:    26/09/2018
 */
//INCLUSÃO DAS BIBLIOTECAS
#include <servo.h>
//DEFINIÇÃO DE PINOS
#define pinSensor 7
#define pinRele1 2

#define servoPin 4
//INTERVALO DE LEITURA
#define intervalo 2000

//WiFi
char ssid[] = "ContraQuantos";                // SSID / nome da rede WiFi que deseja se conectar
char pass[] = "1poucomais...";  

//int status = WL_IDLE_STATUS;
// Senha da rede WiFi que deseja se conectar
//WiFiEspServer server(80);
//
//RingBuffer buf(8); //BUFFER PARA AUMENTAR A VELOCIDADE E REDUZIR A ALOCAÇÃO DE MEMÓRIA 

#define ID_MQTT  "BCI01"            //Informe um ID unico e seu. Caso sejam usados IDs repetidos a ultima conexão irá sobrepor a anterior. 
#define TOPIC_PUBLISH "BCIBotao1"    //Informe um Tópico único. Caso sejam usados tópicos em duplicidade, o último irá eliminar o anterior.
//PubSubClient MQTT(wifiClient);        // Instancia o Cliente MQTT passando o objeto espClient

//Declaração das Funções
//void mantemConexoes();  //Garante que as conexoes com WiFi e MQTT Broker se mantenham ativas
//void conectaWiFi();     //Faz conexão com WiFi
//void conectaMQTT();     //Faz conexão com Broker MQTT
//void enviaPacote();     //

//MQTT Server
const char* BROKER_MQTT = "iot.eclipse.org"; //URL do broker MQTT que se deseja utilizar
int BROKER_PORT = 1883;                      // Porta do Broker MQTT

unsigned long delayIntervalo;
dht sensorDHT;

Servo mainServo;
int val = 10;
int potpin = A2;
int valAnterior;

const int quente = 25; //setando valor quente
const double frio = 25; 
double temp_atual = 20;

//CRIANDO VARIAVEIS E INSTANCIANDO OBJETOS


void setup()
{
  // INICIANDO MONITOR SERIAL  
  mainServo.attach(servoPin);
  Serial.begin(115200);
  pinMode(pinRele1, OUTPUT);

  //MQTT 
  
//  conectaWiFi();
//  MQTT.setServer(BROKER_MQTT, BROKER_PORT);   
//  Serial.print("antesserial");
//  WiFi.init(&Serial); //INICIALIZA A COMUNICAÇÃO SERIAL COM O SHIELD WIFI ESP8266
//  Serial.println("depoisSerial");
//  WiFi.config(IPAddress(192,168,0,110)); 
//  //COLOQUE UMA FA
//  //INÍCIO - VERIFICA SE O SHIELD ESTÁ ACOPLADO AO ARDUINO, CONECTA A REDE SEM FIO E INICIA O WEBSERVER
//  Serial.println("verifica Status");
//  Serial.println(WiFi.status());
//  if(WiFi.status() == WL_NO_SHIELD){
//      Serial.println("Não encontrou o shield");
//
//    while (true);
//      Serial.println("dentro do While ");
//
//  }
//  Serial.println("verificaConexao");
//  while(status != WL_CONNECTED){
//    Serial.println("chegouConexao");
//    status = WiFi.begin(ssid, pass);
//    Serial.println("passouConexao");
//  }
//  server.begin();
  //FIM - VERIFICA SE O SHIELD ESTÁ ACOPLADO AO ARDUINO
  
  //IMPRIMINDO INFORMAÇÕES SOBRE A BIBLIOTECA
  Serial.print("VERSAO DA BIBLIOTECA: ");
  Serial.println(DHT_LIB_VERSION);
  Serial.println();
  Serial.println("Status,\tTempo(uS),\tUmidade(%),\tTemperatura(C)");
  
}

void loop()
{
    //mantemConexoes();
    
    if ( (millis() - delayIntervalo) > intervalo ) {
      //LEITURA DOS DADOS
      unsigned long start = micros();
      int chk = sensorDHT.read11(pinSensor);
      unsigned long stop = micros();
      
      // VERIFICA SE HOUVE ERRO
      switch (chk)
      {
      case DHTLIB_OK:
          
          Serial.print("OK,\t");
          break;
      case DHTLIB_ERROR_CHECKSUM:
          Serial.print("Checksum error,\t");
          break;
      case DHTLIB_ERROR_TIMEOUT:
          Serial.print("Time out error,\t");
          break;
      case DHTLIB_ERROR_CONNECT:
          Serial.print("Connect error,\t");
          break;
      case DHTLIB_ERROR_ACK_L:
          Serial.print("Ack Low error,\t");
          
          break;
      case DHTLIB_ERROR_ACK_H:
          Serial.print("Ack High error,\t");
          break;
      default:
          Serial.print("Unknown error,\t");
          break;
      }
  
      
      // EXIBINDO DADOS LIDOS
      Serial.print(stop - start);
      Serial.print(", \t\t");
      Serial.print(sensorDHT.humidity, 1 /*FORMATAÇÃO PARA UMA CASA DECIMAL*/);
      Serial.print(",\t\t");
      Serial.println(sensorDHT.temperature, 1 /*FORMATAÇÃO PARA UMA CASA DECIMAL*/);
      int sensor = sensorDHT.temperature;
      float voltagem = (sensor / 1024.0) * 5.0;
        float tempC = (voltagem - .5) * 100;
        float tempF = (tempC * 1.8) + 32;
        temp_atual = sensorDHT.temperature;
        
        if (24 < 25 ) { //led verde
          Serial.println("caiu");
          digitalWrite(pinRele1, LOW);
          //mainServo.write(60);
          delay(1000);
        }
        else if (sensor > quente) { //led vermelho
          digitalWrite(pinRele1, HIGH);
          if(val > 179){
          //digitalWrite(pinRele1, 0);
          mainServo.write(10); 
          delay(100);
          val = 10;
          }

          mainServo.write(val);
          val = val + 10;
      
          delay(12);
        }
        else { //led amarelo
          //digitalWrite(pinRele1, 0);
          mainServo.write(170);
          delay(6);
        }
      

      delayIntervalo = millis();
    };
}

//void mantemConexoes() {
//    if (!MQTT.connected()) {
//       Serial.print("MQTT ");
//
//       conectaMQTT(); 
//    }
//   Serial.print("cleiton");
//
//    conectaWiFi(); //se não há conexão com o WiFI, a conexão é refeita
//}

//Função conectaWifi
//void conectaWiFi() {
  

//  if (WiFi.status() == WL_CONNECTED) {
//     return;
//  }
        
//  Serial.println("Conectando-se na rede: ");
//  Serial.print(SSID);
//  Serial.println("  Aguarde!");

//  WiFi.begin(SSID, PASSWORD); // Conecta na rede WI-FI  
//  while (WiFi.status() != WL_CONNECTED) {
//      delay(100);
//      Serial.print(".");
//  }
//  
//  Serial.println();
//  Serial.print("Conectado com sucesso, na rede: ");
//  Serial.print(SSID);  
//  Serial.print("  IP obtido: ");
//  Serial.println(WiFi.localIP()); 
//}

//void conectaMQTT() { 
//    while (!MQTT.connected()) {
//        Serial.print("Conectando ao Broker MQTT: ");
//        Serial.println(BROKER_MQTT);
//        if (MQTT.connect(ID_MQTT)) {
//            Serial.println("Conectado ao Broker com sucesso!");
//        } 
//        else {
//            Serial.println("Noo foi possivel se conectar ao broker.");
//            Serial.println("Nova tentatica de conexao em 10s");
//            delay(10000);
//        }
//    }
//}
