
#include <LiquidCrystal.h>
// estado do jogo (0 p/ não iniciado e 1 p/ iniciado)
int estado = 0;
// estado para verificar se o tempo chegou a 0 ou não
int temp = 0;
// estado para verificar se a resposta ocorreu ou nao
int resp = 0;

// pinos dos LEDs, botões e buzzer
int pin_ledE = 10; //Pino do LED Esquerdo
int pin_ledD = 9; // Pino do LED Direito

int pin_botaoE = 3; // Pino do botão Esquerdo
int pin_botaoD = 4; // Pino do botão Direito

int pin_botaoIni = 2; // Pino do botão Iniciar e Reiniciar

int pin_buzzer = 13; // Pino do buzzer

// vetor para a sequência de memória
int sequencia[10];
int sequencia_botao[10];

// contador para a sequência / tempo
int cont = 0;

LiquidCrystal lcd(12, 11, 8, 7, 6, 5);

void setup() {
  pinMode(pin_ledE, OUTPUT);
  pinMode(pin_ledD, OUTPUT);
  pinMode(pin_botaoIni, INPUT_PULLUP);
  pinMode(pin_botaoE, INPUT_PULLUP);
  pinMode(pin_botaoD, INPUT_PULLUP);
  pinMode(pin_buzzer, OUTPUT);
  
  lcd.begin(16, 2);

  attachInterrupt(digitalPinToInterrupt(pin_botaoIni), setar, FALLING);
  Serial.begin(9600);
}

void loop() {
  // LCD na tela inicial
  if (estado == 0) {
    lcd.setCursor(0, 0);
    lcd.print("Pressione o");
    lcd.setCursor(0, 1);
    lcd.print("botao p/ iniciar");
  }
  // LCD na parte 1
  if (estado == 1) {
    memoria();
    for (int i = 0; i < 10; i++){
      Serial.println(sequencia[i]);
    }
    if (digitando() && estado != 0) {
      Serial.println(estado);
      som(0);
      lcd.clear();
      lcd.setCursor(4, 0);
      lcd.print("Parabens!   ");
      lcd.setCursor(4, 1);
      lcd.print("Acertou!       ");
      delay(2000);
      estado = 2;
    } 
    else if(!digitando()){
      som(1);
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print("Errado!     ");
      lcd.setCursor(0, 1);
      lcd.print("Tente de novo");
      delay(2000);
      estado = 0;
    } else{
      som(1);
      lcd.clear();
      lcd.setCursor(6, 0);
      lcd.print("Jogo");
      lcd.setCursor(4, 1);
      lcd.print("resetado");
      delay(2000);
    }
  }
  // LCD na parte 2
  if (estado == 2){
    perguntas();
    if(estado == 0 && temp != 1){
      som(1);
      lcd.clear();
      lcd.setCursor(6, 0);
      lcd.print("Jogo");
      lcd.setCursor(4, 1);
      lcd.print("resetado");
      delay(2000);
    }else{
      temp = 0;
    }
  }
  
  // LCD na parte 3
  if(estado == 3){
    pergunta_final();
    if(estado == 0 && resp != 1){
      som(1);
      lcd.clear();
      lcd.setCursor(6, 0);
      lcd.print("Jogo");
      lcd.setCursor(4, 1);
      lcd.print("resetado");
      delay(2000);
    } else{
        resp = 0;
    }
  }
}

//--------------------- SETA ESTADOS -------------------------

void setar() {
  if (estado == 0) {
    //------------------- ARRUMAR --------------------------
      estado = 1;
      //som(2);
    } else {
      estado = 0;
      som(3);
      resetar_jogo();
      Serial.println("Jogo resetado!");
    }
}

//------------------------- PARTE 1 --------------------------

