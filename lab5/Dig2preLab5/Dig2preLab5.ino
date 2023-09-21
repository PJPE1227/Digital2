#define LED0 RED_LED
#define LED1 GREEN_LED
#define LED2 BLUE_LED

int start = 0;
int P1 = 0;
int flag1 = 0;
int cont1 = 0;
int P2 = 0;
int cont2 = 0;
int flag2 = 0;
int key1 = 0;
int key2 = 0;

void setup() {
  pinMode(LED0, OUTPUT);
  pinMode(LED1, OUTPUT);
  pinMode(LED2, OUTPUT);

  pinMode(3, OUTPUT);
  pinMode(4, OUTPUT);
  pinMode(5, OUTPUT);
  pinMode(6, OUTPUT);
  pinMode(7, OUTPUT);
  pinMode(8, OUTPUT);
  pinMode(9, OUTPUT);
  pinMode(10, OUTPUT);

  pinMode(23, OUTPUT);
  pinMode(24, OUTPUT);
  pinMode(25, OUTPUT);
  pinMode(26, OUTPUT);
  pinMode(27, OUTPUT);
  pinMode(28, OUTPUT);
  pinMode(29, OUTPUT);
  pinMode(2, OUTPUT);
  
  pinMode(PUSH2, INPUT_PULLUP);
  pinMode(PUSH1, INPUT_PULLUP);
  pinMode(19, INPUT_PULLUP);
}

