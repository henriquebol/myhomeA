#include <SPI.h>
#include <Ethernet.h>
//#include <aJSON.h>
/*
{
"andar": 1,
"arduino": 2,
"locais": [
  suite: 1 
    sensores:
      temperatura: 1,
      luminosidade: 2,
      presença: 3,
      energia: 4,
      som: 5
     acionadores: 
   quarto: 2,
   escritório: 3
} 
*/
//LED
const int ledPin = 13;

//LM35
const int LM35 = A0; // Define o pino que lera a saída do LM35
float temperatura; // Variável que armazenará a temperatura medida

//PIR
int pinopir = 7; //Pino ligado ao sensor PIR
int presenca; //Variavel para guardar valor do sensor

//LDR
int portaLDR = A1; //Porta analógica utilizada pelo LDR 
int valorLDR = 0; //Variavel para guardar valor do sensor
float luminosidade = 0; //Variavel para guardar valor em %

//TEMPO
long previousMillis = 0; // Variável de controle do tempo
long interval = 30000; // Tempo em ms do intervalo a ser executado

//Variáveis
int i; //Variável para contagem

//ETHERNET
byte mac[] = { 0x90, 0xA2, 0xDA, 0x0D, 0x7D, 0x54 };
IPAddress ip(192,168,0,200);
char server[] = "myhomes.herokuapp.com";
int serverPort = 80;
//char server[] = "192.168.0.37";
//int serverPort = 3000;
EthernetClient client;

void sendPost (String sendData);

void setup() {
  //Pisca o led 13 a cada 5 segundos se não conseguir ip
  pinMode(ledPin, OUTPUT); 
  digitalWrite(ledPin, LOW);
  
  pinMode(pinopir, INPUT); //Define pino sensor como entrada
  Serial.begin(9600); // inicializa a comunicação serial

  //IP
  Ethernet.begin(mac, ip);
  Serial.print("Conectado! IP:");
  Serial.println(Ethernet.localIP());
  digitalWrite(ledPin, HIGH); // Led 13 acesso se conectado
}

void loop() {
  
  unsigned long currentMillis = millis(); //Tempo atual em ms

  //PIR
    presenca = digitalRead(pinopir); //Le o valor do sensor PIR
    if (presenca == HIGH) { //Caso seja detectado um movimento
      //Serial.println("Movimento !!!"); 
      //Envia ao servidor
    }

// Envia ao servidor a cada 5 minuto
  if (currentMillis - previousMillis > interval) { 
    Serial.println(currentMillis);
    Serial.println(previousMillis);
    Serial.println(interval);
 
    previousMillis = currentMillis;    // Salva o tempo atual
    
    //Temperatura
    temperatura = 0;
    for(i=0;i<=10;i++) { //Faz 10 vezes a leitura do sensor
      temperatura += (float(analogRead(LM35))*5/(1023))/0.01;
      delay(10); //Delay se 10 milissegundos
    }

    //LDR
    valorLDR = 0;
    for(i=0;i<=10;i++) { //Faz 10 vezes a leitura do sensor
      valorLDR += analogRead(portaLDR);  //Lê o valor fornecido pelo LDR 
      delay(10); //Delay se 10 milissegundos
    }
    luminosidade = map(valorLDR/10, 0, 1023, 100, 0); // Converte o valor lido do LDR

    Serial.print("Temperatura: ");
    Serial.println(temperatura/10);
    Serial.print("Valor lido do LDR: ");   
    Serial.print(valorLDR/10); // Mostra o valor lido do LDR no monitor serial  
    Serial.print("- Luminosidade: ");  
    Serial.print(luminosidade); // Mostra o valor da luminosidade no monitor serial 
    Serial.println("%");  

    String data = "{ \"sensor_id\": 1, \"value\": " + String(temperatura/10) + " }";
    //String data = "sensor_id=1&value=125";

    Serial.print(data);
    sendPost(data); // Envia dados por POST ao server

  if (client.available()) {
    char c = client.read();
    Serial.print(c);
  }

  if (!client.connected()) {
    Serial.println();
    Serial.println("disconnecting.");
    client.stop();
  }
  }
}

void sendPost (String sendData) {
  if (client.connect(server, serverPort)) {
      Serial.print("Conectado ao server");
      client.println("POST /api/v1/sensors/measure HTTP/1.1");
      client.print("Host: "); client.println(server);
      client.println("User-Agent: Arduino/1.0");
      client.println("Accept: application/json");
      client.println("Content-Type: application/json");
      //client.println("Content-Type: application/x-www-form-urlencoded");
      client.print("Content-Length: "); client.println(sendData.length());
      client.println("Connection: close");
      client.println();
      client.println(sendData);
    }
}
