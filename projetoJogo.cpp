//
// Eric Song Watanabe - RA: 24.123.028-3
//
// Rafael Iamashita Becsei - RA: 24.123.018-4
//
//
//
//
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

void setup() { //definindo os pinos para cada componente
  pinMode(pin_ledE, OUTPUT);
  pinMode(pin_ledD, OUTPUT);
  pinMode(pin_botaoIni, INPUT_PULLUP);
  pinMode(pin_botaoE, INPUT_PULLUP);
  pinMode(pin_botaoD, INPUT_PULLUP);
  pinMode(pin_buzzer, OUTPUT);
  
  lcd.begin(16, 2); //iniciando o lcd

  attachInterrupt(digitalPinToInterrupt(pin_botaoIni), setar, FALLING); //funcao de interrupção para alterar o estado do botao
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
  if (estado == 1) { //caso o estado esteja em 1 (1ª fase)
    memoria(); //a função para a fase de memória é chamada e a fase começa
    for (int i = 0; i < 10; i++){
      Serial.println(sequencia[i]); //impressao da sequencia no serial (p/ auxiliar nos testes)
    }
    if (digitando() && estado != 0) { //caso o botao de desistencia nao for pressionado e o usuario vencer a fase
      Serial.println(estado);
      som(0); //som de vitoria
      lcd.clear();
      lcd.setCursor(4, 0);
      lcd.print("Parabens!   ");
      lcd.setCursor(4, 1);
      lcd.print("Acertou!       ");
      delay(2000);
      estado = 2;
    } 
    else if(!digitando()){ //caso o usuario perder a fase
      som(1); //som de derrota
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print("Errado!     ");
      lcd.setCursor(0, 1);
      lcd.print("Tente de novo");
      delay(2000);
      estado = 0;
    } else{ //caso o botao de desistencia for pressionado
      som(1); //som de derrota
      lcd.clear();
      lcd.setCursor(6, 0);
      lcd.print("Jogo");
      lcd.setCursor(4, 1);
      lcd.print("resetado");
      delay(2000);
    }
  }
  // LCD na parte 2
  if (estado == 2){ //caso o estado esteja em 2 (2ª fase)
    perguntas(); //a função para a fase das perguntas é chamada e a fase começa
    if(estado == 0 && temp != 1){ //caso o botao de desistencia for pressionado ou o usuario perdeu a fase
      som(1); //som de derrota
      lcd.clear();
      lcd.setCursor(6, 0);
      lcd.print("Jogo");
      lcd.setCursor(4, 1);
      lcd.print("resetado");
      delay(2000);
    }else{
      temp = 0; //variavel volta ao estado inicial
    }
  }
  
  // LCD na parte 3
  if(estado == 3){ //caso o estado esteja em 3 (3ª fase)
    pergunta_final(); //a função para a fase final é chamada e a fase começa
    if(estado == 0 && resp != 1){ //caso o botao de desistencia for pressionado ou o usuario perdeu a fase
      som(1); //som de derrota
      lcd.clear();
      lcd.setCursor(6, 0);
      lcd.print("Jogo");
      lcd.setCursor(4, 1);
      lcd.print("resetado");
      delay(2000);
    } else{
        resp = 0; //variavel volta ao estado inicial
    }
  }
}

//--------------------- SETA ESTADOS -------------------------

void setar() {
  if (estado == 0) { //caso o estado estiver em 0 (menu), ele tornará 1 (1ª fase), ou seja, inicia o jogo
      estado = 1;
      //som(2);
    } else { //caso o estado estiver diferente de zero (qualquer uma das fases), ele tornará 0 (menu), ou seja, reinicia o jogo 
      estado = 0;
      som(3); //som de desistencia
      resetar_jogo();
      Serial.println("Jogo resetado!");
    }
}

//------------------------- PARTE 1 --------------------------