void loop() {
  start = digitalRead(PUSH1);
  P1 = digitalRead(PUSH2);
  P2 = digitalRead(19);
  if(P1 == 0){
      flag1 = 1;
    }
  if(P2 == 0){
      flag2 = 1;
    }
   if(flag1 == 1 && P1 == 1){
      cont1++;
      flag1 = 0;
    }
    if(flag2 == 1 && P2 == 1){
      cont2++;
      flag2 = 0;
    }
    if(cont1 >= 8){
      cont1 = 8;  
    }
    if(cont2 >= 8){
      cont2 = 8;  
    }
    if(cont1 >= 8){
      cont2 = 0;  
      digitalWrite(LED0, HIGH);
      delay(10);
      digitalWrite(LED2, HIGH);
    }
    if(cont2 >= 8){
      cont1 = 0; 
      digitalWrite(LED1, HIGH);
      delay(10);
      digitalWrite(LED2, HIGH); 
    }
  semaforo();
  switch(cont1){
    case 1:
      digitalWrite(3, HIGH);
      digitalWrite(4, LOW);
      digitalWrite(5, LOW);
      digitalWrite(6, LOW);
      digitalWrite(7, LOW);
      digitalWrite(8, LOW);
      digitalWrite(9, LOW);
      digitalWrite(10, LOW);
      break;
    case 2:
      digitalWrite(3, LOW);
      digitalWrite(4, HIGH);
      digitalWrite(5, LOW);
      digitalWrite(6, LOW);
      digitalWrite(7, LOW);
      digitalWrite(8, LOW);
      digitalWrite(9, LOW);
      digitalWrite(10, LOW);
      break;
    case 3:
      digitalWrite(3, LOW);
      digitalWrite(4, LOW);
      digitalWrite(5, HIGH);
      digitalWrite(6, LOW);
      digitalWrite(7, LOW);
      digitalWrite(8, LOW);
      digitalWrite(9, LOW);
      digitalWrite(10, LOW);
      break;
    case 4:
      digitalWrite(3, LOW);
      digitalWrite(4, LOW);
      digitalWrite(5, LOW);
      digitalWrite(6, HIGH);
      digitalWrite(7, LOW);
      digitalWrite(8, LOW);
      digitalWrite(9, LOW);
      digitalWrite(10, LOW);
      break;
    case 5:
      digitalWrite(3, LOW);
      digitalWrite(4, LOW);
      digitalWrite(5, LOW);
      digitalWrite(6, LOW);
      digitalWrite(7, HIGH);
      digitalWrite(8, LOW);
      digitalWrite(9, LOW);
      digitalWrite(10, LOW);
      break;
    case 6:
      digitalWrite(3, LOW);
      digitalWrite(4, LOW);
      digitalWrite(5, LOW);
      digitalWrite(6, LOW);
      digitalWrite(7, LOW);
      digitalWrite(8, HIGH);
      digitalWrite(9, LOW);
      digitalWrite(10, LOW);
      break;
    case 7:
      digitalWrite(3, LOW);
      digitalWrite(4, LOW);
      digitalWrite(5, LOW);
      digitalWrite(6, LOW);
      digitalWrite(7, LOW);
      digitalWrite(8, LOW);
      digitalWrite(9, HIGH);
      digitalWrite(10, LOW);
      break;
    case 8:
      digitalWrite(3, LOW);
      digitalWrite(4, LOW);
      digitalWrite(5, LOW);
      digitalWrite(6, LOW);
      digitalWrite(7, LOW);
      digitalWrite(8, LOW);
      digitalWrite(9, LOW);
      digitalWrite(10, HIGH);
      break;
    default:
      digitalWrite(3, LOW);
      digitalWrite(4, LOW);
      digitalWrite(5, LOW);
      digitalWrite(6, LOW);
      digitalWrite(7, LOW);
      digitalWrite(8, LOW);
      digitalWrite(9, LOW);
      digitalWrite(10, LOW);
      break;
  }
  switch(cont2){
    case 1:
      digitalWrite(2, HIGH);
      digitalWrite(23, LOW);
      digitalWrite(24, LOW);
      digitalWrite(25, LOW);
      digitalWrite(26, LOW);
      digitalWrite(27, LOW);
      digitalWrite(28, LOW);
      digitalWrite(29, LOW);
      break;
    case 2:
      digitalWrite(2, LOW);
      digitalWrite(23, HIGH);
      digitalWrite(24, LOW);
      digitalWrite(25, LOW);
      digitalWrite(26, LOW);
      digitalWrite(27, LOW);
      digitalWrite(28, LOW);
      digitalWrite(29, LOW);
      break;
    case 3:
      digitalWrite(2, LOW);
      digitalWrite(23, LOW);
      digitalWrite(24, HIGH);
      digitalWrite(25, LOW);
      digitalWrite(26, LOW);
      digitalWrite(27, LOW);
      digitalWrite(28, LOW);
      digitalWrite(29, LOW);
      break;
    case 4:
      digitalWrite(2, LOW);
      digitalWrite(23, LOW);
      digitalWrite(24, LOW);
      digitalWrite(25, HIGH);
      digitalWrite(26, LOW);
      digitalWrite(27, LOW);
      digitalWrite(28, LOW);
      digitalWrite(29, LOW);
      break;
    case 5:
      digitalWrite(2, LOW);
      digitalWrite(23, LOW);
      digitalWrite(24, LOW);
      digitalWrite(25, LOW);
      digitalWrite(26, HIGH);
      digitalWrite(27, LOW);
      digitalWrite(28, LOW);
      digitalWrite(29, LOW);
      break;
    case 6:
      digitalWrite(2, LOW);
      digitalWrite(23, LOW);
      digitalWrite(24, LOW);
      digitalWrite(25, LOW);
      digitalWrite(26, LOW);
      digitalWrite(27, HIGH);
      digitalWrite(28, LOW);
      digitalWrite(29, LOW);
      break;
    case 7:
      digitalWrite(2, LOW);
      digitalWrite(23, LOW);
      digitalWrite(24, LOW);
      digitalWrite(25, LOW);
      digitalWrite(26, LOW);
      digitalWrite(27, LOW);
      digitalWrite(28, HIGH);
      digitalWrite(29, LOW);
      break;
    case 8:
      digitalWrite(2, LOW);
      digitalWrite(23, LOW);
      digitalWrite(24, LOW);
      digitalWrite(25, LOW);
      digitalWrite(26, LOW);
      digitalWrite(27, LOW);
      digitalWrite(28, LOW);
      digitalWrite(29, HIGH);
      break;
    default:
      digitalWrite(2, LOW);
      digitalWrite(23, LOW);
      digitalWrite(24, LOW);
      digitalWrite(25, LOW);
      digitalWrite(26, LOW);
      digitalWrite(27, LOW);
      digitalWrite(28, LOW);
      digitalWrite(29, LOW);
      break;
  }
}

void semaforo(){
    if(start == 0){
      digitalWrite(LED0, LOW);
      digitalWrite(LED1, LOW);
      digitalWrite(LED2, LOW);
      cont1 = 0;
      cont2 = 0;
      digitalWrite(LED0, HIGH);
      digitalWrite(LED1, LOW);
      delay(1000);
      digitalWrite(LED0, HIGH);
      digitalWrite(LED1, HIGH);
      delay(1000);
      digitalWrite(LED0, LOW);
      digitalWrite(LED1, HIGH);
      delay(1000);
    }else{
        digitalWrite(LED0, LOW);
        digitalWrite(LED1, LOW);
      }
  }