void memoria() {
  randomSeed(analogRead(0));
  if (estado == 1) {
    lcd.clear();
    lcd.setCursor(4, 0);
    lcd.print("Fase da");
    lcd.setCursor(4, 1);
    lcd.print("memoria");
    delay(2000);
    if (estado == 0) return;
    lcd.clear();
    lcd.setCursor(3, 0);
    lcd.print("Memorize a");
    lcd.setCursor(3, 1);
    lcd.print("sequencia");
    delay(2000);
    if (estado == 0) return;

    for (int i = 0; i < 10; i++) {
      if (estado == 0) return; 
      int num = random(2); 
      sequencia[i] = num;

      if (num == 0) {
        digitalWrite(pin_ledE, HIGH);
        delay(500);
        digitalWrite(pin_ledE, LOW);
      } else {
        digitalWrite(pin_ledD, HIGH);
        delay(500);
        digitalWrite(pin_ledD, LOW);
      }
      delay(500); 
      if (estado == 0) return; 
    }

    lcd.clear();
    lcd.setCursor(4, 0);
    lcd.print("Repita a");
    lcd.setCursor(3, 1);
    lcd.print("Sequencia:");
    cont = 0;
    if (estado == 0) return;
  }
}

//------------------- RESPOSTA PARTE 1 ------------------------

bool digitando() {
  cont = 0;
  while (cont < 10 && estado == 1) {
    if (digitalRead(pin_botaoE) == LOW) {
      sequencia_botao[cont] = 0;
      if (sequencia_botao[cont] != sequencia[cont]) {
        return false; 
      }
      lcd.setCursor(13, 1);
      lcd.print(cont + 1);
      cont++;
      delay(300);
    } else if (digitalRead(pin_botaoD) == LOW) {
      sequencia_botao[cont] = 1;
      if (sequencia_botao[cont] != sequencia[cont]) {
        return false; 
      }
      lcd.clear();
      lcd.setCursor(4, 0);
      lcd.print("Repita a");
      lcd.setCursor(2, 1);
      lcd.print("Sequencia:");
      lcd.setCursor(13, 1);
      lcd.print(cont + 1);
      cont++;
      delay(300);
    }
  }
  return true; 
}

//---------------- BANCO DE PERGUNTAS PARTE 2 -----------------

const char* bancodedados_perguntas[13] = {
  "O Arduino possui portas digitais?",
  "As portas digitais funcionam com 5V?",
  "A computacao movel eh sempre segura?",
  "A tecnologia 5G eh mais rapida que a 4G?",
  "Os servomotores sao um tipo de atuador?",
  "Um rele pode ser considerado um atuador?",
  "O Arduino pode ler sinais analogicos?",
  "Um potenciometro eh um sensor analogico?",
  "Um LDR fornece saida digital?",
  "Um LDR fornece saida digital?",
  "O sinal analogico eh sempre continuo?",
  "Um termistor tem saida digital?"
};

//---------------- BANCO DE RESPOSTAS PARTE 2 -----------------

bool bancodedados_respostas[13] = {
  true, 
  true,   
  false,  
  true, 
  true,  
  true,  
  true,   
  true,  
  false,  
  false,  
  false,  
  false  
};


//------------------------ PARTE 2 ----------------------------


int respostas_corretas = 0;
int num_perguntas[5];