void memoria() { //funcao para o jogo de memoria
  randomSeed(analogRead(0)); //gera uma semente aleatoria para a sequencia de led
  if (estado == 1) { //caso o estado esteja em 1 (1ª fase)
    lcd.clear();
    lcd.setCursor(4, 0);
    lcd.print("Fase da");
    lcd.setCursor(4, 1);
    lcd.print("memoria");
    delay(2000);
    if (estado == 0) return; //caso o botao de desistencia for pressionado
    lcd.clear();
    lcd.setCursor(3, 0);
    lcd.print("Memorize a");
    lcd.setCursor(3, 1);
    lcd.print("sequencia");
    delay(2000);
    if (estado == 0) return; //caso o botao de desistencia for pressionado

    for (int i = 0; i < 10; i++) { //loop para gerar um numero aleatorio (0 ou 1) para a sequencia de leds e armazenando no array
      if (estado == 0) return; //caso o botao de desistencia for pressionado 
      int num = random(2); 
      sequencia[i] = num;

      if (num == 0) { //caso o valor gerado seja 0, acende o led esquerdo
        digitalWrite(pin_ledE, HIGH);
        delay(500);
        digitalWrite(pin_ledE, LOW);
      } else { //caso o valor gerado seja 1, acende o led direito
        digitalWrite(pin_ledD, HIGH);
        delay(500);
        digitalWrite(pin_ledD, LOW);
      }
      delay(500); 
      if (estado == 0) return; //caso o botao de desistencia for pressionado 
    }

    lcd.clear();
    lcd.setCursor(4, 0);
    lcd.print("Repita a");
    lcd.setCursor(3, 1);
    lcd.print("Sequencia:");
    cont = 0;
    if (estado == 0) return; //caso o botao de desistencia for pressionado
  }
}

//------------------- RESPOSTA PARTE 1 ------------------------

