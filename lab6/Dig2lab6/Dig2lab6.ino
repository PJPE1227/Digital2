#include <SPI.h>
#include <SD.h>

File myFile0;
File myFile1;
File myFile2;
File myFile3;
File myFile4;

char string[1024];
char dibujo[1024];
String draw;
char byteRead;
int receivedInt;;
int cont = 0;

int contador = 0;
int key = 0;

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  while(!Serial){}
  SPI.setModule(0);

  Serial.println(' ');
  Serial.print("Initializing SD card...");
  pinMode(12, OUTPUT);

  if(!SD.begin(12)){
    Serial.println("initialization failed!");
    return;  
  }
  Serial.println("initialization done.");
  
  Serial.println("----------------------");
  Serial.println("Eliga entre:  ");
  Serial.println("1. mostrar Imagen1 = a");
  Serial.println("2. mostrar Imagen2 = b");
  Serial.println("3. mostrar Imagen3 = c");
  Serial.println("4. crear Imagen4 = d  ");
  Serial.println("5. mostrar Imagen4 = e");
  Serial.println("6. borrar Imagen4 = f ");
  Serial.println("----------------------");

  
}

void loop() {
  // put your main code here, to run repeatedly: 
  menu();
  picked();
  delay(500);

}

void menu(){
  if(Serial.available() != 0){
    int availableBytes = Serial.available();
    for(int i = 0; i < availableBytes; i++){
      string[i] = Serial.read();  
    }  
    cont = 1;  
    if(string[0] == '7' || string[0] == 10){
      cont = 0;
    }
  }
  
}

void picked(){
  if(cont == 1){
    Serial.println("Usted eligio: ");
    if(string[0] == 'a'){
      Serial.println("Imagen1 : ");  
      myFile0 = SD.open("ghost1.txt");
      if(myFile0){
        Serial.println("ghost1.txt");
        while(myFile0.available()){
          Serial.write(myFile0.read());  
        }  
        myFile0.close();
      }
    }else if(string[0] == 'b'){
      Serial.println("Imagen2 : ");
      myFile1 = SD.open("skull1.txt");
      if(myFile1){
        Serial.println("skull1.txt");
        while(myFile1.available()){
          Serial.write(myFile1.read());  
        }  
        myFile1.close();
      }
    }else if(string[0] == 'c'){
      Serial.println("Imagen3 : ");
      myFile2 = SD.open("fire1.txt");
      if(myFile2){
        Serial.println("fire1.txt");
        while(myFile2.available()){
          Serial.write(myFile2.read());  
        }  
        myFile2.close();
      }
    }else if(string[0] == 'd'){
      if(SD.exists("imagen4.txt")){
        Serial.println("La imagen4 ya esiste, borrela para crear una nueva");
      }else{
        Serial.println("creando imagen4.txt...");
        myFile3 = SD.open("imagen4.txt", FILE_WRITE);
        if(myFile3){
          Serial.println("Ingrese su imagen ascii y presione 7 cuando este terminado: ");  
          char c;  
          while(true){
            if(Serial.available()){
              c = Serial.read();
              if(c == '7'){
                break;  
              }  
              draw += c;
            }  
          }
          myFile3.println(draw);
          myFile3.close();
        }
        myFile3.close();
      }
    }else if(string[0] == 'e'){
      if(SD.exists("imagen4.txt")){
        Serial.println("imagen4 : ");
        myFile4 = SD.open("imagen4.txt");
        if(myFile4){
          while(myFile4.available()){
            Serial.write(myFile4.read());  
          }  
          myFile4.close();
        }
      }else{
        Serial.println("imagen4.txt no existe");  
      }
    }else if(string[0] == 'f'){
      if(SD.exists("imagen4.txt")){
        Serial.print("borrando imagen4.txt....");
        SD.remove("imagen4.txt");
        Serial.println("imagen4.txt se ha borrado");
      }else{
        Serial.println("imagen4.txt no existe...");  
      }
    }else{
      Serial.println("Opcion invalida");
    }
    for(int i = 0; i < 1024; i++){
      string[i] = 0;
    }
    contador = 0;
    draw = "";
    Serial.println(" ");
    Serial.println("----------------------");
    Serial.println("Eliga entre:  ");
    Serial.println("1. mostrar Imagen1 = a");
    Serial.println("2. mostrar Imagen2 = b");
    Serial.println("3. mostrar Imagen3 = c");
    Serial.println("4. crear Imagen4 = d  ");
    Serial.println("5. mostrar Imagen4 = e");
    Serial.println("6. borrar Imagen4 = f ");
    Serial.println("----------------------");
    cont = 0;
  } 
}