void perguntas() {
  randomSeed(analogRead(0));
  bool pulou = false;
  cont = 0;
  if (estado == 0) return;
  if (estado == 2) {
    bool resposta = false;
    lcd.clear();
    lcd.setCursor(4, 0);
    lcd.print("Fase das");
    lcd.setCursor(4, 1);
    lcd.print("perguntas");
    delay(2000);
    lcd.clear();
    lcd.setCursor(3, 0);
    lcd.print("Responda as");
    lcd.setCursor(4, 1);
    lcd.print("perguntas");
    delay(2000);
    if (estado == 0) return;
    for (int j = 0; j < 5; j++){
      int num = random(13);
        for(int c = 0; c < 5; c++){
          if(num_perguntas[c] == num){
              num = random(13);
            }
        }
        num_perguntas[j] = num;
    }
    
    for (int i = 0; i < 5; i++) {
      cont++;
      if (estado == 0) return;
      lcd.clear();
      lcd.setCursor(2, 0);
      lcd.print("Responda com");
      lcd.setCursor(3, 1);
      lcd.print("Sim ou Nao");
      delay(1500);
      
      lcd.clear();
      int num = num_perguntas[i]; 
      lcd.setCursor(0, 0);
      lcd.print(bancodedados_perguntas[num]);
      delay(1000);
      
      for (int pos = 0; pos < strlen(bancodedados_perguntas[num]) - 16; pos++) {
        if (estado == 0) return;
        lcd.scrollDisplayLeft();  
        delay(300);
      }
      lcd.clear();
      lcd.setCursor(3, 0);
      lcd.print("Questao ");
      lcd.print(cont);
      Serial.println(bancodedados_respostas[num]);
      lcd.setCursor(3, 1);
      lcd.print("Sim / Nao");
      delay(2000);
      unsigned long tempo = millis();
      Serial.println("tempo:");
      Serial.println(tempo);
      int resp = 0;

      if (estado == 0) return;
      lcd.clear();

      while((millis() - tempo) < 10000) {
        delay(2);
        lcd.setCursor(3, 0);
        lcd.print("Tempo: ");
        lcd.print((tempo + 10000 - millis())/1000 + 1);
        if(((tempo + 10000 - millis())/1000 + 1) <= 3){
          som(4);
        }
        if(((tempo + 10000 - millis()) + 1000) < 10000 ){
          lcd.setCursor(11,0);
          lcd.print(" ");
        }
        lcd.setCursor(3, 1);
        lcd.print("Sim / Nao");

        if(digitalRead(pin_botaoE) == LOW) {
          resp = 1;
          resposta = true;
          Serial.print(resposta);
          break;
        } else if (digitalRead(pin_botaoD) == LOW) {
          resp = 1;
          resposta = false;
          Serial.print(resposta);
          break;
        }
      }
      
      if(resp == 0) {
        if (!pulou) {
          i--;
          cont--;
          som(0);
          int num2 = random(13);
          num_perguntas[i] = num2;
          pulou = true;
          lcd.clear();
          lcd.setCursor(4, 0);
          lcd.print("Pulando...");
          delay(1500);
        } else {
          som(1);
          temp = 1;
          lcd.clear();
          lcd.setCursor(6, 0);
          lcd.print("Tempo");
          lcd.setCursor(4, 1);
          lcd.print("Esgotado");
          delay(1500); 
          estado = 0;
          return;
        }
      }
     
      if(bancodedados_respostas[num] == resposta) { 
        if(bancodedados_respostas[num] == true) {
          som(0);
          respostas_corretas++;
          lcd.clear();
          lcd.setCursor(0, 0);
          lcd.print("Resposta Correta: ");
          lcd.setCursor(2, 1);
          lcd.print("*Sim / Nao");
          delay(1000); 
        } else {
          som(0);
          respostas_corretas++;
          lcd.clear();
          lcd.setCursor(0, 0);
          lcd.print("Resposta Correta: ");
          lcd.setCursor(2, 1);
          lcd.print("Sim / *Nao");
          delay(1000);
        }
      } else {
        if(bancodedados_respostas[num] == true) {
          som(1);
          lcd.clear();
          lcd.setCursor(0, 0);
          lcd.print("Resposta Errada: ");
          lcd.setCursor(2, 1);
          lcd.print("*Sim / Nao");
          delay(1000);
          estado = 0;
          return;
        } else {
          som(1);
          lcd.clear();
          lcd.setCursor(0, 0);
          lcd.print("Resposta Errada: ");
          lcd.setCursor(2, 1);
          lcd.print("Sim / *Nao");
          delay(1000);
          estado = 0;
          return;
        }
      }
      if(respostas_corretas == 5){
        estado = 3;
        return;
      }
      lcd.clear();
    }
  }
}

//---------------- PERGUNTA PARTE 3 -----------------

const char* pergunta_final_texto = "Voce gostou do jogo?";

//---------------- RESPOSTA PARTE 3 -----------------

