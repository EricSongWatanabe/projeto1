#include <LiquidCrystal.h>

// estado do jogo (0 p/ não iniciado e 1 p/ iniciado)
int estado = 0;
// estado para verificar se o tempo chegou a 0 ou não
int temp = 0;

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
unsigned long ultimoTempo = 0;

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
    lcd.print("Pressione o      ");
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
      som(8);
      lcd.clear();
      lcd.setCursor(4, 0);
      lcd.print("Parabens!   ");
      lcd.setCursor(4, 1);
      lcd.print("Acertou!       ");
      delay(2000);
      estado = 2;
    } 
    else if(!digitando()){
      som(2);
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print("Errado!     ");
      lcd.setCursor(0, 1);
      lcd.print("Tente de novo");
      delay(2000);
      estado = 0;
    } else{
      som(3);
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
      som(3);
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
  if (estado == 3){
    pergunta_final();
  }
}

//--------------------- SETA ESTADOS -------------------------

void setar() {
  if (estado == 0) {
    //------------------- ARRUMAR --------------------------
      estado = 1;
    } else {
      estado = 0;
      som(7);
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
      //som(0);
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
      //som(0);
      cont++;
      delay(300);
    }
  }
  return true; 
}

//---------------- BANCO DE PERGUNTAS PARTE 2 -----------------

const char* bancodedados_perguntas[25] = {
  "O Arduino possui portas digitais?",
  "É possível conectar um LED a uma porta digital?",
  "As portas digitais podem ler sinais analógicos?",
  "Um botão pode ser conectado a uma porta digital?",
  "As portas digitais podem ser configuradas como pull-up?",
  "As portas digitais funcionam com 5V?",
  "As portas digitais funcionam com tensões acima de 5V?",
  "A computação móvel é sempre segura?",
  "A tecnologia 5G eh mais rapida que a 4G?",
  "O iOS eh um sistema operacional da Microsoft?",
  "O LCD 16x2 mostra 16 caracteres em 3 linhas?",
  "O display LCD faz uso de cristais liquidos?",
  "Eh preciso um resistor para controlar um motor CC?",
  "Os servomotores sao um tipo de atuador?",
  "Um rele pode ser considerado um atuador?",
  "O Arduino pode ler sinais analogicos?",
  "Um potenciometro eh um sensor analogico?",
  "O Arduino apenas possui entradas analogicas?",
  "Um LDR fornece saida digital?",
  "A funcao analogWrite() le valores analogicos?",
  "Um LDR fornece saida digital?",
  "A funcao analogWrite() le valores analogicos?",
  "O sinal analogico eh sempre continuo?",
  "Um termistor tem saida digital?",
  "O valor lido pelo analogRead() varia de 0 a 255?"
};

//---------------- BANCO DE RESPOSTAS PARTE 2 -----------------

bool bancodedados_respostas[25] = {
  true,   
  true,   
  false,  
  false,  
  true,  
  true,  
  true,   
  false,  
  true,  
  false,  
  false,  
  true,   
  false, 
  true,  
  true,  
  true,   
  true,   
  false, 
  false,  
  false,  
  false,  
  false,  
  false,  
  false,  
  true    
};

//------------------------ PARTE 2 ----------------------------

void perguntas() {
  randomSeed(analogRead(0));
  cont = 0;
  int skips = 1;
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
    lcd.clear();
    lcd.setCursor(2, 0);
    lcd.print("Responda com");
    lcd.setCursor(3, 1);
    lcd.print("Sim ou Nao");
    delay(2000);
    if (estado == 0) return;

    for (int q = 0; q <= 4; q++) {
      cont++;
      if (estado == 0) return;
      lcd.clear();
      lcd.setCursor(1, 0);
      lcd.print("Questao ");
      lcd.print(cont);
      lcd.setCursor(11, 0);
      lcd.print("de 5");
      delay(2000);
      lcd.clear();
      int num = random(25); 
      lcd.setCursor(0, 0);
      lcd.print(bancodedados_perguntas[num]);
      delay(1000);
      Serial.println(bancodedados_respostas[num]);

      for (int pos = 0; pos < strlen(bancodedados_perguntas[num]) - 16; pos++) {
        if (estado == 0) return;
        lcd.scrollDisplayLeft();  
        delay(400);
      }
      unsigned long tempo = millis();
      int resp = 0;
      if (estado == 0) return;
      lcd.clear();
      bool relogio = false;  
      while((millis() - tempo) < 10000) {
        delay(2);
        lcd.setCursor(3, 0);
        lcd.print("Tempo: ");
        lcd.print((tempo + 10000 - millis())/1000 + 1);
        if(((tempo + 10000 - millis()) + 1000) < 10000 ) {
          lcd.setCursor(11,0);
          lcd.print(" ");
        }
        //if(((tempo + 10000 - millis()) + 1000) < 6000) { 
        //  if(tempo - ultimoTempo >= 500){ 
        //    if (relogio == false) {
        //      som(5);
        //      relogio = true;
        //    } else if(relogio == true) {
        //      som(6);
        //      relogio = false;
        //    }
        //    ultimoTempo = tempo; 
        //  }
        //}
        lcd.setCursor(3, 1);
        lcd.print("Sim / Nao");

        if(digitalRead(pin_botaoE) == LOW) {
          resposta = true;
          resp = 1;
          break;
        } else if (digitalRead(pin_botaoD) == LOW) {
          resposta = false;
          resp = 1;
          break;
        }
      }
      if(resp == 0) {
        if(skips != 1) {
          temp = 1;
          som(2);
          lcd.clear();
          lcd.setCursor(5, 0);
          lcd.print("Tempo");
          lcd.setCursor(4, 1);
          lcd.print("Esgotado");
          delay(1500); 
          estado = 0;
          return;
        } else {
          som(1);
          lcd.clear();
          lcd.setCursor(4, 0);
          lcd.print("Pergunta");
          lcd.setCursor(5, 1);
          lcd.print("Pulada");
          delay(1500);
          skips--;
        }
      }
      if(bancodedados_respostas[num] == resposta) { 
        if(bancodedados_respostas[num] == true) {
          som(0);
          lcd.clear();
          lcd.setCursor(0, 0);
          lcd.print("Resposta Correta: ");
          lcd.setCursor(2, 1);
          lcd.print("*Sim / Nao");
          delay(1500); 
        } else {
          som(0);
          lcd.clear();
          lcd.setCursor(0, 0);
          lcd.print("Resposta Correta: ");
          lcd.setCursor(2, 1);
          lcd.print("Sim / *Nao");
          delay(1500);
        }
      }
      else {
        if(bancodedados_respostas[num] == true) {
          som(2);
          lcd.clear();
          lcd.setCursor(0, 0);
          lcd.print("Resposta Errada: ");
          lcd.setCursor(2, 1);
          lcd.print("*Sim / Nao");
          delay(1500);
          estado = 0;
          temp = 1;
          return;
        } else {
          som(2);
          lcd.clear();
          lcd.setCursor(0, 0);
          lcd.print("Resposta Errada: ");
          lcd.setCursor(2, 1);
          lcd.print("Sim / *Nao");
          delay(1500);
          estado = 0;
          temp = 1;
          return;
        }
      }
      lcd.clear();
      if(q == 4){
        som(8);
        lcd.setCursor(4, 0);
        lcd.print("Parabens!   ");
        lcd.setCursor(4, 1);
        lcd.print("Acertou!       ");
        delay(2000);
        estado = 3;
        return;
      }
    }
  }
}


