# SegurancaAutomotiva
Projeto de conclusão de um curso Técnico de Automação Industrial sobre Segurança Automotiva do IFRJ *campus* Volta Redonda.

## Motivação
A motivação para o projeto foi a crescente no número de acidentes devido a desatenção no trânsito, comumente causada pelo uso indevido de celulares. 

## O Projeto
O projeto teve como seu objetivo principal aumentar a atenção do motorista durante a condução. Os objetivos secundários foram ler com sucesso os dados da rede CAN do carro, visualizar parte dos dados através de um aplicativo Android e também permitir ao motorista que conseguisse fazer uma ligação de emergência através do acionamento de um botão ou automaticamente através do acionamento do Airbag.


## Principais Materiais
- [Arduino Mega]
- [ESP32 WROOM]
- [GSM/GPRS SIM900 SHIELD]
- [CAN BUS 2.0 SHIELD]
- [Android Studio]

Além dos principais materiais, se utilizou um dispositivo chamado canalyzer para simular as tramas da Rede CAN de um carro qualquer. 

## Objetivo Deste Repositório
Este repositório tem a função de armazenar o código do projeto e disponibilizar de forma gratuita nosso código. Se você deseja saber mais do projeto recomendo que **leia o nosso artigo**, que também está neste repositório!

## Integrantes do Grupo
Os integrantes do grupo também estão listados no nosso artigo, bem como as informações acadêmicas, informações de contato e nome do nosso orientador.

## Detalhes Finais
Recomendamos que se utilize as bibliotecas no arquivo "Bibliotecas.zip", principalmente na biblioteca do Módulo CAN, visto que foram feitas alterações diretamente no código da biblioteca.O código "Final.ino" é o código do Arduino Mega sem comentários, enquanto o código "MonitoramentoSeguranca_Central.ino" é o código do Arduino Mega comentado. O código do ESP32 comunicando com o Arduino é o "esp32toArduinoBuffer.ino". Qualquer necessidade de contato com o grupo pode me contactar através do github.