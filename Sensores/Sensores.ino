int i; //Variável para contagem

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
long interval = 60000; // Tempo em ms do intervalo a ser executado

void setup() {
  pinMode(pinopir, INPUT); //Define pino sensor como entrada
  Serial.begin(9600); // inicializa a comunicação serial
}

void loop() {
  
  unsigned long currentMillis = millis(); //Tempo atual em ms
  
  //Temperatura
  temperatura = 0;
  for(i=0;i<=10;i++) { //Faz 10 vezes a leitura do sensor
    temperatura += (float(analogRead(LM35))*5/(1023))/0.01;
    delay(10); //Delay se 10 milissegundos
  }

  //PIR
  presenca = digitalRead(pinopir); //Le o valor do sensor PIR
  if (presenca == HIGH) { //Caso seja detectado um movimento
    //Serial.println("Movimento !!!"); 
    //Envia ao servidor
  }

  //LDR
  valorLDR = 0;
  for(i=0;i<=10;i++) { //Faz 10 vezes a leitura do sensor
    valorLDR += analogRead(portaLDR);  //Lê o valor fornecido pelo LDR 
    delay(10); //Delay se 10 milissegundos
  }
  luminosidade = map(valorLDR/10, 0, 1023, 100, 0); // Converte o valor lido do LDR

  // Envia ao servidor a cada 5 minuto
  if (currentMillis - previousMillis > interval) { 
    previousMillis = currentMillis;    // Salva o tempo atual
    Serial.print("Temperatura: ");
    Serial.println(temperatura/10);
    Serial.print("Valor lido do LDR: ");   
    Serial.print(valorLDR/10); // Mostra o valor lido do LDR no monitor serial  
    Serial.print(" = Luminosidade: ");  
    Serial.print(luminosidade); // Mostra o valor da luminosidade no monitor serial 
    Serial.println("%");  
  }
}