bool resposta_final = true;

//------------------------ PARTE 3 ----------------------------

void pergunta_final(){
  if (estado == 0) return;
  if (estado == 3){
    bool resposta = false;
    lcd.clear();
    lcd.setCursor(4, 0);
    lcd.print("Pergunta");
    lcd.setCursor(5, 1);
    lcd.print("Final");
    delay(2000);
    lcd.clear();
    if (estado == 0) return;
    
    if (estado == 0) return;
    lcd.clear();
    lcd.setCursor(2, 0);
    lcd.print("Responda com");
    lcd.setCursor(3, 1);
    lcd.print("Sim ou Nao");
    delay(1500);
    
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print(pergunta_final_texto);
    delay(1000);
    
    for (int pos = 0; pos < strlen(pergunta_final_texto) - 16; pos++) {
      if (estado == 0) return;
      lcd.scrollDisplayLeft();  
      delay(500);
    }
    
    lcd.clear();
    lcd.setCursor(1, 0);
    lcd.print("Questao Final");
    lcd.setCursor(3, 1);
    lcd.print("Sim / Nao");
    delay(2000);
    unsigned long tempo = millis();

    if (estado == 0) return;
    lcd.clear();
    
    while((millis() - tempo) < 10000) {
      delay(2);
      lcd.setCursor(3, 0);
      lcd.print("Tempo: ");
      lcd.print((tempo + 10000 - millis())/1000 + 1);
      if(((tempo + 10000 - millis()) + 1000) < 10000 ){
        lcd.setCursor(11,0);
        lcd.print(" ");
      }
      if(((tempo + 10000 - millis())/1000 + 1) <= 3){
          som(4);
      }
      lcd.setCursor(3, 1);
      lcd.print("Sim / Nao");
  
      if(digitalRead(pin_botaoE) == LOW) {
        resposta_final = true;
        resp = 1;
        break;
      } else if (digitalRead(pin_botaoD) == LOW) {
        resposta_final = false;
        resp = 1;
        break;
      }
    }
    
    if(resposta_final == true){
      lcd.clear();
      lcd.setCursor(4, 0);
      lcd.print("PARABENS!");
      lcd.setCursor(3, 1);
      lcd.print("VOCE VENCEU!");
      som(2);
      delay(4000);
      lcd.clear();
      estado = 0;
      return;
    } else{
      som(1);
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print("Resposta Errada: ");
      lcd.setCursor(2, 1);
      lcd.print("*Sim / Nao");
      delay(2000);
      lcd.clear();
      estado = 0;
      return;
    }
  
    
  }
}

//--------------------------- SOM ----------------------------

void som(int sons) {
  if (sons == 0) { // acerto
    tone(pin_buzzer, 1200, 300);
    delay(100);
    tone(pin_buzzer, 1400, 300);
  } else if (sons == 1) { // erro
    tone(pin_buzzer, 400, 500);
    delay(50);
    tone(pin_buzzer, 350, 500);
  } else if (sons == 2) { // vitoria
    tone(pin_buzzer, 1000, 300);
    delay(300);
    tone(pin_buzzer, 1200, 300);
    delay(300);
    tone(pin_buzzer, 1400, 300);
    delay(300);
    tone(pin_buzzer, 1600, 600);
    delay(600);
    tone(pin_buzzer, 1400, 300);
    delay(300);
    tone(pin_buzzer, 1600, 600);
  } else if (sons == 3) { // desistência
    tone(pin_buzzer, 200, 500);
    delay(100);
    tone(pin_buzzer, 150, 500);
  } else if (sons == 4) { // tempo acabando
    for (int i = 1000; i < 2000; i += 200) {
      tone(pin_buzzer, i, 150);
      delay(150);
    }
  }
}

//----------------------- LIMPA ARRAY -------------------------

void resetar_jogo() {
  cont = 0;
  for (int i = 0; i < 10; i++) {
    sequencia[i] = 0;
    sequencia_botao[i] = 0;
  }
  lcd.clear();
}
