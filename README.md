# Jogo da Memória em C++

## Introdução
-Desenvolvimento de um projeto de Arduino, implementado em C++ para criação de um jogo com três etapas, Fase da Memória, Fase das perguntas e Fase da Pergunta Final. O jogo testará a memória do jogador, além do conhecimento acerca dos tópicos passados nas aulas de Computação Móvel. Tendo como objetivo, a construção lógica e física do jogo, utilizando os recursos oferecidos pelo Tinkercad e, posteriormente, na implementação material no Arduino.

## Metodologia
-Inicialmente, para a aplicação do jogo foi utilizado o Tinkercad, para fazer a esquematização do Arduino e elaboração do código. Em termos de Arduino, foram usados: 1 Arduino UNO, 1 LED vermelho, 1 LED verde, 3 Botões, 3 Resistores, 1 Buzzer, 1 Display LCD 16x2 e 1 Potenciômetro. O projeto foi desenvolvido em etapas. A primeira consistiu na montagem do circuito com os componentes necessários. Em seguida, foi aplicada a lógica de funcionamento do jogo no sistema implementado. Por fim, todo o conjunto foi posto em prática e o jogo foi testado no Arduino físico. Em relação ao código comentado, é possível ver no arquivo [projetoJogo.cpp](https://github.com/EricSongWatanabe/projeto1/blob/main/projetoJogo.cpp).

## Experimentos
-Circuito no Tinkercad
![experimento1](https://github.com/user-attachments/assets/771ec08f-19e1-4e99-8985-8406f09841d0)

-Circuito no Arduino
![experimento 2](https://github.com/user-attachments/assets/3f41322d-419c-4860-9666-85b6b3693c45)

![experimento3](https://github.com/user-attachments/assets/4483eb64-b146-4339-a993-731adf9f8242)

-Video do funcionamento
https://github.com/user-attachments/assets/a993ef00-449d-4347-8919-aa198c56afcb

## Conclusão
-Durante a realização do jogo, enfrentamos diversos problemas com o código, o principal deles foi com relação ao delay dentro do loop do cronômetro, na fase de perguntas, em que foi preciso utilizar a função millis para fazer a contagem regressiva do tempo. Além disso, também tivemos alguns problemas na parte prática, ou seja, aplicação no Arduino, onde problemas que antes não ocorriam no Tinkercad começaram a acontecer. Nestes casos tentamos adaptar o programa para resolvê-los ou minimizá-los, realizando testes constantes e sempre buscando evitar erros futuros. Já em relação ao esquema físico, foi preciso a verificação de cada componente e local onde foi posicionado para fazer o projeto funcionar.