//------------------------- PARTE 3 --------------------------

void pergunta_final(){
  if(estado == 0) return;
  if(estado == 3){
    lcd.clear();
    lcd.setCursor(4, 0);
    lcd.print("Fase da");
    lcd.setCursor(1, 1);
    lcd.print("Pergunta Final");
    delay(2000);
    int resp = 0;
    lcd.clear();
    while(resp == 0){
      if(estado == 0) return;
      lcd.setCursor(0, 0);
      lcd.print("Gostou do Jogo?");
      lcd.setCursor(3, 1);
      lcd.print("Sim / Nao");
      if(digitalRead(pin_botaoE) == LOW) {
        resp = 1;
        som(4);
        char* respfinal = "Voce acertou todas as perguntas";
        lcd.setCursor(0, 0);
        lcd.print("    Parabens!   ");
        lcd.setCursor(0, 1);
        lcd.print(respfinal);
        delay(1000);
        for (int pos = 0; pos < strlen(respfinal) - 16; pos++) {
          if (estado == 0) return;
          lcd.scrollDisplayLeft();  
          delay(350);
        }
        lcd.clear();
        lcd.setCursor(0, 0);
        lcd.print("Agradecemos o");
        lcd.setCursor(0, 1);
        lcd.print("seu feedback! ;)");
        delay(2000);
        estado = 0;
        return;
      } else if (digitalRead(pin_botaoD) == LOW) {
        resp = 1;
        som(8);
        lcd.clear();
        lcd.setCursor(0, 0);
        lcd.print("Agradecemos o");
        lcd.setCursor(0, 1);
        lcd.print("seu feedback! ;)");
        delay(2000);
        estado = 0;
        return;
      }
    }
  }
}


//--------------------------- SOM ----------------------------

void som(int sons) {
  // questao correta
  if (sons == 0) { 
    tone(pin_buzzer, 1000, 200); 
    delay(200);
    tone(pin_buzzer, 1200, 200);
  } 
  // questao pulada
  else if(sons == 1) { 
    tone(pin_buzzer, 800, 500);  
  } 
  // questao incorreta
  else if(sons == 2) { 
    tone(pin_buzzer, 600, 300);  
    delay(300);
    tone(pin_buzzer, 400, 300);
  } 
  // desistir
  else if(sons == 3) { 
    tone(pin_buzzer, 300, 1000);
  }
  // vitoria
  else if(sons == 4) { 
    tone(pin_buzzer, 1000, 300);  
    delay(300);
    tone(pin_buzzer, 1200, 300);  
    delay(300);
    tone(pin_buzzer, 1500, 500);
  }
  // tempo acabando 1
  else if(sons == 5) { 
    tone(pin_buzzer, 800, 100);  
  }
  // tempo acabando 2
  else if(sons == 6) { 
    tone(pin_buzzer, 600, 100);  
  }
  // reiniciando
  else if(sons == 7) {
    tone(pin_buzzer, 1000, 200); 
    delay(200);
    tone(pin_buzzer, 800, 200);  
    delay(200);
    tone(pin_buzzer, 600, 200); 
    delay(200);
    tone(pin_buzzer, 800, 200);   
    delay(200);
    tone(pin_buzzer, 1000, 200);  
  }
  // vitoria p/ fase
  else if(sons == 8) {
    tone(pin_buzzer, 1200, 200);  
    delay(200);
    tone(pin_buzzer, 1400, 200);  
    delay(200);
    tone(pin_buzzer, 1000, 300);  
    delay(300);
    tone(pin_buzzer, 800, 300);   
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