bool digitando() { //funcao dedicada para o periodo em que o usuario esta "digitando" a sequencia
  cont = 0;
  while (cont < 10 && estado == 1) { //loop é ativo enquanto o usuario nao aperta 10 botoes para a sequencia ou o botao de desistencia nao foi pressionado
    if (digitalRead(pin_botaoE) == LOW) { //ao clicar o botao esquerdo, é armazenado o valor 0 no array da sequencia
      sequencia_botao[cont] = 0;
      if (sequencia_botao[cont] != sequencia[cont]) { //caso o botão clicado esteja incorreto, retorna que o usuario perdeu
        return false; 
      }
      lcd.setCursor(13, 1);
      lcd.print(cont + 1);
      cont++;
      delay(300);
    } else if (digitalRead(pin_botaoD) == LOW) { //ao clicar o botao direito, é armazenado o valor 1 no array da sequencia
      sequencia_botao[cont] = 1;
      if (sequencia_botao[cont] != sequencia[cont]) {  //caso o botão clicado esteja incorreto, retorna que o usuario perdeu
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

const char* bancodedados_perguntas[13] = { //banco de perguntas para a fase 2
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

bool bancodedados_respostas[13] = { //respostas para cada pergunta no banco (emparelhada)
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


int respostas_corretas = 0; //variavel dedicada a contar quantas respostas foram respondidas corretamente
int num_perguntas[5]; //array para armazenar as perguntas sorteadas

void perguntas() { //funcao para a fase de perguntas
  randomSeed(analogRead(0)); //funcao para gerar uma semente aleatória para sortear as perguntas
  bool pulou = false; //variavel para caso o usuario pulou uma pergunta
  cont = 0; //contador para as perguntas
  if (estado == 0) return; //caso o botao de desistencia for pressionado
  if (estado == 2) { //enquanto o estado ainda esta em 2
    bool resposta = false; //redefine a variavel 'resposta' para false
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
    if (estado == 0) return; //caso o botao de desistencia for pressionado
    for (int j = 0; j < 5; j++){ //sortea 5 perguntas aleatorias e armazena no array
      int num = random(13);
        for(int c = 0; c < 5; c++){
          if(num_perguntas[c] == num){
              num = random(13);
            }
        }
        num_perguntas[j] = num;
    }
    
    for (int i = 0; i < 5; i++) { //loop ativo enquanto nao é respondido as 5 perguntas
      cont++;
      if (estado == 0) return; //caso o botao de desistencia for pressionado
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
      
      for (int pos = 0; pos < strlen(bancodedados_perguntas[num]) - 16; pos++) { //loop destinado para a funcao scrollDisplayLeft (move os caracteres da direita para esquerda, em casos de perguntas longas)
        if (estado == 0) return; //caso o botao de desistencia for pressionado
        lcd.scrollDisplayLeft();  
        delay(300);
      }
      lcd.clear();
      lcd.setCursor(3, 0);
      lcd.print("Questao ");
      lcd.print(cont);
      Serial.println(bancodedados_respostas[num]); //mostrar a resposta da pergunta no serial (p/ auxiliar nos testes)
      lcd.setCursor(3, 1);
      lcd.print("Sim / Nao");
      delay(2000);
      unsigned long tempo = millis(); //define o tempo no momento atual desde que o programa foi iniciado
      int resp = 0; //variavel para definir se o usuario respondeu ou nao

      if (estado == 0) return; //caso o botao de desistencia for pressionado
      lcd.clear();

      while((millis() - tempo) < 10000) { //enquanto a diferenca entre o momento atual e a variavel tempo estiver dentro de 10 segundos
        delay(2);
        lcd.setCursor(3, 0);
        lcd.print("Tempo: ");
        lcd.print((tempo + 10000 - millis())/1000 + 1); //mostra o tempo no lcd em contagem regressiva
        if(((tempo + 10000 - millis())/1000 + 1) <= 3){ //faz o som de "tempo esgotando" quando o tempo atingir 3 segundos restantes
          som(4);
        }
        if(((tempo + 10000 - millis()) + 1000) < 10000 ){
          lcd.setCursor(11,0);
          lcd.print(" ");
        }
        lcd.setCursor(3, 1);
        lcd.print("Sim / Nao");

        if(digitalRead(pin_botaoE) == LOW) { //caso o botao da esquerda for pressionado, é constado que o usuario respondeu "SIM"
          resp = 1; //a variavel define que o usuario respondeu
          resposta = true; //resposta "SIM"
          break;
        } else if (digitalRead(pin_botaoD) == LOW) { //caso o botao da direita for pressionado, é constado que o usuario respondeu "NAO"
          resp = 1; //a variavel define que o usuario respondeu
          resposta = false; //resposta "NAO"
          break;
        }
      }
      
      if(resp == 0) { //caso o usuario nao responda dentro do tempo
        if (!pulou) { //se a opcao de pular a questao ainda nao foi utilizada
          i--; //volta a contagem e a pergunta
          cont--;
          som(0); //som de vitoria
          int num2 = random(13); //gera uma nova pergunta
          num_perguntas[i] = num2; //armazena no array no lugar da pergunta pulada
          pulou = true;
          lcd.clear();
          lcd.setCursor(4, 0);
          lcd.print("Pulando...");
          delay(1500);
        } else { //caso a opcao de pular a questao ja foi utilizada
          som(1); //som de derrota
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
     
      if(bancodedados_respostas[num] == resposta) { //confere se a resposta do usuario é igual a do banco de perguntas e respostas
        if(bancodedados_respostas[num] == true) { //se a resposta correta for "SIM"
          som(0); //som de vitoria
          respostas_corretas++;
          lcd.clear();
          lcd.setCursor(0, 0);
          lcd.print("Resposta Correta: ");
          lcd.setCursor(2, 1);
          lcd.print("*Sim / Nao");
          delay(1000); 
        } else { //se a resposta correta for "NAO"
          som(0); //som de vitoria
          respostas_corretas++;
          lcd.clear();
          lcd.setCursor(0, 0);
          lcd.print("Resposta Correta: ");
          lcd.setCursor(2, 1);
          lcd.print("Sim / *Nao");
          delay(1000);
        }
      } else { //se a resposta do usuario estiver incorreta
        if(bancodedados_respostas[num] == true) { //se a resposta correta for "SIM"
          som(1); //som de derrota
          lcd.clear();
          lcd.setCursor(0, 0);
          lcd.print("Resposta Errada: ");
          lcd.setCursor(2, 1);
          lcd.print("*Sim / Nao");
          delay(1000);
          estado = 0;
          return;
        } else { //se a resposta correta for "NAO"
          som(1); //som de derrota
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
      if(respostas_corretas == 5){ //caso o usuario respondeu todas as questoes corretamente
        estado = 3; //altera o estado para 3 (3ª fase)
        return;
      }
      lcd.clear();
    }
  }
}

//---------------- PERGUNTA PARTE 3 -----------------

const char* pergunta_final_texto = "Voce gostou do jogo?"; //pergunta final

//---------------- RESPOSTA PARTE 3 -----------------

bool resposta_final = true; //resposta para a pergunta final

//------------------------ PARTE 3 ----------------------------

void pergunta_final(){ //funcao para a pergunta final
  if (estado == 0) return; //caso o botao de desistencia for pressionado
  if (estado == 3){ //caso o estado ainda esteja em 3
    bool resposta = false;
    lcd.clear();
    lcd.setCursor(4, 0);
    lcd.print("Pergunta");
    lcd.setCursor(5, 1);
    lcd.print("Final");
    delay(2000);
    lcd.clear();
    if (estado == 0) return; //caso o botao de desistencia for pressionado
    
    if (estado == 0) return; //caso o botao de desistencia for pressionado
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
    
    for (int pos = 0; pos < strlen(pergunta_final_texto) - 16; pos++) { //loop destinado para a funcao scrollDisplayLeft (move os caracteres da direita para esquerda, em casos de perguntas longas)
      if (estado == 0) return; //caso o botao de desistencia for pressionado
      lcd.scrollDisplayLeft();  
      delay(500);
    }
    
    lcd.clear();
    lcd.setCursor(1, 0);
    lcd.print("Questao Final");
    lcd.setCursor(3, 1);
    lcd.print("Sim / Nao");
    delay(2000);
    unsigned long tempo = millis(); //define o tempo no momento atual

    if (estado == 0) return; //caso o botao de desistencia for pressionado
    lcd.clear();
    
    while((millis() - tempo) < 10000) { //enquanto a diferença do tempo no momento atual para a variavel 'tempo' é menor que 10 segundos
      delay(2);
      lcd.setCursor(3, 0);
      lcd.print("Tempo: ");
      lcd.print((tempo + 10000 - millis())/1000 + 1);
      if(((tempo + 10000 - millis()) + 1000) < 10000 ){
        lcd.setCursor(11,0);
        lcd.print(" ");
      }
      if(((tempo + 10000 - millis())/1000 + 1) <= 3){ //faz o som de "tempo esgotando" quando o tempo atingir 3 segundos restantes
          som(4);
      }
      lcd.setCursor(3, 1);
      lcd.print("Sim / Nao");
  
      if(digitalRead(pin_botaoE) == LOW) { //caso o usuario responda "SIM"
        resposta_final = true; //define a resposta como "SIM"
        resp = 1; //define que o usuario respondeu a pergunta
        break;
      } else if (digitalRead(pin_botaoD) == LOW) { //caso o usuario responda "NAO"
        resposta_final = false; //define a resposta como "NAO"
        resp = 1; //define que o usuario respondeu a pergunta
        break;
      }
    }
    
    if(resposta_final == true){ //caso o usuario respondeu corretamente, ele vence o jogo
      lcd.clear();
      lcd.setCursor(4, 0);
      lcd.print("PARABENS!");
      lcd.setCursor(3, 1);
      lcd.print("VOCE VENCEU!");
      som(2); // som de vitoria
      delay(4000);
      lcd.clear();
      estado = 0;
      return;
    } else{ //caso o usuario respondeu incorretamente, ele perde o jogo
      som(1); //som de derrota
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

void som(int sons) { //funcao para cada som
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

void resetar_jogo() { //reseta todas as variaveis e arrays
  cont = 0;
  for (int i = 0; i < 10; i++) {
    sequencia[i] = 0;
    sequencia_botao[i] = 0;
  }
  lcd.clear();
}
