//***************************************************************************************************************************************
/* Librería para el uso de la pantalla ILI9341 en modo 8 bits
 * Basado en el código de martinayotte - https://www.stm32duino.com/viewtopic.php?t=637
 * Adaptación, migración y creación de nuevas funciones: Pablo Mazariegos y José Morales
 * Con ayuda de: José Guerra
 * IE3027: Electrónica Digital 2 - 2019
 */
//***************************************************************************************************************************************
#include <stdint.h>
#include <stdbool.h>
#include <TM4C123GH6PM.h>
#include "inc/hw_ints.h"
#include "inc/hw_memmap.h"
#include "inc/hw_types.h"
#include "driverlib/debug.h"
#include "driverlib/gpio.h"
#include "driverlib/interrupt.h"
#include "driverlib/rom_map.h"
#include "driverlib/rom.h"
#include "driverlib/sysctl.h"
#include "driverlib/timer.h"

#include "bitmaps.h"
#include "font.h"
#include "lcd_registers.h"

#define LCD_RST PD_0
#define LCD_CS PD_1
#define LCD_RS PD_2
#define LCD_WR PD_3
#define LCD_RD PE_1
int DPINS[] = {PB_0, PB_1, PB_2, PB_3, PB_4, PB_5, PB_6, PB_7};  
//***************************************************************************************************************************************
// Functions Prototypes
//***************************************************************************************************************************************
void LCD_Init(void);
void LCD_CMD(uint8_t cmd);
void LCD_DATA(uint8_t data);
void SetWindows(unsigned int x1, unsigned int y1, unsigned int x2, unsigned int y2);
void LCD_Clear(unsigned int c);
void H_line(unsigned int x, unsigned int y, unsigned int l, unsigned int c);
void V_line(unsigned int x, unsigned int y, unsigned int l, unsigned int c);
void Rect(unsigned int x, unsigned int y, unsigned int w, unsigned int h, unsigned int c);
void FillRect(unsigned int x, unsigned int y, unsigned int w, unsigned int h, unsigned int c);
void LCD_Print(String text, int x, int y, int fontSize, int color, int background);

void LCD_Bitmap(unsigned int x, unsigned int y, unsigned int width, unsigned int height, unsigned char bitmap[]);
void LCD_Sprite(int x, int y, int width, int height, unsigned char bitmap[],int columns, int index, char flip, char offset);
int ascii2hex(int a);
void mapeo_SD(char doc[], int x, int y, int w);

extern uint8_t gameOver[];
extern uint8_t test0[];
extern uint8_t menu[];

extern uint8_t tankP2Forward[];
extern uint8_t tankP2Backward[];
extern uint8_t tankP2LR[];

int enemy1Life = 0;
int enemy1x = 266;
int enemy1y = 0;
int enemy1Pace = 0;
int xEne1Bullet = 0;
int yEne1Bullet = 0;
int enemy1BulletPace = 0;

int enemy2Life = 1;
int enemy2x = 74;
int enemy2y = 0;
int enemy2Pace = 0;
int xEne2Bullet = 0;
int yEne2Bullet = 0;
int enemy2BulletPace = 0;

int enemy3Life = 1;
int enemy3x = 170;
int enemy3y = 0;
int enemy3Pace = 0;
int xEne3Bullet = 0;
int yEne3Bullet = 0;
int enemy3BulletPace = 0;

int enemy4Life = 1;
int enemy4x = 26;
int enemy4y = 0;
int enemy4Pace = 0;
int xEne4Bullet = 0;
int yEne4Bullet = 0;
int enemy4BulletPace = 0;

int enemy5Life = 1;
int enemy5x = 122;
int enemy5y = 0;
int enemy5Pace = 0;
int xEne5Bullet = 0;
int yEne5Bullet = 0;
int enemy5BulletPace = 0;

int enemy6Life = 1;
int enemy6x = 218;
int enemy6y = 0;
int enemy6Pace = 0;
int xEne6Bullet = 0;
int yEne6Bullet = 0;
int enemy6BulletPace = 0;

int tripleWave1 = 0;

int enemy7Life = 1;
int enemy7x = 74;
int enemy7y = 0;
int enemy7Pace = 0;
int xEne7Bullet = 0;
int yEne7Bullet = 0;
int enemy7BulletPace = 0;

int enemy8Life = 1;
int enemy8x = 170;
int enemy8y = 0;
int enemy8Pace = 0;
int xEne8Bullet = 0;
int yEne8Bullet = 0;
int enemy8BulletPace = 0;

int enemy9Life = 1;
int enemy9x = 266;
int enemy9y = 0;
int enemy9Pace = 0;
int xEne9Bullet = 0;
int yEne9Bullet = 0;
int enemy9BulletPace = 0;

int tripleWave2 = 0;

int enemy10Life = 1;
int enemy10x = 26;
int enemy10y = 0;
int enemy10Pace = 0;
int xEne10Bullet = 0;
int yEne10Bullet = 0;
int enemy10BulletPace = 0;

int enemy11Life = 1;
int enemy11x = 122;
int enemy11y = 0;
int enemy11Pace = 0;
int xEne11Bullet = 0;
int yEne11Bullet = 0;
int enemy11BulletPace = 0;

int enemy12Life = 1;
int enemy12x = 170;
int enemy12y = 0;
int enemy12Pace = 0;
int xEne12Bullet = 0;
int yEne12Bullet = 0;
int enemy12BulletPace = 0;

int enemy13Life = 1;
int enemy13x = 266;
int enemy13y = 0;
int enemy13Pace = 0;
int xEne13Bullet = 0;
int yEne13Bullet = 0;
int enemy13BulletPace = 0;

int tripleWave3 = 0;

int enemy14Life = 1;
int enemy14x = 26;
int enemy14y = 0;
int enemy14Pace = 0;
int xEne14Bullet = 0;
int yEne14Bullet = 0;
int enemy14BulletPace = 0;

int enemy15Life = 1;
int enemy15x = 74;
int enemy15y = 0;
int enemy15Pace = 0;
int xEne15Bullet = 0;
int yEne15Bullet = 0;
int enemy15BulletPace = 0;

int enemy16Life = 1;
int enemy16x = 218;
int enemy16y = 0;
int enemy16Pace = 0;
int xEne16Bullet = 0;
int yEne16Bullet = 0;
int enemy16BulletPace = 0;

int enemy17Life = 1;
int enemy17x = 266;
int enemy17y = 0;
int enemy17Pace = 0;
int xEne17Bullet = 0;
int yEne17Bullet = 0;
int enemy17BulletPace = 0;

int tripleWave4 = 0;

int enemy18Life = 1;
int enemy18x = 26;
int enemy18y = 0;
int enemy18Pace = 0;
int xEne18Bullet = 0;
int yEne18Bullet = 0;
int enemy18BulletPace = 0;

int enemy19Life = 1;
int enemy19x = 74;
int enemy19y = 0;
int enemy19Pace = 0;
int xEne19Bullet = 0;
int yEne19Bullet = 0;
int enemy19BulletPace = 0;

int enemy20Life = 1;
int enemy20x = 122;
int enemy20y = 0;
int enemy20Pace = 0;
int xEne20Bullet = 0;
int yEne20Bullet = 0;
int enemy20BulletPace = 0;

int enemy21Life = 1;
int enemy21x = 170;
int enemy21y = 0;
int enemy21Pace = 0;
int xEne21Bullet = 0;
int yEne21Bullet = 0;
int enemy21BulletPace = 0;

int enemy22Life = 1;
int enemy22x = 218;
int enemy22y = 0;
int enemy22Pace = 0;
int xEne22Bullet = 0;
int yEne22Bullet = 0;
int enemy22BulletPace = 0;

int enemy23Life = 1;
int enemy23x = 266;
int enemy23y = 0;
int enemy23Pace = 0;
int xEne23Bullet = 0;
int yEne23Bullet = 0;
int enemy23BulletPace = 0;

int tripleWave5 = 0;

int xP1 = 0;
int yP1 = 209;
int P1facing = 0;
int triggerP1 = 0;
int P1bullDirection = 0;
int xP1bullet = 0;
int yP1bullet = 0;
int P1Life = 0;

int xP2 = 290;
int yP2 = 209;
int P2facing = 0;
int triggerP2 = 0;
int P2bullDirection = 0;
int xP2bullet = 0;
int yP2bullet = 0;
int P2Life = 0;

String text1 = "VICTORY";
String text2 = "1 player";
String text3 = "2 players";

int state = 0;
int selection = 0;
int turn = 0;

#include <SPI.h>
#include <SD.h>

File myFile;
//***************************************************************************************************************************************
// Inicialización
//***************************************************************************************************************************************
void setup() {
  SPI.setModule(0);
  SysCtlClockSet(SYSCTL_SYSDIV_2_5|SYSCTL_USE_PLL|SYSCTL_OSC_MAIN|SYSCTL_XTAL_16MHZ);
  Serial.begin(115200);
  GPIOPadConfigSet(GPIO_PORTB_BASE, 0 | 1 | 2 | 3 | 4 | 5 | 6 | 7, GPIO_STRENGTH_8MA, GPIO_PIN_TYPE_STD_WPU);
  Serial.println("Inicio");
  LCD_Init();
  LCD_Clear(0x00);

  Serial.println("inicializando tarjedo SD...");
  pinMode(12, OUTPUT);

  if(!SD.begin(12)){
    Serial.println("initialization failed");
    return;  
  }
  Serial.println("initialization done");
  mapeo_SD("menu.txt", 32, 25, 256);
  //LCD_Bitmap(32,25, 256, 190, menu);
}

//***************************************************************************************************************************************
// Loop Infinito
//***************************************************************************************************************************************
void loop() {
  //int i =30;
  int yPlayer1 = analogRead(PE_2);
  int xPlayer1 = analogRead(PE_3);
  int buttonP1 = analogRead(PE_5);
  
  int yPlayer2 = analogRead(PE_4);
  int xPlayer2 = analogRead(PE_0);
  int buttonP2 = 0;
  
  int animy = (yP1/5)%2;
  int animx = (xP1/5)%2;
  
  int animy2 = (yP2/5)%2;
  int animx2 = (xP2/5)%2;

  if(state == 0){
    if(yPlayer1 > 2300){//down
      LCD_Bitmap(97, 130, 13, 13, pick); 
      selection = 2;
      for(int i = 0; i < 13; i++){
          H_line(97, 115+i, 13, 0x00);
      }
    }
    if(yPlayer1 < 1700){//up
      LCD_Bitmap(97, 115, 13, 13, pick);
      selection = 1;
      for(int i = 0; i < 13; i++){
        H_line(97, 130+i, 13, 0x00);  
      }
    }
    if(buttonP1 < 10 && selection == 1){
      state = 1;  
    }else if(buttonP1 <10 && selection == 2){
      state = 2;  
    } 
  }else if(state == 1){
    if(turn == 0){
      LCD_Clear(0x00); 
      turn = 1; 
    };
    if(buttonP1 < 10 && P1Life == 0){
      if(triggerP1 == 0 && P1facing == 1){//shooting up
        triggerP1 = 1;
        P1bullDirection = 1;
        xP1bullet = xP1 + 9;
        yP1bullet = yP1 - 10;  
      }  
      if(triggerP1 == 0 && P1facing == 4){//shooting right
        triggerP1 = 1;
        P1bullDirection = 2;
        xP1bullet = xP1 + 30;
        yP1bullet = yP1 + 10;  
      } 
      if(triggerP1 == 0 && P1facing == 2){//shooting left
        triggerP1 = 1;
        P1bullDirection = 3;
        xP1bullet = xP1 - 10;
        yP1bullet = yP1 + 10;  
      }
    }
    if(yPlayer1 > 2300 && P1Life == 0){//down
      yP1++;
      P1facing = 3;
      if(yP1 == 211){
        yP1 = 210;
      }
      LCD_Sprite(xP1, yP1, 30, 30, tankP1Backward, 2,animy, 0, 0);
      H_line(xP1, yP1-1, 30, 0x00);
    }
    if(yPlayer1 < 1700 && P1Life == 0){//up 
      yP1--;
      P1facing = 1;
      if(yP1 == -1){
        yP1 = 0;  
      }
      LCD_Sprite(xP1, yP1, 30, 30, tankP1Forward, 2,animy, 0, 0);
      H_line(xP1, yP1+30, 30, 0x00);
    }
    if(xPlayer1 > 2300 && P1Life == 0){//left
      xP1--;
      P1facing = 2;
      if(xP1 == -1){
        xP1 = 0;  
      } 
      LCD_Sprite(xP1, yP1, 30, 30, tankP1LR, 2, animx, 1, 0);
      V_line(xP1+30, yP1, 30, 0x00);
    }
    if(xPlayer1  < 1700 && P1Life == 0){//right
      xP1++;
      P1facing = 4;
      if(xP1 == 291){
        xP1 = 290;  
      }  
      LCD_Sprite(xP1, yP1, 30, 30, tankP1LR, 2, animx, 0, 0);
      V_line(xP1-1, yP1, 30, 0x00);
    }
  
  //**********************************************************************************************************************************
  //Enemy #1 code
  //**********************************************************************************************************************************
    if(enemy1Life == 0){
      if(enemy1y == 0){
        xEne1Bullet = enemy1x + 8;
        yEne1Bullet = enemy1y + 30;
      }
      if(enemy1y == 90){
        xEne1Bullet = enemy1x + 8;
        yEne1Bullet = enemy1y + 30; 
      }
      int animEne1 = (enemy1y/5)%2;
      LCD_Sprite(enemy1x, enemy1y, 28, 30, enemyTankForward, 2, animEne1, 0, 0);
      H_line(enemy1x, enemy1y-1, 28,0x00);
      LCD_Bitmap(xEne1Bullet, yEne1Bullet, 10, 10, bulletDown);
      H_line(xEne1Bullet, yEne1Bullet, 10, 0x00);
      if(enemy1Pace == 0){
        enemy1y++; 
        enemy1Pace = 1;
      }else if(enemy1Pace == 1){
        enemy1Pace = 2;  
        yEne1Bullet++;
        if(yEne1Bullet >= 250){
          yEne1Bullet = 245;  
        }
      }else if(enemy1Pace == 2){
        enemy1Pace = 3;
      }else if(enemy1Pace == 3){
        enemy1Pace = 4;  
        yEne1Bullet++;
        if(yEne1Bullet >= 250){
          yEne1Bullet = 245;  
        }
      }else if(enemy1Pace == 4){
        enemy1Pace =  5;
      }else if(enemy1Pace == 5){
        enemy1Pace =  0;
        yEne1Bullet++;
        if(yEne1Bullet >= 250){
          yEne1Bullet = 245;  
        }
      }
   
      if((enemy1y == 209)||((xP1+29)>enemy1x && (xP1+1)<(enemy1x+28) && yP1<(enemy1y+30) && (yP1+30)>enemy1y)){
        enemy1Life = 1;
        P1Life = 1;
        LCD_Sprite(xP1, yP1, 30, 30, dead, 2, 1, 0, 0);
        mapeo_SD("gameOver.txt", 67, 75, 186);
        delay(3000);
        LCD_Clear(0x00);
        enemy1Life = 0;
        enemy1y = 0;
        enemy2y = 0;
        enemy3y = 0;
        enemy4y = 0;
        enemy5y = 0;
        enemy6y = 0;
        enemy7y = 0;
        enemy8y = 0;
        enemy9y = 0;
        enemy10y = 0;
        enemy11y = 0;
        enemy12y = 0;
        enemy13y = 0;
        enemy14y = 0;
        enemy15y = 0;
        enemy16y = 0;
        enemy17y = 0;
        enemy18y = 0;
        enemy19y = 0;
        enemy20y = 0;
        enemy21y = 0;
        enemy22y = 0;
        enemy23y = 0;
        tripleWave1 = 0;
        tripleWave2 = 0;
        tripleWave3 = 0;
        tripleWave4 = 0;
        tripleWave5 = 0;
        P1Life = 0;
        xP1 = 0;
        yP1 = 209;
        state = 0;
        turn = 0;
        mapeo_SD("menu.txt", 32, 25, 256);
      }
      if((xP1+29)>xEne1Bullet && (xP1+1)<(xEne1Bullet+10) && yP1<(yEne1Bullet+10) && (yP1+30)>yEne1Bullet){
        enemy1Life = 1;
        P1Life = 1;
        LCD_Sprite(xP1, yP1, 30, 30, dead, 2, 1, 0, 0);
        mapeo_SD("gameOver.txt", 67, 75, 186);
        delay(3000);
        LCD_Clear(0x00);
        enemy1Life = 0;
        enemy1y = 0;
        enemy2y = 0;
        enemy3y = 0;
        enemy4y = 0;
        enemy5y = 0;
        enemy6y = 0;
        enemy7y = 0;
        enemy8y = 0;
        enemy9y = 0;
        enemy10y = 0;
        enemy11y = 0;
        enemy12y = 0;
        enemy13y = 0;
        enemy14y = 0;
        enemy15y = 0;
        enemy16y = 0;
        enemy17y = 0;
        enemy18y = 0;
        enemy19y = 0;
        enemy20y = 0;
        enemy21y = 0;
        enemy22y = 0;
        enemy23y = 0;
        tripleWave1 = 0;
        tripleWave2 = 0;
        tripleWave3 = 0;
        tripleWave4 = 0;
        tripleWave5 = 0;
        P1Life = 0;
        xP1 = 0;
        yP1 = 209;
        state = 0;
        turn = 0;
        mapeo_SD("menu.txt", 32, 25, 256);
      }
    }
  //**********************************************************************************************************************************
  //Enemy #1 code
  //**********************************************************************************************************************************
  
  //**********************************************************************************************************************************
  //Enemy #2 code
  //**********************************************************************************************************************************
    if(enemy2Life == 0){
      if(enemy2y == 0){
        xEne2Bullet = enemy2x + 8;
        yEne2Bullet = enemy2y + 30;
      }
      if(enemy2y == 90){
        xEne2Bullet = enemy2x + 8;
        yEne2Bullet = enemy2y + 30; 
      }
      int animEne2 = (enemy2y/5)%2;
      LCD_Sprite(enemy2x, enemy2y, 28, 30, enemyTankForward, 2, animEne2, 0, 0);
      H_line(enemy2x, enemy2y-1, 28,0x00);
      LCD_Bitmap(xEne2Bullet, yEne2Bullet, 10, 10, bulletDown);
      H_line(xEne2Bullet, yEne2Bullet, 10, 0x00);
      if(enemy2Pace == 0){
        enemy2y++; 
        enemy2Pace = 1;
      }else if(enemy2Pace == 1){
        enemy2Pace = 2;  
        yEne2Bullet++;
        if(yEne2Bullet >= 250){
          yEne2Bullet = 245;  
        }
      }else if(enemy2Pace == 2){
        enemy2Pace = 3;
      }else if(enemy2Pace == 3){
        enemy2Pace = 4;  
        yEne2Bullet++;
        if(yEne2Bullet >= 250){
          yEne2Bullet = 245;  
        }
      }else if(enemy2Pace == 4){
        enemy2Pace =  5;
      }else if(enemy2Pace == 5){
        enemy2Pace =  0;
        yEne2Bullet++;
        if(yEne2Bullet >= 250){
          yEne2Bullet = 245;  
        }
      }
   
      if((enemy2y == 209)||((xP1+29)>enemy2x && (xP1+1)<(enemy2x+28) && yP1<(enemy2y+30) && (yP1+30)>enemy2y)){
        enemy2Life = 1;
        P1Life = 1;
        LCD_Sprite(xP1, yP1, 30, 30, dead, 2, 1, 0, 0);
        mapeo_SD("gameOver.txt", 67, 75, 186);
        delay(3000);
        LCD_Clear(0x00);
        enemy1Life = 0;
        enemy1y = 0;
        enemy2y = 0;
        enemy3y = 0;
        enemy4y = 0;
        enemy5y = 0;
        enemy6y = 0;
        enemy7y = 0;
        enemy8y = 0;
        enemy9y = 0;
        enemy10y = 0;
        enemy11y = 0;
        enemy12y = 0;
        enemy13y = 0;
        enemy14y = 0;
        enemy15y = 0;
        enemy16y = 0;
        enemy17y = 0;
        enemy18y = 0;
        enemy19y = 0;
        enemy20y = 0;
        enemy21y = 0;
        enemy22y = 0;
        enemy23y = 0;
        tripleWave1 = 0;
        tripleWave2 = 0;
        tripleWave3 = 0;
        tripleWave4 = 0;
        tripleWave5 = 0;
        P1Life = 0;
        xP1 = 0;
        yP1 = 209;
        state = 0;
        turn = 0;
        mapeo_SD("menu.txt", 32, 25, 256);
      }
      if((xP1+29)>xEne2Bullet && (xP1+1)<(xEne2Bullet+10) && yP1<(yEne2Bullet+10) && (yP1+30)>yEne2Bullet){
        enemy2Life = 1;
        LCD_Sprite(xP1, yP1, 30, 30, dead, 2, 1, 0, 0);
        mapeo_SD("gameOver.txt", 67, 75, 186);
        delay(3000);
        LCD_Clear(0x00);
        enemy1Life = 0;
        enemy1y = 0;
        enemy2y = 0;
        enemy3y = 0;
        enemy4y = 0;
        enemy5y = 0;
        enemy6y = 0;
        enemy7y = 0;
        enemy8y = 0;
        enemy9y = 0;
        enemy10y = 0;
        enemy11y = 0;
        enemy12y = 0;
        enemy13y = 0;
        enemy14y = 0;
        enemy15y = 0;
        enemy16y = 0;
        enemy17y = 0;
        enemy18y = 0;
        enemy19y = 0;
        enemy20y = 0;
        enemy21y = 0;
        enemy22y = 0;
        enemy23y = 0;
        tripleWave1 = 0;
        tripleWave2 = 0;
        tripleWave3 = 0;
        tripleWave4 = 0;
        tripleWave5 = 0;
        P1Life = 0;
        xP1 = 0;
        yP1 = 209;
        state = 0;
        turn = 0;
        mapeo_SD("menu.txt", 32, 25, 256);
      }
    }
  //**********************************************************************************************************************************
  //Enemy #2 code
  //**********************************************************************************************************************************
  
  //**********************************************************************************************************************************
  //Enemy #3 code
  //**********************************************************************************************************************************
    if(enemy3Life == 0){
      if(enemy3y == 0){
        xEne3Bullet = enemy3x + 8;
        yEne3Bullet = enemy3y + 30;
      }
      if(enemy3y == 90){
        xEne3Bullet = enemy3x + 8;
        yEne3Bullet = enemy3y + 30; 
      }
      int animEne3 = (enemy3y/5)%2;
      LCD_Sprite(enemy3x, enemy3y, 28, 30, enemyTankForward, 2, animEne3, 0, 0);
      H_line(enemy3x, enemy3y-1, 28,0x00);
      LCD_Bitmap(xEne3Bullet, yEne3Bullet, 10, 10, bulletDown);
      H_line(xEne3Bullet, yEne3Bullet, 10, 0x00);
      if(enemy3Pace == 0){
        enemy3y++; 
        enemy3Pace = 1;
      }else if(enemy3Pace == 1){
        enemy3Pace = 2;  
        yEne3Bullet++;
        if(yEne3Bullet >= 250){
          yEne3Bullet = 245;  
        }
      }else if(enemy3Pace == 2){
        enemy3Pace = 3;
      }else if(enemy3Pace == 3){
        enemy3Pace = 4;  
        yEne3Bullet++;
        if(yEne3Bullet >= 250){
          yEne3Bullet = 245;  
        }
      }else if(enemy3Pace == 4){
        enemy3Pace =  5;
      }else if(enemy3Pace == 5){
        enemy3Pace =  0;
        yEne3Bullet++;
        if(yEne3Bullet >= 250){
          yEne3Bullet = 245;  
        }
      }
   
      if((enemy3y == 209)||((xP1+29)>enemy3x && (xP1+1)<(enemy3x+28) && yP1<(enemy3y+30) && (yP1+30)>enemy3y)){
        enemy3Life = 1;
        P1Life = 1;
        LCD_Sprite(xP1, yP1, 30, 30, dead, 2, 1, 0, 0);
        mapeo_SD("gameOver.txt", 67, 75, 186);
        delay(3000);
        LCD_Clear(0x00);
        enemy1Life = 0;
        enemy1y = 0;
        enemy2y = 0;
        enemy3y = 0;
        enemy4y = 0;
        enemy5y = 0;
        enemy6y = 0;
        enemy7y = 0;
        enemy8y = 0;
        enemy9y = 0;
        enemy10y = 0;
        enemy11y = 0;
        enemy12y = 0;
        enemy13y = 0;
        enemy14y = 0;
        enemy15y = 0;
        enemy16y = 0;
        enemy17y = 0;
        enemy18y = 0;
        enemy19y = 0;
        enemy20y = 0;
        enemy21y = 0;
        enemy22y = 0;
        enemy23y = 0;
        tripleWave1 = 0;
        tripleWave2 = 0;
        tripleWave3 = 0;
        tripleWave4 = 0;
        tripleWave5 = 0;
        P1Life = 0;
        xP1 = 0;
        yP1 = 209;
        state = 0;
        turn = 0;
        mapeo_SD("menu.txt", 32, 25, 256);
      }
      if((xP1+29)>xEne3Bullet && (xP1+1)<(xEne3Bullet+10) && yP1<(yEne3Bullet+10) && (yP1+30)>yEne3Bullet){
        enemy3Life = 1;
        P1Life = 1;
        LCD_Sprite(xP1, yP1, 30, 30, dead, 2, 1, 0, 0);
        mapeo_SD("gameOver.txt", 67, 75, 186);
        delay(3000);
        LCD_Clear(0x00);
        enemy1Life = 0;
        enemy1y = 0;
        enemy2y = 0;
        enemy3y = 0;
        enemy4y = 0;
        enemy5y = 0;
        enemy6y = 0;
        enemy7y = 0;
        enemy8y = 0;
        enemy9y = 0;
        enemy10y = 0;
        enemy11y = 0;
        enemy12y = 0;
        enemy13y = 0;
        enemy14y = 0;
        enemy15y = 0;
        enemy16y = 0;
        enemy17y = 0;
        enemy18y = 0;
        enemy19y = 0;
        enemy20y = 0;
        enemy21y = 0;
        enemy22y = 0;
        enemy23y = 0;
        tripleWave1 = 0;
        tripleWave2 = 0;
        tripleWave3 = 0;
        tripleWave4 = 0;
        tripleWave5 = 0;
        P1Life = 0;
        xP1 = 0;
        yP1 = 209;
        state = 0;
        turn = 0;
        mapeo_SD("menu.txt", 32, 25, 256);
      }
    }
  //**********************************************************************************************************************************
  //Enemy #3 code
  //**********************************************************************************************************************************
  
  //**********************************************************************************************************************************
  //Enemy #4 code
  //**********************************************************************************************************************************
    if(enemy4Life == 0){
      if(enemy4y == 0){
        xEne4Bullet = enemy4x + 8;
        yEne4Bullet = enemy4y + 30;
      }
      if(enemy4y == 110){
        xEne4Bullet = enemy4x + 8;
        yEne4Bullet = enemy4y + 30; 
      }
      int animEne4 = (enemy4y/5)%2;
      LCD_Sprite(enemy4x, enemy4y, 28, 30, enemyTankForward, 2, animEne4, 0, 0);
      H_line(enemy4x, enemy4y-1, 28,0x00);
      LCD_Bitmap(xEne4Bullet, yEne4Bullet, 10, 10, bulletDown);
      H_line(xEne4Bullet, yEne4Bullet, 10, 0x00);
      if(enemy4Pace == 0){
        enemy4y++; 
        enemy4Pace = 1;
      }else if(enemy4Pace == 1){
        enemy4Pace = 2;  
        //yEne4Bullet++;
        if(yEne4Bullet >= 250){
          yEne4Bullet = 245;  
        }
      }else if(enemy4Pace == 2){
        enemy4y++;
        enemy4Pace = 3;
      }else if(enemy4Pace == 3){
        enemy4Pace = 4;  
        //yEne4Bullet++;
        if(yEne4Bullet >= 250){
          yEne4Bullet = 245;  
        }
      }else if(enemy4Pace == 4){
        enemy4y++;
        enemy4Pace =  5;
      }else if(enemy4Pace == 5){
        enemy4Pace =  0;
        //yEne4Bullet++;
        if(yEne4Bullet >= 250){
          yEne4Bullet = 245;  
        }
      }
      yEne4Bullet++;
      if(yEne4Bullet >=250){
        yEne4Bullet = 245;  
      }
   
      if((enemy4y == 209)||((xP1+29)>enemy4x && (xP1+1)<(enemy4x+28) && yP1<(enemy4y+30) && (yP1+30)>enemy4y)){
        enemy4Life = 1;
        enemy5Life = 1;
        enemy6Life = 1;
        P1Life = 1;
        LCD_Sprite(xP1, yP1, 30, 30, dead, 2, 1, 0, 0);
        mapeo_SD("gameOver.txt", 67, 75, 186);
        delay(3000);
        LCD_Clear(0x00);
        enemy1Life = 0;
        enemy1y = 0;
        enemy2y = 0;
        enemy3y = 0;
        enemy4y = 0;
        enemy5y = 0;
        enemy6y = 0;
        enemy7y = 0;
        enemy8y = 0;
        enemy9y = 0;
        enemy10y = 0;
        enemy11y = 0;
        enemy12y = 0;
        enemy13y = 0;
        enemy14y = 0;
        enemy15y = 0;
        enemy16y = 0;
        enemy17y = 0;
        enemy18y = 0;
        enemy19y = 0;
        enemy20y = 0;
        enemy21y = 0;
        enemy22y = 0;
        enemy23y = 0;
        tripleWave1 = 0;
        tripleWave2 = 0;
        tripleWave3 = 0;
        tripleWave4 = 0;
        tripleWave5 = 0;
        P1Life = 0;
        xP1 = 0;
        yP1 = 209;
        state = 0;
        turn = 0;
        mapeo_SD("menu.txt", 32, 25, 256);
      }
      if((xP1+29)>xEne4Bullet && (xP1+1)<(xEne4Bullet+10) && yP1<(yEne4Bullet+10) && (yP1+30)>yEne4Bullet){
        enemy4Life = 1;
        enemy5Life = 1;
        enemy6Life = 1;
        P1Life = 1;
        LCD_Sprite(xP1, yP1, 30, 30, dead, 2, 1, 0, 0);
        mapeo_SD("gameOver.txt", 67, 75, 186);
        delay(3000);
        LCD_Clear(0x00);
        enemy1Life = 0;
        enemy1y = 0;
        enemy2y = 0;
        enemy3y = 0;
        enemy4y = 0;
        enemy5y = 0;
        enemy6y = 0;
        enemy7y = 0;
        enemy8y = 0;
        enemy9y = 0;
        enemy10y = 0;
        enemy11y = 0;
        enemy12y = 0;
        enemy13y = 0;
        enemy14y = 0;
        enemy15y = 0;
        enemy16y = 0;
        enemy17y = 0;
        enemy18y = 0;
        enemy19y = 0;
        enemy20y = 0;
        enemy21y = 0;
        enemy22y = 0;
        enemy23y = 0;
        tripleWave1 = 0;
        tripleWave2 = 0;
        tripleWave3 = 0;
        tripleWave4 = 0;
        tripleWave5 = 0;
        P1Life = 0;
        xP1 = 0;
        yP1 = 209;
        state = 0;
        turn = 0;
        mapeo_SD("menu.txt", 32, 25, 256);
      }
    }
  //**********************************************************************************************************************************
  //Enemy #4 code
  //**********************************************************************************************************************************
  
  //**********************************************************************************************************************************
  //Enemy #5 code
  //**********************************************************************************************************************************
    if(enemy5Life == 0){
      if(enemy5y == 0){
        xEne5Bullet = enemy5x + 8;
        yEne5Bullet = enemy5y + 30;
      }
      if(enemy5y == 110){
        xEne5Bullet = enemy5x + 8;
        yEne5Bullet = enemy5y + 30; 
      }
      int animEne5 = (enemy5y/5)%2;
      LCD_Sprite(enemy5x, enemy5y, 28, 30, enemyTankForward, 2, animEne5, 0, 0);
      H_line(enemy5x, enemy5y-1, 28,0x00);
      LCD_Bitmap(xEne5Bullet, yEne5Bullet, 10, 10, bulletDown);
      H_line(xEne5Bullet, yEne5Bullet, 10, 0x00);
      if(enemy5Pace == 0){
        enemy5y++; 
        enemy5Pace = 1;
      }else if(enemy5Pace == 1){
        enemy5Pace = 2;  
        //yEne5Bullet++;
        if(yEne5Bullet >= 250){
          yEne5Bullet = 245;  
        }
      }else if(enemy5Pace == 2){
        enemy5y++;
        enemy5Pace = 3;
      }else if(enemy5Pace == 3){
        enemy5Pace = 4;  
        //yEne5Bullet++;
        if(yEne5Bullet >= 250){
          yEne5Bullet = 245;  
        }
      }else if(enemy5Pace == 4){
        enemy5y++;
        enemy5Pace =  5;
      }else if(enemy5Pace == 5){
        enemy5Pace =  0;
        //yEne5Bullet++;
        if(yEne5Bullet >= 250){
          yEne5Bullet = 245;  
        }
      }
      yEne5Bullet++;
      if(yEne5Bullet >=250){
        yEne5Bullet = 245;  
      }
   
      if((enemy5y == 209)||((xP1+29)>enemy5x && (xP1+1)<(enemy5x+28) && yP1<(enemy5y+30) && (yP1+30)>enemy5y)){
        enemy4Life = 1;
        enemy5Life = 1;
        enemy6Life = 1;
        P1Life = 1;
        LCD_Sprite(xP1, yP1, 30, 30, dead, 2, 1, 0, 0);
        mapeo_SD("gameOver.txt", 67, 75, 186);
        delay(3000);
        LCD_Clear(0x00);
        enemy1Life = 0;
        enemy1y = 0;
        enemy2y = 0;
        enemy3y = 0;
        enemy4y = 0;
        enemy5y = 0;
        enemy6y = 0;
        enemy7y = 0;
        enemy8y = 0;
        enemy9y = 0;
        enemy10y = 0;
        enemy11y = 0;
        enemy12y = 0;
        enemy13y = 0;
        enemy14y = 0;
        enemy15y = 0;
        enemy16y = 0;
        enemy17y = 0;
        enemy18y = 0;
        enemy19y = 0;
        enemy20y = 0;
        enemy21y = 0;
        enemy22y = 0;
        enemy23y = 0;
        tripleWave1 = 0;
        tripleWave2 = 0;
        tripleWave3 = 0;
        tripleWave4 = 0;
        tripleWave5 = 0;
        P1Life = 0;
        xP1 = 0;
        yP1 = 209;
        state = 0;
        turn = 0;
        mapeo_SD("menu.txt", 32, 25, 256);
      }
      if((xP1+29)>xEne5Bullet && (xP1+1)<(xEne5Bullet+10) && yP1<(yEne5Bullet+10) && (yP1+30)>yEne5Bullet){
        enemy4Life = 1;
        enemy5Life = 1;
        enemy6Life = 1;
        P1Life = 1;
        LCD_Sprite(xP1, yP1, 30, 30, dead, 2, 1, 0, 0);
        mapeo_SD("gameOver.txt", 67, 75, 186);
        delay(3000);
        LCD_Clear(0x00);
        enemy1Life = 0;
        enemy1y = 0;
        enemy2y = 0;
        enemy3y = 0;
        enemy4y = 0;
        enemy5y = 0;
        enemy6y = 0;
        enemy7y = 0;
        enemy8y = 0;
        enemy9y = 0;
        enemy10y = 0;
        enemy11y = 0;
        enemy12y = 0;
        enemy13y = 0;
        enemy14y = 0;
        enemy15y = 0;
        enemy16y = 0;
        enemy17y = 0;
        enemy18y = 0;
        enemy19y = 0;
        enemy20y = 0;
        enemy21y = 0;
        enemy22y = 0;
        enemy23y = 0;
        tripleWave1 = 0;
        tripleWave2 = 0;
        tripleWave3 = 0;
        tripleWave4 = 0;
        tripleWave5 = 0;
        P1Life = 0;
        xP1 = 0;
        yP1 = 209;
        state = 0;
        turn = 0;
        mapeo_SD("menu.txt", 32, 25, 256);
      }
    }
  //**********************************************************************************************************************************
  //Enemy #5 code
  //**********************************************************************************************************************************
  
  //**********************************************************************************************************************************
  //Enemy #6 code
  //**********************************************************************************************************************************
    if(enemy6Life == 0){
      if(enemy6y == 0){
        xEne6Bullet = enemy6x + 8;
        yEne6Bullet = enemy6y + 30;
      }
      if(enemy6y == 110){
        xEne6Bullet = enemy6x + 8;
        yEne6Bullet = enemy6y + 30; 
      }
      int animEne6 = (enemy6y/5)%2;
      LCD_Sprite(enemy6x, enemy6y, 28, 30, enemyTankForward, 2, animEne6, 0, 0);
      H_line(enemy6x, enemy6y-1, 28,0x00);
      LCD_Bitmap(xEne6Bullet, yEne6Bullet, 10, 10, bulletDown);
      H_line(xEne6Bullet, yEne6Bullet, 10, 0x00);
      if(enemy6Pace == 0){
        enemy6y++; 
        enemy6Pace = 1;
      }else if(enemy6Pace == 1){
        enemy6Pace = 2;  
        //yEne6Bullet++;
        if(yEne6Bullet >= 250){
          yEne6Bullet = 245;  
        }
      }else if(enemy6Pace == 2){
        enemy6y++;
        enemy6Pace = 3;
      }else if(enemy6Pace == 3){
        enemy6Pace = 4;  
        //yEne6Bullet++;
        if(yEne6Bullet >= 250){
          yEne6Bullet = 245;  
        }
      }else if(enemy6Pace == 4){
        enemy6y++;
        enemy6Pace =  5;
      }else if(enemy6Pace == 5){
        enemy6Pace =  0;
        //yEne6Bullet++;
        if(yEne6Bullet >= 250){
          yEne6Bullet = 245;  
        }
      }
      yEne6Bullet++;
      if(yEne6Bullet >=250){
        yEne6Bullet = 245;  
      }
   
      if((enemy6y == 209)||((xP1+29)>enemy6x && (xP1+1)<(enemy6x+28) && yP1<(enemy6y+30) && (yP1+30)>enemy6y)){
        enemy4Life = 1;
        enemy5Life = 1;
        enemy6Life = 1;
        P1Life = 1;
        LCD_Sprite(xP1, yP1, 30, 30, dead, 2, 1, 0, 0);
        mapeo_SD("gameOver.txt", 67, 75, 186);
        delay(3000);
        LCD_Clear(0x00);
        enemy1Life = 0;
        enemy1y = 0;
        enemy2y = 0;
        enemy3y = 0;
        enemy4y = 0;
        enemy5y = 0;
        enemy6y = 0;
        enemy7y = 0;
        enemy8y = 0;
        enemy9y = 0;
        enemy10y = 0;
        enemy11y = 0;
        enemy12y = 0;
        enemy13y = 0;
        enemy14y = 0;
        enemy15y = 0;
        enemy16y = 0;
        enemy17y = 0;
        enemy18y = 0;
        enemy19y = 0;
        enemy20y = 0;
        enemy21y = 0;
        enemy22y = 0;
        enemy23y = 0;
        tripleWave1 = 0;
        tripleWave2 = 0;
        tripleWave3 = 0;
        tripleWave4 = 0;
        tripleWave5 = 0;
        P1Life = 0;
        xP1 = 0;
        yP1 = 209;
        state = 0;
        turn = 0;
        mapeo_SD("menu.txt", 32, 25, 256);
      }
      if((xP1+29)>xEne6Bullet && (xP1+1)<(xEne6Bullet+10) && yP1<(yEne6Bullet+10) && (yP1+30)>yEne6Bullet){
        enemy4Life = 1;
        enemy5Life = 1;
        enemy6Life = 1;
        P1Life = 1;
        LCD_Sprite(xP1, yP1, 30, 30, dead, 2, 1, 0, 0);
        mapeo_SD("gameOver.txt", 67, 75, 186);
        delay(3000);
        LCD_Clear(0x00);
        enemy1Life = 0;
        enemy1y = 0;
        enemy2y = 0;
        enemy3y = 0;
        enemy4y = 0;
        enemy5y = 0;
        enemy6y = 0;
        enemy7y = 0;
        enemy8y = 0;
        enemy9y = 0;
        enemy10y = 0;
        enemy11y = 0;
        enemy12y = 0;
        enemy13y = 0;
        enemy14y = 0;
        enemy15y = 0;
        enemy16y = 0;
        enemy17y = 0;
        enemy18y = 0;
        enemy19y = 0;
        enemy20y = 0;
        enemy21y = 0;
        enemy22y = 0;
        enemy23y = 0;
        tripleWave1 = 0;
        tripleWave2 = 0;
        tripleWave3 = 0;
        tripleWave4 = 0;
        tripleWave5 = 0;
        P1Life = 0;
        xP1 = 0;
        yP1 = 209;
        state = 0;
        turn = 0;
        mapeo_SD("menu.txt", 32, 25, 256);
      }
    }
  //**********************************************************************************************************************************
  //Enemy #6 code
  //**********************************************************************************************************************************
  
  //**********************************************************************************************************************************
  //Enemy #7 code
  //**********************************************************************************************************************************
    if(enemy7Life == 0){
      if(enemy7y == 0){
        xEne7Bullet = enemy7x + 8;
        yEne7Bullet = enemy7y + 30;
      }
      if(enemy7y == 110){
        xEne7Bullet = enemy7x + 8;
        yEne7Bullet = enemy7y + 30; 
      }
      int animEne7 = (enemy7y/5)%2;
      LCD_Sprite(enemy7x, enemy7y, 28, 30, enemyTankForward, 2, animEne7, 0, 0);
      H_line(enemy7x, enemy7y-1, 28,0x00);
      LCD_Bitmap(xEne7Bullet, yEne7Bullet, 10, 10, bulletDown);
      H_line(xEne7Bullet, yEne7Bullet, 10, 0x00);
      if(enemy7Pace == 0){
        enemy7y++; 
        enemy7Pace = 1;
      }else if(enemy7Pace == 1){
        enemy7Pace = 2;  
        //yEne7Bullet++;
        if(yEne7Bullet >= 250){
          yEne7Bullet = 245;  
        }
      }else if(enemy7Pace == 2){
        enemy7y++;
        enemy7Pace = 3;
      }else if(enemy7Pace == 3){
        enemy7Pace = 4;  
        //yEne7Bullet++;
        if(yEne7Bullet >= 250){
          yEne7Bullet = 245;  
        }
      }else if(enemy7Pace == 4){
        enemy7y++;
        enemy7Pace =  5;
      }else if(enemy7Pace == 5){
        enemy7Pace =  0;
        //yEne7Bullet++;
        if(yEne7Bullet >= 250){
          yEne7Bullet = 245;  
        }
      }
      yEne7Bullet++;
      if(yEne7Bullet >=250){
        yEne7Bullet = 245;  
      }
   
      if((enemy7y == 209)||((xP1+29)>enemy7x && (xP1+1)<(enemy7x+28) && yP1<(enemy7y+30) && (yP1+30)>enemy7y)){
        enemy7Life = 1;
        enemy8Life = 1;
        enemy9Life = 1;
        P1Life = 1;
        LCD_Sprite(xP1, yP1, 30, 30, dead, 2, 1, 0, 0);
        mapeo_SD("gameOver.txt", 67, 75, 186);
        delay(3000);
        LCD_Clear(0x00);
        enemy1Life = 0;
        enemy1y = 0;
        enemy2y = 0;
        enemy3y = 0;
        enemy4y = 0;
        enemy5y = 0;
        enemy6y = 0;
        enemy7y = 0;
        enemy8y = 0;
        enemy9y = 0;
        enemy10y = 0;
        enemy11y = 0;
        enemy12y = 0;
        enemy13y = 0;
        enemy14y = 0;
        enemy15y = 0;
        enemy16y = 0;
        enemy17y = 0;
        enemy18y = 0;
        enemy19y = 0;
        enemy20y = 0;
        enemy21y = 0;
        enemy22y = 0;
        enemy23y = 0;
        tripleWave1 = 0;
        tripleWave2 = 0;
        tripleWave3 = 0;
        tripleWave4 = 0;
        tripleWave5 = 0;
        P1Life = 0;
        xP1 = 0;
        yP1 = 209;
        state = 0;
        turn = 0;
        mapeo_SD("menu.txt", 32, 25, 256);
      }
      if((xP1+29)>xEne7Bullet && (xP1+1)<(xEne7Bullet+10) && yP1<(yEne7Bullet+10) && (yP1+30)>yEne7Bullet){
        enemy7Life = 1;
        enemy8Life = 1;
        enemy9Life = 1;
        P1Life = 1;
        LCD_Sprite(xP1, yP1, 30, 30, dead, 2, 1, 0, 0);
        mapeo_SD("gameOver.txt", 67, 75, 186);
        delay(3000);
        LCD_Clear(0x00);
        enemy1Life = 0;
        enemy1y = 0;
        enemy2y = 0;
        enemy3y = 0;
        enemy4y = 0;
        enemy5y = 0;
        enemy6y = 0;
        enemy7y = 0;
        enemy8y = 0;
        enemy9y = 0;
        enemy10y = 0;
        enemy11y = 0;
        enemy12y = 0;
        enemy13y = 0;
        enemy14y = 0;
        enemy15y = 0;
        enemy16y = 0;
        enemy17y = 0;
        enemy18y = 0;
        enemy19y = 0;
        enemy20y = 0;
        enemy21y = 0;
        enemy22y = 0;
        enemy23y = 0;
        tripleWave1 = 0;
        tripleWave2 = 0;
        tripleWave3 = 0;
        tripleWave4 = 0;
        tripleWave5 = 0;
        P1Life = 0;
        xP1 = 0;
        yP1 = 209;
        state = 0;
        turn = 0;
        mapeo_SD("menu.txt", 32, 25, 256);
      }
    }
  //**********************************************************************************************************************************
  //Enemy #7 code
  //**********************************************************************************************************************************
  
  //**********************************************************************************************************************************
  //Enemy #8 code
  //**********************************************************************************************************************************
    if(enemy8Life == 0){
      if(enemy8y == 0){
        xEne8Bullet = enemy8x + 8;
        yEne8Bullet = enemy8y + 30;
      }
      if(enemy8y == 110){
        xEne8Bullet = enemy8x + 8;
        yEne8Bullet = enemy8y + 30; 
      }
      int animEne8 = (enemy8y/5)%2;
      LCD_Sprite(enemy8x, enemy8y, 28, 30, enemyTankForward, 2, animEne8, 0, 0);
      H_line(enemy8x, enemy8y-1, 28,0x00);
      LCD_Bitmap(xEne8Bullet, yEne8Bullet, 10, 10, bulletDown);
      H_line(xEne8Bullet, yEne8Bullet, 10, 0x00);
      if(enemy8Pace == 0){
        enemy8y++; 
        enemy8Pace = 1;
      }else if(enemy8Pace == 1){
        enemy8Pace = 2;  
        //yEne8Bullet++;
        if(yEne8Bullet >= 250){
          yEne8Bullet = 245;  
        }
      }else if(enemy8Pace == 2){
        enemy8y++;
        enemy8Pace = 3;
      }else if(enemy8Pace == 3){
        enemy8Pace = 4;  
        //yEne8Bullet++;
        if(yEne8Bullet >= 250){
          yEne8Bullet = 245;  
        }
      }else if(enemy8Pace == 4){
        enemy8y++;
        enemy8Pace =  5;
      }else if(enemy8Pace == 5){
        enemy8Pace =  0;
        //yEne8Bullet++;
        if(yEne8Bullet >= 250){
          yEne8Bullet = 245;  
        }
      }
      yEne8Bullet++;
      if(yEne8Bullet >=250){
        yEne8Bullet = 245;  
      }
   
      if((enemy8y == 209)||((xP1+29)>enemy8x && (xP1+1)<(enemy8x+28) && yP1<(enemy8y+30) && (yP1+30)>enemy8y)){
        enemy7Life = 1;
        enemy8Life = 1;
        enemy9Life = 1;
        P1Life = 1;
        LCD_Sprite(xP1, yP1, 30, 30, dead, 2, 1, 0, 0);
        mapeo_SD("gameOver.txt", 67, 75, 186);
        delay(3000);
        LCD_Clear(0x00);
        enemy1Life = 0;
        enemy1y = 0;
        enemy2y = 0;
        enemy3y = 0;
        enemy4y = 0;
        enemy5y = 0;
        enemy6y = 0;
        enemy7y = 0;
        enemy8y = 0;
        enemy9y = 0;
        enemy10y = 0;
        enemy11y = 0;
        enemy12y = 0;
        enemy13y = 0;
        enemy14y = 0;
        enemy15y = 0;
        enemy16y = 0;
        enemy17y = 0;
        enemy18y = 0;
        enemy19y = 0;
        enemy20y = 0;
        enemy21y = 0;
        enemy22y = 0;
        enemy23y = 0;
        tripleWave1 = 0;
        tripleWave2 = 0;
        tripleWave3 = 0;
        tripleWave4 = 0;
        tripleWave5 = 0;
        P1Life = 0;
        xP1 = 0;
        yP1 = 209;
        state = 0;
        turn = 0;
        mapeo_SD("menu.txt", 32, 25, 256);
      }
      if((xP1+29)>xEne8Bullet && (xP1+1)<(xEne8Bullet+10) && yP1<(yEne8Bullet+10) && (yP1+30)>yEne8Bullet){
        enemy7Life = 1;
        enemy8Life = 1;
        enemy9Life = 1;
        P1Life = 1;
        LCD_Sprite(xP1, yP1, 30, 30, dead, 2, 1, 0, 0);
        mapeo_SD("gameOver.txt", 67, 75, 186);
        delay(3000);
        LCD_Clear(0x00);
        enemy1Life = 0;
        enemy1y = 0;
        enemy2y = 0;
        enemy3y = 0;
        enemy4y = 0;
        enemy5y = 0;
        enemy6y = 0;
        enemy7y = 0;
        enemy8y = 0;
        enemy9y = 0;
        enemy10y = 0;
        enemy11y = 0;
        enemy12y = 0;
        enemy13y = 0;
        enemy14y = 0;
        enemy15y = 0;
        enemy16y = 0;
        enemy17y = 0;
        enemy18y = 0;
        enemy19y = 0;
        enemy20y = 0;
        enemy21y = 0;
        enemy22y = 0;
        enemy23y = 0;
        tripleWave1 = 0;
        tripleWave2 = 0;
        tripleWave3 = 0;
        tripleWave4 = 0;
        tripleWave5 = 0;
        P1Life = 0;
        xP1 = 0;
        yP1 = 209;
        state = 0;
        turn = 0;
        mapeo_SD("menu.txt", 32, 25, 256);
      }
    }
  //**********************************************************************************************************************************
  //Enemy #8 code
  //**********************************************************************************************************************************
  
  //**********************************************************************************************************************************
  //Enemy #9 code
  //**********************************************************************************************************************************
    if(enemy9Life == 0){
      if(enemy9y == 0){
        xEne9Bullet = enemy9x + 8;
        yEne9Bullet = enemy9y + 30;
      }
      if(enemy9y == 110){
        xEne9Bullet = enemy9x + 8;
        yEne9Bullet = enemy9y + 30; 
      }
      int animEne9 = (enemy9y/5)%2;
      LCD_Sprite(enemy9x, enemy9y, 28, 30, enemyTankForward, 2, animEne9, 0, 0);
      H_line(enemy9x, enemy9y-1, 28,0x00);
      LCD_Bitmap(xEne9Bullet, yEne9Bullet, 10, 10, bulletDown);
      H_line(xEne9Bullet, yEne9Bullet, 10, 0x00);
      if(enemy9Pace == 0){
        enemy9y++; 
        enemy9Pace = 1;
      }else if(enemy9Pace == 1){
        enemy9Pace = 2;  
        //yEne9Bullet++;
        if(yEne9Bullet >= 250){
          yEne9Bullet = 245;  
        }
      }else if(enemy9Pace == 2){
        enemy9y++;
        enemy9Pace = 3;
      }else if(enemy9Pace == 3){
        enemy9Pace = 4;  
        //yEne9Bullet++;
        if(yEne9Bullet >= 250){
          yEne9Bullet = 245;  
        }
      }else if(enemy9Pace == 4){
        enemy9y++;
        enemy9Pace =  5;
      }else if(enemy9Pace == 5){
        enemy9Pace =  0;
        //yEne9Bullet++;
        if(yEne9Bullet >= 250){
          yEne9Bullet = 245;  
        }
      }
      yEne9Bullet++;
      if(yEne9Bullet >=250){
        yEne9Bullet = 245;  
      }
   
      if((enemy9y == 209)||((xP1+29)>enemy9x && (xP1+1)<(enemy9x+28) && yP1<(enemy9y+30) && (yP1+30)>enemy9y)){
        enemy7Life = 1;
        enemy8Life = 1;
        enemy9Life = 1;
        P1Life = 1;
        LCD_Sprite(xP1, yP1, 30, 30, dead, 2, 1, 0, 0);
        mapeo_SD("gameOver.txt", 67, 75, 186);
        delay(3000);
        LCD_Clear(0x00);
        enemy1Life = 0;
        enemy1y = 0;
        enemy2y = 0;
        enemy3y = 0;
        enemy4y = 0;
        enemy5y = 0;
        enemy6y = 0;
        enemy7y = 0;
        enemy8y = 0;
        enemy9y = 0;
        enemy10y = 0;
        enemy11y = 0;
        enemy12y = 0;
        enemy13y = 0;
        enemy14y = 0;
        enemy15y = 0;
        enemy16y = 0;
        enemy17y = 0;
        enemy18y = 0;
        enemy19y = 0;
        enemy20y = 0;
        enemy21y = 0;
        enemy22y = 0;
        enemy23y = 0;
        tripleWave1 = 0;
        tripleWave2 = 0;
        tripleWave3 = 0;
        tripleWave4 = 0;
        tripleWave5 = 0;
        P1Life = 0;
        xP1 = 0;
        yP1 = 209;
        state = 0;
        turn = 0;
        mapeo_SD("menu.txt", 32, 25, 256);
      }
      if((xP1+29)>xEne9Bullet && (xP1+1)<(xEne9Bullet+10) && yP1<(yEne9Bullet+10) && (yP1+30)>yEne9Bullet){
        enemy7Life = 1;
        enemy8Life = 1;
        enemy9Life = 1;
        P1Life = 1;
        LCD_Sprite(xP1, yP1, 30, 30, dead, 2, 1, 0, 0);
        mapeo_SD("gameOver.txt", 67, 75, 186);
        delay(3000);
        LCD_Clear(0x00);
        enemy1Life = 0;
        enemy1y = 0;
        enemy2y = 0;
        enemy3y = 0;
        enemy4y = 0;
        enemy5y = 0;
        enemy6y = 0;
        enemy7y = 0;
        enemy8y = 0;
        enemy9y = 0;
        enemy10y = 0;
        enemy11y = 0;
        enemy12y = 0;
        enemy13y = 0;
        enemy14y = 0;
        enemy15y = 0;
        enemy16y = 0;
        enemy17y = 0;
        enemy18y = 0;
        enemy19y = 0;
        enemy20y = 0;
        enemy21y = 0;
        enemy22y = 0;
        enemy23y = 0;
        tripleWave1 = 0;
        tripleWave2 = 0;
        tripleWave3 = 0;
        tripleWave4 = 0;
        tripleWave5 = 0;
        P1Life = 0;
        xP1 = 0;
        yP1 = 209;
        state = 0;
        turn = 0;
        mapeo_SD("menu.txt", 32, 25, 256);
      }
    }
  //**********************************************************************************************************************************
  //Enemy #9 code
  //**********************************************************************************************************************************
  
  //**********************************************************************************************************************************
  //Enemy #10 code
  //**********************************************************************************************************************************
    if(enemy10Life == 0){
      if(enemy10y == 0){
        xEne10Bullet = enemy10x + 8;
        yEne10Bullet = enemy10y + 30;
      }
      if(enemy10y == 110){
        xEne10Bullet = enemy10x + 8;
        yEne10Bullet = enemy10y + 30; 
      }
      int animEne10 = (enemy10y/5)%2;
      LCD_Sprite(enemy10x, enemy10y, 28, 30, enemyTankForward, 2, animEne10, 0, 0);
      H_line(enemy10x, enemy10y-1, 28,0x00);
      LCD_Bitmap(xEne10Bullet, yEne10Bullet, 10, 10, bulletDown);
      H_line(xEne10Bullet, yEne10Bullet, 10, 0x00);
      if(enemy10Pace == 0){
        enemy10y++; 
        enemy10Pace = 1;
      }else if(enemy10Pace == 1){
        enemy10Pace = 2;  
        //yEne10Bullet++;
        if(yEne10Bullet >= 250){
          yEne10Bullet = 245;  
        }
      }else if(enemy10Pace == 2){
        enemy10y++;
        enemy10Pace = 3;
      }else if(enemy10Pace == 3){
        enemy10Pace = 4;  
        //yEne10Bullet++;
        if(yEne10Bullet >= 250){
          yEne10Bullet = 245;  
        }
      }else if(enemy10Pace == 4){
        enemy10y++;
        enemy10Pace =  5;
      }else if(enemy10Pace == 5){
        enemy10Pace =  0;
        //yEne10Bullet++;
        if(yEne10Bullet >= 250){
          yEne10Bullet = 245;  
        }
      }
      yEne10Bullet++;
      if(yEne10Bullet >=250){
        yEne10Bullet = 245;  
      }
   
      if((enemy10y == 209)||((xP1+29)>enemy10x && (xP1+1)<(enemy10x+28) && yP1<(enemy10y+30) && (yP1+30)>enemy10y)){
        enemy10Life = 1;
        enemy11Life = 1;
        enemy12Life = 1;
        enemy13Life = 1;
        P1Life = 1;
        LCD_Sprite(xP1, yP1, 30, 30, dead, 2, 1, 0, 0);
        mapeo_SD("gameOver.txt", 67, 75, 186);
        delay(3000);
        LCD_Clear(0x00);
        enemy1Life = 0;
        enemy1y = 0;
        enemy2y = 0;
        enemy3y = 0;
        enemy4y = 0;
        enemy5y = 0;
        enemy6y = 0;
        enemy7y = 0;
        enemy8y = 0;
        enemy9y = 0;
        enemy10y = 0;
        enemy11y = 0;
        enemy12y = 0;
        enemy13y = 0;
        enemy14y = 0;
        enemy15y = 0;
        enemy16y = 0;
        enemy17y = 0;
        enemy18y = 0;
        enemy19y = 0;
        enemy20y = 0;
        enemy21y = 0;
        enemy22y = 0;
        enemy23y = 0;
        tripleWave1 = 0;
        tripleWave2 = 0;
        tripleWave3 = 0;
        tripleWave4 = 0;
        tripleWave5 = 0;
        P1Life = 0;
        xP1 = 0;
        yP1 = 209;
        state = 0;
        turn = 0;
        mapeo_SD("menu.txt", 32, 25, 256);
      }
      if((xP1+29)>xEne10Bullet && (xP1+1)<(xEne10Bullet+10) && yP1<(yEne10Bullet+10) && (yP1+30)>yEne10Bullet){
        enemy10Life = 1;
        enemy11Life = 1;
        enemy12Life = 1;
        enemy13Life = 1;
        P1Life = 1;
        LCD_Sprite(xP1, yP1, 30, 30, dead, 2, 1, 0, 0);
        mapeo_SD("gameOver.txt", 67, 75, 186);
        delay(3000);
        LCD_Clear(0x00);
        enemy1Life = 0;
        enemy1y = 0;
        enemy2y = 0;
        enemy3y = 0;
        enemy4y = 0;
        enemy5y = 0;
        enemy6y = 0;
        enemy7y = 0;
        enemy8y = 0;
        enemy9y = 0;
        enemy10y = 0;
        enemy11y = 0;
        enemy12y = 0;
        enemy13y = 0;
        enemy14y = 0;
        enemy15y = 0;
        enemy16y = 0;
        enemy17y = 0;
        enemy18y = 0;
        enemy19y = 0;
        enemy20y = 0;
        enemy21y = 0;
        enemy22y = 0;
        enemy23y = 0;
        tripleWave1 = 0;
        tripleWave2 = 0;
        tripleWave3 = 0;
        tripleWave4 = 0;
        tripleWave5 = 0;
        P1Life = 0;
        xP1 = 0;
        yP1 = 209;
        state = 0;
        turn = 0;
        mapeo_SD("menu.txt", 32, 25, 256);
      }
    }
  //**********************************************************************************************************************************
  //Enemy #10 code
  //**********************************************************************************************************************************
  
  //**********************************************************************************************************************************
  //Enemy #11 code
  //**********************************************************************************************************************************
    if(enemy11Life == 0){
      if(enemy11y == 0){
        xEne11Bullet = enemy11x + 8;
        yEne11Bullet = enemy11y + 30;
      }
      if(enemy11y == 110){
        xEne11Bullet = enemy11x + 8;
        yEne11Bullet = enemy11y + 30; 
      }
      int animEne11 = (enemy11y/5)%2;
      LCD_Sprite(enemy11x, enemy11y, 28, 30, enemyTankForward, 2, animEne11, 0, 0);
      H_line(enemy11x, enemy11y-1, 28,0x00);
      LCD_Bitmap(xEne11Bullet, yEne11Bullet, 10, 10, bulletDown);
      H_line(xEne11Bullet, yEne11Bullet, 10, 0x00);
      if(enemy11Pace == 0){
        enemy11y++; 
        enemy11Pace = 1;
      }else if(enemy11Pace == 1){
        enemy11Pace = 2;  
        //yEne11Bullet++;
        if(yEne11Bullet >= 250){
          yEne11Bullet = 245;  
        }
      }else if(enemy11Pace == 2){
        enemy11y++;
        enemy11Pace = 3;
      }else if(enemy11Pace == 3){
        enemy11Pace = 4;  
        //yEne11Bullet++;
        if(yEne11Bullet >= 250){
          yEne11Bullet = 245;  
        }
      }else if(enemy11Pace == 4){
        enemy11y++;
        enemy11Pace =  5;
      }else if(enemy11Pace == 5){
        enemy11Pace =  0;
        //yEne11Bullet++;
        if(yEne11Bullet >= 250){
          yEne11Bullet = 245;  
        }
      }
      yEne11Bullet++;
      if(yEne11Bullet >=250){
        yEne11Bullet = 245;  
      }
   
      if((enemy11y == 209)||((xP1+29)>enemy11x && (xP1+1)<(enemy11x+28) && yP1<(enemy11y+30) && (yP1+30)>enemy11y)){
        enemy10Life = 1;
        enemy11Life = 1;
        enemy12Life = 1;
        enemy13Life = 1;
        P1Life = 1;
        LCD_Sprite(xP1, yP1, 30, 30, dead, 2, 1, 0, 0);
        mapeo_SD("gameOver.txt", 67, 75, 186);
        delay(3000);
        LCD_Clear(0x00);
        enemy1Life = 0;
        enemy1y = 0;
        enemy2y = 0;
        enemy3y = 0;
        enemy4y = 0;
        enemy5y = 0;
        enemy6y = 0;
        enemy7y = 0;
        enemy8y = 0;
        enemy9y = 0;
        enemy10y = 0;
        enemy11y = 0;
        enemy12y = 0;
        enemy13y = 0;
        enemy14y = 0;
        enemy15y = 0;
        enemy16y = 0;
        enemy17y = 0;
        enemy18y = 0;
        enemy19y = 0;
        enemy20y = 0;
        enemy21y = 0;
        enemy22y = 0;
        enemy23y = 0;
        tripleWave1 = 0;
        tripleWave2 = 0;
        tripleWave3 = 0;
        tripleWave4 = 0;
        tripleWave5 = 0;
        P1Life = 0;
        xP1 = 0;
        yP1 = 209;
        state = 0;
        turn = 0;
        mapeo_SD("menu.txt", 32, 25, 256);
      }
      if((xP1+29)>xEne11Bullet && (xP1+1)<(xEne11Bullet+10) && yP1<(yEne11Bullet+10) && (yP1+30)>yEne11Bullet){
        enemy10Life = 1;
        enemy11Life = 1;
        enemy12Life = 1;
        enemy13Life = 1;
        P1Life = 1;
        LCD_Sprite(xP1, yP1, 30, 30, dead, 2, 1, 0, 0);
        mapeo_SD("gameOver.txt", 67, 75, 186);
        delay(3000);
        LCD_Clear(0x00);
        enemy1Life = 0;
        enemy1y = 0;
        enemy2y = 0;
        enemy3y = 0;
        enemy4y = 0;
        enemy5y = 0;
        enemy6y = 0;
        enemy7y = 0;
        enemy8y = 0;
        enemy9y = 0;
        enemy10y = 0;
        enemy11y = 0;
        enemy12y = 0;
        enemy13y = 0;
        enemy14y = 0;
        enemy15y = 0;
        enemy16y = 0;
        enemy17y = 0;
        enemy18y = 0;
        enemy19y = 0;
        enemy20y = 0;
        enemy21y = 0;
        enemy22y = 0;
        enemy23y = 0;
        tripleWave1 = 0;
        tripleWave2 = 0;
        tripleWave3 = 0;
        tripleWave4 = 0;
        tripleWave5 = 0;
        P1Life = 0;
        xP1 = 0;
        yP1 = 209;
        state = 0;
        turn = 0;
        mapeo_SD("menu.txt", 32, 25, 256);
      }
    }
  //**********************************************************************************************************************************
  //Enemy #11 code
  //**********************************************************************************************************************************
  
  //**********************************************************************************************************************************
  //Enemy #12 code
  //**********************************************************************************************************************************
    if(enemy12Life == 0){
      if(enemy12y == 0){
        xEne12Bullet = enemy12x + 8;
        yEne12Bullet = enemy12y + 30;
      }
      if(enemy12y == 110){
        xEne12Bullet = enemy12x + 8;
        yEne12Bullet = enemy12y + 30; 
      }
      int animEne12 = (enemy12y/5)%2;
      LCD_Sprite(enemy12x, enemy12y, 28, 30, enemyTankForward, 2, animEne12, 0, 0);
      H_line(enemy12x, enemy12y-1, 28,0x00);
      LCD_Bitmap(xEne12Bullet, yEne12Bullet, 10, 10, bulletDown);
      H_line(xEne12Bullet, yEne12Bullet, 10, 0x00);
      if(enemy12Pace == 0){
        enemy12y++; 
        enemy12Pace = 1;
      }else if(enemy12Pace == 1){
        enemy12Pace = 2;  
        //yEne12Bullet++;
        if(yEne12Bullet >= 250){
          yEne12Bullet = 245;  
        }
      }else if(enemy12Pace == 2){
        enemy12y++;
        enemy12Pace = 3;
      }else if(enemy12Pace == 3){
        enemy12Pace = 4;  
        //yEne12Bullet++;
        if(yEne12Bullet >= 250){
          yEne12Bullet = 245;  
        }
      }else if(enemy12Pace == 4){
        enemy12y++;
        enemy12Pace =  5;
      }else if(enemy12Pace == 5){
        enemy12Pace =  0;
        //yEne12Bullet++;
        if(yEne12Bullet >= 250){
          yEne12Bullet = 245;  
        }
      }
      yEne12Bullet++;
      if(yEne12Bullet >=250){
        yEne12Bullet = 245;  
      }
   
      if((enemy12y == 209)||((xP1+29)>enemy12x && (xP1+1)<(enemy12x+28) && yP1<(enemy12y+30) && (yP1+30)>enemy12y)){
        enemy10Life = 1;
        enemy11Life = 1;
        enemy12Life = 1;
        enemy13Life = 1;
        P1Life = 1;
        LCD_Sprite(xP1, yP1, 30, 30, dead, 2, 1, 0, 0);
        mapeo_SD("gameOver.txt", 67, 75, 186);
        delay(3000);
        LCD_Clear(0x00);
        enemy1Life = 0;
        enemy1y = 0;
        enemy2y = 0;
        enemy3y = 0;
        enemy4y = 0;
        enemy5y = 0;
        enemy6y = 0;
        enemy7y = 0;
        enemy8y = 0;
        enemy9y = 0;
        enemy10y = 0;
        enemy11y = 0;
        enemy12y = 0;
        enemy13y = 0;
        enemy14y = 0;
        enemy15y = 0;
        enemy16y = 0;
        enemy17y = 0;
        enemy18y = 0;
        enemy19y = 0;
        enemy20y = 0;
        enemy21y = 0;
        enemy22y = 0;
        enemy23y = 0;
        tripleWave1 = 0;
        tripleWave2 = 0;
        tripleWave3 = 0;
        tripleWave4 = 0;
        tripleWave5 = 0;
        P1Life = 0;
        xP1 = 0;
        yP1 = 209;
        state = 0;
        turn = 0;
        mapeo_SD("menu.txt", 32, 25, 256);
      }
      if((xP1+29)>xEne12Bullet && (xP1+1)<(xEne12Bullet+10) && yP1<(yEne12Bullet+10) && (yP1+30)>yEne12Bullet){
        enemy10Life = 1;
        enemy11Life = 1;
        enemy12Life = 1;
        enemy13Life = 1;
        P1Life = 1;
        LCD_Sprite(xP1, yP1, 30, 30, dead, 2, 1, 0, 0);
        mapeo_SD("gameOver.txt", 67, 75, 186);
        delay(3000);
        LCD_Clear(0x00);
        enemy1Life = 0;
        enemy1y = 0;
        enemy2y = 0;
        enemy3y = 0;
        enemy4y = 0;
        enemy5y = 0;
        enemy6y = 0;
        enemy7y = 0;
        enemy8y = 0;
        enemy9y = 0;
        enemy10y = 0;
        enemy11y = 0;
        enemy12y = 0;
        enemy13y = 0;
        enemy14y = 0;
        enemy15y = 0;
        enemy16y = 0;
        enemy17y = 0;
        enemy18y = 0;
        enemy19y = 0;
        enemy20y = 0;
        enemy21y = 0;
        enemy22y = 0;
        enemy23y = 0;
        tripleWave1 = 0;
        tripleWave2 = 0;
        tripleWave3 = 0;
        tripleWave4 = 0;
        tripleWave5 = 0;
        P1Life = 0;
        xP1 = 0;
        yP1 = 209;
        state = 0;
        turn = 0;
        mapeo_SD("menu.txt", 32, 25, 256);
      }
    }
  //**********************************************************************************************************************************
  //Enemy #12 code
  //**********************************************************************************************************************************
  
  //**********************************************************************************************************************************
  //Enemy #13 code
  //**********************************************************************************************************************************
    if(enemy13Life == 0){
      if(enemy13y == 0){
        xEne13Bullet = enemy13x + 8;
        yEne13Bullet = enemy13y + 30;
      }
      if(enemy13y == 110){
        xEne13Bullet = enemy13x + 8;
        yEne13Bullet = enemy13y + 30; 
      }
      int animEne13 = (enemy13y/5)%2;
      LCD_Sprite(enemy13x, enemy13y, 28, 30, enemyTankForward, 2, animEne13, 0, 0);
      H_line(enemy13x, enemy13y-1, 28,0x00);
      LCD_Bitmap(xEne13Bullet, yEne13Bullet, 10, 10, bulletDown);
      H_line(xEne13Bullet, yEne13Bullet, 10, 0x00);
      if(enemy13Pace == 0){
        enemy13y++; 
        enemy13Pace = 1;
      }else if(enemy13Pace == 1){
        enemy13Pace = 2;  
        //yEne13Bullet++;
        if(yEne13Bullet >= 250){
          yEne13Bullet = 245;  
        }
      }else if(enemy13Pace == 2){
        enemy13y++;
        enemy13Pace = 3;
      }else if(enemy13Pace == 3){
        enemy13Pace = 4;  
        //yEne13Bullet++;
        if(yEne13Bullet >= 250){
          yEne13Bullet = 245;  
        }
      }else if(enemy13Pace == 4){
        enemy13y++;
        enemy13Pace =  5;
      }else if(enemy13Pace == 5){
        enemy13Pace =  0;
        //yEne13Bullet++;
        if(yEne13Bullet >= 250){
          yEne13Bullet = 245;  
        }
      }
      yEne13Bullet++;
      if(yEne13Bullet >=250){
        yEne13Bullet = 245;  
      }
   
      if((enemy13y == 209)||((xP1+29)>enemy13x && (xP1+1)<(enemy13x+28) && yP1<(enemy13y+30) && (yP1+30)>enemy13y)){
        enemy10Life = 1;
        enemy11Life = 1;
        enemy12Life = 1;
        enemy13Life = 1;
        P1Life = 1;
        LCD_Sprite(xP1, yP1, 30, 30, dead, 2, 1, 0, 0);
        mapeo_SD("gameOver.txt", 67, 75, 186);
        delay(3000);
        LCD_Clear(0x00);
        enemy1Life = 0;
        enemy1y = 0;
        enemy2y = 0;
        enemy3y = 0;
        enemy4y = 0;
        enemy5y = 0;
        enemy6y = 0;
        enemy7y = 0;
        enemy8y = 0;
        enemy9y = 0;
        enemy10y = 0;
        enemy11y = 0;
        enemy12y = 0;
        enemy13y = 0;
        enemy14y = 0;
        enemy15y = 0;
        enemy16y = 0;
        enemy17y = 0;
        enemy18y = 0;
        enemy19y = 0;
        enemy20y = 0;
        enemy21y = 0;
        enemy22y = 0;
        enemy23y = 0;
        tripleWave1 = 0;
        tripleWave2 = 0;
        tripleWave3 = 0;
        tripleWave4 = 0;
        tripleWave5 = 0;
        P1Life = 0;
        xP1 = 0;
        yP1 = 209;
        state = 0;
        turn = 0;
        mapeo_SD("menu.txt", 32, 25, 256);
      }
      if((xP1+29)>xEne13Bullet && (xP1+1)<(xEne13Bullet+10) && yP1<(yEne13Bullet+10) && (yP1+30)>yEne13Bullet){
        enemy10Life = 1;
        enemy11Life = 1;
        enemy12Life = 1;
        enemy13Life = 1;
        P1Life = 1;
        LCD_Sprite(xP1, yP1, 30, 30, dead, 2, 1, 0, 0);
        mapeo_SD("gameOver.txt", 67, 75, 186);
        delay(3000);
        LCD_Clear(0x00);
        enemy1Life = 0;
        enemy1y = 0;
        enemy2y = 0;
        enemy3y = 0;
        enemy4y = 0;
        enemy5y = 0;
        enemy6y = 0;
        enemy7y = 0;
        enemy8y = 0;
        enemy9y = 0;
        enemy10y = 0;
        enemy11y = 0;
        enemy12y = 0;
        enemy13y = 0;
        enemy14y = 0;
        enemy15y = 0;
        enemy16y = 0;
        enemy17y = 0;
        enemy18y = 0;
        enemy19y = 0;
        enemy20y = 0;
        enemy21y = 0;
        enemy22y = 0;
        enemy23y = 0;
        tripleWave1 = 0;
        tripleWave2 = 0;
        tripleWave3 = 0;
        tripleWave4 = 0;
        tripleWave5 = 0;
        P1Life = 0;
        xP1 = 0;
        yP1 = 209;
        state = 0;
        turn = 0;
        mapeo_SD("menu.txt", 32, 25, 256);
      }
    }
  //**********************************************************************************************************************************
  //Enemy #13 code
  //**********************************************************************************************************************************
  
  //**********************************************************************************************************************************
  //Enemy #14 code
  //**********************************************************************************************************************************
    if(enemy14Life == 0){
      if(enemy14y == 0){
        xEne14Bullet = enemy14x + 8;
        yEne14Bullet = enemy14y + 30;
      }
      if(enemy14y == 110){
        xEne14Bullet = enemy14x + 8;
        yEne14Bullet = enemy14y + 30; 
      }
      int animEne14 = (enemy14y/5)%2;
      LCD_Sprite(enemy14x, enemy14y, 28, 30, enemyTankForward, 2, animEne14, 0, 0);
      H_line(enemy14x, enemy14y-1, 28,0x00);
      LCD_Bitmap(xEne14Bullet, yEne14Bullet, 10, 10, bulletDown);
      H_line(xEne14Bullet, yEne14Bullet, 10, 0x00);
      if(enemy14Pace == 0){
        enemy14y++; 
        enemy14Pace = 1;
      }else if(enemy14Pace == 1){
        enemy14Pace = 2;  
        //yEne14Bullet++;
        if(yEne14Bullet >= 250){
          yEne14Bullet = 245;  
        }
      }else if(enemy14Pace == 2){
        enemy14y++;
        enemy14Pace = 3;
      }else if(enemy14Pace == 3){
        enemy14Pace = 4;  
        //yEne14Bullet++;
        if(yEne14Bullet >= 250){
          yEne14Bullet = 245;  
        }
      }else if(enemy14Pace == 4){
        enemy14y++;
        enemy14Pace =  5;
      }else if(enemy14Pace == 5){
        enemy14Pace =  0;
        //yEne14Bullet++;
        if(yEne14Bullet >= 250){
          yEne14Bullet = 245;  
        }
      }
      yEne14Bullet++;
      if(yEne14Bullet >=250){
        yEne14Bullet = 245;  
      }
   
      if((enemy14y == 209)||((xP1+29)>enemy14x && (xP1+1)<(enemy14x+28) && yP1<(enemy14y+30) && (yP1+30)>enemy14y)){
        enemy14Life = 1;
        enemy15Life = 1;
        enemy16Life = 1;
        enemy17Life = 1;
        P1Life = 1;
        LCD_Sprite(xP1, yP1, 30, 30, dead, 2, 1, 0, 0);
        mapeo_SD("gameOver.txt", 67, 75, 186);
        delay(3000);
        LCD_Clear(0x00);
        enemy1Life = 0;
        enemy1y = 0;
        enemy2y = 0;
        enemy3y = 0;
        enemy4y = 0;
        enemy5y = 0;
        enemy6y = 0;
        enemy7y = 0;
        enemy8y = 0;
        enemy9y = 0;
        enemy10y = 0;
        enemy11y = 0;
        enemy12y = 0;
        enemy13y = 0;
        enemy14y = 0;
        enemy15y = 0;
        enemy16y = 0;
        enemy17y = 0;
        enemy18y = 0;
        enemy19y = 0;
        enemy20y = 0;
        enemy21y = 0;
        enemy22y = 0;
        enemy23y = 0;
        tripleWave1 = 0;
        tripleWave2 = 0;
        tripleWave3 = 0;
        tripleWave4 = 0;
        tripleWave5 = 0;
        P1Life = 0;
        xP1 = 0;
        yP1 = 209;
        state = 0;
        turn = 0;
        mapeo_SD("menu.txt", 32, 25, 256);
      }
      if((xP1+29)>xEne14Bullet && (xP1+1)<(xEne14Bullet+10) && yP1<(yEne14Bullet+10) && (yP1+30)>yEne14Bullet){
        enemy14Life = 1;
        enemy15Life = 1;
        enemy16Life = 1;
        enemy17Life = 1;
        P1Life = 1;
        LCD_Sprite(xP1, yP1, 30, 30, dead, 2, 1, 0, 0);
        mapeo_SD("gameOver.txt", 67, 75, 186);
        delay(3000);
        LCD_Clear(0x00);
        enemy1Life = 0;
        enemy1y = 0;
        enemy2y = 0;
        enemy3y = 0;
        enemy4y = 0;
        enemy5y = 0;
        enemy6y = 0;
        enemy7y = 0;
        enemy8y = 0;
        enemy9y = 0;
        enemy10y = 0;
        enemy11y = 0;
        enemy12y = 0;
        enemy13y = 0;
        enemy14y = 0;
        enemy15y = 0;
        enemy16y = 0;
        enemy17y = 0;
        enemy18y = 0;
        enemy19y = 0;
        enemy20y = 0;
        enemy21y = 0;
        enemy22y = 0;
        enemy23y = 0;
        tripleWave1 = 0;
        tripleWave2 = 0;
        tripleWave3 = 0;
        tripleWave4 = 0;
        tripleWave5 = 0;
        P1Life = 0;
        xP1 = 0;
        yP1 = 209;
        state = 0;
        turn = 0;
        mapeo_SD("menu.txt", 32, 25, 256);
      }
    }
  //**********************************************************************************************************************************
  //Enemy #14 code
  //**********************************************************************************************************************************
  
  //**********************************************************************************************************************************
  //Enemy #15 code
  //**********************************************************************************************************************************
    if(enemy15Life == 0){
      if(enemy15y == 0){
        xEne15Bullet = enemy15x + 8;
        yEne15Bullet = enemy15y + 30;
      }
      if(enemy15y == 110){
        xEne15Bullet = enemy15x + 8;
        yEne15Bullet = enemy15y + 30; 
      }
      int animEne15 = (enemy15y/5)%2;
      LCD_Sprite(enemy15x, enemy15y, 28, 30, enemyTankForward, 2, animEne15, 0, 0);
      H_line(enemy15x, enemy15y-1, 28,0x00);
      LCD_Bitmap(xEne15Bullet, yEne15Bullet, 10, 10, bulletDown);
      H_line(xEne15Bullet, yEne15Bullet, 10, 0x00);
      if(enemy15Pace == 0){
        enemy15y++; 
        enemy15Pace = 1;
      }else if(enemy15Pace == 1){
        enemy15Pace = 2;  
        //yEne15Bullet++;
        if(yEne15Bullet >= 250){
          yEne15Bullet = 245;  
        }
      }else if(enemy15Pace == 2){
        enemy15y++;
        enemy15Pace = 3;
      }else if(enemy15Pace == 3){
        enemy15Pace = 4;  
        //yEne15Bullet++;
        if(yEne15Bullet >= 250){
          yEne15Bullet = 245;  
        }
      }else if(enemy15Pace == 4){
        enemy15y++;
        enemy15Pace =  5;
      }else if(enemy15Pace == 5){
        enemy15Pace =  0;
        //yEne15Bullet++;
        if(yEne15Bullet >= 250){
          yEne15Bullet = 245;  
        }
      }
      yEne15Bullet++;
      if(yEne15Bullet >=250){
        yEne15Bullet = 245;  
      }
   
      if((enemy15y == 209)||((xP1+29)>enemy15x && (xP1+1)<(enemy15x+28) && yP1<(enemy15y+30) && (yP1+30)>enemy15y)){
        enemy14Life = 1;
        enemy15Life = 1;
        enemy16Life = 1;
        enemy17Life = 1;
        P1Life = 1;
        LCD_Sprite(xP1, yP1, 30, 30, dead, 2, 1, 0, 0);
        mapeo_SD("gameOver.txt", 67, 75, 186);
        delay(3000);
        LCD_Clear(0x00);
        enemy1Life = 0;
        enemy1y = 0;
        enemy2y = 0;
        enemy3y = 0;
        enemy4y = 0;
        enemy5y = 0;
        enemy6y = 0;
        enemy7y = 0;
        enemy8y = 0;
        enemy9y = 0;
        enemy10y = 0;
        enemy11y = 0;
        enemy12y = 0;
        enemy13y = 0;
        enemy14y = 0;
        enemy15y = 0;
        enemy16y = 0;
        enemy17y = 0;
        enemy18y = 0;
        enemy19y = 0;
        enemy20y = 0;
        enemy21y = 0;
        enemy22y = 0;
        enemy23y = 0;
        tripleWave1 = 0;
        tripleWave2 = 0;
        tripleWave3 = 0;
        tripleWave4 = 0;
        tripleWave5 = 0;
        P1Life = 0;
        xP1 = 0;
        yP1 = 209;
        state = 0;
        turn = 0;
        mapeo_SD("menu.txt", 32, 25, 256);
      }
      if((xP1+29)>xEne15Bullet && (xP1+1)<(xEne15Bullet+10) && yP1<(yEne15Bullet+10) && (yP1+30)>yEne15Bullet){
        enemy14Life = 1;
        enemy15Life = 1;
        enemy16Life = 1;
        enemy17Life = 1;
        P1Life = 1;
        LCD_Sprite(xP1, yP1, 30, 30, dead, 2, 1, 0, 0);
        mapeo_SD("gameOver.txt", 67, 75, 186);
        delay(3000);
        LCD_Clear(0x00);
        enemy1Life = 0;
        enemy1y = 0;
        enemy2y = 0;
        enemy3y = 0;
        enemy4y = 0;
        enemy5y = 0;
        enemy6y = 0;
        enemy7y = 0;
        enemy8y = 0;
        enemy9y = 0;
        enemy10y = 0;
        enemy11y = 0;
        enemy12y = 0;
        enemy13y = 0;
        enemy14y = 0;
        enemy15y = 0;
        enemy16y = 0;
        enemy17y = 0;
        enemy18y = 0;
        enemy19y = 0;
        enemy20y = 0;
        enemy21y = 0;
        enemy22y = 0;
        enemy23y = 0;
        tripleWave1 = 0;
        tripleWave2 = 0;
        tripleWave3 = 0;
        tripleWave4 = 0;
        tripleWave5 = 0;
        P1Life = 0;
        xP1 = 0;
        yP1 = 209;
        state = 0;
        turn = 0;
        mapeo_SD("menu.txt", 32, 25, 256);
      }
    }
  //**********************************************************************************************************************************
  //Enemy #15 code
  //**********************************************************************************************************************************
  
  //**********************************************************************************************************************************
  //Enemy #16 code
  //**********************************************************************************************************************************
    if(enemy16Life == 0){
      if(enemy16y == 0){
        xEne16Bullet = enemy16x + 8;
        yEne16Bullet = enemy16y + 30;
      }
      if(enemy16y == 110){
        xEne16Bullet = enemy16x + 8;
        yEne16Bullet = enemy16y + 30; 
      }
      int animEne16 = (enemy16y/5)%2;
      LCD_Sprite(enemy16x, enemy16y, 28, 30, enemyTankForward, 2, animEne16, 0, 0);
      H_line(enemy16x, enemy16y-1, 28,0x00);
      LCD_Bitmap(xEne16Bullet, yEne16Bullet, 10, 10, bulletDown);
      H_line(xEne16Bullet, yEne16Bullet, 10, 0x00);
      if(enemy16Pace == 0){
        enemy16y++; 
        enemy16Pace = 1;
      }else if(enemy16Pace == 1){
        enemy16Pace = 2;  
        //yEne16Bullet++;
        if(yEne16Bullet >= 250){
          yEne16Bullet = 245;  
        }
      }else if(enemy16Pace == 2){
        enemy16y++;
        enemy16Pace = 3;
      }else if(enemy16Pace == 3){
        enemy16Pace = 4;  
        //yEne16Bullet++;
        if(yEne16Bullet >= 250){
          yEne16Bullet = 245;  
        }
      }else if(enemy16Pace == 4){
        enemy16y++;
        enemy16Pace =  5;
      }else if(enemy16Pace == 5){
        enemy16Pace =  0;
        //yEne16Bullet++;
        if(yEne16Bullet >= 250){
          yEne16Bullet = 245;  
        }
      }
      yEne16Bullet++;
      if(yEne16Bullet >=250){
        yEne16Bullet = 245;  
      }
   
      if((enemy16y == 209)||((xP1+29)>enemy16x && (xP1+1)<(enemy16x+28) && yP1<(enemy16y+30) && (yP1+30)>enemy16y)){
        enemy14Life = 1;
        enemy15Life = 1;
        enemy16Life = 1;
        enemy17Life = 1;
        P1Life = 1;
        LCD_Sprite(xP1, yP1, 30, 30, dead, 2, 1, 0, 0);
        mapeo_SD("gameOver.txt", 67, 75, 186);
        delay(3000);
        LCD_Clear(0x00);
        enemy1Life = 0;
        enemy1y = 0;
        enemy2y = 0;
        enemy3y = 0;
        enemy4y = 0;
        enemy5y = 0;
        enemy6y = 0;
        enemy7y = 0;
        enemy8y = 0;
        enemy9y = 0;
        enemy10y = 0;
        enemy11y = 0;
        enemy12y = 0;
        enemy13y = 0;
        enemy14y = 0;
        enemy15y = 0;
        enemy16y = 0;
        enemy17y = 0;
        enemy18y = 0;
        enemy19y = 0;
        enemy20y = 0;
        enemy21y = 0;
        enemy22y = 0;
        enemy23y = 0;
        tripleWave1 = 0;
        tripleWave2 = 0;
        tripleWave3 = 0;
        tripleWave4 = 0;
        tripleWave5 = 0;
        P1Life = 0;
        xP1 = 0;
        yP1 = 209;
        state = 0;
        turn = 0;
        mapeo_SD("menu.txt", 32, 25, 256);
      }
      if((xP1+29)>xEne16Bullet && (xP1+1)<(xEne16Bullet+10) && yP1<(yEne16Bullet+10) && (yP1+30)>yEne16Bullet){
        enemy14Life = 1;
        enemy15Life = 1;
        enemy16Life = 1;
        enemy17Life = 1;
        P1Life = 1;
        LCD_Sprite(xP1, yP1, 30, 30, dead, 2, 1, 0, 0);
        mapeo_SD("gameOver.txt", 67, 75, 186);
        delay(3000);
        LCD_Clear(0x00);
        enemy1Life = 0;
        enemy1y = 0;
        enemy2y = 0;
        enemy3y = 0;
        enemy4y = 0;
        enemy5y = 0;
        enemy6y = 0;
        enemy7y = 0;
        enemy8y = 0;
        enemy9y = 0;
        enemy10y = 0;
        enemy11y = 0;
        enemy12y = 0;
        enemy13y = 0;
        enemy14y = 0;
        enemy15y = 0;
        enemy16y = 0;
        enemy17y = 0;
        enemy18y = 0;
        enemy19y = 0;
        enemy20y = 0;
        enemy21y = 0;
        enemy22y = 0;
        enemy23y = 0;
        tripleWave1 = 0;
        tripleWave2 = 0;
        tripleWave3 = 0;
        tripleWave4 = 0;
        tripleWave5 = 0;
        P1Life = 0;
        xP1 = 0;
        yP1 = 209;
        state = 0;
        turn = 0;
        mapeo_SD("menu.txt", 32, 25, 256);
      }
    }
  //**********************************************************************************************************************************
  //Enemy #16 code
  //**********************************************************************************************************************************
  
  //**********************************************************************************************************************************
  //Enemy #17 code
  //**********************************************************************************************************************************
    if(enemy17Life == 0){
      if(enemy17y == 0){
        xEne17Bullet = enemy17x + 8;
        yEne17Bullet = enemy17y + 30;
      }
      if(enemy17y == 110){
        xEne17Bullet = enemy17x + 8;
        yEne17Bullet = enemy17y + 30; 
      }
      int animEne17 = (enemy17y/5)%2;
      LCD_Sprite(enemy17x, enemy17y, 28, 30, enemyTankForward, 2, animEne17, 0, 0);
      H_line(enemy17x, enemy17y-1, 28,0x00);
      LCD_Bitmap(xEne17Bullet, yEne17Bullet, 10, 10, bulletDown);
      H_line(xEne17Bullet, yEne17Bullet, 10, 0x00);
      if(enemy17Pace == 0){
        enemy17y++; 
        enemy17Pace = 1;
      }else if(enemy17Pace == 1){
        enemy17Pace = 2;  
        //yEne17Bullet++;
        if(yEne17Bullet >= 250){
          yEne17Bullet = 245;  
        }
      }else if(enemy17Pace == 2){
        enemy17y++;
        enemy17Pace = 3;
      }else if(enemy17Pace == 3){
        enemy17Pace = 4;  
        //yEne17Bullet++;
        if(yEne17Bullet >= 250){
          yEne17Bullet = 245;  
        }
      }else if(enemy17Pace == 4){
        enemy17y++;
        enemy17Pace =  5;
      }else if(enemy17Pace == 5){
        enemy17Pace =  0;
        //yEne17Bullet++;
        if(yEne17Bullet >= 250){
          yEne17Bullet = 245;  
        }
      }
      yEne17Bullet++;
      if(yEne17Bullet >=250){
        yEne17Bullet = 245;  
      }
   
      if((enemy17y == 209)||((xP1+29)>enemy17x && (xP1+1)<(enemy17x+28) && yP1<(enemy17y+30) && (yP1+30)>enemy17y)){
        enemy14Life = 1;
        enemy15Life = 1;
        enemy16Life = 1;
        enemy17Life = 1;
        P1Life = 1;
        LCD_Sprite(xP1, yP1, 30, 30, dead, 2, 1, 0, 0);
        mapeo_SD("gameOver.txt", 67, 75, 186);
        delay(3000);
        LCD_Clear(0x00);
        enemy1Life = 0;
        enemy1y = 0;
        enemy2y = 0;
        enemy3y = 0;
        enemy4y = 0;
        enemy5y = 0;
        enemy6y = 0;
        enemy7y = 0;
        enemy8y = 0;
        enemy9y = 0;
        enemy10y = 0;
        enemy11y = 0;
        enemy12y = 0;
        enemy13y = 0;
        enemy14y = 0;
        enemy15y = 0;
        enemy16y = 0;
        enemy17y = 0;
        enemy18y = 0;
        enemy19y = 0;
        enemy20y = 0;
        enemy21y = 0;
        enemy22y = 0;
        enemy23y = 0;
        tripleWave1 = 0;
        tripleWave2 = 0;
        tripleWave3 = 0;
        tripleWave4 = 0;
        tripleWave5 = 0;
        P1Life = 0;
        xP1 = 0;
        yP1 = 209;
        state = 0;
        turn = 0;
        mapeo_SD("menu.txt", 32, 25, 256);
      }
      if((xP1+29)>xEne17Bullet && (xP1+1)<(xEne17Bullet+10) && yP1<(yEne17Bullet+10) && (yP1+30)>yEne17Bullet){
        enemy14Life = 1;
        enemy15Life = 1;
        enemy16Life = 1;
        enemy17Life = 1;
        P1Life = 1;
        LCD_Sprite(xP1, yP1, 30, 30, dead, 2, 1, 0, 0);
        mapeo_SD("gameOver.txt", 67, 75, 186);
        delay(3000);
        LCD_Clear(0x00);
        enemy1Life = 0;
        enemy1y = 0;
        enemy2y = 0;
        enemy3y = 0;
        enemy4y = 0;
        enemy5y = 0;
        enemy6y = 0;
        enemy7y = 0;
        enemy8y = 0;
        enemy9y = 0;
        enemy10y = 0;
        enemy11y = 0;
        enemy12y = 0;
        enemy13y = 0;
        enemy14y = 0;
        enemy15y = 0;
        enemy16y = 0;
        enemy17y = 0;
        enemy18y = 0;
        enemy19y = 0;
        enemy20y = 0;
        enemy21y = 0;
        enemy22y = 0;
        enemy23y = 0;
        tripleWave1 = 0;
        tripleWave2 = 0;
        tripleWave3 = 0;
        tripleWave4 = 0;
        tripleWave5 = 0;
        P1Life = 0;
        xP1 = 0;
        yP1 = 209;
        state = 0;
        turn = 0;
        mapeo_SD("menu.txt", 32, 25, 256);
      }
    }
  //**********************************************************************************************************************************
  //Enemy #17 code
  //**********************************************************************************************************************************
  
  //**********************************************************************************************************************************
  //Enemy #18 code
  //**********************************************************************************************************************************
    if(enemy18Life == 0){
      if(enemy18y == 0){
        xEne18Bullet = enemy18x + 8;
        yEne18Bullet = enemy18y + 30;
      }
      if(enemy18y == 110){
        xEne18Bullet = enemy18x + 8;
        yEne18Bullet = enemy18y + 30; 
      }
      int animEne18 = (enemy18y/5)%2;
      LCD_Sprite(enemy18x, enemy18y, 28, 30, enemyTankForward, 2, animEne18, 0, 0);
      H_line(enemy18x, enemy18y-1, 28,0x00);
      LCD_Bitmap(xEne18Bullet, yEne18Bullet, 10, 10, bulletDown);
      H_line(xEne18Bullet, yEne18Bullet, 10, 0x00);
      if(enemy18Pace == 0){
        enemy18y++; 
        enemy18Pace = 1;
      }else if(enemy18Pace == 1){
        enemy18Pace = 2;  
        //yEne18Bullet++;
        if(yEne18Bullet >= 250){
          yEne18Bullet = 245;  
        }
      }else if(enemy18Pace == 2){
        enemy18y++;
        enemy18Pace = 3;
      }else if(enemy18Pace == 3){
        enemy18Pace = 4;  
        //yEne18Bullet++;
        if(yEne18Bullet >= 250){
          yEne18Bullet = 245;  
        }
      }else if(enemy18Pace == 4){
        enemy18y++;
        enemy18Pace =  5;
      }else if(enemy18Pace == 5){
        enemy18Pace =  0;
        //yEne18Bullet++;
        if(yEne18Bullet >= 250){
          yEne18Bullet = 245;  
        }
      }
      yEne18Bullet++;
      if(yEne18Bullet >=250){
        yEne18Bullet = 245;  
      }
   
      if((enemy18y == 209)||((xP1+29)>enemy18x && (xP1+1)<(enemy18x+28) && yP1<(enemy18y+30) && (yP1+30)>enemy18y)){
        enemy18Life = 1;
        enemy19Life = 1;
        enemy20Life = 1;
        enemy21Life = 1;
        enemy22Life = 1;
        enemy23Life = 1;
        P1Life = 1;
        LCD_Sprite(xP1, yP1, 30, 30, dead, 2, 1, 0, 0);
        mapeo_SD("gameOver.txt", 67, 75, 186);
        delay(3000);
        LCD_Clear(0x00);
        enemy1Life = 0;
        enemy1y = 0;
        enemy2y = 0;
        enemy3y = 0;
        enemy4y = 0;
        enemy5y = 0;
        enemy6y = 0;
        enemy7y = 0;
        enemy8y = 0;
        enemy9y = 0;
        enemy10y = 0;
        enemy11y = 0;
        enemy12y = 0;
        enemy13y = 0;
        enemy14y = 0;
        enemy15y = 0;
        enemy16y = 0;
        enemy17y = 0;
        enemy18y = 0;
        enemy19y = 0;
        enemy20y = 0;
        enemy21y = 0;
        enemy22y = 0;
        enemy23y = 0;
        tripleWave1 = 0;
        tripleWave2 = 0;
        tripleWave3 = 0;
        tripleWave4 = 0;
        tripleWave5 = 0;
        P1Life = 0;
        xP1 = 0;
        yP1 = 209;
        state = 0;
        turn = 0;
        mapeo_SD("menu.txt", 32, 25, 256);
      }
      if((xP1+29)>xEne18Bullet && (xP1+1)<(xEne18Bullet+10) && yP1<(yEne18Bullet+10) && (yP1+30)>yEne18Bullet){
        enemy18Life = 1;
        enemy19Life = 1;
        enemy20Life = 1;
        enemy21Life = 1;
        enemy22Life = 1;
        enemy23Life = 1;
        P1Life = 1;
        LCD_Sprite(xP1, yP1, 30, 30, dead, 2, 1, 0, 0);
        mapeo_SD("gameOver.txt", 67, 75, 186);
        delay(3000);
        LCD_Clear(0x00);
        enemy1Life = 0;
        enemy1y = 0;
        enemy2y = 0;
        enemy3y = 0;
        enemy4y = 0;
        enemy5y = 0;
        enemy6y = 0;
        enemy7y = 0;
        enemy8y = 0;
        enemy9y = 0;
        enemy10y = 0;
        enemy11y = 0;
        enemy12y = 0;
        enemy13y = 0;
        enemy14y = 0;
        enemy15y = 0;
        enemy16y = 0;
        enemy17y = 0;
        enemy18y = 0;
        enemy19y = 0;
        enemy20y = 0;
        enemy21y = 0;
        enemy22y = 0;
        enemy23y = 0;
        tripleWave1 = 0;
        tripleWave2 = 0;
        tripleWave3 = 0;
        tripleWave4 = 0;
        tripleWave5 = 0;
        P1Life = 0;
        xP1 = 0;
        yP1 = 209;
        state = 0;
        turn = 0;
        mapeo_SD("menu.txt", 32, 25, 256);
      }
    }
  //**********************************************************************************************************************************
  //Enemy #18 code
  //**********************************************************************************************************************************
  
  //**********************************************************************************************************************************
  //Enemy #19 code
  //**********************************************************************************************************************************
    if(enemy19Life == 0){
      if(enemy19y == 0){
        xEne19Bullet = enemy19x + 8;
        yEne19Bullet = enemy19y + 30;
      }
      if(enemy19y == 110){
        xEne19Bullet = enemy19x + 8;
        yEne19Bullet = enemy19y + 30; 
      }
      int animEne19 = (enemy19y/5)%2;
      LCD_Sprite(enemy19x, enemy19y, 28, 30, enemyTankForward, 2, animEne19, 0, 0);
      H_line(enemy19x, enemy19y-1, 28,0x00);
      LCD_Bitmap(xEne19Bullet, yEne19Bullet, 10, 10, bulletDown);
      H_line(xEne19Bullet, yEne19Bullet, 10, 0x00);
      if(enemy19Pace == 0){
        enemy19y++; 
        enemy19Pace = 1;
      }else if(enemy19Pace == 1){
        enemy19Pace = 2;  
        //yEne19Bullet++;
        if(yEne19Bullet >= 250){
          yEne19Bullet = 245;  
        }
      }else if(enemy19Pace == 2){
        enemy19y++;
        enemy19Pace = 3;
      }else if(enemy19Pace == 3){
        enemy19Pace = 4;  
        //yEne19Bullet++;
        if(yEne19Bullet >= 250){
          yEne19Bullet = 245;  
        }
      }else if(enemy19Pace == 4){
        enemy19y++;
        enemy19Pace =  5;
      }else if(enemy19Pace == 5){
        enemy19Pace =  0;
        //yEne19Bullet++;
        if(yEne19Bullet >= 250){
          yEne19Bullet = 245;  
        }
      }
      yEne19Bullet++;
      if(yEne19Bullet >=250){
        yEne19Bullet = 245;  
      }
   
      if((enemy19y == 209)||((xP1+29)>enemy19x && (xP1+1)<(enemy19x+28) && yP1<(enemy19y+30) && (yP1+30)>enemy19y)){
        enemy18Life = 1;
        enemy19Life = 1;
        enemy20Life = 1;
        enemy21Life = 1;
        enemy22Life = 1;
        enemy23Life = 1;
        P1Life = 1;
        LCD_Sprite(xP1, yP1, 30, 30, dead, 2, 1, 0, 0);
        mapeo_SD("gameOver.txt", 67, 75, 186);
        delay(3000);
        LCD_Clear(0x00);
        enemy1Life = 0;
        enemy1y = 0;
        enemy2y = 0;
        enemy3y = 0;
        enemy4y = 0;
        enemy5y = 0;
        enemy6y = 0;
        enemy7y = 0;
        enemy8y = 0;
        enemy9y = 0;
        enemy10y = 0;
        enemy11y = 0;
        enemy12y = 0;
        enemy13y = 0;
        enemy14y = 0;
        enemy15y = 0;
        enemy16y = 0;
        enemy17y = 0;
        enemy18y = 0;
        enemy19y = 0;
        enemy20y = 0;
        enemy21y = 0;
        enemy22y = 0;
        enemy23y = 0;
        tripleWave1 = 0;
        tripleWave2 = 0;
        tripleWave3 = 0;
        tripleWave4 = 0;
        tripleWave5 = 0;
        P1Life = 0;
        xP1 = 0;
        yP1 = 209;
        state = 0;
        turn = 0;
        mapeo_SD("menu.txt", 32, 25, 256);
      }
      if((xP1+29)>xEne19Bullet && (xP1+1)<(xEne19Bullet+10) && yP1<(yEne19Bullet+10) && (yP1+30)>yEne19Bullet){
        enemy18Life = 1;
        enemy19Life = 1;
        enemy20Life = 1;
        enemy21Life = 1;
        enemy22Life = 1;
        enemy23Life = 1;
        P1Life = 1;
        LCD_Sprite(xP1, yP1, 30, 30, dead, 2, 1, 0, 0);
        mapeo_SD("gameOver.txt", 67, 75, 186);
        delay(3000);
        LCD_Clear(0x00);
        enemy1Life = 0;
        enemy1y = 0;
        enemy2y = 0;
        enemy3y = 0;
        enemy4y = 0;
        enemy5y = 0;
        enemy6y = 0;
        enemy7y = 0;
        enemy8y = 0;
        enemy9y = 0;
        enemy10y = 0;
        enemy11y = 0;
        enemy12y = 0;
        enemy13y = 0;
        enemy14y = 0;
        enemy15y = 0;
        enemy16y = 0;
        enemy17y = 0;
        enemy18y = 0;
        enemy19y = 0;
        enemy20y = 0;
        enemy21y = 0;
        enemy22y = 0;
        enemy23y = 0;
        tripleWave1 = 0;
        tripleWave2 = 0;
        tripleWave3 = 0;
        tripleWave4 = 0;
        tripleWave5 = 0;
        P1Life = 0;
        xP1 = 0;
        yP1 = 209;
        state = 0;
        turn = 0;
        mapeo_SD("menu.txt", 32, 25, 256);
      }
    }
  //**********************************************************************************************************************************
  //Enemy #19 code
  //**********************************************************************************************************************************
  
  //**********************************************************************************************************************************
  //Enemy #20 code
  //**********************************************************************************************************************************
    if(enemy20Life == 0){
      if(enemy20y == 0){
        xEne20Bullet = enemy20x + 8;
        yEne20Bullet = enemy20y + 30;
      }
      if(enemy20y == 110){
        xEne20Bullet = enemy20x + 8;
        yEne20Bullet = enemy20y + 30; 
      }
      int animEne20 = (enemy20y/5)%2;
      LCD_Sprite(enemy20x, enemy20y, 28, 30, enemyTankForward, 2, animEne20, 0, 0);
      H_line(enemy20x, enemy20y-1, 28,0x00);
      LCD_Bitmap(xEne20Bullet, yEne20Bullet, 10, 10, bulletDown);
      H_line(xEne20Bullet, yEne20Bullet, 10, 0x00);
      if(enemy20Pace == 0){
        enemy20y++; 
        enemy20Pace = 1;
      }else if(enemy20Pace == 1){
        enemy20Pace = 2;  
        //yEne20Bullet++;
        if(yEne20Bullet >= 250){
          yEne20Bullet = 245;  
        }
      }else if(enemy20Pace == 2){
        enemy20y++;
        enemy20Pace = 3;
      }else if(enemy20Pace == 3){
        enemy20Pace = 4;  
        //yEne20Bullet++;
        if(yEne20Bullet >= 250){
          yEne20Bullet = 245;  
        }
      }else if(enemy20Pace == 4){
        enemy20y++;
        enemy20Pace =  5;
      }else if(enemy20Pace == 5){
        enemy20Pace =  0;
        //yEne20Bullet++;
        if(yEne20Bullet >= 250){
          yEne20Bullet = 245;  
        }
      }
      yEne20Bullet++;
      if(yEne20Bullet >=250){
        yEne20Bullet = 245;  
      }
   
      if((enemy20y == 209)||((xP1+29)>enemy20x && (xP1+1)<(enemy20x+28) && yP1<(enemy20y+30) && (yP1+30)>enemy20y)){
        enemy18Life = 1;
        enemy19Life = 1;
        enemy20Life = 1;
        enemy21Life = 1;
        enemy22Life = 1;
        enemy23Life = 1;
        P1Life = 1;
        LCD_Sprite(xP1, yP1, 30, 30, dead, 2, 1, 0, 0);
        mapeo_SD("gameOver.txt", 67, 75, 186);
        delay(3000);
        LCD_Clear(0x00);
        enemy1Life = 0;
        enemy1y = 0;
        enemy2y = 0;
        enemy3y = 0;
        enemy4y = 0;
        enemy5y = 0;
        enemy6y = 0;
        enemy7y = 0;
        enemy8y = 0;
        enemy9y = 0;
        enemy10y = 0;
        enemy11y = 0;
        enemy12y = 0;
        enemy13y = 0;
        enemy14y = 0;
        enemy15y = 0;
        enemy16y = 0;
        enemy17y = 0;
        enemy18y = 0;
        enemy19y = 0;
        enemy20y = 0;
        enemy21y = 0;
        enemy22y = 0;
        enemy23y = 0;
        tripleWave1 = 0;
        tripleWave2 = 0;
        tripleWave3 = 0;
        tripleWave4 = 0;
        tripleWave5 = 0;
        P1Life = 0;
        xP1 = 0;
        yP1 = 209;
        state = 0;
        turn = 0;
        mapeo_SD("menu.txt", 32, 25, 256);
      }
      if((xP1+29)>xEne20Bullet && (xP1+1)<(xEne20Bullet+10) && yP1<(yEne20Bullet+10) && (yP1+30)>yEne20Bullet){
        enemy18Life = 1;
        enemy19Life = 1;
        enemy20Life = 1;
        enemy21Life = 1;
        enemy22Life = 1;
        enemy23Life = 1;
        P1Life = 1;
        LCD_Sprite(xP1, yP1, 30, 30, dead, 2, 1, 0, 0);
        mapeo_SD("gameOver.txt", 67, 75, 186);
        delay(3000);
        LCD_Clear(0x00);
        enemy1Life = 0;
        enemy1y = 0;
        enemy2y = 0;
        enemy3y = 0;
        enemy4y = 0;
        enemy5y = 0;
        enemy6y = 0;
        enemy7y = 0;
        enemy8y = 0;
        enemy9y = 0;
        enemy10y = 0;
        enemy11y = 0;
        enemy12y = 0;
        enemy13y = 0;
        enemy14y = 0;
        enemy15y = 0;
        enemy16y = 0;
        enemy17y = 0;
        enemy18y = 0;
        enemy19y = 0;
        enemy20y = 0;
        enemy21y = 0;
        enemy22y = 0;
        enemy23y = 0;
        tripleWave1 = 0;
        tripleWave2 = 0;
        tripleWave3 = 0;
        tripleWave4 = 0;
        tripleWave5 = 0;
        P1Life = 0;
        xP1 = 0;
        yP1 = 209;
        state = 0;
        turn = 0;
        mapeo_SD("menu.txt", 32, 25, 256);
      }
    }
  //**********************************************************************************************************************************
  //Enemy #20 code
  //**********************************************************************************************************************************
  
  //**********************************************************************************************************************************
  //Enemy #21 code
  //**********************************************************************************************************************************
    if(enemy21Life == 0){
      if(enemy21y == 0){
        xEne21Bullet = enemy21x + 8;
        yEne21Bullet = enemy21y + 30;
      }
      if(enemy21y == 110){
        xEne21Bullet = enemy21x + 8;
        yEne21Bullet = enemy21y + 30; 
      }
      int animEne21 = (enemy21y/5)%2;
      LCD_Sprite(enemy21x, enemy21y, 28, 30, enemyTankForward, 2, animEne21, 0, 0);
      H_line(enemy21x, enemy21y-1, 28,0x00);
      LCD_Bitmap(xEne21Bullet, yEne21Bullet, 10, 10, bulletDown);
      H_line(xEne21Bullet, yEne21Bullet, 10, 0x00);
      if(enemy21Pace == 0){
        enemy21y++; 
        enemy21Pace = 1;
      }else if(enemy21Pace == 1){
        enemy21Pace = 2;  
        //yEne21Bullet++;
        if(yEne21Bullet >= 250){
          yEne21Bullet = 245;  
        }
      }else if(enemy21Pace == 2){
        enemy21y++;
        enemy21Pace = 3;
      }else if(enemy21Pace == 3){
        enemy21Pace = 4;  
        //yEne21Bullet++;
        if(yEne21Bullet >= 250){
          yEne21Bullet = 245;  
        }
      }else if(enemy21Pace == 4){
        enemy21y++;
        enemy21Pace =  5;
      }else if(enemy21Pace == 5){
        enemy21Pace =  0;
        //yEne21Bullet++;
        if(yEne21Bullet >= 250){
          yEne21Bullet = 245;  
        }
      }
      yEne21Bullet++;
      if(yEne21Bullet >=250){
        yEne21Bullet = 245;  
      }
   
      if((enemy21y == 209)||((xP1+29)>enemy21x && (xP1+1)<(enemy21x+28) && yP1<(enemy21y+30) && (yP1+30)>enemy21y)){
        enemy18Life = 1;
        enemy19Life = 1;
        enemy20Life = 1;
        enemy21Life = 1;
        enemy22Life = 1;
        enemy23Life = 1;
        P1Life = 1;
        LCD_Sprite(xP1, yP1, 30, 30, dead, 2, 1, 0, 0);
        mapeo_SD("gameOver.txt", 67, 75, 186);
        delay(3000);
        LCD_Clear(0x00);
        enemy1Life = 0;
        enemy1y = 0;
        enemy2y = 0;
        enemy3y = 0;
        enemy4y = 0;
        enemy5y = 0;
        enemy6y = 0;
        enemy7y = 0;
        enemy8y = 0;
        enemy9y = 0;
        enemy10y = 0;
        enemy11y = 0;
        enemy12y = 0;
        enemy13y = 0;
        enemy14y = 0;
        enemy15y = 0;
        enemy16y = 0;
        enemy17y = 0;
        enemy18y = 0;
        enemy19y = 0;
        enemy20y = 0;
        enemy21y = 0;
        enemy22y = 0;
        enemy23y = 0;
        tripleWave1 = 0;
        tripleWave2 = 0;
        tripleWave3 = 0;
        tripleWave4 = 0;
        tripleWave5 = 0;
        P1Life = 0;
        xP1 = 0;
        yP1 = 209;
        state = 0;
        turn = 0;
        mapeo_SD("menu.txt", 32, 25, 256);
      }
      if((xP1+29)>xEne21Bullet && (xP1+1)<(xEne21Bullet+10) && yP1<(yEne21Bullet+10) && (yP1+30)>yEne21Bullet){
        enemy18Life = 1;
        enemy19Life = 1;
        enemy20Life = 1;
        enemy21Life = 1;
        enemy22Life = 1;
        enemy23Life = 1;
        P1Life = 1;
        LCD_Sprite(xP1, yP1, 30, 30, dead, 2, 1, 0, 0);
        mapeo_SD("gameOver.txt", 67, 75, 186);
        delay(3000);
        LCD_Clear(0x00);
        enemy1Life = 0;
        enemy1y = 0;
        enemy2y = 0;
        enemy3y = 0;
        enemy4y = 0;
        enemy5y = 0;
        enemy6y = 0;
        enemy7y = 0;
        enemy8y = 0;
        enemy9y = 0;
        enemy10y = 0;
        enemy11y = 0;
        enemy12y = 0;
        enemy13y = 0;
        enemy14y = 0;
        enemy15y = 0;
        enemy16y = 0;
        enemy17y = 0;
        enemy18y = 0;
        enemy19y = 0;
        enemy20y = 0;
        enemy21y = 0;
        enemy22y = 0;
        enemy23y = 0;
        tripleWave1 = 0;
        tripleWave2 = 0;
        tripleWave3 = 0;
        tripleWave4 = 0;
        tripleWave5 = 0;
        P1Life = 0;
        xP1 = 0;
        yP1 = 209;
        state = 0;
        turn = 0;
        mapeo_SD("menu.txt", 32, 25, 256);
      }
    }
  //**********************************************************************************************************************************
  //Enemy #21 code
  //**********************************************************************************************************************************
  
  //**********************************************************************************************************************************
  //Enemy #22 code
  //**********************************************************************************************************************************
    if(enemy22Life == 0){
      if(enemy22y == 0){
        xEne22Bullet = enemy22x + 8;
        yEne22Bullet = enemy22y + 30;
      }
      if(enemy22y == 110){
        xEne22Bullet = enemy22x + 8;
        yEne22Bullet = enemy22y + 30; 
      }
      int animEne22 = (enemy22y/5)%2;
      LCD_Sprite(enemy22x, enemy22y, 28, 30, enemyTankForward, 2, animEne22, 0, 0);
      H_line(enemy22x, enemy22y-1, 28,0x00);
      LCD_Bitmap(xEne22Bullet, yEne22Bullet, 10, 10, bulletDown);
      H_line(xEne22Bullet, yEne22Bullet, 10, 0x00);
      if(enemy22Pace == 0){
        enemy22y++; 
        enemy22Pace = 1;
      }else if(enemy22Pace == 1){
        enemy22Pace = 2;  
        //yEne22Bullet++;
        if(yEne22Bullet >= 250){
          yEne22Bullet = 245;  
        }
      }else if(enemy22Pace == 2){
        enemy22y++;
        enemy22Pace = 3;
      }else if(enemy22Pace == 3){
        enemy22Pace = 4;  
        //yEne22Bullet++;
        if(yEne22Bullet >= 250){
          yEne22Bullet = 245;  
        }
      }else if(enemy22Pace == 4){
        enemy22y++;
        enemy22Pace =  5;
      }else if(enemy22Pace == 5){
        enemy22Pace =  0;
        //yEne22Bullet++;
        if(yEne22Bullet >= 250){
          yEne22Bullet = 245;  
        }
      }
      yEne22Bullet++;
      if(yEne22Bullet >=250){
        yEne22Bullet = 245;  
      }
   
      if((enemy22y == 209)||((xP1+29)>enemy22x && (xP1+1)<(enemy22x+28) && yP1<(enemy22y+30) && (yP1+30)>enemy22y)){
        enemy18Life = 1;
        enemy19Life = 1;
        enemy20Life = 1;
        enemy21Life = 1;
        enemy22Life = 1;
        enemy23Life = 1;
        P1Life = 1;
        LCD_Sprite(xP1, yP1, 30, 30, dead, 2, 1, 0, 0);
        mapeo_SD("gameOver.txt", 67, 75, 186);
        delay(3000);
        LCD_Clear(0x00);
        enemy1Life = 0;
        enemy1y = 0;
        enemy2y = 0;
        enemy3y = 0;
        enemy4y = 0;
        enemy5y = 0;
        enemy6y = 0;
        enemy7y = 0;
        enemy8y = 0;
        enemy9y = 0;
        enemy10y = 0;
        enemy11y = 0;
        enemy12y = 0;
        enemy13y = 0;
        enemy14y = 0;
        enemy15y = 0;
        enemy16y = 0;
        enemy17y = 0;
        enemy18y = 0;
        enemy19y = 0;
        enemy20y = 0;
        enemy21y = 0;
        enemy22y = 0;
        enemy23y = 0;
        tripleWave1 = 0;
        tripleWave2 = 0;
        tripleWave3 = 0;
        tripleWave4 = 0;
        tripleWave5 = 0;
        P1Life = 0;
        xP1 = 0;
        yP1 = 209;
        state = 0;
        turn = 0;
        mapeo_SD("menu.txt", 32, 25, 256);
      }
      if((xP1+29)>xEne22Bullet && (xP1+1)<(xEne22Bullet+10) && yP1<(yEne22Bullet+10) && (yP1+30)>yEne22Bullet){
        enemy18Life = 1;
        enemy19Life = 1;
        enemy20Life = 1;
        enemy21Life = 1;
        enemy22Life = 1;
        enemy23Life = 1;
        P1Life = 1;
        LCD_Sprite(xP1, yP1, 30, 30, dead, 2, 1, 0, 0);
        mapeo_SD("gameOver.txt", 67, 75, 186);
        delay(3000);
        LCD_Clear(0x00);
        enemy1Life = 0;
        enemy1y = 0;
        enemy2y = 0;
        enemy3y = 0;
        enemy4y = 0;
        enemy5y = 0;
        enemy6y = 0;
        enemy7y = 0;
        enemy8y = 0;
        enemy9y = 0;
        enemy10y = 0;
        enemy11y = 0;
        enemy12y = 0;
        enemy13y = 0;
        enemy14y = 0;
        enemy15y = 0;
        enemy16y = 0;
        enemy17y = 0;
        enemy18y = 0;
        enemy19y = 0;
        enemy20y = 0;
        enemy21y = 0;
        enemy22y = 0;
        enemy23y = 0;
        tripleWave1 = 0;
        tripleWave2 = 0;
        tripleWave3 = 0;
        tripleWave4 = 0;
        tripleWave5 = 0;
        P1Life = 0;
        xP1 = 0;
        yP1 = 209;
        state = 0;
        turn = 0;
        mapeo_SD("menu.txt", 32, 25, 256);
      }
    }
  //**********************************************************************************************************************************
  //Enemy #22 code
  //**********************************************************************************************************************************
  
  //**********************************************************************************************************************************
  //Enemy #23 code
  //**********************************************************************************************************************************
    if(enemy23Life == 0){
      if(enemy23y == 0){
        xEne23Bullet = enemy23x + 8;
        yEne23Bullet = enemy23y + 30;
      }
      if(enemy23y == 110){
        xEne23Bullet = enemy23x + 8;
        yEne23Bullet = enemy23y + 30; 
      }
      int animEne23 = (enemy23y/5)%2;
      LCD_Sprite(enemy23x, enemy23y, 28, 30, enemyTankForward, 2, animEne23, 0, 0);
      H_line(enemy23x, enemy23y-1, 28,0x00);
      LCD_Bitmap(xEne23Bullet, yEne23Bullet, 10, 10, bulletDown);
      H_line(xEne23Bullet, yEne23Bullet, 10, 0x00);
      if(enemy23Pace == 0){
        enemy23y++; 
        enemy23Pace = 1;
      }else if(enemy23Pace == 1){
        enemy23Pace = 2;  
        //yEne23Bullet++;
        if(yEne23Bullet >= 250){
          yEne23Bullet = 245;  
        }
      }else if(enemy23Pace == 2){
        enemy23y++;
        enemy23Pace = 3;
      }else if(enemy23Pace == 3){
        enemy23Pace = 4;  
        //yEne23Bullet++;
        if(yEne23Bullet >= 250){
          yEne23Bullet = 245;  
        }
      }else if(enemy23Pace == 4){
        enemy23y++;
        enemy23Pace =  5;
      }else if(enemy23Pace == 5){
        enemy23Pace =  0;
        //yEne23Bullet++;
        if(yEne23Bullet >= 250){
          yEne23Bullet = 245;  
        }
      }
      yEne23Bullet++;
      if(yEne23Bullet >=250){
        yEne23Bullet = 245;  
      }
   
      if((enemy23y == 209)||((xP1+29)>enemy23x && (xP1+1)<(enemy23x+28) && yP1<(enemy23y+30) && (yP1+30)>enemy23y)){
        enemy18Life = 1;
        enemy19Life = 1;
        enemy20Life = 1;
        enemy21Life = 1;
        enemy22Life = 1;
        enemy23Life = 1;
        P1Life = 1;
        LCD_Sprite(xP1, yP1, 30, 30, dead, 2, 1, 0, 0);
        mapeo_SD("gameOver.txt", 67, 75, 186);
        delay(3000);
        LCD_Clear(0x00);
        enemy1Life = 0;
        enemy1y = 0;
        enemy2y = 0;
        enemy3y = 0;
        enemy4y = 0;
        enemy5y = 0;
        enemy6y = 0;
        enemy7y = 0;
        enemy8y = 0;
        enemy9y = 0;
        enemy10y = 0;
        enemy11y = 0;
        enemy12y = 0;
        enemy13y = 0;
        enemy14y = 0;
        enemy15y = 0;
        enemy16y = 0;
        enemy17y = 0;
        enemy18y = 0;
        enemy19y = 0;
        enemy20y = 0;
        enemy21y = 0;
        enemy22y = 0;
        enemy23y = 0;
        tripleWave1 = 0;
        tripleWave2 = 0;
        tripleWave3 = 0;
        tripleWave4 = 0;
        tripleWave5 = 0;
        P1Life = 0;
        xP1 = 0;
        yP1 = 209;
        state = 0;
        turn = 0;
        mapeo_SD("menu.txt", 32, 25, 256);
      }
      if((xP1+29)>xEne23Bullet && (xP1+1)<(xEne23Bullet+10) && yP1<(yEne23Bullet+10) && (yP1+30)>yEne23Bullet){
        enemy18Life = 1;
        enemy19Life = 1;
        enemy20Life = 1;
        enemy21Life = 1;
        enemy22Life = 1;
        enemy23Life = 1;
        P1Life = 1;
        LCD_Sprite(xP1, yP1, 30, 30, dead, 2, 1, 0, 0);
        mapeo_SD("gameOver.txt", 67, 75, 186);
        delay(3000);
        LCD_Clear(0x00);
        enemy1Life = 0;
        enemy1y = 0;
        enemy2y = 0;
        enemy3y = 0;
        enemy4y = 0;
        enemy5y = 0;
        enemy6y = 0;
        enemy7y = 0;
        enemy8y = 0;
        enemy9y = 0;
        enemy10y = 0;
        enemy11y = 0;
        enemy12y = 0;
        enemy13y = 0;
        enemy14y = 0;
        enemy15y = 0;
        enemy16y = 0;
        enemy17y = 0;
        enemy18y = 0;
        enemy19y = 0;
        enemy20y = 0;
        enemy21y = 0;
        enemy22y = 0;
        enemy23y = 0;
        tripleWave1 = 0;
        tripleWave2 = 0;
        tripleWave3 = 0;
        tripleWave4 = 0;
        tripleWave5 = 0;
        P1Life = 0;
        xP1 = 0;
        yP1 = 209;
        state = 0;
        turn = 0;
        mapeo_SD("menu.txt", 32, 25, 256);
      }
    }
  //**********************************************************************************************************************************
  //Enemy #23 code
  //**********************************************************************************************************************************
  
  //**********************************************************************************************************************************
  //Player #1 bullet collision
  //**********************************************************************************************************************************  
    if(yP1bullet <= (enemy1y+28) && (xP1bullet+9)>enemy1x && (xP1bullet+1)<(enemy1x+28) && enemy1Life == 0){//Enemy #1
      enemy1Life = 1;
      enemy2Life = 0;
      LCD_Sprite(enemy1x, enemy1y, 30, 30, dead, 2, 1, 0, 0);
      LCD_Sprite(xEne1Bullet-10, yEne1Bullet-10, 30, 30, dead, 2, 0, 0, 0);
      delay(15);
      for(int i = 0; i<31; i++){
        H_line(enemy1x, enemy1y+i-1, 30, 0x00);
        H_line(xEne1Bullet-10, yEne1Bullet-11+i, 30, 0x00);
      }
      for(int i = 0; i <= 11; i++){
        V_line(xP1bullet+10-i, yP1bullet, 10, 0x00);  
      }
      triggerP1 = 0;
      xP1bullet = -10;
      yP1bullet = -10;
    }
  
    if(yP1bullet <= (enemy2y+28) && (xP1bullet+9)>enemy2x && (xP1bullet+1)<(enemy2x+28) && enemy2Life == 0){//Enemy #2
      enemy2Life = 1;
      enemy3Life = 0;
      LCD_Sprite(enemy2x, enemy2y, 30, 30, dead, 2, 1, 0, 0);
      LCD_Sprite(xEne2Bullet-10, yEne2Bullet-10, 30, 30, dead, 2, 0, 0, 0);
      delay(15);
      for(int i = 0; i<31; i++){
        H_line(enemy2x, enemy2y+i-1, 30, 0x00);
        H_line(xEne2Bullet-10, yEne2Bullet-11+i, 30, 0x00);
      }
      for(int i = 0; i <= 11; i++){
        V_line(xP1bullet+10-i, yP1bullet, 10, 0x00);  
      }
      triggerP1 = 0;
      xP1bullet = -10;
      yP1bullet = -10;
    }
  
    if(yP1bullet <= (enemy3y+28) && (xP1bullet+9)>enemy3x && (xP1bullet+1)<(enemy3x+28) && enemy3Life == 0){//Enemy #3
      enemy3Life = 1;
      enemy4Life = 0;
      enemy5Life = 0;
      enemy6Life = 0;
      LCD_Sprite(enemy3x, enemy3y, 30, 30, dead, 2, 1, 0, 0);
      LCD_Sprite(xEne3Bullet-10, yEne3Bullet-10, 30, 30, dead, 2, 0, 0, 0);
      delay(15);
      for(int i = 0; i<31; i++){
        H_line(enemy3x, enemy3y+i-1, 30, 0x00);
        H_line(xEne3Bullet-10, yEne3Bullet-11+i, 30, 0x00);
      }
      for(int i = 0; i <= 11; i++){
        V_line(xP1bullet+10-i, yP1bullet, 10, 0x00);  
      }
      triggerP1 = 0;
      xP1bullet = -10;
      yP1bullet = -10;
    }
  
    if(yP1bullet <= (enemy4y+28) && (xP1bullet+9)>enemy4x && (xP1bullet+1)<(enemy4x+28) && enemy4Life == 0){//Enemy #4
      enemy4Life = 1;
      tripleWave1++;
      if(tripleWave1 == 3){
        tripleWave1 = 0;
        enemy7Life = 0;
        enemy8Life = 0;
        enemy9Life = 0;
      }
      LCD_Sprite(enemy4x, enemy4y, 30, 30, dead, 2, 1, 0, 0);
      LCD_Sprite(xEne4Bullet-10, yEne4Bullet-10, 30, 30, dead, 2, 0, 0, 0);
      delay(15);
      for(int i = 0; i<31; i++){
        H_line(enemy4x, enemy4y+i-1, 30, 0x00);
        H_line(xEne4Bullet-10, yEne4Bullet-11+i, 30, 0x00);
      }
      for(int i = 0; i <= 11; i++){
        V_line(xP1bullet+10-i, yP1bullet, 10, 0x00);  
      }
      triggerP1 = 0;
      xP1bullet = -10;
      yP1bullet = -10;
    }
  
    if(yP1bullet <= (enemy5y+28) && (xP1bullet+9)>enemy5x && (xP1bullet+1)<(enemy5x+28) && enemy5Life == 0){//Enemy #5
      enemy5Life = 1;
      tripleWave1++;
      if(tripleWave1 == 3){
        tripleWave1 = 0;
        enemy7Life = 0;
        enemy8Life = 0;
        enemy9Life = 0;
      }
      LCD_Sprite(enemy5x, enemy5y, 30, 30, dead, 2, 1, 0, 0);
      LCD_Sprite(xEne5Bullet-10, yEne5Bullet-10, 30, 30, dead, 2, 0, 0, 0);
      delay(15);
      for(int i = 0; i<31; i++){
        H_line(enemy5x, enemy5y+i-1, 30, 0x00);
        H_line(xEne5Bullet-10, yEne5Bullet-11+i, 30, 0x00);
      }
      for(int i = 0; i <= 11; i++){
        V_line(xP1bullet+10-i, yP1bullet, 10, 0x00);  
      }
      triggerP1 = 0;
      xP1bullet = -10;
      yP1bullet = -10;
    }
  
    if(yP1bullet <= (enemy6y+28) && (xP1bullet+9)>enemy6x && (xP1bullet+1)<(enemy6x+28) && enemy6Life == 0){//Enemy #6
      enemy6Life = 1;
      tripleWave1++;
      if(tripleWave1 == 3){
        tripleWave1 = 0;
        enemy7Life = 0;
        enemy8Life = 0;
        enemy9Life = 0;
      }
      LCD_Sprite(enemy6x, enemy6y, 30, 30, dead, 2, 1, 0, 0);
      LCD_Sprite(xEne6Bullet-10, yEne6Bullet-10, 30, 30, dead, 2, 0, 0, 0);
      delay(15);
      for(int i = 0; i<31; i++){
        H_line(enemy6x, enemy6y+i-1, 30, 0x00);
        H_line(xEne6Bullet-10, yEne6Bullet-11+i, 30, 0x00);
      }
      for(int i = 0; i <= 11; i++){
        V_line(xP1bullet+10-i, yP1bullet, 10, 0x00);  
      }
      triggerP1 = 0;
      xP1bullet = -10;
      yP1bullet = -10;
    }
  
    if(yP1bullet <= (enemy7y+28) && (xP1bullet+9)>enemy7x && (xP1bullet+1)<(enemy7x+28) && enemy7Life == 0){//Enemy #7
      enemy7Life = 1;
      tripleWave2++;
      if(tripleWave2 == 3){
        tripleWave2 = 0;
        enemy10Life = 0;
        enemy11Life = 0;
        enemy12Life = 0;
        enemy13Life = 0;  
      }
      LCD_Sprite(enemy7x, enemy7y, 30, 30, dead, 2, 1, 0, 0);
      LCD_Sprite(xEne7Bullet-10, yEne7Bullet-10, 30, 30, dead, 2, 0, 0, 0);
      delay(15);
      for(int i = 0; i<31; i++){
        H_line(enemy7x, enemy7y+i-1, 30, 0x00);
        H_line(xEne7Bullet-10, yEne7Bullet-11+i, 30, 0x00);
      }
      for(int i = 0; i <= 11; i++){
        V_line(xP1bullet+10-i, yP1bullet, 10, 0x00);  
      }
      triggerP1 = 0;
      xP1bullet = -10;
      yP1bullet = -10;
    }
  
    if(yP1bullet <= (enemy8y+28) && (xP1bullet+9)>enemy8x && (xP1bullet+1)<(enemy8x+28) && enemy8Life == 0){//Enemy #8
      enemy8Life = 1;
      tripleWave2++;
      if(tripleWave2 == 3){
        tripleWave2 = 0;
        enemy10Life = 0;
        enemy11Life = 0;
        enemy12Life = 0;
        enemy13Life = 0;  
      }
      LCD_Sprite(enemy8x, enemy8y, 30, 30, dead, 2, 1, 0, 0);
      LCD_Sprite(xEne8Bullet-10, yEne8Bullet-10, 30, 30, dead, 2, 0, 0, 0);
      delay(15);
      for(int i = 0; i<31; i++){
        H_line(enemy8x, enemy8y+i-1, 30, 0x00);
        H_line(xEne8Bullet-10, yEne8Bullet-11+i, 30, 0x00);
      }
      for(int i = 0; i <= 11; i++){
        V_line(xP1bullet+10-i, yP1bullet, 10, 0x00);  
      }
      triggerP1 = 0;
      xP1bullet = -10;
      yP1bullet = -10;
    }
  
    if(yP1bullet <= (enemy9y+28) && (xP1bullet+9)>enemy9x && (xP1bullet+1)<(enemy9x+28) && enemy9Life == 0){//Enemy #9
      enemy9Life = 1;
      tripleWave2++;
      if(tripleWave2 == 3){
        tripleWave2 = 0;
        enemy10Life = 0;
        enemy11Life = 0;
        enemy12Life = 0;
        enemy13Life = 0;  
      }
      LCD_Sprite(enemy9x, enemy9y, 30, 30, dead, 2, 1, 0, 0);
      LCD_Sprite(xEne9Bullet-10, yEne9Bullet-10, 30, 30, dead, 2, 0, 0, 0);
      delay(15);
      for(int i = 0; i<31; i++){
        H_line(enemy9x, enemy9y+i-1, 30, 0x00);
        H_line(xEne9Bullet-10, yEne9Bullet-11+i, 30, 0x00);
      }
      for(int i = 0; i <= 11; i++){
        V_line(xP1bullet+10-i, yP1bullet, 10, 0x00);  
      }
      triggerP1 = 0;
      xP1bullet = -10;
      yP1bullet = -10;
    }
  
    if(yP1bullet <= (enemy10y+28) && (xP1bullet+9)>enemy10x && (xP1bullet+1)<(enemy10x+28) && enemy10Life == 0){//Enemy #10
      enemy10Life = 1;
      tripleWave3++;
      if(tripleWave3 == 4){
        tripleWave3 = 0;
        enemy14Life = 0;
        enemy15Life = 0;
        enemy16Life = 0;
        enemy17Life = 0;  
      }
      LCD_Sprite(enemy10x, enemy10y, 30, 30, dead, 2, 1, 0, 0);
      LCD_Sprite(xEne10Bullet-10, yEne10Bullet-10, 30, 30, dead, 2, 0, 0, 0);
      delay(15);
      for(int i = 0; i<31; i++){
        H_line(enemy10x, enemy10y+i-1, 30, 0x00);
        H_line(xEne10Bullet-10, yEne10Bullet-11+i, 30, 0x00);
      }
      for(int i = 0; i <= 11; i++){
        V_line(xP1bullet+10-i, yP1bullet, 10, 0x00);  
      }
      triggerP1 = 0;
      xP1bullet = -10;
      yP1bullet = -10;
    }
  
    if(yP1bullet <= (enemy11y+28) && (xP1bullet+9)>enemy11x && (xP1bullet+1)<(enemy11x+28) && enemy11Life == 0){//Enemy #11
      enemy11Life = 1;
      tripleWave3++;
      if(tripleWave3 == 4){
        tripleWave3 = 0;
        enemy14Life = 0;
        enemy15Life = 0;
        enemy16Life = 0;
        enemy17Life = 0;  
      }
      LCD_Sprite(enemy11x, enemy11y, 30, 30, dead, 2, 1, 0, 0);
      LCD_Sprite(xEne11Bullet-10, yEne11Bullet-10, 30, 30, dead, 2, 0, 0, 0);
      delay(15);
      for(int i = 0; i<31; i++){
        H_line(enemy11x, enemy11y+i-1, 30, 0x00);
        H_line(xEne11Bullet-10, yEne11Bullet-11+i, 30, 0x00);
      }
      for(int i = 0; i <= 11; i++){
        V_line(xP1bullet+10-i, yP1bullet, 10, 0x00);  
      }
      triggerP1 = 0;
      xP1bullet = -10;
      yP1bullet = -10;
    }
  
    if(yP1bullet <= (enemy12y+28) && (xP1bullet+9)>enemy12x && (xP1bullet+1)<(enemy12x+28) && enemy12Life == 0){//Enemy #12
      enemy12Life = 1;
      tripleWave3++;
      if(tripleWave3 == 4){
        tripleWave3 = 0;
        enemy14Life = 0;
        enemy15Life = 0;
        enemy16Life = 0;
        enemy17Life = 0;  
      }
      LCD_Sprite(enemy12x, enemy12y, 30, 30, dead, 2, 1, 0, 0);
      LCD_Sprite(xEne12Bullet-10, yEne12Bullet-10, 30, 30, dead, 2, 0, 0, 0);
      delay(15);
      for(int i = 0; i<31; i++){
        H_line(enemy12x, enemy12y+i-1, 30, 0x00);
        H_line(xEne12Bullet-10, yEne12Bullet-11+i, 30, 0x00);
      }
      for(int i = 0; i <= 11; i++){
        V_line(xP1bullet+10-i, yP1bullet, 10, 0x00);  
      }
      triggerP1 = 0;
      xP1bullet = -10;
      yP1bullet = -10;
    }
  
    if(yP1bullet <= (enemy13y+28) && (xP1bullet+9)>enemy13x && (xP1bullet+1)<(enemy13x+28) && enemy13Life == 0){//Enemy #13
      enemy13Life = 1;
      tripleWave3++;
      if(tripleWave3 == 4){
        tripleWave3 = 0;
        enemy14Life = 0;
        enemy15Life = 0;
        enemy16Life = 0;
        enemy17Life = 0;  
      }
      LCD_Sprite(enemy13x, enemy13y, 30, 30, dead, 2, 1, 0, 0);
      LCD_Sprite(xEne13Bullet-10, yEne13Bullet-10, 30, 30, dead, 2, 0, 0, 0);
      delay(15);
      for(int i = 0; i<31; i++){
        H_line(enemy13x, enemy13y+i-1, 30, 0x00);
        H_line(xEne13Bullet-10, yEne13Bullet-11+i, 30, 0x00);
      }
      for(int i = 0; i <= 11; i++){
        V_line(xP1bullet+10-i, yP1bullet, 10, 0x00);  
      }
      triggerP1 = 0;
      xP1bullet = -10;
      yP1bullet = -10;
    }
  
    if(yP1bullet <= (enemy14y+28) && (xP1bullet+9)>enemy14x && (xP1bullet+1)<(enemy14x+28) && enemy14Life == 0){//Enemy #14
      enemy14Life = 1;
      tripleWave4++;
      if(tripleWave4 == 4){
        tripleWave4 = 0;
        enemy18Life = 0;
        enemy19Life = 0;
        enemy20Life = 0;
        enemy21Life = 0;
        enemy22Life = 0;
        enemy23Life = 0;
      }
      LCD_Sprite(enemy14x, enemy14y, 30, 30, dead, 2, 1, 0, 0);
      LCD_Sprite(xEne14Bullet-10, yEne14Bullet-10, 30, 30, dead, 2, 0, 0, 0);
      delay(15);
      for(int i = 0; i<31; i++){
        H_line(enemy14x, enemy14y+i-1, 30, 0x00);
        H_line(xEne14Bullet-10, yEne14Bullet-11+i, 30, 0x00);
      }
      for(int i = 0; i <= 11; i++){
        V_line(xP1bullet+10-i, yP1bullet, 10, 0x00);  
      }
      triggerP1 = 0;
      xP1bullet = -10;
      yP1bullet = -10;
    }
  
    if(yP1bullet <= (enemy15y+28) && (xP1bullet+9)>enemy15x && (xP1bullet+1)<(enemy15x+28) && enemy15Life == 0){//Enemy #15
      enemy15Life = 1;
      tripleWave4++;
      if(tripleWave4 == 4){
        tripleWave4 = 0;
        enemy18Life = 0;
        enemy19Life = 0;
        enemy20Life = 0;
        enemy21Life = 0;
        enemy22Life = 0;
        enemy23Life = 0;
      }
      LCD_Sprite(enemy15x, enemy15y, 30, 30, dead, 2, 1, 0, 0);
      LCD_Sprite(xEne15Bullet-10, yEne15Bullet-10, 30, 30, dead, 2, 0, 0, 0);
      delay(15);
      for(int i = 0; i<31; i++){
        H_line(enemy15x, enemy15y+i-1, 30, 0x00);
        H_line(xEne15Bullet-10, yEne15Bullet-11+i, 30, 0x00);
      }
      for(int i = 0; i <= 11; i++){
        V_line(xP1bullet+10-i, yP1bullet, 10, 0x00);  
      }
      triggerP1 = 0;
      xP1bullet = -10;
      yP1bullet = -10;
    }
  
    if(yP1bullet <= (enemy16y+28) && (xP1bullet+9)>enemy16x && (xP1bullet+1)<(enemy16x+28) && enemy16Life == 0){//Enemy #16
      enemy16Life = 1;
      tripleWave4++;
      if(tripleWave4 == 4){
        tripleWave4 = 0;
        enemy18Life = 0;
        enemy19Life = 0;
        enemy20Life = 0;
        enemy21Life = 0;
        enemy22Life = 0;
        enemy23Life = 0;
      }
      LCD_Sprite(enemy16x, enemy16y, 30, 30, dead, 2, 1, 0, 0);
      LCD_Sprite(xEne16Bullet-10, yEne16Bullet-10, 30, 30, dead, 2, 0, 0, 0);
      delay(15);
      for(int i = 0; i<31; i++){
        H_line(enemy16x, enemy16y+i-1, 30, 0x00);
        H_line(xEne16Bullet-10, yEne16Bullet-11+i, 30, 0x00);
      }
      for(int i = 0; i <= 11; i++){
        V_line(xP1bullet+10-i, yP1bullet, 10, 0x00);  
      }
      triggerP1 = 0;
      xP1bullet = -10;
      yP1bullet = -10;
    }
  
    if(yP1bullet <= (enemy17y+28) && (xP1bullet+9)>enemy17x && (xP1bullet+1)<(enemy17x+28) && enemy17Life == 0){//Enemy #17
      enemy17Life = 1;
      tripleWave4++;
      if(tripleWave4 == 4){
        tripleWave4 = 0;
        enemy18Life = 0;
        enemy19Life = 0;
        enemy20Life = 0;
        enemy21Life = 0;
        enemy22Life = 0;
        enemy23Life = 0;
      }
      LCD_Sprite(enemy17x, enemy17y, 30, 30, dead, 2, 1, 0, 0);
      LCD_Sprite(xEne17Bullet-10, yEne17Bullet-10, 30, 30, dead, 2, 0, 0, 0);
      delay(15);
      for(int i = 0; i<31; i++){
        H_line(enemy17x, enemy17y+i-1, 30, 0x00);
        H_line(xEne17Bullet-10, yEne17Bullet-11+i, 30, 0x00);
      }
      for(int i = 0; i <= 11; i++){
        V_line(xP1bullet+10-i, yP1bullet, 10, 0x00);  
      }
      triggerP1 = 0;
      xP1bullet = -10;
      yP1bullet = -10;
    }
  
    if(yP1bullet <= (enemy18y+28) && (xP1bullet+9)>enemy18x && (xP1bullet+1)<(enemy18x+28) && enemy18Life == 0){//Enemy #18
      enemy18Life = 1;
      tripleWave5++;
      LCD_Sprite(enemy18x, enemy18y, 30, 30, dead, 2, 1, 0, 0);
      LCD_Sprite(xEne18Bullet-10, yEne18Bullet-10, 30, 30, dead, 2, 0, 0, 0);
      delay(15);
      for(int i = 0; i<31; i++){
        H_line(enemy18x, enemy18y+i-1, 30, 0x00);
        H_line(xEne18Bullet-10, yEne18Bullet-11+i, 30, 0x00);
      }
      for(int i = 0; i <= 11; i++){
        V_line(xP1bullet+10-i, yP1bullet, 10, 0x00);  
      }
      triggerP1 = 0;
      xP1bullet = -10;
      yP1bullet = -10;
      if(tripleWave5 == 6){
        tripleWave5 = 0;
        LCD_Print(text1, 105, 100, 2, 0xffff, 0x00);
        delay(3000);
        LCD_Clear(0x00);
        enemy1Life = 0;
        enemy1y = 0;
        enemy2y = 0;
        enemy3y = 0;
        enemy4y = 0;
        enemy5y = 0;
        enemy6y = 0;
        enemy7y = 0;
        enemy8y = 0;
        enemy9y = 0;
        enemy10y = 0;
        enemy11y = 0;
        enemy12y = 0;
        enemy13y = 0;
        enemy14y = 0;
        enemy15y = 0;
        enemy16y = 0;
        enemy17y = 0;
        enemy18y = 0;
        enemy19y = 0;
        enemy20y = 0;
        enemy21y = 0;
        enemy22y = 0;
        enemy23y = 0;
        P1Life = 0;
        xP1 = 0;
        yP1 = 209;
        state = 0;
        turn = 0;
        mapeo_SD("menu.txt", 32, 25, 256);
      }
    }
  
    if(yP1bullet <= (enemy19y+28) && (xP1bullet+9)>enemy19x && (xP1bullet+1)<(enemy19x+28) && enemy19Life == 0){//Enemy #19
      enemy19Life = 1;
      tripleWave5++;
      LCD_Sprite(enemy19x, enemy19y, 30, 30, dead, 2, 1, 0, 0);
      LCD_Sprite(xEne19Bullet-10, yEne19Bullet-10, 30, 30, dead, 2, 0, 0, 0);
      delay(15);
      for(int i = 0; i<31; i++){
        H_line(enemy19x, enemy19y+i-1, 30, 0x00);
        H_line(xEne19Bullet-10, yEne19Bullet-11+i, 30, 0x00);
      }
      for(int i = 0; i <= 11; i++){
        V_line(xP1bullet+10-i, yP1bullet, 10, 0x00);  
      }
      triggerP1 = 0;
      xP1bullet = -10;
      yP1bullet = -10;
      if(tripleWave5 == 6){
        tripleWave5 = 0;
        LCD_Print(text1, 105, 100, 2, 0xffff, 0x00);
        delay(3000);
        LCD_Clear(0x00);
        enemy1Life = 0;
        enemy1y = 0;
        enemy2y = 0;
        enemy3y = 0;
        enemy4y = 0;
        enemy5y = 0;
        enemy6y = 0;
        enemy7y = 0;
        enemy8y = 0;
        enemy9y = 0;
        enemy10y = 0;
        enemy11y = 0;
        enemy12y = 0;
        enemy13y = 0;
        enemy14y = 0;
        enemy15y = 0;
        enemy16y = 0;
        enemy17y = 0;
        enemy18y = 0;
        enemy19y = 0;
        enemy20y = 0;
        enemy21y = 0;
        enemy22y = 0;
        enemy23y = 0;
        P1Life = 0;
        xP1 = 0;
        yP1 = 209;
        state = 0;
        turn = 0;
        mapeo_SD("menu.txt", 32, 25, 256);
      }
    }
  
    if(yP1bullet <= (enemy20y+28) && (xP1bullet+9)>enemy20x && (xP1bullet+1)<(enemy20x+28) && enemy20Life == 0){//Enemy #20
      enemy20Life = 1;
      tripleWave5++;
      LCD_Sprite(enemy20x, enemy20y, 30, 30, dead, 2, 1, 0, 0);
      LCD_Sprite(xEne20Bullet-10, yEne20Bullet-10, 30, 30, dead, 2, 0, 0, 0);
      delay(15);
      for(int i = 0; i<31; i++){
        H_line(enemy20x, enemy20y+i-1, 30, 0x00);
        H_line(xEne20Bullet-10, yEne20Bullet-11+i, 30, 0x00);
      }
      for(int i = 0; i <= 11; i++){
        V_line(xP1bullet+10-i, yP1bullet, 10, 0x00);  
      }
      triggerP1 = 0;
      xP1bullet = -10;
      yP1bullet = -10;
      if(tripleWave5 == 6){
        tripleWave5 = 0;
        LCD_Print(text1, 105, 100, 2, 0xffff, 0x00);
        delay(3000);
        LCD_Clear(0x00);
        enemy1Life = 0;
        enemy1y = 0;
        enemy2y = 0;
        enemy3y = 0;
        enemy4y = 0;
        enemy5y = 0;
        enemy6y = 0;
        enemy7y = 0;
        enemy8y = 0;
        enemy9y = 0;
        enemy10y = 0;
        enemy11y = 0;
        enemy12y = 0;
        enemy13y = 0;
        enemy14y = 0;
        enemy15y = 0;
        enemy16y = 0;
        enemy17y = 0;
        enemy18y = 0;
        enemy19y = 0;
        enemy20y = 0;
        enemy21y = 0;
        enemy22y = 0;
        enemy23y = 0;
        P1Life = 0;
        xP1 = 0;
        yP1 = 209;
        state = 0;
        turn = 0;
        mapeo_SD("menu.txt", 32, 25, 256);
      }
    }
  
    if(yP1bullet <= (enemy21y+28) && (xP1bullet+9)>enemy21x && (xP1bullet+1)<(enemy21x+28) && enemy21Life == 0){//Enemy #21
      enemy21Life = 1;
      tripleWave5++;
      LCD_Sprite(enemy21x, enemy21y, 30, 30, dead, 2, 1, 0, 0);
      LCD_Sprite(xEne21Bullet-10, yEne21Bullet-10, 30, 30, dead, 2, 0, 0, 0);
      delay(15);
      for(int i = 0; i<31; i++){
        H_line(enemy21x, enemy21y+i-1, 30, 0x00);
        H_line(xEne21Bullet-10, yEne21Bullet-11+i, 30, 0x00);
      }
      for(int i = 0; i <= 11; i++){
        V_line(xP1bullet+10-i, yP1bullet, 10, 0x00);  
      }
      triggerP1 = 0;
      xP1bullet = -10;
      yP1bullet = -10;
      if(tripleWave5 == 6){
        tripleWave5 = 0;
        LCD_Print(text1, 105, 100, 2, 0xffff, 0x00);
        delay(3000);
        LCD_Clear(0x00);
        enemy1Life = 0;
        enemy1y = 0;
        enemy2y = 0;
        enemy3y = 0;
        enemy4y = 0;
        enemy5y = 0;
        enemy6y = 0;
        enemy7y = 0;
        enemy8y = 0;
        enemy9y = 0;
        enemy10y = 0;
        enemy11y = 0;
        enemy12y = 0;
        enemy13y = 0;
        enemy14y = 0;
        enemy15y = 0;
        enemy16y = 0;
        enemy17y = 0;
        enemy18y = 0;
        enemy19y = 0;
        enemy20y = 0;
        enemy21y = 0;
        enemy22y = 0;
        enemy23y = 0;
        P1Life = 0;
        xP1 = 0;
        yP1 = 209;
        state = 0;
        turn = 0;
        mapeo_SD("menu.txt", 32, 25, 256);
      }
    }
  
    if(yP1bullet <= (enemy22y+28) && (xP1bullet+9)>enemy22x && (xP1bullet+1)<(enemy22x+28) && enemy22Life == 0){//Enemy #22
      enemy22Life = 1;
      tripleWave5++;
      LCD_Sprite(enemy22x, enemy22y, 30, 30, dead, 2, 1, 0, 0);
      LCD_Sprite(xEne22Bullet-10, yEne22Bullet-10, 30, 30, dead, 2, 0, 0, 0);
      delay(15);
      for(int i = 0; i<31; i++){
        H_line(enemy22x, enemy22y+i-1, 30, 0x00);
        H_line(xEne22Bullet-10, yEne22Bullet-11+i, 30, 0x00);
      }
      for(int i = 0; i <= 11; i++){
        V_line(xP1bullet+10-i, yP1bullet, 10, 0x00);  
      }
      triggerP1 = 0;
      xP1bullet = -10;
      yP1bullet = -10;
      if(tripleWave5 == 6){
        tripleWave5 = 0;
        LCD_Print(text1, 105, 100, 2, 0xffff, 0x00);
        delay(3000);
        LCD_Clear(0x00);
        enemy1Life = 0;
        enemy1y = 0;
        enemy2y = 0;
        enemy3y = 0;
        enemy4y = 0;
        enemy5y = 0;
        enemy6y = 0;
        enemy7y = 0;
        enemy8y = 0;
        enemy9y = 0;
        enemy10y = 0;
        enemy11y = 0;
        enemy12y = 0;
        enemy13y = 0;
        enemy14y = 0;
        enemy15y = 0;
        enemy16y = 0;
        enemy17y = 0;
        enemy18y = 0;
        enemy19y = 0;
        enemy20y = 0;
        enemy21y = 0;
        enemy22y = 0;
        enemy23y = 0;
        P1Life = 0;
        xP1 = 0;
        yP1 = 209;
        state = 0;
        turn = 0;
        mapeo_SD("menu.txt", 32, 25, 256);
      }
    }
  
    if(yP1bullet <= (enemy23y+28) && (xP1bullet+9)>enemy23x && (xP1bullet+1)<(enemy23x+28) && enemy23Life == 0){//Enemy #23
      enemy23Life = 1;
      tripleWave5++;
      LCD_Sprite(enemy23x, enemy23y, 30, 30, dead, 2, 1, 0, 0);
      LCD_Sprite(xEne23Bullet-10, yEne23Bullet-10, 30, 30, dead, 2, 0, 0, 0);
      delay(15);
      for(int i = 0; i<31; i++){
        H_line(enemy23x, enemy23y+i-1, 30, 0x00);
        H_line(xEne23Bullet-10, yEne23Bullet-11+i, 30, 0x00);
      }
      for(int i = 0; i <= 11; i++){
        V_line(xP1bullet+10-i, yP1bullet, 10, 0x00);  
      }
      triggerP1 = 0;
      xP1bullet = -10;
      yP1bullet = -10;
      if(tripleWave5 == 6){
        tripleWave5 = 0;
        LCD_Print(text1, 105, 100, 2, 0xffff, 0x00);
        delay(3000);
        LCD_Clear(0x00);
        enemy1Life = 0;
        enemy1y = 0;
        enemy2y = 0;
        enemy3y = 0;
        enemy4y = 0;
        enemy5y = 0;
        enemy6y = 0;
        enemy7y = 0;
        enemy8y = 0;
        enemy9y = 0;
        enemy10y = 0;
        enemy11y = 0;
        enemy12y = 0;
        enemy13y = 0;
        enemy14y = 0;
        enemy15y = 0;
        enemy16y = 0;
        enemy17y = 0;
        enemy18y = 0;
        enemy19y = 0;
        enemy20y = 0;
        enemy21y = 0;
        enemy22y = 0;
        enemy23y = 0;
        P1Life = 0;
        xP1 = 0;
        yP1 = 209;
        state = 0;
        turn = 0;
        mapeo_SD("menu.txt", 32, 25, 256);
      }
    }
  //**********************************************************************************************************************************
  //Player #1 bullet collision
  //**********************************************************************************************************************************
  
  //**********************************************************************************************************************************
  //Player #1 bullet direction
  //**********************************************************************************************************************************  
    if(triggerP1==1 && P1bullDirection == 1){//P1 bullet moving up
      LCD_Bitmap(xP1bullet, yP1bullet, 10, 10, bulletUp);
      H_line(xP1bullet, yP1bullet+10, 10, 0x00);
      yP1bullet--;
      LCD_Bitmap(xP1bullet, yP1bullet, 10, 10, bulletUp);
      H_line(xP1bullet, yP1bullet+10, 10, 0x00);
      yP1bullet--;
      if(yP1bullet <= 0){
        for(int i = 10; i >= 0; i--){
          H_line(xP1bullet, yP1bullet + i, 10, 0x00);  
        }
        triggerP1 = 0;
        xP1bullet = -10;
        yP1bullet = -10;
      }
    }else if(triggerP1==1 && P1bullDirection == 2){//P1 bullet moving right
      LCD_Bitmap(xP1bullet, yP1bullet, 10, 10, bulletRight);
      V_line(xP1bullet-1, yP1bullet, 10, 0x00);
      xP1bullet++;
      LCD_Bitmap(xP1bullet, yP1bullet, 10, 10, bulletRight);
      V_line(xP1bullet-1, yP1bullet, 10, 0x00);
      xP1bullet++;
      
      if((xP1bullet+10) >= 320){
        for(int i = 0; i <= 11; i++){
          V_line(xP1bullet+9-i, yP1bullet, 10, 0x00);  
        }
        triggerP1 = 0;
        xP1bullet = -10;
        yP1bullet = -10;
      }
    }else if(triggerP1==1 && P1bullDirection == 3){//P1 bullet moving left
      LCD_Bitmap(xP1bullet, yP1bullet, 10, 10, bulletLeft);
      V_line(xP1bullet+10, yP1bullet, 10, 0x00);
      xP1bullet--;
      LCD_Bitmap(xP1bullet, yP1bullet, 10, 10, bulletLeft);
      V_line(xP1bullet+10, yP1bullet, 10, 0x00);
      xP1bullet--;
      
      if(xP1bullet <= 0){
        for(int i = 0; i <= 11; i++){
          V_line(xP1bullet+i, yP1bullet, 10, 0x00);  
        }
        triggerP1 = 0;
        xP1bullet = -10;
        yP1bullet = -10;
      }
    }else{
      triggerP1 = 0;  
      xP1bullet = -10;
      yP1bullet = -10;
    }
  //**********************************************************************************************************************************
  //Player #1 bullet direction
  //**********************************************************************************************************************************
    delay(10);
  }else if(state == 2){
//**********************************************************************************************************************************
//**********************************************************************************************************************************
//**********************************************************************************************************************************
//**********************************************************************************************************************************
// 2 players
//**********************************************************************************************************************************
//**********************************************************************************************************************************
//**********************************************************************************************************************************
//**********************************************************************************************************************************
    if(turn == 0){
      LCD_Clear(0x00); 
      turn = 1; 
    };
    if(buttonP1 < 10 && P1Life == 0){
      if(triggerP1 == 0 && P1facing == 1){//shooting up
        triggerP1 = 1;
        P1bullDirection = 1;
        xP1bullet = xP1 + 9;
        yP1bullet = yP1 - 10;  
      }  
      if(triggerP1 == 0 && P1facing == 4){//shooting right
        triggerP1 = 1;
        P1bullDirection = 2;
        xP1bullet = xP1 + 30;
        yP1bullet = yP1 + 10;  
      } 
      if(triggerP1 == 0 && P1facing == 2){//shooting left
        triggerP1 = 1;
        P1bullDirection = 3;
        xP1bullet = xP1 - 10;
        yP1bullet = yP1 + 10;  
      }
    }
//Player2***************************************************************************************************************************
    if(buttonP2 < 10 && P2Life == 0){
      if(triggerP2 == 0 && P2facing == 1){//shooting up
        triggerP2 = 1;
        P2bullDirection = 1;
        xP2bullet = xP2 + 9;
        yP2bullet = yP2 - 10;  
      }  
      if(triggerP2 == 0 && P2facing == 4){//shooting right
        triggerP2 = 1;
        P2bullDirection = 2;
        xP2bullet = xP2 + 30;
        yP2bullet = yP2 + 10;  
      } 
      if(triggerP2 == 0 && P2facing == 2){//shooting left
        triggerP2 = 1;
        P2bullDirection = 3;
        xP2bullet = xP2 - 10;
        yP2bullet = yP2 + 10;  
      }
    }
//**********************************************************************************************************************************
//controles P1
//**********************************************************************************************************************************
    if(yPlayer1 > 2300 && P1Life == 0){//down
      yP1++;
      P1facing = 3;
      if(yP1 == 211){
        yP1 = 210;
      }
      LCD_Sprite(xP1, yP1, 30, 30, tankP1Backward, 2,animy, 0, 0);
      H_line(xP1, yP1-1, 30, 0x00);
    }
    if(yPlayer1 < 1700 && P1Life == 0){//up 
      yP1--;
      P1facing = 1;
      if(yP1 == -1){
        yP1 = 0;  
      }
      LCD_Sprite(xP1, yP1, 30, 30, tankP1Forward, 2,animy, 0, 0);
      H_line(xP1, yP1+30, 30, 0x00);
    }
    if(xPlayer1 > 2300 && P1Life == 0){//left
      xP1--;
      P1facing = 2;
      if(xP1 == -1){
        xP1 = 0;  
      } 
      LCD_Sprite(xP1, yP1, 30, 30, tankP1LR, 2, animx, 1, 0);
      V_line(xP1+30, yP1, 30, 0x00);
    }
    if(xPlayer1  < 1700 && P1Life == 0){//right
      xP1++;
      P1facing = 4;
      if(xP1 == 291){
        xP1 = 290;  
      }  
      LCD_Sprite(xP1, yP1, 30, 30, tankP1LR, 2, animx, 0, 0);
      V_line(xP1-1, yP1, 30, 0x00);
    }

//**********************************************************************************************************************************
//controles P2
//**********************************************************************************************************************************
    if(yPlayer2 > 2300 && P2Life == 0){//down
      yP2++;
      P2facing = 3;
      if(yP2 == 211){
        yP2 = 210;
      }
      LCD_Sprite(xP2, yP2, 30, 30, tankP2Backward, 2,animy2, 0, 0);
      H_line(xP2, yP2-1, 30, 0x00);
    }
    if(yPlayer2 < 1700 && P2Life == 0){//up 
      yP2--;
      P2facing = 1;
      if(yP2 == -1){
        yP2 = 0;  
      }
      LCD_Sprite(xP2, yP2, 30, 30, tankP2Forward, 2,animy2, 0, 0);
      H_line(xP2, yP2+30, 30, 0x00);
    }
    if(xPlayer2 > 2300 && P2Life == 0){//left
      xP2--;
      P2facing = 2;
      if(xP2 == -1){
        xP2 = 0;  
      } 
      LCD_Sprite(xP2, yP2, 30, 30, tankP2LR, 2, animx2, 1, 0);
      V_line(xP2+30, yP2, 30, 0x00);
    }
    if(xPlayer2  < 1700 && P2Life == 0){//right
      xP2++;
      P2facing = 4;
      if(xP2 == 291){
        xP2 = 290;  
      }  
      LCD_Sprite(xP2, yP2, 30, 30, tankP2LR, 2, animx2, 0, 0);
      V_line(xP2-1, yP2, 30, 0x00);
    }
  
  //**********************************************************************************************************************************
  //Enemy #1 code
  //**********************************************************************************************************************************
    if(enemy1Life == 0){
      if(enemy1y == 0){
        xEne1Bullet = enemy1x + 8;
        yEne1Bullet = enemy1y + 30;
      }
      if(enemy1y == 90){
        xEne1Bullet = enemy1x + 8;
        yEne1Bullet = enemy1y + 30; 
      }
      int animEne1 = (enemy1y/5)%2;
      LCD_Sprite(enemy1x, enemy1y, 28, 30, enemyTankForward, 2, animEne1, 0, 0);
      H_line(enemy1x, enemy1y-1, 28,0x00);
      LCD_Bitmap(xEne1Bullet, yEne1Bullet, 10, 10, bulletDown);
      H_line(xEne1Bullet, yEne1Bullet, 10, 0x00);
      if(enemy1Pace == 0){
        enemy1y++; 
        enemy1Pace = 1;
      }else if(enemy1Pace == 1){
        enemy1Pace = 2;  
        yEne1Bullet++;
        if(yEne1Bullet >= 250){
          yEne1Bullet = 245;  
        }
      }else if(enemy1Pace == 2){
        enemy1Pace = 3;
      }else if(enemy1Pace == 3){
        enemy1Pace = 4;  
        yEne1Bullet++;
        if(yEne1Bullet >= 250){
          yEne1Bullet = 245;  
        }
      }else if(enemy1Pace == 4){
        enemy1Pace =  5;
      }else if(enemy1Pace == 5){
        enemy1Pace =  0;
        yEne1Bullet++;
        if(yEne1Bullet >= 250){
          yEne1Bullet = 245;  
        }
      }
   
      if((enemy1y == 209)||((xP1+29)>enemy1x && (xP1+1)<(enemy1x+28) && yP1<(enemy1y+30) && (yP1+30)>enemy1y)
      ||((xP2+29)>enemy1x && (xP2+1)<(enemy1x+28) && yP2<(enemy1y+30) && (yP2+30)>enemy1y)){
        enemy1Life = 1;
        P1Life = 1;
        P2Life = 1;
        LCD_Sprite(xP1, yP1, 30, 30, dead, 2, 1, 0, 0);
        LCD_Sprite(xP2, yP2, 30, 30, dead, 2, 1, 0, 0);
        mapeo_SD("gameOver.txt", 67, 75, 186);
        delay(3000);
        LCD_Clear(0x00);
        enemy1Life = 0;
        enemy1y = 0;
        enemy2y = 0;
        enemy3y = 0;
        enemy4y = 0;
        enemy5y = 0;
        enemy6y = 0;
        enemy7y = 0;
        enemy8y = 0;
        enemy9y = 0;
        enemy10y = 0;
        enemy11y = 0;
        enemy12y = 0;
        enemy13y = 0;
        enemy14y = 0;
        enemy15y = 0;
        enemy16y = 0;
        enemy17y = 0;
        enemy18y = 0;
        enemy19y = 0;
        enemy20y = 0;
        enemy21y = 0;
        enemy22y = 0;
        enemy23y = 0;
        tripleWave1 = 0;
        tripleWave2 = 0;
        tripleWave3 = 0;
        tripleWave4 = 0;
        tripleWave5 = 0;
        P1Life = 0;
        P2Life = 0;
        xP1 = 0;
        yP1 = 209;
        xP2 = 290;
        yP2 = 209;
        state = 0;
        turn = 0;
        mapeo_SD("menu.txt", 32, 25, 256);
      }
      if(((xP1+29)>xEne1Bullet && (xP1+1)<(xEne1Bullet+10) && yP1<(yEne1Bullet+10) && (yP1+30)>yEne1Bullet)
      || ((xP2+29)>xEne1Bullet && (xP2+1)<(xEne1Bullet+10) && yP2<(yEne1Bullet+10) && (yP2+30)>yEne1Bullet)){
        enemy1Life = 1;
        P1Life = 1;
        P2Life = 1;
        LCD_Sprite(xP1, yP1, 30, 30, dead, 2, 1, 0, 0);
        LCD_Sprite(xP2, yP2, 30, 30, dead, 2, 1, 0, 0);
        mapeo_SD("gameOver.txt", 67, 75, 186);
        delay(3000);
        LCD_Clear(0x00);
        enemy1Life = 0;
        enemy1y = 0;
        enemy2y = 0;
        enemy3y = 0;
        enemy4y = 0;
        enemy5y = 0;
        enemy6y = 0;
        enemy7y = 0;
        enemy8y = 0;
        enemy9y = 0;
        enemy10y = 0;
        enemy11y = 0;
        enemy12y = 0;
        enemy13y = 0;
        enemy14y = 0;
        enemy15y = 0;
        enemy16y = 0;
        enemy17y = 0;
        enemy18y = 0;
        enemy19y = 0;
        enemy20y = 0;
        enemy21y = 0;
        enemy22y = 0;
        enemy23y = 0;
        tripleWave1 = 0;
        tripleWave2 = 0;
        tripleWave3 = 0;
        tripleWave4 = 0;
        tripleWave5 = 0;
        P1Life = 0;
        P2Life = 0;
        xP1 = 0;
        yP1 = 209;
        xP2 = 290;
        yP2 = 209;
        state = 0;
        turn = 0;
        mapeo_SD("menu.txt", 32, 25, 256);
      }
    }
  //**********************************************************************************************************************************
  //Enemy #1 code
  //**********************************************************************************************************************************
  
  //**********************************************************************************************************************************
  //Enemy #2 code
  //**********************************************************************************************************************************
    if(enemy2Life == 0){
      if(enemy2y == 0){
        xEne2Bullet = enemy2x + 8;
        yEne2Bullet = enemy2y + 30;
      }
      if(enemy2y == 90){
        xEne2Bullet = enemy2x + 8;
        yEne2Bullet = enemy2y + 30; 
      }
      int animEne2 = (enemy2y/5)%2;
      LCD_Sprite(enemy2x, enemy2y, 28, 30, enemyTankForward, 2, animEne2, 0, 0);
      H_line(enemy2x, enemy2y-1, 28,0x00);
      LCD_Bitmap(xEne2Bullet, yEne2Bullet, 10, 10, bulletDown);
      H_line(xEne2Bullet, yEne2Bullet, 10, 0x00);
      if(enemy2Pace == 0){
        enemy2y++; 
        enemy2Pace = 1;
      }else if(enemy2Pace == 1){
        enemy2Pace = 2;  
        yEne2Bullet++;
        if(yEne2Bullet >= 250){
          yEne2Bullet = 245;  
        }
      }else if(enemy2Pace == 2){
        enemy2Pace = 3;
      }else if(enemy2Pace == 3){
        enemy2Pace = 4;  
        yEne2Bullet++;
        if(yEne2Bullet >= 250){
          yEne2Bullet = 245;  
        }
      }else if(enemy2Pace == 4){
        enemy2Pace =  5;
      }else if(enemy2Pace == 5){
        enemy2Pace =  0;
        yEne2Bullet++;
        if(yEne2Bullet >= 250){
          yEne2Bullet = 245;  
        }
      }
   
      if((enemy2y == 209)||((xP1+29)>enemy2x && (xP1+1)<(enemy2x+28) && yP1<(enemy2y+30) && (yP1+30)>enemy2y)
      || ((xP2+29)>enemy2x && (xP2+1)<(enemy2x+28) && yP2<(enemy2y+30) && (yP2+30)>enemy2y)){
        enemy2Life = 1;
        P1Life = 1;
        P2Life = 1;
        LCD_Sprite(xP1, yP1, 30, 30, dead, 2, 1, 0, 0);
        LCD_Sprite(xP2, yP2, 30, 30, dead, 2, 1, 0, 0);
        mapeo_SD("gameOver.txt", 67, 75, 186);
        delay(3000);
        LCD_Clear(0x00);
        enemy1Life = 0;
        enemy1y = 0;
        enemy2y = 0;
        enemy3y = 0;
        enemy4y = 0;
        enemy5y = 0;
        enemy6y = 0;
        enemy7y = 0;
        enemy8y = 0;
        enemy9y = 0;
        enemy10y = 0;
        enemy11y = 0;
        enemy12y = 0;
        enemy13y = 0;
        enemy14y = 0;
        enemy15y = 0;
        enemy16y = 0;
        enemy17y = 0;
        enemy18y = 0;
        enemy19y = 0;
        enemy20y = 0;
        enemy21y = 0;
        enemy22y = 0;
        enemy23y = 0;
        tripleWave1 = 0;
        tripleWave2 = 0;
        tripleWave3 = 0;
        tripleWave4 = 0;
        tripleWave5 = 0;
        P1Life = 0;
        P2Life = 0;
        xP1 = 0;
        yP1 = 209;
        xP2 = 290;
        yP2 = 209;
        state = 0;
        turn = 0;
        mapeo_SD("menu.txt", 32, 25, 256);
      }
      if(((xP1+29)>xEne2Bullet && (xP1+1)<(xEne2Bullet+10) && yP1<(yEne2Bullet+10) && (yP1+30)>yEne2Bullet)
      || ((xP2+29)>xEne2Bullet && (xP2+1)<(xEne2Bullet+10) && yP2<(yEne2Bullet+10) && (yP2+30)>yEne2Bullet)){
        enemy2Life = 1;
        P1Life = 1;
        P2Life = 1;
        LCD_Sprite(xP1, yP1, 30, 30, dead, 2, 1, 0, 0);
        LCD_Sprite(xP2, yP2, 30, 30, dead, 2, 1, 0, 0);
        mapeo_SD("gameOver.txt", 67, 75, 186);
        delay(3000);
        LCD_Clear(0x00);
        enemy1Life = 0;
        enemy1y = 0;
        enemy2y = 0;
        enemy3y = 0;
        enemy4y = 0;
        enemy5y = 0;
        enemy6y = 0;
        enemy7y = 0;
        enemy8y = 0;
        enemy9y = 0;
        enemy10y = 0;
        enemy11y = 0;
        enemy12y = 0;
        enemy13y = 0;
        enemy14y = 0;
        enemy15y = 0;
        enemy16y = 0;
        enemy17y = 0;
        enemy18y = 0;
        enemy19y = 0;
        enemy20y = 0;
        enemy21y = 0;
        enemy22y = 0;
        enemy23y = 0;
        tripleWave1 = 0;
        tripleWave2 = 0;
        tripleWave3 = 0;
        tripleWave4 = 0;
        tripleWave5 = 0;
        P1Life = 0;
        P2Life = 0;
        xP1 = 0;
        yP1 = 209;
        xP2 = 290;
        yP2 = 209;
        state = 0;
        turn = 0;
        mapeo_SD("menu.txt", 32, 25, 256);
      }
    }
  //**********************************************************************************************************************************
  //Enemy #2 code
  //**********************************************************************************************************************************
  
  //**********************************************************************************************************************************
  //Enemy #3 code
  //**********************************************************************************************************************************
    if(enemy3Life == 0){
      if(enemy3y == 0){
        xEne3Bullet = enemy3x + 8;
        yEne3Bullet = enemy3y + 30;
      }
      if(enemy3y == 90){
        xEne3Bullet = enemy3x + 8;
        yEne3Bullet = enemy3y + 30; 
      }
      int animEne3 = (enemy3y/5)%2;
      LCD_Sprite(enemy3x, enemy3y, 28, 30, enemyTankForward, 2, animEne3, 0, 0);
      H_line(enemy3x, enemy3y-1, 28,0x00);
      LCD_Bitmap(xEne3Bullet, yEne3Bullet, 10, 10, bulletDown);
      H_line(xEne3Bullet, yEne3Bullet, 10, 0x00);
      if(enemy3Pace == 0){
        enemy3y++; 
        enemy3Pace = 1;
      }else if(enemy3Pace == 1){
        enemy3Pace = 2;  
        yEne3Bullet++;
        if(yEne3Bullet >= 250){
          yEne3Bullet = 245;  
        }
      }else if(enemy3Pace == 2){
        enemy3Pace = 3;
      }else if(enemy3Pace == 3){
        enemy3Pace = 4;  
        yEne3Bullet++;
        if(yEne3Bullet >= 250){
          yEne3Bullet = 245;  
        }
      }else if(enemy3Pace == 4){
        enemy3Pace =  5;
      }else if(enemy3Pace == 5){
        enemy3Pace =  0;
        yEne3Bullet++;
        if(yEne3Bullet >= 250){
          yEne3Bullet = 245;  
        }
      }
   
      if((enemy3y == 209)||((xP1+29)>enemy3x && (xP1+1)<(enemy3x+28) && yP1<(enemy3y+30) && (yP1+30)>enemy3y)
      || ((xP2+29)>enemy3x && (xP2+1)<(enemy3x+28) && yP2<(enemy3y+30) && (yP2+30)>enemy3y)){
        enemy3Life = 1;
        P1Life = 1;
        P2Life = 1;
        LCD_Sprite(xP1, yP1, 30, 30, dead, 2, 1, 0, 0);
        LCD_Sprite(xP2, yP2, 30, 30, dead, 2, 1, 0, 0);
        mapeo_SD("gameOver.txt", 67, 75, 186);
        delay(3000);
        LCD_Clear(0x00);
        enemy1Life = 0;
        enemy1y = 0;
        enemy2y = 0;
        enemy3y = 0;
        enemy4y = 0;
        enemy5y = 0;
        enemy6y = 0;
        enemy7y = 0;
        enemy8y = 0;
        enemy9y = 0;
        enemy10y = 0;
        enemy11y = 0;
        enemy12y = 0;
        enemy13y = 0;
        enemy14y = 0;
        enemy15y = 0;
        enemy16y = 0;
        enemy17y = 0;
        enemy18y = 0;
        enemy19y = 0;
        enemy20y = 0;
        enemy21y = 0;
        enemy22y = 0;
        enemy23y = 0;
        tripleWave1 = 0;
        tripleWave2 = 0;
        tripleWave3 = 0;
        tripleWave4 = 0;
        tripleWave5 = 0;
        P1Life = 0;
        P2Life = 0;
        xP1 = 0;
        yP1 = 209;
        xP2 = 290;
        yP2 = 209;
        state = 0;
        turn = 0;
        mapeo_SD("menu.txt", 32, 25, 256);
      }
      if(((xP1+29)>xEne3Bullet && (xP1+1)<(xEne3Bullet+10) && yP1<(yEne3Bullet+10) && (yP1+30)>yEne3Bullet)
      || ((xP2+29)>xEne3Bullet && (xP2+1)<(xEne3Bullet+10) && yP2<(yEne3Bullet+10) && (yP2+30)>yEne3Bullet)){
        enemy3Life = 1;
        P1Life = 1;
        P2Life = 1;
        LCD_Sprite(xP1, yP1, 30, 30, dead, 2, 1, 0, 0);
        LCD_Sprite(xP2, yP2, 30, 30, dead, 2, 1, 0, 0);
        mapeo_SD("gameOver.txt", 67, 75, 186);
        delay(3000);
        LCD_Clear(0x00);
        enemy1Life = 0;
        enemy1y = 0;
        enemy2y = 0;
        enemy3y = 0;
        enemy4y = 0;
        enemy5y = 0;
        enemy6y = 0;
        enemy7y = 0;
        enemy8y = 0;
        enemy9y = 0;
        enemy10y = 0;
        enemy11y = 0;
        enemy12y = 0;
        enemy13y = 0;
        enemy14y = 0;
        enemy15y = 0;
        enemy16y = 0;
        enemy17y = 0;
        enemy18y = 0;
        enemy19y = 0;
        enemy20y = 0;
        enemy21y = 0;
        enemy22y = 0;
        enemy23y = 0;
        tripleWave1 = 0;
        tripleWave2 = 0;
        tripleWave3 = 0;
        tripleWave4 = 0;
        tripleWave5 = 0;
        P1Life = 0;
        P2Life = 0;
        xP1 = 0;
        yP1 = 209;
        xP2 = 290;
        yP2 = 209;
        state = 0;
        turn = 0;
        mapeo_SD("menu.txt", 32, 25, 256);
      }
    }
  //**********************************************************************************************************************************
  //Enemy #3 code
  //**********************************************************************************************************************************
  
  //**********************************************************************************************************************************
  //Enemy #4 code
  //**********************************************************************************************************************************
    if(enemy4Life == 0){
      if(enemy4y == 0){
        xEne4Bullet = enemy4x + 8;
        yEne4Bullet = enemy4y + 30;
      }
      if(enemy4y == 110){
        xEne4Bullet = enemy4x + 8;
        yEne4Bullet = enemy4y + 30; 
      }
      int animEne4 = (enemy4y/5)%2;
      LCD_Sprite(enemy4x, enemy4y, 28, 30, enemyTankForward, 2, animEne4, 0, 0);
      H_line(enemy4x, enemy4y-1, 28,0x00);
      LCD_Bitmap(xEne4Bullet, yEne4Bullet, 10, 10, bulletDown);
      H_line(xEne4Bullet, yEne4Bullet, 10, 0x00);
      if(enemy4Pace == 0){
        enemy4y++; 
        enemy4Pace = 1;
      }else if(enemy4Pace == 1){
        enemy4Pace = 2;  
        //yEne4Bullet++;
        if(yEne4Bullet >= 250){
          yEne4Bullet = 245;  
        }
      }else if(enemy4Pace == 2){
        enemy4y++;
        enemy4Pace = 3;
      }else if(enemy4Pace == 3){
        enemy4Pace = 4;  
        //yEne4Bullet++;
        if(yEne4Bullet >= 250){
          yEne4Bullet = 245;  
        }
      }else if(enemy4Pace == 4){
        enemy4y++;
        enemy4Pace =  5;
      }else if(enemy4Pace == 5){
        enemy4Pace =  0;
        //yEne4Bullet++;
        if(yEne4Bullet >= 250){
          yEne4Bullet = 245;  
        }
      }
      yEne4Bullet++;
      if(yEne4Bullet >=250){
        yEne4Bullet = 245;  
      }
   
      if((enemy4y == 209)||((xP1+29)>enemy4x && (xP1+1)<(enemy4x+28) && yP1<(enemy4y+30) && (yP1+30)>enemy4y)
      || ((xP2+29)>enemy4x && (xP2+1)<(enemy4x+28) && yP2<(enemy4y+30) && (yP2+30)>enemy4y)){
        enemy4Life = 1;
        enemy5Life = 1;
        enemy6Life = 1;
        P1Life = 1;
        P2Life = 1;
        LCD_Sprite(xP1, yP1, 30, 30, dead, 2, 1, 0, 0);
        LCD_Sprite(xP2, yP2, 30, 30, dead, 2, 1, 0, 0);
        mapeo_SD("gameOver.txt", 67, 75, 186);
        delay(3000);
        LCD_Clear(0x00);
        enemy1Life = 0;
        enemy1y = 0;
        enemy2y = 0;
        enemy3y = 0;
        enemy4y = 0;
        enemy5y = 0;
        enemy6y = 0;
        enemy7y = 0;
        enemy8y = 0;
        enemy9y = 0;
        enemy10y = 0;
        enemy11y = 0;
        enemy12y = 0;
        enemy13y = 0;
        enemy14y = 0;
        enemy15y = 0;
        enemy16y = 0;
        enemy17y = 0;
        enemy18y = 0;
        enemy19y = 0;
        enemy20y = 0;
        enemy21y = 0;
        enemy22y = 0;
        enemy23y = 0;
        tripleWave1 = 0;
        tripleWave2 = 0;
        tripleWave3 = 0;
        tripleWave4 = 0;
        tripleWave5 = 0;
        P1Life = 0;
        P2Life = 0;
        xP1 = 0;
        yP1 = 209;
        xP2 = 290;
        yP2 = 209;
        state = 0;
        turn = 0;
        mapeo_SD("menu.txt", 32, 25, 256);
      }
      if(((xP1+29)>xEne4Bullet && (xP1+1)<(xEne4Bullet+10) && yP1<(yEne4Bullet+10) && (yP1+30)>yEne4Bullet)
      || ((xP2+29)>xEne4Bullet && (xP2+1)<(xEne4Bullet+10) && yP2<(yEne4Bullet+10) && (yP2+30)>yEne4Bullet)){
        enemy4Life = 1;
        enemy5Life = 1;
        enemy6Life = 1;
        P1Life = 1;
        P2Life = 1;
        LCD_Sprite(xP1, yP1, 30, 30, dead, 2, 1, 0, 0);
        LCD_Sprite(xP2, yP2, 30, 30, dead, 2, 1, 0, 0);
        mapeo_SD("gameOver.txt", 67, 75, 186);
        delay(3000);
        LCD_Clear(0x00);
        enemy1Life = 0;
        enemy1y = 0;
        enemy2y = 0;
        enemy3y = 0;
        enemy4y = 0;
        enemy5y = 0;
        enemy6y = 0;
        enemy7y = 0;
        enemy8y = 0;
        enemy9y = 0;
        enemy10y = 0;
        enemy11y = 0;
        enemy12y = 0;
        enemy13y = 0;
        enemy14y = 0;
        enemy15y = 0;
        enemy16y = 0;
        enemy17y = 0;
        enemy18y = 0;
        enemy19y = 0;
        enemy20y = 0;
        enemy21y = 0;
        enemy22y = 0;
        enemy23y = 0;
        tripleWave1 = 0;
        tripleWave2 = 0;
        tripleWave3 = 0;
        tripleWave4 = 0;
        tripleWave5 = 0;
        P1Life = 0;
        P2Life = 0;
        xP1 = 0;
        yP1 = 209;
        xP2 = 290;
        yP2 = 209;
        state = 0;
        turn = 0;
        mapeo_SD("menu.txt", 32, 25, 256);
      }
    }
  //**********************************************************************************************************************************
  //Enemy #4 code
  //**********************************************************************************************************************************
  
  //**********************************************************************************************************************************
  //Enemy #5 code
  //**********************************************************************************************************************************
    if(enemy5Life == 0){
      if(enemy5y == 0){
        xEne5Bullet = enemy5x + 8;
        yEne5Bullet = enemy5y + 30;
      }
      if(enemy5y == 110){
        xEne5Bullet = enemy5x + 8;
        yEne5Bullet = enemy5y + 30; 
      }
      int animEne5 = (enemy5y/5)%2;
      LCD_Sprite(enemy5x, enemy5y, 28, 30, enemyTankForward, 2, animEne5, 0, 0);
      H_line(enemy5x, enemy5y-1, 28,0x00);
      LCD_Bitmap(xEne5Bullet, yEne5Bullet, 10, 10, bulletDown);
      H_line(xEne5Bullet, yEne5Bullet, 10, 0x00);
      if(enemy5Pace == 0){
        enemy5y++; 
        enemy5Pace = 1;
      }else if(enemy5Pace == 1){
        enemy5Pace = 2;  
        //yEne5Bullet++;
        if(yEne5Bullet >= 250){
          yEne5Bullet = 245;  
        }
      }else if(enemy5Pace == 2){
        enemy5y++;
        enemy5Pace = 3;
      }else if(enemy5Pace == 3){
        enemy5Pace = 4;  
        //yEne5Bullet++;
        if(yEne5Bullet >= 250){
          yEne5Bullet = 245;  
        }
      }else if(enemy5Pace == 4){
        enemy5y++;
        enemy5Pace =  5;
      }else if(enemy5Pace == 5){
        enemy5Pace =  0;
        //yEne5Bullet++;
        if(yEne5Bullet >= 250){
          yEne5Bullet = 245;  
        }
      }
      yEne5Bullet++;
      if(yEne5Bullet >=250){
        yEne5Bullet = 245;  
      }
   
      if((enemy5y == 209)||((xP1+29)>enemy5x && (xP1+1)<(enemy5x+28) && yP1<(enemy5y+30) && (yP1+30)>enemy5y)
      || ((xP2+29)>enemy5x && (xP2+1)<(enemy5x+28) && yP2<(enemy5y+30) && (yP2+30)>enemy5y)){
        enemy4Life = 1;
        enemy5Life = 1;
        enemy6Life = 1;
        P1Life = 1;
        P2Life = 1;
        LCD_Sprite(xP1, yP1, 30, 30, dead, 2, 1, 0, 0);
        LCD_Sprite(xP2, yP2, 30, 30, dead, 2, 1, 0, 0);
        mapeo_SD("gameOver.txt", 67, 75, 186);
        delay(3000);
        LCD_Clear(0x00);
        enemy1Life = 0;
        enemy1y = 0;
        enemy2y = 0;
        enemy3y = 0;
        enemy4y = 0;
        enemy5y = 0;
        enemy6y = 0;
        enemy7y = 0;
        enemy8y = 0;
        enemy9y = 0;
        enemy10y = 0;
        enemy11y = 0;
        enemy12y = 0;
        enemy13y = 0;
        enemy14y = 0;
        enemy15y = 0;
        enemy16y = 0;
        enemy17y = 0;
        enemy18y = 0;
        enemy19y = 0;
        enemy20y = 0;
        enemy21y = 0;
        enemy22y = 0;
        enemy23y = 0;
        tripleWave1 = 0;
        tripleWave2 = 0;
        tripleWave3 = 0;
        tripleWave4 = 0;
        tripleWave5 = 0;
        P1Life = 0;
        P2Life = 0;
        xP1 = 0;
        yP1 = 209;
        xP2 = 290;
        yP2 = 209;
        state = 0;
        turn = 0;
        mapeo_SD("menu.txt", 32, 25, 256);
      }
      if(((xP1+29)>xEne5Bullet && (xP1+1)<(xEne5Bullet+10) && yP1<(yEne5Bullet+10) && (yP1+30)>yEne5Bullet)
      || ((xP2+29)>xEne5Bullet && (xP2+1)<(xEne5Bullet+10) && yP2<(yEne5Bullet+10) && (yP2+30)>yEne5Bullet)){
        enemy4Life = 1;
        enemy5Life = 1;
        enemy6Life = 1;
        P1Life = 1;
        P2Life = 1;
        LCD_Sprite(xP1, yP1, 30, 30, dead, 2, 1, 0, 0);
        LCD_Sprite(xP2, yP2, 30, 30, dead, 2, 1, 0, 0);
        mapeo_SD("gameOver.txt", 67, 75, 186);
        delay(3000);
        LCD_Clear(0x00);
        enemy1Life = 0;
        enemy1y = 0;
        enemy2y = 0;
        enemy3y = 0;
        enemy4y = 0;
        enemy5y = 0;
        enemy6y = 0;
        enemy7y = 0;
        enemy8y = 0;
        enemy9y = 0;
        enemy10y = 0;
        enemy11y = 0;
        enemy12y = 0;
        enemy13y = 0;
        enemy14y = 0;
        enemy15y = 0;
        enemy16y = 0;
        enemy17y = 0;
        enemy18y = 0;
        enemy19y = 0;
        enemy20y = 0;
        enemy21y = 0;
        enemy22y = 0;
        enemy23y = 0;
        tripleWave1 = 0;
        tripleWave2 = 0;
        tripleWave3 = 0;
        tripleWave4 = 0;
        tripleWave5 = 0;
        P1Life = 0;
        P2Life = 0;
        xP1 = 0;
        yP1 = 209;
        xP2 = 290;
        yP2 = 209;
        state = 0;
        turn = 0;
        mapeo_SD("menu.txt", 32, 25, 256);
      }
    }
  //**********************************************************************************************************************************
  //Enemy #5 code
  //**********************************************************************************************************************************
  
  //**********************************************************************************************************************************
  //Enemy #6 code
  //**********************************************************************************************************************************
    if(enemy6Life == 0){
      if(enemy6y == 0){
        xEne6Bullet = enemy6x + 8;
        yEne6Bullet = enemy6y + 30;
      }
      if(enemy6y == 110){
        xEne6Bullet = enemy6x + 8;
        yEne6Bullet = enemy6y + 30; 
      }
      int animEne6 = (enemy6y/5)%2;
      LCD_Sprite(enemy6x, enemy6y, 28, 30, enemyTankForward, 2, animEne6, 0, 0);
      H_line(enemy6x, enemy6y-1, 28,0x00);
      LCD_Bitmap(xEne6Bullet, yEne6Bullet, 10, 10, bulletDown);
      H_line(xEne6Bullet, yEne6Bullet, 10, 0x00);
      if(enemy6Pace == 0){
        enemy6y++; 
        enemy6Pace = 1;
      }else if(enemy6Pace == 1){
        enemy6Pace = 2;  
        //yEne6Bullet++;
        if(yEne6Bullet >= 250){
          yEne6Bullet = 245;  
        }
      }else if(enemy6Pace == 2){
        enemy6y++;
        enemy6Pace = 3;
      }else if(enemy6Pace == 3){
        enemy6Pace = 4;  
        //yEne6Bullet++;
        if(yEne6Bullet >= 250){
          yEne6Bullet = 245;  
        }
      }else if(enemy6Pace == 4){
        enemy6y++;
        enemy6Pace =  5;
      }else if(enemy6Pace == 5){
        enemy6Pace =  0;
        //yEne6Bullet++;
        if(yEne6Bullet >= 250){
          yEne6Bullet = 245;  
        }
      }
      yEne6Bullet++;
      if(yEne6Bullet >=250){
        yEne6Bullet = 245;  
      }
   
      if((enemy6y == 209)||((xP1+29)>enemy6x && (xP1+1)<(enemy6x+28) && yP1<(enemy6y+30) && (yP1+30)>enemy6y)
      || ((xP2+29)>enemy6x && (xP2+1)<(enemy6x+28) && yP2<(enemy6y+30) && (yP2+30)>enemy6y)){
        enemy4Life = 1;
        enemy5Life = 1;
        enemy6Life = 1;
        P1Life = 1;
        P2Life = 1;
        LCD_Sprite(xP1, yP1, 30, 30, dead, 2, 1, 0, 0);
        LCD_Sprite(xP2, yP2, 30, 30, dead, 2, 1, 0, 0);
        mapeo_SD("gameOver.txt", 67, 75, 186);
        delay(3000);
        LCD_Clear(0x00);
        enemy1Life = 0;
        enemy1y = 0;
        enemy2y = 0;
        enemy3y = 0;
        enemy4y = 0;
        enemy5y = 0;
        enemy6y = 0;
        enemy7y = 0;
        enemy8y = 0;
        enemy9y = 0;
        enemy10y = 0;
        enemy11y = 0;
        enemy12y = 0;
        enemy13y = 0;
        enemy14y = 0;
        enemy15y = 0;
        enemy16y = 0;
        enemy17y = 0;
        enemy18y = 0;
        enemy19y = 0;
        enemy20y = 0;
        enemy21y = 0;
        enemy22y = 0;
        enemy23y = 0;
        tripleWave1 = 0;
        tripleWave2 = 0;
        tripleWave3 = 0;
        tripleWave4 = 0;
        tripleWave5 = 0;
        P1Life = 0;
        P2Life = 0;
        xP1 = 0;
        yP1 = 209;
        xP2 = 290;
        yP2 = 209;
        state = 0;
        turn = 0;
        mapeo_SD("menu.txt", 32, 25, 256);
      }
      if(((xP1+29)>xEne6Bullet && (xP1+1)<(xEne6Bullet+10) && yP1<(yEne6Bullet+10) && (yP1+30)>yEne6Bullet)
      || ((xP2+29)>xEne6Bullet && (xP2+1)<(xEne6Bullet+10) && yP2<(yEne6Bullet+10) && (yP2+30)>yEne6Bullet)){
        enemy4Life = 1;
        enemy5Life = 1;
        enemy6Life = 1;
        P1Life = 1;
        P2Life = 1;
        LCD_Sprite(xP1, yP1, 30, 30, dead, 2, 1, 0, 0);
        LCD_Sprite(xP2, yP2, 30, 30, dead, 2, 1, 0, 0);
        mapeo_SD("gameOver.txt", 67, 75, 186);
        delay(3000);
        LCD_Clear(0x00);
        enemy1Life = 0;
        enemy1y = 0;
        enemy2y = 0;
        enemy3y = 0;
        enemy4y = 0;
        enemy5y = 0;
        enemy6y = 0;
        enemy7y = 0;
        enemy8y = 0;
        enemy9y = 0;
        enemy10y = 0;
        enemy11y = 0;
        enemy12y = 0;
        enemy13y = 0;
        enemy14y = 0;
        enemy15y = 0;
        enemy16y = 0;
        enemy17y = 0;
        enemy18y = 0;
        enemy19y = 0;
        enemy20y = 0;
        enemy21y = 0;
        enemy22y = 0;
        enemy23y = 0;
        tripleWave1 = 0;
        tripleWave2 = 0;
        tripleWave3 = 0;
        tripleWave4 = 0;
        tripleWave5 = 0;
        P1Life = 0;
        P2Life = 0;
        xP1 = 0;
        yP1 = 209;
        xP2 = 290;
        yP2 = 209;
        state = 0;
        turn = 0;
        mapeo_SD("menu.txt", 32, 25, 256);
      }
    }
  //**********************************************************************************************************************************
  //Enemy #6 code
  //**********************************************************************************************************************************
  
  //**********************************************************************************************************************************
  //Enemy #7 code
  //**********************************************************************************************************************************
    if(enemy7Life == 0){
      if(enemy7y == 0){
        xEne7Bullet = enemy7x + 8;
        yEne7Bullet = enemy7y + 30;
      }
      if(enemy7y == 110){
        xEne7Bullet = enemy7x + 8;
        yEne7Bullet = enemy7y + 30; 
      }
      int animEne7 = (enemy7y/5)%2;
      LCD_Sprite(enemy7x, enemy7y, 28, 30, enemyTankForward, 2, animEne7, 0, 0);
      H_line(enemy7x, enemy7y-1, 28,0x00);
      LCD_Bitmap(xEne7Bullet, yEne7Bullet, 10, 10, bulletDown);
      H_line(xEne7Bullet, yEne7Bullet, 10, 0x00);
      if(enemy7Pace == 0){
        enemy7y++; 
        enemy7Pace = 1;
      }else if(enemy7Pace == 1){
        enemy7Pace = 2;  
        //yEne7Bullet++;
        if(yEne7Bullet >= 250){
          yEne7Bullet = 245;  
        }
      }else if(enemy7Pace == 2){
        enemy7y++;
        enemy7Pace = 3;
      }else if(enemy7Pace == 3){
        enemy7Pace = 4;  
        //yEne7Bullet++;
        if(yEne7Bullet >= 250){
          yEne7Bullet = 245;  
        }
      }else if(enemy7Pace == 4){
        enemy7y++;
        enemy7Pace =  5;
      }else if(enemy7Pace == 5){
        enemy7Pace =  0;
        //yEne7Bullet++;
        if(yEne7Bullet >= 250){
          yEne7Bullet = 245;  
        }
      }
      yEne7Bullet++;
      if(yEne7Bullet >=250){
        yEne7Bullet = 245;  
      }
   
      if((enemy7y == 209)||((xP1+29)>enemy7x && (xP1+1)<(enemy7x+28) && yP1<(enemy7y+30) && (yP1+30)>enemy7y)
      || ((xP2+29)>enemy7x && (xP2+1)<(enemy7x+28) && yP2<(enemy7y+30) && (yP2+30)>enemy7y)){
        enemy7Life = 1;
        enemy8Life = 1;
        enemy9Life = 1;
        P1Life = 1;
        P2Life = 1;
        LCD_Sprite(xP1, yP1, 30, 30, dead, 2, 1, 0, 0);
        LCD_Sprite(xP2, yP2, 30, 30, dead, 2, 1, 0, 0);
        mapeo_SD("gameOver.txt", 67, 75, 186);
        delay(3000);
        LCD_Clear(0x00);
        enemy1Life = 0;
        enemy1y = 0;
        enemy2y = 0;
        enemy3y = 0;
        enemy4y = 0;
        enemy5y = 0;
        enemy6y = 0;
        enemy7y = 0;
        enemy8y = 0;
        enemy9y = 0;
        enemy10y = 0;
        enemy11y = 0;
        enemy12y = 0;
        enemy13y = 0;
        enemy14y = 0;
        enemy15y = 0;
        enemy16y = 0;
        enemy17y = 0;
        enemy18y = 0;
        enemy19y = 0;
        enemy20y = 0;
        enemy21y = 0;
        enemy22y = 0;
        enemy23y = 0;
        tripleWave1 = 0;
        tripleWave2 = 0;
        tripleWave3 = 0;
        tripleWave4 = 0;
        tripleWave5 = 0;
        P1Life = 0;
        P2Life = 0;
        xP1 = 0;
        yP1 = 209;
        xP2 = 290;
        yP2 = 209;
        state = 0;
        turn = 0;
        mapeo_SD("menu.txt", 32, 25, 256);
      }
      if(((xP1+29)>xEne7Bullet && (xP1+1)<(xEne7Bullet+10) && yP1<(yEne7Bullet+10) && (yP1+30)>yEne7Bullet)
      || ((xP2+29)>xEne7Bullet && (xP2+1)<(xEne7Bullet+10) && yP2<(yEne7Bullet+10) && (yP2+30)>yEne7Bullet)){
        enemy7Life = 1;
        enemy8Life = 1;
        enemy9Life = 1;
        P1Life = 1;
        P2Life = 1;
        LCD_Sprite(xP1, yP1, 30, 30, dead, 2, 1, 0, 0);
        LCD_Sprite(xP2, yP2, 30, 30, dead, 2, 1, 0, 0);
        mapeo_SD("gameOver.txt", 67, 75, 186);
        delay(3000);
        LCD_Clear(0x00);
        enemy1Life = 0;
        enemy1y = 0;
        enemy2y = 0;
        enemy3y = 0;
        enemy4y = 0;
        enemy5y = 0;
        enemy6y = 0;
        enemy7y = 0;
        enemy8y = 0;
        enemy9y = 0;
        enemy10y = 0;
        enemy11y = 0;
        enemy12y = 0;
        enemy13y = 0;
        enemy14y = 0;
        enemy15y = 0;
        enemy16y = 0;
        enemy17y = 0;
        enemy18y = 0;
        enemy19y = 0;
        enemy20y = 0;
        enemy21y = 0;
        enemy22y = 0;
        enemy23y = 0;
        tripleWave1 = 0;
        tripleWave2 = 0;
        tripleWave3 = 0;
        tripleWave4 = 0;
        tripleWave5 = 0;
        P1Life = 0;
        P2Life = 0;
        xP1 = 0;
        yP1 = 209;
        xP2 = 290;
        yP2 = 209;
        state = 0;
        turn = 0;
        mapeo_SD("menu.txt", 32, 25, 256);
      }
    }
  //**********************************************************************************************************************************
  //Enemy #7 code
  //**********************************************************************************************************************************
  
  //**********************************************************************************************************************************
  //Enemy #8 code
  //**********************************************************************************************************************************
    if(enemy8Life == 0){
      if(enemy8y == 0){
        xEne8Bullet = enemy8x + 8;
        yEne8Bullet = enemy8y + 30;
      }
      if(enemy8y == 110){
        xEne8Bullet = enemy8x + 8;
        yEne8Bullet = enemy8y + 30; 
      }
      int animEne8 = (enemy8y/5)%2;
      LCD_Sprite(enemy8x, enemy8y, 28, 30, enemyTankForward, 2, animEne8, 0, 0);
      H_line(enemy8x, enemy8y-1, 28,0x00);
      LCD_Bitmap(xEne8Bullet, yEne8Bullet, 10, 10, bulletDown);
      H_line(xEne8Bullet, yEne8Bullet, 10, 0x00);
      if(enemy8Pace == 0){
        enemy8y++; 
        enemy8Pace = 1;
      }else if(enemy8Pace == 1){
        enemy8Pace = 2;  
        //yEne8Bullet++;
        if(yEne8Bullet >= 250){
          yEne8Bullet = 245;  
        }
      }else if(enemy8Pace == 2){
        enemy8y++;
        enemy8Pace = 3;
      }else if(enemy8Pace == 3){
        enemy8Pace = 4;  
        //yEne8Bullet++;
        if(yEne8Bullet >= 250){
          yEne8Bullet = 245;  
        }
      }else if(enemy8Pace == 4){
        enemy8y++;
        enemy8Pace =  5;
      }else if(enemy8Pace == 5){
        enemy8Pace =  0;
        //yEne8Bullet++;
        if(yEne8Bullet >= 250){
          yEne8Bullet = 245;  
        }
      }
      yEne8Bullet++;
      if(yEne8Bullet >=250){
        yEne8Bullet = 245;  
      }
   
      if((enemy8y == 209)||((xP1+29)>enemy8x && (xP1+1)<(enemy8x+28) && yP1<(enemy8y+30) && (yP1+30)>enemy8y)
      || ((xP2+29)>enemy8x && (xP2+1)<(enemy8x+28) && yP2<(enemy8y+30) && (yP2+30)>enemy8y)){
        enemy7Life = 1;
        enemy8Life = 1;
        enemy9Life = 1;
        P1Life = 1;
        P2Life = 1;
        LCD_Sprite(xP1, yP1, 30, 30, dead, 2, 1, 0, 0);
        LCD_Sprite(xP2, yP2, 30, 30, dead, 2, 1, 0, 0);
        mapeo_SD("gameOver.txt", 67, 75, 186);
        delay(3000);
        LCD_Clear(0x00);
        enemy1Life = 0;
        enemy1y = 0;
        enemy2y = 0;
        enemy3y = 0;
        enemy4y = 0;
        enemy5y = 0;
        enemy6y = 0;
        enemy7y = 0;
        enemy8y = 0;
        enemy9y = 0;
        enemy10y = 0;
        enemy11y = 0;
        enemy12y = 0;
        enemy13y = 0;
        enemy14y = 0;
        enemy15y = 0;
        enemy16y = 0;
        enemy17y = 0;
        enemy18y = 0;
        enemy19y = 0;
        enemy20y = 0;
        enemy21y = 0;
        enemy22y = 0;
        enemy23y = 0;
        tripleWave1 = 0;
        tripleWave2 = 0;
        tripleWave3 = 0;
        tripleWave4 = 0;
        tripleWave5 = 0;
        P1Life = 0;
        P2Life = 0;
        xP1 = 0;
        yP1 = 209;
        xP2 = 290;
        yP2 = 209;
        state = 0;
        turn = 0;
        mapeo_SD("menu.txt", 32, 25, 256);
      }
      if(((xP1+29)>xEne8Bullet && (xP1+1)<(xEne8Bullet+10) && yP1<(yEne8Bullet+10) && (yP1+30)>yEne8Bullet)
      || ((xP2+29)>xEne8Bullet && (xP2+1)<(xEne8Bullet+10) && yP2<(yEne8Bullet+10) && (yP2+30)>yEne8Bullet)){
        enemy7Life = 1;
        enemy8Life = 1;
        enemy9Life = 1;
        P1Life = 1;
        P2Life = 1;
        LCD_Sprite(xP1, yP1, 30, 30, dead, 2, 1, 0, 0);
        LCD_Sprite(xP2, yP2, 30, 30, dead, 2, 1, 0, 0);
        mapeo_SD("gameOver.txt", 67, 75, 186);
        delay(3000);
        LCD_Clear(0x00);
        enemy1Life = 0;
        enemy1y = 0;
        enemy2y = 0;
        enemy3y = 0;
        enemy4y = 0;
        enemy5y = 0;
        enemy6y = 0;
        enemy7y = 0;
        enemy8y = 0;
        enemy9y = 0;
        enemy10y = 0;
        enemy11y = 0;
        enemy12y = 0;
        enemy13y = 0;
        enemy14y = 0;
        enemy15y = 0;
        enemy16y = 0;
        enemy17y = 0;
        enemy18y = 0;
        enemy19y = 0;
        enemy20y = 0;
        enemy21y = 0;
        enemy22y = 0;
        enemy23y = 0;
        tripleWave1 = 0;
        tripleWave2 = 0;
        tripleWave3 = 0;
        tripleWave4 = 0;
        tripleWave5 = 0;
        P1Life = 0;
        P2Life = 0;
        xP1 = 0;
        yP1 = 209;
        xP2 = 290;
        yP2 = 209;
        state = 0;
        turn = 0;
        mapeo_SD("menu.txt", 32, 25, 256);
      }
    }
  //**********************************************************************************************************************************
  //Enemy #8 code
  //**********************************************************************************************************************************
  
  //**********************************************************************************************************************************
  //Enemy #9 code
  //**********************************************************************************************************************************
    if(enemy9Life == 0){
      if(enemy9y == 0){
        xEne9Bullet = enemy9x + 8;
        yEne9Bullet = enemy9y + 30;
      }
      if(enemy9y == 110){
        xEne9Bullet = enemy9x + 8;
        yEne9Bullet = enemy9y + 30; 
      }
      int animEne9 = (enemy9y/5)%2;
      LCD_Sprite(enemy9x, enemy9y, 28, 30, enemyTankForward, 2, animEne9, 0, 0);
      H_line(enemy9x, enemy9y-1, 28,0x00);
      LCD_Bitmap(xEne9Bullet, yEne9Bullet, 10, 10, bulletDown);
      H_line(xEne9Bullet, yEne9Bullet, 10, 0x00);
      if(enemy9Pace == 0){
        enemy9y++; 
        enemy9Pace = 1;
      }else if(enemy9Pace == 1){
        enemy9Pace = 2;  
        //yEne9Bullet++;
        if(yEne9Bullet >= 250){
          yEne9Bullet = 245;  
        }
      }else if(enemy9Pace == 2){
        enemy9y++;
        enemy9Pace = 3;
      }else if(enemy9Pace == 3){
        enemy9Pace = 4;  
        //yEne9Bullet++;
        if(yEne9Bullet >= 250){
          yEne9Bullet = 245;  
        }
      }else if(enemy9Pace == 4){
        enemy9y++;
        enemy9Pace =  5;
      }else if(enemy9Pace == 5){
        enemy9Pace =  0;
        //yEne9Bullet++;
        if(yEne9Bullet >= 250){
          yEne9Bullet = 245;  
        }
      }
      yEne9Bullet++;
      if(yEne9Bullet >=250){
        yEne9Bullet = 245;  
      }
   
      if((enemy9y == 209)||((xP1+29)>enemy9x && (xP1+1)<(enemy9x+28) && yP1<(enemy9y+30) && (yP1+30)>enemy9y)
      || ((xP2+29)>enemy9x && (xP2+1)<(enemy9x+28) && yP2<(enemy9y+30) && (yP2+30)>enemy9y)){
        enemy7Life = 1;
        enemy8Life = 1;
        enemy9Life = 1;
        P1Life = 1;
        P2Life = 1;
        LCD_Sprite(xP1, yP1, 30, 30, dead, 2, 1, 0, 0);
        LCD_Sprite(xP2, yP2, 30, 30, dead, 2, 1, 0, 0);
        mapeo_SD("gameOver.txt", 67, 75, 186);
        delay(3000);
        LCD_Clear(0x00);
        enemy1Life = 0;
        enemy1y = 0;
        enemy2y = 0;
        enemy3y = 0;
        enemy4y = 0;
        enemy5y = 0;
        enemy6y = 0;
        enemy7y = 0;
        enemy8y = 0;
        enemy9y = 0;
        enemy10y = 0;
        enemy11y = 0;
        enemy12y = 0;
        enemy13y = 0;
        enemy14y = 0;
        enemy15y = 0;
        enemy16y = 0;
        enemy17y = 0;
        enemy18y = 0;
        enemy19y = 0;
        enemy20y = 0;
        enemy21y = 0;
        enemy22y = 0;
        enemy23y = 0;
        tripleWave1 = 0;
        tripleWave2 = 0;
        tripleWave3 = 0;
        tripleWave4 = 0;
        tripleWave5 = 0;
        P1Life = 0;
        P2Life = 0;
        xP1 = 0;
        yP1 = 209;
        xP2 = 290;
        yP2 = 290;
        state = 0;
        turn = 0;
        mapeo_SD("menu.txt", 32, 25, 256);
      }
      if(((xP1+29)>xEne9Bullet && (xP1+1)<(xEne9Bullet+10) && yP1<(yEne9Bullet+10) && (yP1+30)>yEne9Bullet)
      || ((xP2+29)>xEne9Bullet && (xP2+1)<(xEne9Bullet+10) && yP2<(yEne9Bullet+10) && (yP2+30)>yEne9Bullet)){
        enemy7Life = 1;
        enemy8Life = 1;
        enemy9Life = 1;
        P1Life = 1;
        P2Life = 1;
        LCD_Sprite(xP1, yP1, 30, 30, dead, 2, 1, 0, 0);
        LCD_Sprite(xP2, yP2, 30, 30, dead, 2, 1, 0, 0);
        mapeo_SD("gameOver.txt", 67, 75, 186);
        delay(3000);
        LCD_Clear(0x00);
        enemy1Life = 0;
        enemy1y = 0;
        enemy2y = 0;
        enemy3y = 0;
        enemy4y = 0;
        enemy5y = 0;
        enemy6y = 0;
        enemy7y = 0;
        enemy8y = 0;
        enemy9y = 0;
        enemy10y = 0;
        enemy11y = 0;
        enemy12y = 0;
        enemy13y = 0;
        enemy14y = 0;
        enemy15y = 0;
        enemy16y = 0;
        enemy17y = 0;
        enemy18y = 0;
        enemy19y = 0;
        enemy20y = 0;
        enemy21y = 0;
        enemy22y = 0;
        enemy23y = 0;
        tripleWave1 = 0;
        tripleWave2 = 0;
        tripleWave3 = 0;
        tripleWave4 = 0;
        tripleWave5 = 0;
        P1Life = 0;
        P2Life = 0;
        xP1 = 0;
        yP1 = 209;
        xP2 = 290;
        yP2 = 209;
        state = 0;
        turn = 0;
        mapeo_SD("menu.txt", 32, 25, 256);
      }
    }
  //**********************************************************************************************************************************
  //Enemy #9 code
  //**********************************************************************************************************************************
  
  //**********************************************************************************************************************************
  //Enemy #10 code
  //**********************************************************************************************************************************
    if(enemy10Life == 0){
      if(enemy10y == 0){
        xEne10Bullet = enemy10x + 8;
        yEne10Bullet = enemy10y + 30;
      }
      if(enemy10y == 110){
        xEne10Bullet = enemy10x + 8;
        yEne10Bullet = enemy10y + 30; 
      }
      int animEne10 = (enemy10y/5)%2;
      LCD_Sprite(enemy10x, enemy10y, 28, 30, enemyTankForward, 2, animEne10, 0, 0);
      H_line(enemy10x, enemy10y-1, 28,0x00);
      LCD_Bitmap(xEne10Bullet, yEne10Bullet, 10, 10, bulletDown);
      H_line(xEne10Bullet, yEne10Bullet, 10, 0x00);
      if(enemy10Pace == 0){
        enemy10y++; 
        enemy10Pace = 1;
      }else if(enemy10Pace == 1){
        enemy10Pace = 2;  
        //yEne10Bullet++;
        if(yEne10Bullet >= 250){
          yEne10Bullet = 245;  
        }
      }else if(enemy10Pace == 2){
        enemy10y++;
        enemy10Pace = 3;
      }else if(enemy10Pace == 3){
        enemy10Pace = 4;  
        //yEne10Bullet++;
        if(yEne10Bullet >= 250){
          yEne10Bullet = 245;  
        }
      }else if(enemy10Pace == 4){
        enemy10y++;
        enemy10Pace =  5;
      }else if(enemy10Pace == 5){
        enemy10Pace =  0;
        //yEne10Bullet++;
        if(yEne10Bullet >= 250){
          yEne10Bullet = 245;  
        }
      }
      yEne10Bullet++;
      if(yEne10Bullet >=250){
        yEne10Bullet = 245;  
      }
   
      if((enemy10y == 209)||((xP1+29)>enemy10x && (xP1+1)<(enemy10x+28) && yP1<(enemy10y+30) && (yP1+30)>enemy10y)
      || ((xP2+29)>enemy10x && (xP2+1)<(enemy10x+28) && yP2<(enemy10y+30) && (yP2+30)>enemy10y)){
        enemy10Life = 1;
        enemy11Life = 1;
        enemy12Life = 1;
        enemy13Life = 1;
        P1Life = 1;
        P2Life = 1;
        LCD_Sprite(xP1, yP1, 30, 30, dead, 2, 1, 0, 0);
        LCD_Sprite(xP2, yP2, 30, 30, dead, 2, 1, 0, 0);
        mapeo_SD("gameOver.txt", 67, 75, 186);
        delay(3000);
        LCD_Clear(0x00);
        enemy1Life = 0;
        enemy1y = 0;
        enemy2y = 0;
        enemy3y = 0;
        enemy4y = 0;
        enemy5y = 0;
        enemy6y = 0;
        enemy7y = 0;
        enemy8y = 0;
        enemy9y = 0;
        enemy10y = 0;
        enemy11y = 0;
        enemy12y = 0;
        enemy13y = 0;
        enemy14y = 0;
        enemy15y = 0;
        enemy16y = 0;
        enemy17y = 0;
        enemy18y = 0;
        enemy19y = 0;
        enemy20y = 0;
        enemy21y = 0;
        enemy22y = 0;
        enemy23y = 0;
        tripleWave1 = 0;
        tripleWave2 = 0;
        tripleWave3 = 0;
        tripleWave4 = 0;
        tripleWave5 = 0;
        P1Life = 0;
        P2Life = 0;
        xP1 = 0;
        yP1 = 209;
        xP2 = 290;
        yP2 = 209;
        state = 0;
        turn = 0;
        mapeo_SD("menu.txt", 32, 25, 256);
      }
      if(((xP1+29)>xEne10Bullet && (xP1+1)<(xEne10Bullet+10) && yP1<(yEne10Bullet+10) && (yP1+30)>yEne10Bullet)
      || ((xP2+29)>xEne10Bullet && (xP2+1)<(xEne10Bullet+10) && yP2<(yEne10Bullet+10) && (yP2+30)>yEne10Bullet)){
        enemy10Life = 1;
        enemy11Life = 1;
        enemy12Life = 1;
        enemy13Life = 1;
        P1Life = 1;
        P2Life = 1;
        LCD_Sprite(xP1, yP1, 30, 30, dead, 2, 1, 0, 0);
        LCD_Sprite(xP2, yP2, 30, 30, dead, 2, 1, 0, 0);
        mapeo_SD("gameOver.txt", 67, 75, 186);
        delay(3000);
        LCD_Clear(0x00);
        enemy1Life = 0;
        enemy1y = 0;
        enemy2y = 0;
        enemy3y = 0;
        enemy4y = 0;
        enemy5y = 0;
        enemy6y = 0;
        enemy7y = 0;
        enemy8y = 0;
        enemy9y = 0;
        enemy10y = 0;
        enemy11y = 0;
        enemy12y = 0;
        enemy13y = 0;
        enemy14y = 0;
        enemy15y = 0;
        enemy16y = 0;
        enemy17y = 0;
        enemy18y = 0;
        enemy19y = 0;
        enemy20y = 0;
        enemy21y = 0;
        enemy22y = 0;
        enemy23y = 0;
        tripleWave1 = 0;
        tripleWave2 = 0;
        tripleWave3 = 0;
        tripleWave4 = 0;
        tripleWave5 = 0;
        P1Life = 0;
        P2Life = 0;
        xP1 = 0;
        yP1 = 209;
        xP2 = 290;
        yP2 = 209;
        state = 0;
        turn = 0;
        mapeo_SD("menu.txt", 32, 25, 256);
      }
    }
  //**********************************************************************************************************************************
  //Enemy #10 code
  //**********************************************************************************************************************************
  
  //**********************************************************************************************************************************
  //Enemy #11 code
  //**********************************************************************************************************************************
    if(enemy11Life == 0){
      if(enemy11y == 0){
        xEne11Bullet = enemy11x + 8;
        yEne11Bullet = enemy11y + 30;
      }
      if(enemy11y == 110){
        xEne11Bullet = enemy11x + 8;
        yEne11Bullet = enemy11y + 30; 
      }
      int animEne11 = (enemy11y/5)%2;
      LCD_Sprite(enemy11x, enemy11y, 28, 30, enemyTankForward, 2, animEne11, 0, 0);
      H_line(enemy11x, enemy11y-1, 28,0x00);
      LCD_Bitmap(xEne11Bullet, yEne11Bullet, 10, 10, bulletDown);
      H_line(xEne11Bullet, yEne11Bullet, 10, 0x00);
      if(enemy11Pace == 0){
        enemy11y++; 
        enemy11Pace = 1;
      }else if(enemy11Pace == 1){
        enemy11Pace = 2;  
        //yEne11Bullet++;
        if(yEne11Bullet >= 250){
          yEne11Bullet = 245;  
        }
      }else if(enemy11Pace == 2){
        enemy11y++;
        enemy11Pace = 3;
      }else if(enemy11Pace == 3){
        enemy11Pace = 4;  
        //yEne11Bullet++;
        if(yEne11Bullet >= 250){
          yEne11Bullet = 245;  
        }
      }else if(enemy11Pace == 4){
        enemy11y++;
        enemy11Pace =  5;
      }else if(enemy11Pace == 5){
        enemy11Pace =  0;
        //yEne11Bullet++;
        if(yEne11Bullet >= 250){
          yEne11Bullet = 245;  
        }
      }
      yEne11Bullet++;
      if(yEne11Bullet >=250){
        yEne11Bullet = 245;  
      }
   
      if((enemy11y == 209)||((xP1+29)>enemy11x && (xP1+1)<(enemy11x+28) && yP1<(enemy11y+30) && (yP1+30)>enemy11y)
      || ((xP2+29)>enemy11x && (xP2+1)<(enemy11x+28) && yP2<(enemy11y+30) && (yP2+30)>enemy11y)){
        enemy10Life = 1;
        enemy11Life = 1;
        enemy12Life = 1;
        enemy13Life = 1;
        P1Life = 1;
        P2Life = 1;
        LCD_Sprite(xP1, yP1, 30, 30, dead, 2, 1, 0, 0);
        LCD_Sprite(xP2, yP2, 30, 30, dead, 2, 1, 0, 0);
        mapeo_SD("gameOver.txt", 67, 75, 186);
        delay(3000);
        LCD_Clear(0x00);
        enemy1Life = 0;
        enemy1y = 0;
        enemy2y = 0;
        enemy3y = 0;
        enemy4y = 0;
        enemy5y = 0;
        enemy6y = 0;
        enemy7y = 0;
        enemy8y = 0;
        enemy9y = 0;
        enemy10y = 0;
        enemy11y = 0;
        enemy12y = 0;
        enemy13y = 0;
        enemy14y = 0;
        enemy15y = 0;
        enemy16y = 0;
        enemy17y = 0;
        enemy18y = 0;
        enemy19y = 0;
        enemy20y = 0;
        enemy21y = 0;
        enemy22y = 0;
        enemy23y = 0;
        tripleWave1 = 0;
        tripleWave2 = 0;
        tripleWave3 = 0;
        tripleWave4 = 0;
        tripleWave5 = 0;
        P1Life = 0;
        P2Life = 0;
        xP1 = 0;
        yP1 = 209;
        xP2 = 290;
        yP2 = 209;
        state = 0;
        turn = 0;
        mapeo_SD("menu.txt", 32, 25, 256);
      }
      if(((xP1+29)>xEne11Bullet && (xP1+1)<(xEne11Bullet+10) && yP1<(yEne11Bullet+10) && (yP1+30)>yEne11Bullet)
      || ((xP2+29)>xEne11Bullet && (xP2+1)<(xEne11Bullet+10) && yP2<(yEne11Bullet+10) && (yP2+30)>yEne11Bullet)){
        enemy10Life = 1;
        enemy11Life = 1;
        enemy12Life = 1;
        enemy13Life = 1;
        P1Life = 1;
        P2Life = 1;
        LCD_Sprite(xP1, yP1, 30, 30, dead, 2, 1, 0, 0);
        LCD_Sprite(xP2, yP2, 30, 30, dead, 2, 1, 0, 0);
        mapeo_SD("gameOver.txt", 67, 75, 186);
        delay(3000);
        LCD_Clear(0x00);
        enemy1Life = 0;
        enemy1y = 0;
        enemy2y = 0;
        enemy3y = 0;
        enemy4y = 0;
        enemy5y = 0;
        enemy6y = 0;
        enemy7y = 0;
        enemy8y = 0;
        enemy9y = 0;
        enemy10y = 0;
        enemy11y = 0;
        enemy12y = 0;
        enemy13y = 0;
        enemy14y = 0;
        enemy15y = 0;
        enemy16y = 0;
        enemy17y = 0;
        enemy18y = 0;
        enemy19y = 0;
        enemy20y = 0;
        enemy21y = 0;
        enemy22y = 0;
        enemy23y = 0;
        tripleWave1 = 0;
        tripleWave2 = 0;
        tripleWave3 = 0;
        tripleWave4 = 0;
        tripleWave5 = 0;
        P1Life = 0;
        P2Life = 0;
        xP1 = 0;
        yP1 = 209;
        xP2 = 290;
        yP2 = 209;
        state = 0;
        turn = 0;
        mapeo_SD("menu.txt", 32, 25, 256);
      }
    }
  //**********************************************************************************************************************************
  //Enemy #11 code
  //**********************************************************************************************************************************
  
  //**********************************************************************************************************************************
  //Enemy #12 code
  //**********************************************************************************************************************************
    if(enemy12Life == 0){
      if(enemy12y == 0){
        xEne12Bullet = enemy12x + 8;
        yEne12Bullet = enemy12y + 30;
      }
      if(enemy12y == 110){
        xEne12Bullet = enemy12x + 8;
        yEne12Bullet = enemy12y + 30; 
      }
      int animEne12 = (enemy12y/5)%2;
      LCD_Sprite(enemy12x, enemy12y, 28, 30, enemyTankForward, 2, animEne12, 0, 0);
      H_line(enemy12x, enemy12y-1, 28,0x00);
      LCD_Bitmap(xEne12Bullet, yEne12Bullet, 10, 10, bulletDown);
      H_line(xEne12Bullet, yEne12Bullet, 10, 0x00);
      if(enemy12Pace == 0){
        enemy12y++; 
        enemy12Pace = 1;
      }else if(enemy12Pace == 1){
        enemy12Pace = 2;  
        //yEne12Bullet++;
        if(yEne12Bullet >= 250){
          yEne12Bullet = 245;  
        }
      }else if(enemy12Pace == 2){
        enemy12y++;
        enemy12Pace = 3;
      }else if(enemy12Pace == 3){
        enemy12Pace = 4;  
        //yEne12Bullet++;
        if(yEne12Bullet >= 250){
          yEne12Bullet = 245;  
        }
      }else if(enemy12Pace == 4){
        enemy12y++;
        enemy12Pace =  5;
      }else if(enemy12Pace == 5){
        enemy12Pace =  0;
        //yEne12Bullet++;
        if(yEne12Bullet >= 250){
          yEne12Bullet = 245;  
        }
      }
      yEne12Bullet++;
      if(yEne12Bullet >=250){
        yEne12Bullet = 245;  
      }
   
      if((enemy12y == 209)||((xP1+29)>enemy12x && (xP1+1)<(enemy12x+28) && yP1<(enemy12y+30) && (yP1+30)>enemy12y)
      || ((xP2+29)>enemy12x && (xP2+1)<(enemy12x+28) && yP2<(enemy12y+30) && (yP2+30)>enemy12y)){
        enemy10Life = 1;
        enemy11Life = 1;
        enemy12Life = 1;
        enemy13Life = 1;
        P1Life = 1;
        P2Life = 1;
        LCD_Sprite(xP1, yP1, 30, 30, dead, 2, 1, 0, 0);
        LCD_Sprite(xP2, yP2, 30, 30, dead, 2, 1, 0, 0);
        mapeo_SD("gameOver.txt", 67, 75, 186);
        delay(3000);
        LCD_Clear(0x00);
        enemy1Life = 0;
        enemy1y = 0;
        enemy2y = 0;
        enemy3y = 0;
        enemy4y = 0;
        enemy5y = 0;
        enemy6y = 0;
        enemy7y = 0;
        enemy8y = 0;
        enemy9y = 0;
        enemy10y = 0;
        enemy11y = 0;
        enemy12y = 0;
        enemy13y = 0;
        enemy14y = 0;
        enemy15y = 0;
        enemy16y = 0;
        enemy17y = 0;
        enemy18y = 0;
        enemy19y = 0;
        enemy20y = 0;
        enemy21y = 0;
        enemy22y = 0;
        enemy23y = 0;
        tripleWave1 = 0;
        tripleWave2 = 0;
        tripleWave3 = 0;
        tripleWave4 = 0;
        tripleWave5 = 0;
        P1Life = 0;
        P2Life = 0;
        xP1 = 0;
        yP1 = 209;
        xP2 = 290;
        yP2 = 209;
        state = 0;
        turn = 0;
        mapeo_SD("menu.txt", 32, 25, 256);
      }
      if(((xP1+29)>xEne12Bullet && (xP1+1)<(xEne12Bullet+10) && yP1<(yEne12Bullet+10) && (yP1+30)>yEne12Bullet)
      || ((xP2+29)>xEne12Bullet && (xP2+1)<(xEne12Bullet+10) && yP2<(yEne12Bullet+10) && (yP2+30)>yEne12Bullet)){
        enemy10Life = 1;
        enemy11Life = 1;
        enemy12Life = 1;
        enemy13Life = 1;
        P1Life = 1;
        P2Life = 1;
        LCD_Sprite(xP1, yP1, 30, 30, dead, 2, 1, 0, 0);
        LCD_Sprite(xP2, yP2, 30, 30, dead, 2, 1, 0, 0);
        mapeo_SD("gameOver.txt", 67, 75, 186);
        delay(3000);
        LCD_Clear(0x00);
        enemy1Life = 0;
        enemy1y = 0;
        enemy2y = 0;
        enemy3y = 0;
        enemy4y = 0;
        enemy5y = 0;
        enemy6y = 0;
        enemy7y = 0;
        enemy8y = 0;
        enemy9y = 0;
        enemy10y = 0;
        enemy11y = 0;
        enemy12y = 0;
        enemy13y = 0;
        enemy14y = 0;
        enemy15y = 0;
        enemy16y = 0;
        enemy17y = 0;
        enemy18y = 0;
        enemy19y = 0;
        enemy20y = 0;
        enemy21y = 0;
        enemy22y = 0;
        enemy23y = 0;
        tripleWave1 = 0;
        tripleWave2 = 0;
        tripleWave3 = 0;
        tripleWave4 = 0;
        tripleWave5 = 0;
        P1Life = 0;
        P2Life = 0;
        xP1 = 0;
        yP1 = 209;
        xP2 = 290;
        yP2 = 209;
        state = 0;
        turn = 0;
        mapeo_SD("menu.txt", 32, 25, 256);
      }
    }
  //**********************************************************************************************************************************
  //Enemy #12 code
  //**********************************************************************************************************************************
  
  //**********************************************************************************************************************************
  //Enemy #13 code
  //**********************************************************************************************************************************
    if(enemy13Life == 0){
      if(enemy13y == 0){
        xEne13Bullet = enemy13x + 8;
        yEne13Bullet = enemy13y + 30;
      }
      if(enemy13y == 110){
        xEne13Bullet = enemy13x + 8;
        yEne13Bullet = enemy13y + 30; 
      }
      int animEne13 = (enemy13y/5)%2;
      LCD_Sprite(enemy13x, enemy13y, 28, 30, enemyTankForward, 2, animEne13, 0, 0);
      H_line(enemy13x, enemy13y-1, 28,0x00);
      LCD_Bitmap(xEne13Bullet, yEne13Bullet, 10, 10, bulletDown);
      H_line(xEne13Bullet, yEne13Bullet, 10, 0x00);
      if(enemy13Pace == 0){
        enemy13y++; 
        enemy13Pace = 1;
      }else if(enemy13Pace == 1){
        enemy13Pace = 2;  
        //yEne13Bullet++;
        if(yEne13Bullet >= 250){
          yEne13Bullet = 245;  
        }
      }else if(enemy13Pace == 2){
        enemy13y++;
        enemy13Pace = 3;
      }else if(enemy13Pace == 3){
        enemy13Pace = 4;  
        //yEne13Bullet++;
        if(yEne13Bullet >= 250){
          yEne13Bullet = 245;  
        }
      }else if(enemy13Pace == 4){
        enemy13y++;
        enemy13Pace =  5;
      }else if(enemy13Pace == 5){
        enemy13Pace =  0;
        //yEne13Bullet++;
        if(yEne13Bullet >= 250){
          yEne13Bullet = 245;  
        }
      }
      yEne13Bullet++;
      if(yEne13Bullet >=250){
        yEne13Bullet = 245;  
      }
   
      if((enemy13y == 209)||((xP1+29)>enemy13x && (xP1+1)<(enemy13x+28) && yP1<(enemy13y+30) && (yP1+30)>enemy13y)
      || ((xP2+29)>enemy13x && (xP2+1)<(enemy13x+28) && yP2<(enemy13y+30) && (yP2+30)>enemy13y)){
        enemy10Life = 1;
        enemy11Life = 1;
        enemy12Life = 1;
        enemy13Life = 1;
        P1Life = 1;
        P2Life = 1;
        LCD_Sprite(xP1, yP1, 30, 30, dead, 2, 1, 0, 0);
        LCD_Sprite(xP1, yP1, 30, 30, dead, 2, 1, 0, 0);
        mapeo_SD("gameOver.txt", 67, 75, 186);
        delay(3000);
        LCD_Clear(0x00);
        enemy1Life = 0;
        enemy1y = 0;
        enemy2y = 0;
        enemy3y = 0;
        enemy4y = 0;
        enemy5y = 0;
        enemy6y = 0;
        enemy7y = 0;
        enemy8y = 0;
        enemy9y = 0;
        enemy10y = 0;
        enemy11y = 0;
        enemy12y = 0;
        enemy13y = 0;
        enemy14y = 0;
        enemy15y = 0;
        enemy16y = 0;
        enemy17y = 0;
        enemy18y = 0;
        enemy19y = 0;
        enemy20y = 0;
        enemy21y = 0;
        enemy22y = 0;
        enemy23y = 0;
        tripleWave1 = 0;
        tripleWave2 = 0;
        tripleWave3 = 0;
        tripleWave4 = 0;
        tripleWave5 = 0;
        P1Life = 0;
        P2Life = 0;
        xP1 = 0;
        yP1 = 209;
        xP2 = 290;
        yP2 = 209;
        state = 0;
        turn = 0;
        mapeo_SD("menu.txt", 32, 25, 256);
      }
      if(((xP1+29)>xEne13Bullet && (xP1+1)<(xEne13Bullet+10) && yP1<(yEne13Bullet+10) && (yP1+30)>yEne13Bullet)
      || ((xP2+29)>xEne13Bullet && (xP2+1)<(xEne13Bullet+10) && yP2<(yEne13Bullet+10) && (yP2+30)>yEne13Bullet)){
        enemy10Life = 1;
        enemy11Life = 1;
        enemy12Life = 1;
        enemy13Life = 1;
        P1Life = 1;
        P2Life = 1;
        LCD_Sprite(xP1, yP1, 30, 30, dead, 2, 1, 0, 0);
        LCD_Sprite(xP2, yP2, 30, 30, dead, 2, 1, 0, 0);
        mapeo_SD("gameOver.txt", 67, 75, 186);
        delay(3000);
        LCD_Clear(0x00);
        enemy1Life = 0;
        enemy1y = 0;
        enemy2y = 0;
        enemy3y = 0;
        enemy4y = 0;
        enemy5y = 0;
        enemy6y = 0;
        enemy7y = 0;
        enemy8y = 0;
        enemy9y = 0;
        enemy10y = 0;
        enemy11y = 0;
        enemy12y = 0;
        enemy13y = 0;
        enemy14y = 0;
        enemy15y = 0;
        enemy16y = 0;
        enemy17y = 0;
        enemy18y = 0;
        enemy19y = 0;
        enemy20y = 0;
        enemy21y = 0;
        enemy22y = 0;
        enemy23y = 0;
        tripleWave1 = 0;
        tripleWave2 = 0;
        tripleWave3 = 0;
        tripleWave4 = 0;
        tripleWave5 = 0;
        P1Life = 0;
        P2Life = 0;
        xP1 = 0;
        yP1 = 209;
        xP2 = 290;
        yP2 = 209;
        state = 0;
        turn = 0;
        mapeo_SD("menu.txt", 32, 25, 256);
      }
    }
  //**********************************************************************************************************************************
  //Enemy #13 code
  //**********************************************************************************************************************************
  
  //**********************************************************************************************************************************
  //Enemy #14 code
  //**********************************************************************************************************************************
    if(enemy14Life == 0){
      if(enemy14y == 0){
        xEne14Bullet = enemy14x + 8;
        yEne14Bullet = enemy14y + 30;
      }
      if(enemy14y == 110){
        xEne14Bullet = enemy14x + 8;
        yEne14Bullet = enemy14y + 30; 
      }
      int animEne14 = (enemy14y/5)%2;
      LCD_Sprite(enemy14x, enemy14y, 28, 30, enemyTankForward, 2, animEne14, 0, 0);
      H_line(enemy14x, enemy14y-1, 28,0x00);
      LCD_Bitmap(xEne14Bullet, yEne14Bullet, 10, 10, bulletDown);
      H_line(xEne14Bullet, yEne14Bullet, 10, 0x00);
      if(enemy14Pace == 0){
        enemy14y++; 
        enemy14Pace = 1;
      }else if(enemy14Pace == 1){
        enemy14Pace = 2;  
        //yEne14Bullet++;
        if(yEne14Bullet >= 250){
          yEne14Bullet = 245;  
        }
      }else if(enemy14Pace == 2){
        enemy14y++;
        enemy14Pace = 3;
      }else if(enemy14Pace == 3){
        enemy14Pace = 4;  
        //yEne14Bullet++;
        if(yEne14Bullet >= 250){
          yEne14Bullet = 245;  
        }
      }else if(enemy14Pace == 4){
        enemy14y++;
        enemy14Pace =  5;
      }else if(enemy14Pace == 5){
        enemy14Pace =  0;
        //yEne14Bullet++;
        if(yEne14Bullet >= 250){
          yEne14Bullet = 245;  
        }
      }
      yEne14Bullet++;
      if(yEne14Bullet >=250){
        yEne14Bullet = 245;  
      }
   
      if((enemy14y == 209)||((xP1+29)>enemy14x && (xP1+1)<(enemy14x+28) && yP1<(enemy14y+30) && (yP1+30)>enemy14y)
      || ((xP2+29)>enemy14x && (xP2+1)<(enemy14x+28) && yP2<(enemy14y+30) && (yP2+30)>enemy14y)){
        enemy14Life = 1;
        enemy15Life = 1;
        enemy16Life = 1;
        enemy17Life = 1;
        P1Life = 1;
        P2Life = 1;
        LCD_Sprite(xP1, yP1, 30, 30, dead, 2, 1, 0, 0);
        LCD_Sprite(xP2, yP2, 30, 30, dead, 2, 1, 0, 0);
        mapeo_SD("gameOver.txt", 67, 75, 186);
        delay(3000);
        LCD_Clear(0x00);
        enemy1Life = 0;
        enemy1y = 0;
        enemy2y = 0;
        enemy3y = 0;
        enemy4y = 0;
        enemy5y = 0;
        enemy6y = 0;
        enemy7y = 0;
        enemy8y = 0;
        enemy9y = 0;
        enemy10y = 0;
        enemy11y = 0;
        enemy12y = 0;
        enemy13y = 0;
        enemy14y = 0;
        enemy15y = 0;
        enemy16y = 0;
        enemy17y = 0;
        enemy18y = 0;
        enemy19y = 0;
        enemy20y = 0;
        enemy21y = 0;
        enemy22y = 0;
        enemy23y = 0;
        tripleWave1 = 0;
        tripleWave2 = 0;
        tripleWave3 = 0;
        tripleWave4 = 0;
        tripleWave5 = 0;
        P1Life = 0;
        P2Life = 0;
        xP1 = 0;
        yP1 = 209;
        xP2 = 290;
        yP2 = 209;
        state = 0;
        turn = 0;
        mapeo_SD("menu.txt", 32, 25, 256);
      }
      if(((xP1+29)>xEne14Bullet && (xP1+1)<(xEne14Bullet+10) && yP1<(yEne14Bullet+10) && (yP1+30)>yEne14Bullet)
      || ((xP2+29)>xEne14Bullet && (xP2+1)<(xEne14Bullet+10) && yP2<(yEne14Bullet+10) && (yP2+30)>yEne14Bullet)){
        enemy14Life = 1;
        enemy15Life = 1;
        enemy16Life = 1;
        enemy17Life = 1;
        P1Life = 1;
        P2Life = 1;
        LCD_Sprite(xP1, yP1, 30, 30, dead, 2, 1, 0, 0);
        LCD_Sprite(xP2, yP2, 30, 30, dead, 2, 1, 0, 0);
        mapeo_SD("gameOver.txt", 67, 75, 186);
        delay(3000);
        LCD_Clear(0x00);
        enemy1Life = 0;
        enemy1y = 0;
        enemy2y = 0;
        enemy3y = 0;
        enemy4y = 0;
        enemy5y = 0;
        enemy6y = 0;
        enemy7y = 0;
        enemy8y = 0;
        enemy9y = 0;
        enemy10y = 0;
        enemy11y = 0;
        enemy12y = 0;
        enemy13y = 0;
        enemy14y = 0;
        enemy15y = 0;
        enemy16y = 0;
        enemy17y = 0;
        enemy18y = 0;
        enemy19y = 0;
        enemy20y = 0;
        enemy21y = 0;
        enemy22y = 0;
        enemy23y = 0;
        tripleWave1 = 0;
        tripleWave2 = 0;
        tripleWave3 = 0;
        tripleWave4 = 0;
        tripleWave5 = 0;
        P1Life = 0;
        P2Life = 0;
        xP1 = 0;
        yP1 = 209;
        xP2 = 290;
        yP2 = 209;
        state = 0;
        turn = 0;
        mapeo_SD("menu.txt", 32, 25, 256);
      }
    }
  //**********************************************************************************************************************************
  //Enemy #14 code
  //**********************************************************************************************************************************
  
  //**********************************************************************************************************************************
  //Enemy #15 code
  //**********************************************************************************************************************************
    if(enemy15Life == 0){
      if(enemy15y == 0){
        xEne15Bullet = enemy15x + 8;
        yEne15Bullet = enemy15y + 30;
      }
      if(enemy15y == 110){
        xEne15Bullet = enemy15x + 8;
        yEne15Bullet = enemy15y + 30; 
      }
      int animEne15 = (enemy15y/5)%2;
      LCD_Sprite(enemy15x, enemy15y, 28, 30, enemyTankForward, 2, animEne15, 0, 0);
      H_line(enemy15x, enemy15y-1, 28,0x00);
      LCD_Bitmap(xEne15Bullet, yEne15Bullet, 10, 10, bulletDown);
      H_line(xEne15Bullet, yEne15Bullet, 10, 0x00);
      if(enemy15Pace == 0){
        enemy15y++; 
        enemy15Pace = 1;
      }else if(enemy15Pace == 1){
        enemy15Pace = 2;  
        //yEne15Bullet++;
        if(yEne15Bullet >= 250){
          yEne15Bullet = 245;  
        }
      }else if(enemy15Pace == 2){
        enemy15y++;
        enemy15Pace = 3;
      }else if(enemy15Pace == 3){
        enemy15Pace = 4;  
        //yEne15Bullet++;
        if(yEne15Bullet >= 250){
          yEne15Bullet = 245;  
        }
      }else if(enemy15Pace == 4){
        enemy15y++;
        enemy15Pace =  5;
      }else if(enemy15Pace == 5){
        enemy15Pace =  0;
        //yEne15Bullet++;
        if(yEne15Bullet >= 250){
          yEne15Bullet = 245;  
        }
      }
      yEne15Bullet++;
      if(yEne15Bullet >=250){
        yEne15Bullet = 245;  
      }
   
      if((enemy15y == 209)||((xP1+29)>enemy15x && (xP1+1)<(enemy15x+28) && yP1<(enemy15y+30) && (yP1+30)>enemy15y)
      || ((xP2+29)>enemy15x && (xP2+1)<(enemy15x+28) && yP2<(enemy15y+30) && (yP2+30)>enemy15y)){
        enemy14Life = 1;
        enemy15Life = 1;
        enemy16Life = 1;
        enemy17Life = 1;
        P1Life = 1;
        P2Life = 1;
        LCD_Sprite(xP1, yP1, 30, 30, dead, 2, 1, 0, 0);
        LCD_Sprite(xP2, yP2, 30, 30, dead, 2, 1, 0, 0);
        mapeo_SD("gameOver.txt", 67, 75, 186);
        delay(3000);
        LCD_Clear(0x00);
        enemy1Life = 0;
        enemy1y = 0;
        enemy2y = 0;
        enemy3y = 0;
        enemy4y = 0;
        enemy5y = 0;
        enemy6y = 0;
        enemy7y = 0;
        enemy8y = 0;
        enemy9y = 0;
        enemy10y = 0;
        enemy11y = 0;
        enemy12y = 0;
        enemy13y = 0;
        enemy14y = 0;
        enemy15y = 0;
        enemy16y = 0;
        enemy17y = 0;
        enemy18y = 0;
        enemy19y = 0;
        enemy20y = 0;
        enemy21y = 0;
        enemy22y = 0;
        enemy23y = 0;
        tripleWave1 = 0;
        tripleWave2 = 0;
        tripleWave3 = 0;
        tripleWave4 = 0;
        tripleWave5 = 0;
        P1Life = 0;
        P2Life = 0;
        xP1 = 0;
        yP1 = 209;
        xP2 = 290;
        yP2 = 209;
        state = 0;
        turn = 0;
        mapeo_SD("menu.txt", 32, 25, 256);
      }
      if(((xP1+29)>xEne15Bullet && (xP1+1)<(xEne15Bullet+10) && yP1<(yEne15Bullet+10) && (yP1+30)>yEne15Bullet)
      || ((xP2+29)>xEne15Bullet && (xP2+1)<(xEne15Bullet+10) && yP2<(yEne15Bullet+10) && (yP2+30)>yEne15Bullet)){
        enemy14Life = 1;
        enemy15Life = 1;
        enemy16Life = 1;
        enemy17Life = 1;
        P1Life = 1;
        P2Life = 1;
        LCD_Sprite(xP1, yP1, 30, 30, dead, 2, 1, 0, 0);
        LCD_Sprite(xP2, yP2, 30, 30, dead, 2, 1, 0, 0);
        mapeo_SD("gameOver.txt", 67, 75, 186);
        delay(3000);
        LCD_Clear(0x00);
        enemy1Life = 0;
        enemy1y = 0;
        enemy2y = 0;
        enemy3y = 0;
        enemy4y = 0;
        enemy5y = 0;
        enemy6y = 0;
        enemy7y = 0;
        enemy8y = 0;
        enemy9y = 0;
        enemy10y = 0;
        enemy11y = 0;
        enemy12y = 0;
        enemy13y = 0;
        enemy14y = 0;
        enemy15y = 0;
        enemy16y = 0;
        enemy17y = 0;
        enemy18y = 0;
        enemy19y = 0;
        enemy20y = 0;
        enemy21y = 0;
        enemy22y = 0;
        enemy23y = 0;
        tripleWave1 = 0;
        tripleWave2 = 0;
        tripleWave3 = 0;
        tripleWave4 = 0;
        tripleWave5 = 0;
        P1Life = 0;
        P2Life = 0;
        xP1 = 0;
        yP1 = 209;
        xP2 = 290;
        yP2 = 209;
        state = 0;
        turn = 0;
        mapeo_SD("menu.txt", 32, 25, 256);
      }
    }
  //**********************************************************************************************************************************
  //Enemy #15 code
  //**********************************************************************************************************************************
  
  //**********************************************************************************************************************************
  //Enemy #16 code
  //**********************************************************************************************************************************
    if(enemy16Life == 0){
      if(enemy16y == 0){
        xEne16Bullet = enemy16x + 8;
        yEne16Bullet = enemy16y + 30;
      }
      if(enemy16y == 110){
        xEne16Bullet = enemy16x + 8;
        yEne16Bullet = enemy16y + 30; 
      }
      int animEne16 = (enemy16y/5)%2;
      LCD_Sprite(enemy16x, enemy16y, 28, 30, enemyTankForward, 2, animEne16, 0, 0);
      H_line(enemy16x, enemy16y-1, 28,0x00);
      LCD_Bitmap(xEne16Bullet, yEne16Bullet, 10, 10, bulletDown);
      H_line(xEne16Bullet, yEne16Bullet, 10, 0x00);
      if(enemy16Pace == 0){
        enemy16y++; 
        enemy16Pace = 1;
      }else if(enemy16Pace == 1){
        enemy16Pace = 2;  
        //yEne16Bullet++;
        if(yEne16Bullet >= 250){
          yEne16Bullet = 245;  
        }
      }else if(enemy16Pace == 2){
        enemy16y++;
        enemy16Pace = 3;
      }else if(enemy16Pace == 3){
        enemy16Pace = 4;  
        //yEne16Bullet++;
        if(yEne16Bullet >= 250){
          yEne16Bullet = 245;  
        }
      }else if(enemy16Pace == 4){
        enemy16y++;
        enemy16Pace =  5;
      }else if(enemy16Pace == 5){
        enemy16Pace =  0;
        //yEne16Bullet++;
        if(yEne16Bullet >= 250){
          yEne16Bullet = 245;  
        }
      }
      yEne16Bullet++;
      if(yEne16Bullet >=250){
        yEne16Bullet = 245;  
      }
   
      if((enemy16y == 209)||((xP1+29)>enemy16x && (xP1+1)<(enemy16x+28) && yP1<(enemy16y+30) && (yP1+30)>enemy16y)
      || ((xP2+29)>enemy16x && (xP2+1)<(enemy16x+28) && yP2<(enemy16y+30) && (yP2+30)>enemy16y)){
        enemy14Life = 1;
        enemy15Life = 1;
        enemy16Life = 1;
        enemy17Life = 1;
        P1Life = 1;
        P2Life = 1;
        LCD_Sprite(xP1, yP1, 30, 30, dead, 2, 1, 0, 0);
        LCD_Sprite(xP2, yP2, 30, 30, dead, 2, 1, 0, 0);
        mapeo_SD("gameOver.txt", 67, 75, 186);
        delay(3000);
        LCD_Clear(0x00);
        enemy1Life = 0;
        enemy1y = 0;
        enemy2y = 0;
        enemy3y = 0;
        enemy4y = 0;
        enemy5y = 0;
        enemy6y = 0;
        enemy7y = 0;
        enemy8y = 0;
        enemy9y = 0;
        enemy10y = 0;
        enemy11y = 0;
        enemy12y = 0;
        enemy13y = 0;
        enemy14y = 0;
        enemy15y = 0;
        enemy16y = 0;
        enemy17y = 0;
        enemy18y = 0;
        enemy19y = 0;
        enemy20y = 0;
        enemy21y = 0;
        enemy22y = 0;
        enemy23y = 0;
        tripleWave1 = 0;
        tripleWave2 = 0;
        tripleWave3 = 0;
        tripleWave4 = 0;
        tripleWave5 = 0;
        P1Life = 0;
        P2Life = 0;
        xP1 = 0;
        yP1 = 209;
        xP2 = 290;
        yP2 = 209;
        state = 0;
        turn = 0;
        mapeo_SD("menu.txt", 32, 25, 256);
      }
      if(((xP1+29)>xEne16Bullet && (xP1+1)<(xEne16Bullet+10) && yP1<(yEne16Bullet+10) && (yP1+30)>yEne16Bullet)
      || ((xP2+29)>xEne16Bullet && (xP2+1)<(xEne16Bullet+10) && yP2<(yEne16Bullet+10) && (yP2+30)>yEne16Bullet)){
        enemy14Life = 1;
        enemy15Life = 1;
        enemy16Life = 1;
        enemy17Life = 1;
        P1Life = 1;
        P2Life = 1;
        LCD_Sprite(xP1, yP1, 30, 30, dead, 2, 1, 0, 0);
        LCD_Sprite(xP2, yP2, 30, 30, dead, 2, 1, 0, 0);
        mapeo_SD("gameOver.txt", 67, 75, 186);
        delay(3000);
        LCD_Clear(0x00);
        enemy1Life = 0;
        enemy1y = 0;
        enemy2y = 0;
        enemy3y = 0;
        enemy4y = 0;
        enemy5y = 0;
        enemy6y = 0;
        enemy7y = 0;
        enemy8y = 0;
        enemy9y = 0;
        enemy10y = 0;
        enemy11y = 0;
        enemy12y = 0;
        enemy13y = 0;
        enemy14y = 0;
        enemy15y = 0;
        enemy16y = 0;
        enemy17y = 0;
        enemy18y = 0;
        enemy19y = 0;
        enemy20y = 0;
        enemy21y = 0;
        enemy22y = 0;
        enemy23y = 0;
        tripleWave1 = 0;
        tripleWave2 = 0;
        tripleWave3 = 0;
        tripleWave4 = 0;
        tripleWave5 = 0;
        P1Life = 0;
        P2Life = 0;
        xP1 = 0;
        yP1 = 209;
        xP2 = 290;
        yP2 = 209;
        state = 0;
        turn = 0;
        mapeo_SD("menu.txt", 32, 25, 256);
      }
    }
  //**********************************************************************************************************************************
  //Enemy #16 code
  //**********************************************************************************************************************************
  
  //**********************************************************************************************************************************
  //Enemy #17 code
  //**********************************************************************************************************************************
    if(enemy17Life == 0){
      if(enemy17y == 0){
        xEne17Bullet = enemy17x + 8;
        yEne17Bullet = enemy17y + 30;
      }
      if(enemy17y == 110){
        xEne17Bullet = enemy17x + 8;
        yEne17Bullet = enemy17y + 30; 
      }
      int animEne17 = (enemy17y/5)%2;
      LCD_Sprite(enemy17x, enemy17y, 28, 30, enemyTankForward, 2, animEne17, 0, 0);
      H_line(enemy17x, enemy17y-1, 28,0x00);
      LCD_Bitmap(xEne17Bullet, yEne17Bullet, 10, 10, bulletDown);
      H_line(xEne17Bullet, yEne17Bullet, 10, 0x00);
      if(enemy17Pace == 0){
        enemy17y++; 
        enemy17Pace = 1;
      }else if(enemy17Pace == 1){
        enemy17Pace = 2;  
        //yEne17Bullet++;
        if(yEne17Bullet >= 250){
          yEne17Bullet = 245;  
        }
      }else if(enemy17Pace == 2){
        enemy17y++;
        enemy17Pace = 3;
      }else if(enemy17Pace == 3){
        enemy17Pace = 4;  
        //yEne17Bullet++;
        if(yEne17Bullet >= 250){
          yEne17Bullet = 245;  
        }
      }else if(enemy17Pace == 4){
        enemy17y++;
        enemy17Pace =  5;
      }else if(enemy17Pace == 5){
        enemy17Pace =  0;
        //yEne17Bullet++;
        if(yEne17Bullet >= 250){
          yEne17Bullet = 245;  
        }
      }
      yEne17Bullet++;
      if(yEne17Bullet >=250){
        yEne17Bullet = 245;  
      }
   
      if((enemy17y == 209)||((xP1+29)>enemy17x && (xP1+1)<(enemy17x+28) && yP1<(enemy17y+30) && (yP1+30)>enemy17y)
      || ((xP2+29)>enemy17x && (xP2+1)<(enemy17x+28) && yP2<(enemy17y+30) && (yP2+30)>enemy17y)){
        enemy14Life = 1;
        enemy15Life = 1;
        enemy16Life = 1;
        enemy17Life = 1;
        P1Life = 1;
        P2Life = 1;
        LCD_Sprite(xP1, yP1, 30, 30, dead, 2, 1, 0, 0);
        LCD_Sprite(xP2, yP2, 30, 30, dead, 2, 1, 0, 0);
        mapeo_SD("gameOver.txt", 67, 75, 186);
        delay(3000);
        LCD_Clear(0x00);
        enemy1Life = 0;
        enemy1y = 0;
        enemy2y = 0;
        enemy3y = 0;
        enemy4y = 0;
        enemy5y = 0;
        enemy6y = 0;
        enemy7y = 0;
        enemy8y = 0;
        enemy9y = 0;
        enemy10y = 0;
        enemy11y = 0;
        enemy12y = 0;
        enemy13y = 0;
        enemy14y = 0;
        enemy15y = 0;
        enemy16y = 0;
        enemy17y = 0;
        enemy18y = 0;
        enemy19y = 0;
        enemy20y = 0;
        enemy21y = 0;
        enemy22y = 0;
        enemy23y = 0;
        tripleWave1 = 0;
        tripleWave2 = 0;
        tripleWave3 = 0;
        tripleWave4 = 0;
        tripleWave5 = 0;
        P1Life = 0;
        P2Life = 0;
        xP1 = 0;
        yP1 = 209;
        xP2 = 290;
        yP2 = 209;
        state = 0;
        turn = 0;
        mapeo_SD("menu.txt", 32, 25, 256);
      }
      if(((xP1+29)>xEne17Bullet && (xP1+1)<(xEne17Bullet+10) && yP1<(yEne17Bullet+10) && (yP1+30)>yEne17Bullet)
      || ((xP2+29)>xEne17Bullet && (xP2+1)<(xEne17Bullet+10) && yP2<(yEne17Bullet+10) && (yP2+30)>yEne17Bullet)){
        enemy14Life = 1;
        enemy15Life = 1;
        enemy16Life = 1;
        enemy17Life = 1;
        P1Life = 1;
        P2Life = 1;
        LCD_Sprite(xP1, yP1, 30, 30, dead, 2, 1, 0, 0);
        LCD_Sprite(xP2, yP2, 30, 30, dead, 2, 1, 0, 0);
        mapeo_SD("gameOver.txt", 67, 75, 186);
        delay(3000);
        LCD_Clear(0x00);
        enemy1Life = 0;
        enemy1y = 0;
        enemy2y = 0;
        enemy3y = 0;
        enemy4y = 0;
        enemy5y = 0;
        enemy6y = 0;
        enemy7y = 0;
        enemy8y = 0;
        enemy9y = 0;
        enemy10y = 0;
        enemy11y = 0;
        enemy12y = 0;
        enemy13y = 0;
        enemy14y = 0;
        enemy15y = 0;
        enemy16y = 0;
        enemy17y = 0;
        enemy18y = 0;
        enemy19y = 0;
        enemy20y = 0;
        enemy21y = 0;
        enemy22y = 0;
        enemy23y = 0;
        tripleWave1 = 0;
        tripleWave2 = 0;
        tripleWave3 = 0;
        tripleWave4 = 0;
        tripleWave5 = 0;
        P1Life = 0;
        P2Life = 0;
        xP1 = 0;
        yP1 = 209;
        xP2 = 290;
        yP2 = 209;
        state = 0;
        turn = 0;
        mapeo_SD("menu.txt", 32, 25, 256);
      }
    }
  //**********************************************************************************************************************************
  //Enemy #17 code
  //**********************************************************************************************************************************
  
  //**********************************************************************************************************************************
  //Enemy #18 code
  //**********************************************************************************************************************************
    if(enemy18Life == 0){
      if(enemy18y == 0){
        xEne18Bullet = enemy18x + 8;
        yEne18Bullet = enemy18y + 30;
      }
      if(enemy18y == 110){
        xEne18Bullet = enemy18x + 8;
        yEne18Bullet = enemy18y + 30; 
      }
      int animEne18 = (enemy18y/5)%2;
      LCD_Sprite(enemy18x, enemy18y, 28, 30, enemyTankForward, 2, animEne18, 0, 0);
      H_line(enemy18x, enemy18y-1, 28,0x00);
      LCD_Bitmap(xEne18Bullet, yEne18Bullet, 10, 10, bulletDown);
      H_line(xEne18Bullet, yEne18Bullet, 10, 0x00);
      if(enemy18Pace == 0){
        enemy18y++; 
        enemy18Pace = 1;
      }else if(enemy18Pace == 1){
        enemy18Pace = 2;  
        //yEne18Bullet++;
        if(yEne18Bullet >= 250){
          yEne18Bullet = 245;  
        }
      }else if(enemy18Pace == 2){
        enemy18y++;
        enemy18Pace = 3;
      }else if(enemy18Pace == 3){
        enemy18Pace = 4;  
        //yEne18Bullet++;
        if(yEne18Bullet >= 250){
          yEne18Bullet = 245;  
        }
      }else if(enemy18Pace == 4){
        enemy18y++;
        enemy18Pace =  5;
      }else if(enemy18Pace == 5){
        enemy18Pace =  0;
        //yEne18Bullet++;
        if(yEne18Bullet >= 250){
          yEne18Bullet = 245;  
        }
      }
      yEne18Bullet++;
      if(yEne18Bullet >=250){
        yEne18Bullet = 245;  
      }
   
      if((enemy18y == 209)||((xP1+29)>enemy18x && (xP1+1)<(enemy18x+28) && yP1<(enemy18y+30) && (yP1+30)>enemy18y)
      || ((xP2+29)>enemy18x && (xP2+1)<(enemy18x+28) && yP2<(enemy18y+30) && (yP2+30)>enemy18y)){
        enemy18Life = 1;
        enemy19Life = 1;
        enemy20Life = 1;
        enemy21Life = 1;
        enemy22Life = 1;
        enemy23Life = 1;
        P1Life = 1;
        P2Life = 1;
        LCD_Sprite(xP1, yP1, 30, 30, dead, 2, 1, 0, 0);
        LCD_Sprite(xP2, yP2, 30, 30, dead, 2, 1, 0, 0);
        mapeo_SD("gameOver.txt", 67, 75, 186);
        delay(3000);
        LCD_Clear(0x00);
        enemy1Life = 0;
        enemy1y = 0;
        enemy2y = 0;
        enemy3y = 0;
        enemy4y = 0;
        enemy5y = 0;
        enemy6y = 0;
        enemy7y = 0;
        enemy8y = 0;
        enemy9y = 0;
        enemy10y = 0;
        enemy11y = 0;
        enemy12y = 0;
        enemy13y = 0;
        enemy14y = 0;
        enemy15y = 0;
        enemy16y = 0;
        enemy17y = 0;
        enemy18y = 0;
        enemy19y = 0;
        enemy20y = 0;
        enemy21y = 0;
        enemy22y = 0;
        enemy23y = 0;
        tripleWave1 = 0;
        tripleWave2 = 0;
        tripleWave3 = 0;
        tripleWave4 = 0;
        tripleWave5 = 0;
        P1Life = 0;
        P2Life = 0;
        xP1 = 0;
        yP1 = 209;
        xP2 = 290;
        yP2 = 209;
        state = 0;
        turn = 0;
        mapeo_SD("menu.txt", 32, 25, 256);
      }
      if(((xP1+29)>xEne18Bullet && (xP1+1)<(xEne18Bullet+10) && yP1<(yEne18Bullet+10) && (yP1+30)>yEne18Bullet)
      || ((xP2+29)>xEne18Bullet && (xP2+1)<(xEne18Bullet+10) && yP2<(yEne18Bullet+10) && (yP2+30)>yEne18Bullet)){
        enemy18Life = 1;
        enemy19Life = 1;
        enemy20Life = 1;
        enemy21Life = 1;
        enemy22Life = 1;
        enemy23Life = 1;
        P1Life = 1;
        P2Life = 1;
        LCD_Sprite(xP1, yP1, 30, 30, dead, 2, 1, 0, 0);
        LCD_Sprite(xP2, yP2, 30, 30, dead, 2, 1, 0, 0);
        mapeo_SD("gameOver.txt", 67, 75, 186);
        delay(3000);
        LCD_Clear(0x00);
        enemy1Life = 0;
        enemy1y = 0;
        enemy2y = 0;
        enemy3y = 0;
        enemy4y = 0;
        enemy5y = 0;
        enemy6y = 0;
        enemy7y = 0;
        enemy8y = 0;
        enemy9y = 0;
        enemy10y = 0;
        enemy11y = 0;
        enemy12y = 0;
        enemy13y = 0;
        enemy14y = 0;
        enemy15y = 0;
        enemy16y = 0;
        enemy17y = 0;
        enemy18y = 0;
        enemy19y = 0;
        enemy20y = 0;
        enemy21y = 0;
        enemy22y = 0;
        enemy23y = 0;
        tripleWave1 = 0;
        tripleWave2 = 0;
        tripleWave3 = 0;
        tripleWave4 = 0;
        tripleWave5 = 0;
        P1Life = 0;
        P2Life = 0;
        xP1 = 0;
        yP1 = 209;
        xP2 = 290;
        yP2 = 209;
        state = 0;
        turn = 0;
        mapeo_SD("menu.txt", 32, 25, 256);
      }
    }
  //**********************************************************************************************************************************
  //Enemy #18 code
  //**********************************************************************************************************************************
  
  //**********************************************************************************************************************************
  //Enemy #19 code
  //**********************************************************************************************************************************
    if(enemy19Life == 0){
      if(enemy19y == 0){
        xEne19Bullet = enemy19x + 8;
        yEne19Bullet = enemy19y + 30;
      }
      if(enemy19y == 110){
        xEne19Bullet = enemy19x + 8;
        yEne19Bullet = enemy19y + 30; 
      }
      int animEne19 = (enemy19y/5)%2;
      LCD_Sprite(enemy19x, enemy19y, 28, 30, enemyTankForward, 2, animEne19, 0, 0);
      H_line(enemy19x, enemy19y-1, 28,0x00);
      LCD_Bitmap(xEne19Bullet, yEne19Bullet, 10, 10, bulletDown);
      H_line(xEne19Bullet, yEne19Bullet, 10, 0x00);
      if(enemy19Pace == 0){
        enemy19y++; 
        enemy19Pace = 1;
      }else if(enemy19Pace == 1){
        enemy19Pace = 2;  
        //yEne19Bullet++;
        if(yEne19Bullet >= 250){
          yEne19Bullet = 245;  
        }
      }else if(enemy19Pace == 2){
        enemy19y++;
        enemy19Pace = 3;
      }else if(enemy19Pace == 3){
        enemy19Pace = 4;  
        //yEne19Bullet++;
        if(yEne19Bullet >= 250){
          yEne19Bullet = 245;  
        }
      }else if(enemy19Pace == 4){
        enemy19y++;
        enemy19Pace =  5;
      }else if(enemy19Pace == 5){
        enemy19Pace =  0;
        //yEne19Bullet++;
        if(yEne19Bullet >= 250){
          yEne19Bullet = 245;  
        }
      }
      yEne19Bullet++;
      if(yEne19Bullet >=250){
        yEne19Bullet = 245;  
      }
   
      if((enemy19y == 209)||((xP1+29)>enemy19x && (xP1+1)<(enemy19x+28) && yP1<(enemy19y+30) && (yP1+30)>enemy19y)
      || ((xP2+29)>enemy19x && (xP2+1)<(enemy19x+28) && yP2<(enemy19y+30) && (yP2+30)>enemy19y)){
        enemy18Life = 1;
        enemy19Life = 1;
        enemy20Life = 1;
        enemy21Life = 1;
        enemy22Life = 1;
        enemy23Life = 1;
        P1Life = 1;
        P2Life = 1;
        LCD_Sprite(xP1, yP1, 30, 30, dead, 2, 1, 0, 0);
        LCD_Sprite(xP2, yP2, 30, 30, dead, 2, 1, 0, 0);
        mapeo_SD("gameOver.txt", 67, 75, 186);
        delay(3000);
        LCD_Clear(0x00);
        enemy1Life = 0;
        enemy1y = 0;
        enemy2y = 0;
        enemy3y = 0;
        enemy4y = 0;
        enemy5y = 0;
        enemy6y = 0;
        enemy7y = 0;
        enemy8y = 0;
        enemy9y = 0;
        enemy10y = 0;
        enemy11y = 0;
        enemy12y = 0;
        enemy13y = 0;
        enemy14y = 0;
        enemy15y = 0;
        enemy16y = 0;
        enemy17y = 0;
        enemy18y = 0;
        enemy19y = 0;
        enemy20y = 0;
        enemy21y = 0;
        enemy22y = 0;
        enemy23y = 0;
        tripleWave1 = 0;
        tripleWave2 = 0;
        tripleWave3 = 0;
        tripleWave4 = 0;
        tripleWave5 = 0;
        P1Life = 0;
        P2Life = 0;
        xP1 = 0;
        yP1 = 209;
        xP2 = 290;
        yP2 = 209;
        state = 0;
        turn = 0;
        mapeo_SD("menu.txt", 32, 25, 256);
      }
      if(((xP1+29)>xEne19Bullet && (xP1+1)<(xEne19Bullet+10) && yP1<(yEne19Bullet+10) && (yP1+30)>yEne19Bullet)
      || ((xP2+29)>xEne19Bullet && (xP2+1)<(xEne19Bullet+10) && yP2<(yEne19Bullet+10) && (yP2+30)>yEne19Bullet)){
        enemy18Life = 1;
        enemy19Life = 1;
        enemy20Life = 1;
        enemy21Life = 1;
        enemy22Life = 1;
        enemy23Life = 1;
        P1Life = 1;
        P2Life = 1;
        LCD_Sprite(xP1, yP1, 30, 30, dead, 2, 1, 0, 0);
        LCD_Sprite(xP2, yP2, 30, 30, dead, 2, 1, 0, 0);
        mapeo_SD("gameOver.txt", 67, 75, 186);
        delay(3000);
        LCD_Clear(0x00);
        enemy1Life = 0;
        enemy1y = 0;
        enemy2y = 0;
        enemy3y = 0;
        enemy4y = 0;
        enemy5y = 0;
        enemy6y = 0;
        enemy7y = 0;
        enemy8y = 0;
        enemy9y = 0;
        enemy10y = 0;
        enemy11y = 0;
        enemy12y = 0;
        enemy13y = 0;
        enemy14y = 0;
        enemy15y = 0;
        enemy16y = 0;
        enemy17y = 0;
        enemy18y = 0;
        enemy19y = 0;
        enemy20y = 0;
        enemy21y = 0;
        enemy22y = 0;
        enemy23y = 0;
        tripleWave1 = 0;
        tripleWave2 = 0;
        tripleWave3 = 0;
        tripleWave4 = 0;
        tripleWave5 = 0;
        P1Life = 0;
        P2Life = 0;
        xP1 = 0;
        yP1 = 209;
        xP2 = 290;
        yP2 = 209;
        state = 0;
        turn = 0;
        mapeo_SD("menu.txt", 32, 25, 256);
      }
    }
  //**********************************************************************************************************************************
  //Enemy #19 code
  //**********************************************************************************************************************************
  
  //**********************************************************************************************************************************
  //Enemy #20 code
  //**********************************************************************************************************************************
    if(enemy20Life == 0){
      if(enemy20y == 0){
        xEne20Bullet = enemy20x + 8;
        yEne20Bullet = enemy20y + 30;
      }
      if(enemy20y == 110){
        xEne20Bullet = enemy20x + 8;
        yEne20Bullet = enemy20y + 30; 
      }
      int animEne20 = (enemy20y/5)%2;
      LCD_Sprite(enemy20x, enemy20y, 28, 30, enemyTankForward, 2, animEne20, 0, 0);
      H_line(enemy20x, enemy20y-1, 28,0x00);
      LCD_Bitmap(xEne20Bullet, yEne20Bullet, 10, 10, bulletDown);
      H_line(xEne20Bullet, yEne20Bullet, 10, 0x00);
      if(enemy20Pace == 0){
        enemy20y++; 
        enemy20Pace = 1;
      }else if(enemy20Pace == 1){
        enemy20Pace = 2;  
        //yEne20Bullet++;
        if(yEne20Bullet >= 250){
          yEne20Bullet = 245;  
        }
      }else if(enemy20Pace == 2){
        enemy20y++;
        enemy20Pace = 3;
      }else if(enemy20Pace == 3){
        enemy20Pace = 4;  
        //yEne20Bullet++;
        if(yEne20Bullet >= 250){
          yEne20Bullet = 245;  
        }
      }else if(enemy20Pace == 4){
        enemy20y++;
        enemy20Pace =  5;
      }else if(enemy20Pace == 5){
        enemy20Pace =  0;
        //yEne20Bullet++;
        if(yEne20Bullet >= 250){
          yEne20Bullet = 245;  
        }
      }
      yEne20Bullet++;
      if(yEne20Bullet >=250){
        yEne20Bullet = 245;  
      }
   
      if((enemy20y == 209)||((xP1+29)>enemy20x && (xP1+1)<(enemy20x+28) && yP1<(enemy20y+30) && (yP1+30)>enemy20y)
      || ((xP2+29)>enemy20x && (xP2+1)<(enemy20x+28) && yP2<(enemy20y+30) && (yP2+30)>enemy20y)){
        enemy18Life = 1;
        enemy19Life = 1;
        enemy20Life = 1;
        enemy21Life = 1;
        enemy22Life = 1;
        enemy23Life = 1;
        P1Life = 1;
        P2Life = 1;
        LCD_Sprite(xP1, yP1, 30, 30, dead, 2, 1, 0, 0);
        LCD_Sprite(xP2, yP2, 30, 30, dead, 2, 1, 0, 0);
        mapeo_SD("gameOver.txt", 67, 75, 186);
        delay(3000);
        LCD_Clear(0x00);
        enemy1Life = 0;
        enemy1y = 0;
        enemy2y = 0;
        enemy3y = 0;
        enemy4y = 0;
        enemy5y = 0;
        enemy6y = 0;
        enemy7y = 0;
        enemy8y = 0;
        enemy9y = 0;
        enemy10y = 0;
        enemy11y = 0;
        enemy12y = 0;
        enemy13y = 0;
        enemy14y = 0;
        enemy15y = 0;
        enemy16y = 0;
        enemy17y = 0;
        enemy18y = 0;
        enemy19y = 0;
        enemy20y = 0;
        enemy21y = 0;
        enemy22y = 0;
        enemy23y = 0;
        tripleWave1 = 0;
        tripleWave2 = 0;
        tripleWave3 = 0;
        tripleWave4 = 0;
        tripleWave5 = 0;
        P1Life = 0;
        P2Life = 0;
        xP1 = 0;
        yP1 = 209;
        xP2 = 290;
        yP2 = 209;
        state = 0;
        turn = 0;
        mapeo_SD("menu.txt", 32, 25, 256);
      }
      if(((xP1+29)>xEne20Bullet && (xP1+1)<(xEne20Bullet+10) && yP1<(yEne20Bullet+10) && (yP1+30)>yEne20Bullet)
      || ((xP2+29)>xEne20Bullet && (xP2+1)<(xEne20Bullet+10) && yP2<(yEne20Bullet+10) && (yP2+30)>yEne20Bullet)){
        enemy18Life = 1;
        enemy19Life = 1;
        enemy20Life = 1;
        enemy21Life = 1;
        enemy22Life = 1;
        enemy23Life = 1;
        P1Life = 1;
        P2Life = 1;
        LCD_Sprite(xP1, yP1, 30, 30, dead, 2, 1, 0, 0);
        LCD_Sprite(xP2, yP2, 30, 30, dead, 2, 1, 0, 0);
        mapeo_SD("gameOver.txt", 67, 75, 186);
        delay(3000);
        LCD_Clear(0x00);
        enemy1Life = 0;
        enemy1y = 0;
        enemy2y = 0;
        enemy3y = 0;
        enemy4y = 0;
        enemy5y = 0;
        enemy6y = 0;
        enemy7y = 0;
        enemy8y = 0;
        enemy9y = 0;
        enemy10y = 0;
        enemy11y = 0;
        enemy12y = 0;
        enemy13y = 0;
        enemy14y = 0;
        enemy15y = 0;
        enemy16y = 0;
        enemy17y = 0;
        enemy18y = 0;
        enemy19y = 0;
        enemy20y = 0;
        enemy21y = 0;
        enemy22y = 0;
        enemy23y = 0;
        tripleWave1 = 0;
        tripleWave2 = 0;
        tripleWave3 = 0;
        tripleWave4 = 0;
        tripleWave5 = 0;
        P1Life = 0;
        P2Life = 0;
        xP1 = 0;
        yP1 = 209;
        xP2 = 290;
        yP2 = 209;
        state = 0;
        turn = 0;
        mapeo_SD("menu.txt", 32, 25, 256);
      }
    }
  //**********************************************************************************************************************************
  //Enemy #20 code
  //**********************************************************************************************************************************
  
  //**********************************************************************************************************************************
  //Enemy #21 code
  //**********************************************************************************************************************************
    if(enemy21Life == 0){
      if(enemy21y == 0){
        xEne21Bullet = enemy21x + 8;
        yEne21Bullet = enemy21y + 30;
      }
      if(enemy21y == 110){
        xEne21Bullet = enemy21x + 8;
        yEne21Bullet = enemy21y + 30; 
      }
      int animEne21 = (enemy21y/5)%2;
      LCD_Sprite(enemy21x, enemy21y, 28, 30, enemyTankForward, 2, animEne21, 0, 0);
      H_line(enemy21x, enemy21y-1, 28,0x00);
      LCD_Bitmap(xEne21Bullet, yEne21Bullet, 10, 10, bulletDown);
      H_line(xEne21Bullet, yEne21Bullet, 10, 0x00);
      if(enemy21Pace == 0){
        enemy21y++; 
        enemy21Pace = 1;
      }else if(enemy21Pace == 1){
        enemy21Pace = 2;  
        //yEne21Bullet++;
        if(yEne21Bullet >= 250){
          yEne21Bullet = 245;  
        }
      }else if(enemy21Pace == 2){
        enemy21y++;
        enemy21Pace = 3;
      }else if(enemy21Pace == 3){
        enemy21Pace = 4;  
        //yEne21Bullet++;
        if(yEne21Bullet >= 250){
          yEne21Bullet = 245;  
        }
      }else if(enemy21Pace == 4){
        enemy21y++;
        enemy21Pace =  5;
      }else if(enemy21Pace == 5){
        enemy21Pace =  0;
        //yEne21Bullet++;
        if(yEne21Bullet >= 250){
          yEne21Bullet = 245;  
        }
      }
      yEne21Bullet++;
      if(yEne21Bullet >=250){
        yEne21Bullet = 245;  
      }
   
      if((enemy21y == 209)||((xP1+29)>enemy21x && (xP1+1)<(enemy21x+28) && yP1<(enemy21y+30) && (yP1+30)>enemy21y)
      || ((xP2+29)>enemy21x && (xP2+1)<(enemy21x+28) && yP2<(enemy21y+30) && (yP2+30)>enemy21y)){
        enemy18Life = 1;
        enemy19Life = 1;
        enemy20Life = 1;
        enemy21Life = 1;
        enemy22Life = 1;
        enemy23Life = 1;
        P1Life = 1;
        P2Life = 1;
        LCD_Sprite(xP1, yP1, 30, 30, dead, 2, 1, 0, 0);
        LCD_Sprite(xP2, yP2, 30, 30, dead, 2, 1, 0, 0);
        mapeo_SD("gameOver.txt", 67, 75, 186);
        delay(3000);
        LCD_Clear(0x00);
        enemy1Life = 0;
        enemy1y = 0;
        enemy2y = 0;
        enemy3y = 0;
        enemy4y = 0;
        enemy5y = 0;
        enemy6y = 0;
        enemy7y = 0;
        enemy8y = 0;
        enemy9y = 0;
        enemy10y = 0;
        enemy11y = 0;
        enemy12y = 0;
        enemy13y = 0;
        enemy14y = 0;
        enemy15y = 0;
        enemy16y = 0;
        enemy17y = 0;
        enemy18y = 0;
        enemy19y = 0;
        enemy20y = 0;
        enemy21y = 0;
        enemy22y = 0;
        enemy23y = 0;
        tripleWave1 = 0;
        tripleWave2 = 0;
        tripleWave3 = 0;
        tripleWave4 = 0;
        tripleWave5 = 0;
        P1Life = 0;
        P2Life = 0;
        xP1 = 0;
        yP1 = 209;
        xP2 = 290;
        yP2 = 209;
        state = 0;
        turn = 0;
        mapeo_SD("menu.txt", 32, 25, 256);
      }
      if(((xP1+29)>xEne21Bullet && (xP1+1)<(xEne21Bullet+10) && yP1<(yEne21Bullet+10) && (yP1+30)>yEne21Bullet)
      || ((xP2+29)>xEne21Bullet && (xP2+1)<(xEne21Bullet+10) && yP2<(yEne21Bullet+10) && (yP2+30)>yEne21Bullet)){
        enemy18Life = 1;
        enemy19Life = 1;
        enemy20Life = 1;
        enemy21Life = 1;
        enemy22Life = 1;
        enemy23Life = 1;
        P1Life = 1;
        P2Life = 1;
        LCD_Sprite(xP1, yP1, 30, 30, dead, 2, 1, 0, 0);
        LCD_Sprite(xP2, yP2, 30, 30, dead, 2, 1, 0, 0);
        mapeo_SD("gameOver.txt", 67, 75, 186);
        delay(3000);
        LCD_Clear(0x00);
        enemy1Life = 0;
        enemy1y = 0;
        enemy2y = 0;
        enemy3y = 0;
        enemy4y = 0;
        enemy5y = 0;
        enemy6y = 0;
        enemy7y = 0;
        enemy8y = 0;
        enemy9y = 0;
        enemy10y = 0;
        enemy11y = 0;
        enemy12y = 0;
        enemy13y = 0;
        enemy14y = 0;
        enemy15y = 0;
        enemy16y = 0;
        enemy17y = 0;
        enemy18y = 0;
        enemy19y = 0;
        enemy20y = 0;
        enemy21y = 0;
        enemy22y = 0;
        enemy23y = 0;
        tripleWave1 = 0;
        tripleWave2 = 0;
        tripleWave3 = 0;
        tripleWave4 = 0;
        tripleWave5 = 0;
        P1Life = 0;
        P2Life = 0;
        xP1 = 0;
        yP1 = 209;
        xP2 = 290;
        yP2 = 209;
        state = 0;
        turn = 0;
        mapeo_SD("menu.txt", 32, 25, 256);
      }
    }
  //**********************************************************************************************************************************
  //Enemy #21 code
  //**********************************************************************************************************************************
  
  //**********************************************************************************************************************************
  //Enemy #22 code
  //**********************************************************************************************************************************
    if(enemy22Life == 0){
      if(enemy22y == 0){
        xEne22Bullet = enemy22x + 8;
        yEne22Bullet = enemy22y + 30;
      }
      if(enemy22y == 110){
        xEne22Bullet = enemy22x + 8;
        yEne22Bullet = enemy22y + 30; 
      }
      int animEne22 = (enemy22y/5)%2;
      LCD_Sprite(enemy22x, enemy22y, 28, 30, enemyTankForward, 2, animEne22, 0, 0);
      H_line(enemy22x, enemy22y-1, 28,0x00);
      LCD_Bitmap(xEne22Bullet, yEne22Bullet, 10, 10, bulletDown);
      H_line(xEne22Bullet, yEne22Bullet, 10, 0x00);
      if(enemy22Pace == 0){
        enemy22y++; 
        enemy22Pace = 1;
      }else if(enemy22Pace == 1){
        enemy22Pace = 2;  
        //yEne22Bullet++;
        if(yEne22Bullet >= 250){
          yEne22Bullet = 245;  
        }
      }else if(enemy22Pace == 2){
        enemy22y++;
        enemy22Pace = 3;
      }else if(enemy22Pace == 3){
        enemy22Pace = 4;  
        //yEne22Bullet++;
        if(yEne22Bullet >= 250){
          yEne22Bullet = 245;  
        }
      }else if(enemy22Pace == 4){
        enemy22y++;
        enemy22Pace =  5;
      }else if(enemy22Pace == 5){
        enemy22Pace =  0;
        //yEne22Bullet++;
        if(yEne22Bullet >= 250){
          yEne22Bullet = 245;  
        }
      }
      yEne22Bullet++;
      if(yEne22Bullet >=250){
        yEne22Bullet = 245;  
      }
   
      if((enemy22y == 209)||((xP1+29)>enemy22x && (xP1+1)<(enemy22x+28) && yP1<(enemy22y+30) && (yP1+30)>enemy22y)
      || ((xP2+29)>enemy22x && (xP2+1)<(enemy22x+28) && yP2<(enemy22y+30) && (yP2+30)>enemy22y)){
        enemy18Life = 1;
        enemy19Life = 1;
        enemy20Life = 1;
        enemy21Life = 1;
        enemy22Life = 1;
        enemy23Life = 1;
        P1Life = 1;
        P2Life = 1;
        LCD_Sprite(xP1, yP1, 30, 30, dead, 2, 1, 0, 0);
        LCD_Sprite(xP2, yP2, 30, 30, dead, 2, 1, 0, 0);
        mapeo_SD("gameOver.txt", 67, 75, 186);
        delay(3000);
        LCD_Clear(0x00);
        enemy1Life = 0;
        enemy1y = 0;
        enemy2y = 0;
        enemy3y = 0;
        enemy4y = 0;
        enemy5y = 0;
        enemy6y = 0;
        enemy7y = 0;
        enemy8y = 0;
        enemy9y = 0;
        enemy10y = 0;
        enemy11y = 0;
        enemy12y = 0;
        enemy13y = 0;
        enemy14y = 0;
        enemy15y = 0;
        enemy16y = 0;
        enemy17y = 0;
        enemy18y = 0;
        enemy19y = 0;
        enemy20y = 0;
        enemy21y = 0;
        enemy22y = 0;
        enemy23y = 0;
        tripleWave1 = 0;
        tripleWave2 = 0;
        tripleWave3 = 0;
        tripleWave4 = 0;
        tripleWave5 = 0;
        P1Life = 0;
        P2Life = 0;
        xP1 = 0;
        yP1 = 209;
        xP2 = 290;
        yP2 = 209;
        state = 0;
        turn = 0;
        mapeo_SD("menu.txt", 32, 25, 256);
      }
      if(((xP1+29)>xEne22Bullet && (xP1+1)<(xEne22Bullet+10) && yP1<(yEne22Bullet+10) && (yP1+30)>yEne22Bullet)
      || ((xP2+29)>xEne22Bullet && (xP2+1)<(xEne22Bullet+10) && yP2<(yEne22Bullet+10) && (yP2+30)>yEne22Bullet)){
        enemy18Life = 1;
        enemy19Life = 1;
        enemy20Life = 1;
        enemy21Life = 1;
        enemy22Life = 1;
        enemy23Life = 1;
        P1Life = 1;
        P2Life = 1;
        LCD_Sprite(xP1, yP1, 30, 30, dead, 2, 1, 0, 0);
        LCD_Sprite(xP2, yP2, 30, 30, dead, 2, 1, 0, 0);
        mapeo_SD("gameOver.txt", 67, 75, 186);
        delay(3000);
        LCD_Clear(0x00);
        enemy1Life = 0;
        enemy1y = 0;
        enemy2y = 0;
        enemy3y = 0;
        enemy4y = 0;
        enemy5y = 0;
        enemy6y = 0;
        enemy7y = 0;
        enemy8y = 0;
        enemy9y = 0;
        enemy10y = 0;
        enemy11y = 0;
        enemy12y = 0;
        enemy13y = 0;
        enemy14y = 0;
        enemy15y = 0;
        enemy16y = 0;
        enemy17y = 0;
        enemy18y = 0;
        enemy19y = 0;
        enemy20y = 0;
        enemy21y = 0;
        enemy22y = 0;
        enemy23y = 0;
        tripleWave1 = 0;
        tripleWave2 = 0;
        tripleWave3 = 0;
        tripleWave4 = 0;
        tripleWave5 = 0;
        P1Life = 0;
        P2Life = 0;
        xP1 = 0;
        yP1 = 209;
        xP2 = 290;
        yP2 = 209;
        state = 0;
        turn = 0;
        mapeo_SD("menu.txt", 32, 25, 256);
      }
    }
  //**********************************************************************************************************************************
  //Enemy #22 code
  //**********************************************************************************************************************************
  
  //**********************************************************************************************************************************
  //Enemy #23 code
  //**********************************************************************************************************************************
    if(enemy23Life == 0){
      if(enemy23y == 0){
        xEne23Bullet = enemy23x + 8;
        yEne23Bullet = enemy23y + 30;
      }
      if(enemy23y == 110){
        xEne23Bullet = enemy23x + 8;
        yEne23Bullet = enemy23y + 30; 
      }
      int animEne23 = (enemy23y/5)%2;
      LCD_Sprite(enemy23x, enemy23y, 28, 30, enemyTankForward, 2, animEne23, 0, 0);
      H_line(enemy23x, enemy23y-1, 28,0x00);
      LCD_Bitmap(xEne23Bullet, yEne23Bullet, 10, 10, bulletDown);
      H_line(xEne23Bullet, yEne23Bullet, 10, 0x00);
      if(enemy23Pace == 0){
        enemy23y++; 
        enemy23Pace = 1;
      }else if(enemy23Pace == 1){
        enemy23Pace = 2;  
        //yEne23Bullet++;
        if(yEne23Bullet >= 250){
          yEne23Bullet = 245;  
        }
      }else if(enemy23Pace == 2){
        enemy23y++;
        enemy23Pace = 3;
      }else if(enemy23Pace == 3){
        enemy23Pace = 4;  
        //yEne23Bullet++;
        if(yEne23Bullet >= 250){
          yEne23Bullet = 245;  
        }
      }else if(enemy23Pace == 4){
        enemy23y++;
        enemy23Pace =  5;
      }else if(enemy23Pace == 5){
        enemy23Pace =  0;
        //yEne23Bullet++;
        if(yEne23Bullet >= 250){
          yEne23Bullet = 245;  
        }
      }
      yEne23Bullet++;
      if(yEne23Bullet >=250){
        yEne23Bullet = 245;  
      }
   
      if((enemy23y == 209)||((xP1+29)>enemy23x && (xP1+1)<(enemy23x+28) && yP1<(enemy23y+30) && (yP1+30)>enemy23y)
      || ((xP2+29)>enemy23x && (xP2+1)<(enemy23x+28) && yP2<(enemy23y+30) && (yP2+30)>enemy23y)){
        enemy18Life = 1;
        enemy19Life = 1;
        enemy20Life = 1;
        enemy21Life = 1;
        enemy22Life = 1;
        enemy23Life = 1;
        P1Life = 1;
        P2Life = 1;
        LCD_Sprite(xP1, yP1, 30, 30, dead, 2, 1, 0, 0);
        LCD_Sprite(xP2, yP2, 30, 30, dead, 2, 1, 0, 0);
        mapeo_SD("gameOver.txt", 67, 75, 186);
        delay(3000);
        LCD_Clear(0x00);
        enemy1Life = 0;
        enemy1y = 0;
        enemy2y = 0;
        enemy3y = 0;
        enemy4y = 0;
        enemy5y = 0;
        enemy6y = 0;
        enemy7y = 0;
        enemy8y = 0;
        enemy9y = 0;
        enemy10y = 0;
        enemy11y = 0;
        enemy12y = 0;
        enemy13y = 0;
        enemy14y = 0;
        enemy15y = 0;
        enemy16y = 0;
        enemy17y = 0;
        enemy18y = 0;
        enemy19y = 0;
        enemy20y = 0;
        enemy21y = 0;
        enemy22y = 0;
        enemy23y = 0;
        tripleWave1 = 0;
        tripleWave2 = 0;
        tripleWave3 = 0;
        tripleWave4 = 0;
        tripleWave5 = 0;
        P1Life = 0;
        P2Life = 0;
        xP1 = 0;
        yP1 = 209;
        xP2 = 290;
        yP2 = 209;
        state = 0;
        turn = 0;
        mapeo_SD("menu.txt", 32, 25, 256);
      }
      if(((xP1+29)>xEne23Bullet && (xP1+1)<(xEne23Bullet+10) && yP1<(yEne23Bullet+10) && (yP1+30)>yEne23Bullet)
      || ((xP2+29)>xEne23Bullet && (xP2+1)<(xEne23Bullet+10) && yP2<(yEne23Bullet+10) && (yP2+30)>yEne23Bullet)){
        enemy18Life = 1;
        enemy19Life = 1;
        enemy20Life = 1;
        enemy21Life = 1;
        enemy22Life = 1;
        enemy23Life = 1;
        P1Life = 1;
        P2Life = 1;
        LCD_Sprite(xP1, yP1, 30, 30, dead, 2, 1, 0, 0);
        LCD_Sprite(xP2, yP2, 30, 30, dead, 2, 1, 0, 0);
        mapeo_SD("gameOver.txt", 67, 75, 186);
        delay(3000);
        LCD_Clear(0x00);
        enemy1Life = 0;
        enemy1y = 0;
        enemy2y = 0;
        enemy3y = 0;
        enemy4y = 0;
        enemy5y = 0;
        enemy6y = 0;
        enemy7y = 0;
        enemy8y = 0;
        enemy9y = 0;
        enemy10y = 0;
        enemy11y = 0;
        enemy12y = 0;
        enemy13y = 0;
        enemy14y = 0;
        enemy15y = 0;
        enemy16y = 0;
        enemy17y = 0;
        enemy18y = 0;
        enemy19y = 0;
        enemy20y = 0;
        enemy21y = 0;
        enemy22y = 0;
        enemy23y = 0;
        tripleWave1 = 0;
        tripleWave2 = 0;
        tripleWave3 = 0;
        tripleWave4 = 0;
        tripleWave5 = 0;
        P1Life = 0;
        P2Life = 0;
        xP1 = 0;
        yP1 = 209;
        xP2 = 290;
        yP2 = 209;
        state = 0;
        turn = 0;
        mapeo_SD("menu.txt", 32, 25, 256);
      }
    }
  //**********************************************************************************************************************************
  //Enemy #23 code
  //**********************************************************************************************************************************
  
  //**********************************************************************************************************************************
  //Player #1 bullet collision
  //**********************************************************************************************************************************  
    if(yP1bullet <= (enemy1y+28) && (xP1bullet+9)>enemy1x && (xP1bullet+1)<(enemy1x+28) && enemy1Life == 0){//Enemy #1
      enemy1Life = 1;
      enemy2Life = 0;
      LCD_Sprite(enemy1x, enemy1y, 30, 30, dead, 2, 1, 0, 0);
      LCD_Sprite(xEne1Bullet-10, yEne1Bullet-10, 30, 30, dead, 2, 0, 0, 0);
      delay(15);
      for(int i = 0; i<31; i++){
        H_line(enemy1x, enemy1y+i-1, 30, 0x00);
        H_line(xEne1Bullet-10, yEne1Bullet-11+i, 30, 0x00);
      }
      for(int i = 0; i <= 11; i++){
        V_line(xP1bullet+10-i, yP1bullet, 10, 0x00);  
      }
      triggerP1 = 0;
      xP1bullet = -10;
      yP1bullet = -10;
    }
  
    if(yP1bullet <= (enemy2y+28) && (xP1bullet+9)>enemy2x && (xP1bullet+1)<(enemy2x+28) && enemy2Life == 0){//Enemy #2
      enemy2Life = 1;
      enemy3Life = 0;
      LCD_Sprite(enemy2x, enemy2y, 30, 30, dead, 2, 1, 0, 0);
      LCD_Sprite(xEne2Bullet-10, yEne2Bullet-10, 30, 30, dead, 2, 0, 0, 0);
      delay(15);
      for(int i = 0; i<31; i++){
        H_line(enemy2x, enemy2y+i-1, 30, 0x00);
        H_line(xEne2Bullet-10, yEne2Bullet-11+i, 30, 0x00);
      }
      for(int i = 0; i <= 11; i++){
        V_line(xP1bullet+10-i, yP1bullet, 10, 0x00);  
      }
      triggerP1 = 0;
      xP1bullet = -10;
      yP1bullet = -10;
    }
  
    if(yP1bullet <= (enemy3y+28) && (xP1bullet+9)>enemy3x && (xP1bullet+1)<(enemy3x+28) && enemy3Life == 0){//Enemy #3
      enemy3Life = 1;
      enemy4Life = 0;
      enemy5Life = 0;
      enemy6Life = 0;
      LCD_Sprite(enemy3x, enemy3y, 30, 30, dead, 2, 1, 0, 0);
      LCD_Sprite(xEne3Bullet-10, yEne3Bullet-10, 30, 30, dead, 2, 0, 0, 0);
      delay(15);
      for(int i = 0; i<31; i++){
        H_line(enemy3x, enemy3y+i-1, 30, 0x00);
        H_line(xEne3Bullet-10, yEne3Bullet-11+i, 30, 0x00);
      }
      for(int i = 0; i <= 11; i++){
        V_line(xP1bullet+10-i, yP1bullet, 10, 0x00);  
      }
      triggerP1 = 0;
      xP1bullet = -10;
      yP1bullet = -10;
    }
  
    if(yP1bullet <= (enemy4y+28) && (xP1bullet+9)>enemy4x && (xP1bullet+1)<(enemy4x+28) && enemy4Life == 0){//Enemy #4
      enemy4Life = 1;
      tripleWave1++;
      if(tripleWave1 == 3){
        tripleWave1 = 0;
        enemy7Life = 0;
        enemy8Life = 0;
        enemy9Life = 0;
      }
      LCD_Sprite(enemy4x, enemy4y, 30, 30, dead, 2, 1, 0, 0);
      LCD_Sprite(xEne4Bullet-10, yEne4Bullet-10, 30, 30, dead, 2, 0, 0, 0);
      delay(15);
      for(int i = 0; i<31; i++){
        H_line(enemy4x, enemy4y+i-1, 30, 0x00);
        H_line(xEne4Bullet-10, yEne4Bullet-11+i, 30, 0x00);
      }
      for(int i = 0; i <= 11; i++){
        V_line(xP1bullet+10-i, yP1bullet, 10, 0x00);  
      }
      triggerP1 = 0;
      xP1bullet = -10;
      yP1bullet = -10;
    }
  
    if(yP1bullet <= (enemy5y+28) && (xP1bullet+9)>enemy5x && (xP1bullet+1)<(enemy5x+28) && enemy5Life == 0){//Enemy #5
      enemy5Life = 1;
      tripleWave1++;
      if(tripleWave1 == 3){
        tripleWave1 = 0;
        enemy7Life = 0;
        enemy8Life = 0;
        enemy9Life = 0;
      }
      LCD_Sprite(enemy5x, enemy5y, 30, 30, dead, 2, 1, 0, 0);
      LCD_Sprite(xEne5Bullet-10, yEne5Bullet-10, 30, 30, dead, 2, 0, 0, 0);
      delay(15);
      for(int i = 0; i<31; i++){
        H_line(enemy5x, enemy5y+i-1, 30, 0x00);
        H_line(xEne5Bullet-10, yEne5Bullet-11+i, 30, 0x00);
      }
      for(int i = 0; i <= 11; i++){
        V_line(xP1bullet+10-i, yP1bullet, 10, 0x00);  
      }
      triggerP1 = 0;
      xP1bullet = -10;
      yP1bullet = -10;
    }
  
    if(yP1bullet <= (enemy6y+28) && (xP1bullet+9)>enemy6x && (xP1bullet+1)<(enemy6x+28) && enemy6Life == 0){//Enemy #6
      enemy6Life = 1;
      tripleWave1++;
      if(tripleWave1 == 3){
        tripleWave1 = 0;
        enemy7Life = 0;
        enemy8Life = 0;
        enemy9Life = 0;
      }
      LCD_Sprite(enemy6x, enemy6y, 30, 30, dead, 2, 1, 0, 0);
      LCD_Sprite(xEne6Bullet-10, yEne6Bullet-10, 30, 30, dead, 2, 0, 0, 0);
      delay(15);
      for(int i = 0; i<31; i++){
        H_line(enemy6x, enemy6y+i-1, 30, 0x00);
        H_line(xEne6Bullet-10, yEne6Bullet-11+i, 30, 0x00);
      }
      for(int i = 0; i <= 11; i++){
        V_line(xP1bullet+10-i, yP1bullet, 10, 0x00);  
      }
      triggerP1 = 0;
      xP1bullet = -10;
      yP1bullet = -10;
    }
  
    if(yP1bullet <= (enemy7y+28) && (xP1bullet+9)>enemy7x && (xP1bullet+1)<(enemy7x+28) && enemy7Life == 0){//Enemy #7
      enemy7Life = 1;
      tripleWave2++;
      if(tripleWave2 == 3){
        tripleWave2 = 0;
        enemy10Life = 0;
        enemy11Life = 0;
        enemy12Life = 0;
        enemy13Life = 0;  
      }
      LCD_Sprite(enemy7x, enemy7y, 30, 30, dead, 2, 1, 0, 0);
      LCD_Sprite(xEne7Bullet-10, yEne7Bullet-10, 30, 30, dead, 2, 0, 0, 0);
      delay(15);
      for(int i = 0; i<31; i++){
        H_line(enemy7x, enemy7y+i-1, 30, 0x00);
        H_line(xEne7Bullet-10, yEne7Bullet-11+i, 30, 0x00);
      }
      for(int i = 0; i <= 11; i++){
        V_line(xP1bullet+10-i, yP1bullet, 10, 0x00);  
      }
      triggerP1 = 0;
      xP1bullet = -10;
      yP1bullet = -10;
    }
  
    if(yP1bullet <= (enemy8y+28) && (xP1bullet+9)>enemy8x && (xP1bullet+1)<(enemy8x+28) && enemy8Life == 0){//Enemy #8
      enemy8Life = 1;
      tripleWave2++;
      if(tripleWave2 == 3){
        tripleWave2 = 0;
        enemy10Life = 0;
        enemy11Life = 0;
        enemy12Life = 0;
        enemy13Life = 0;  
      }
      LCD_Sprite(enemy8x, enemy8y, 30, 30, dead, 2, 1, 0, 0);
      LCD_Sprite(xEne8Bullet-10, yEne8Bullet-10, 30, 30, dead, 2, 0, 0, 0);
      delay(15);
      for(int i = 0; i<31; i++){
        H_line(enemy8x, enemy8y+i-1, 30, 0x00);
        H_line(xEne8Bullet-10, yEne8Bullet-11+i, 30, 0x00);
      }
      for(int i = 0; i <= 11; i++){
        V_line(xP1bullet+10-i, yP1bullet, 10, 0x00);  
      }
      triggerP1 = 0;
      xP1bullet = -10;
      yP1bullet = -10;
    }
  
    if(yP1bullet <= (enemy9y+28) && (xP1bullet+9)>enemy9x && (xP1bullet+1)<(enemy9x+28) && enemy9Life == 0){//Enemy #9
      enemy9Life = 1;
      tripleWave2++;
      if(tripleWave2 == 3){
        tripleWave2 = 0;
        enemy10Life = 0;
        enemy11Life = 0;
        enemy12Life = 0;
        enemy13Life = 0;  
      }
      LCD_Sprite(enemy9x, enemy9y, 30, 30, dead, 2, 1, 0, 0);
      LCD_Sprite(xEne9Bullet-10, yEne9Bullet-10, 30, 30, dead, 2, 0, 0, 0);
      delay(15);
      for(int i = 0; i<31; i++){
        H_line(enemy9x, enemy9y+i-1, 30, 0x00);
        H_line(xEne9Bullet-10, yEne9Bullet-11+i, 30, 0x00);
      }
      for(int i = 0; i <= 11; i++){
        V_line(xP1bullet+10-i, yP1bullet, 10, 0x00);  
      }
      triggerP1 = 0;
      xP1bullet = -10;
      yP1bullet = -10;
    }
  
    if(yP1bullet <= (enemy10y+28) && (xP1bullet+9)>enemy10x && (xP1bullet+1)<(enemy10x+28) && enemy10Life == 0){//Enemy #10
      enemy10Life = 1;
      tripleWave3++;
      if(tripleWave3 == 4){
        tripleWave3 = 0;
        enemy14Life = 0;
        enemy15Life = 0;
        enemy16Life = 0;
        enemy17Life = 0;  
      }
      LCD_Sprite(enemy10x, enemy10y, 30, 30, dead, 2, 1, 0, 0);
      LCD_Sprite(xEne10Bullet-10, yEne10Bullet-10, 30, 30, dead, 2, 0, 0, 0);
      delay(15);
      for(int i = 0; i<31; i++){
        H_line(enemy10x, enemy10y+i-1, 30, 0x00);
        H_line(xEne10Bullet-10, yEne10Bullet-11+i, 30, 0x00);
      }
      for(int i = 0; i <= 11; i++){
        V_line(xP1bullet+10-i, yP1bullet, 10, 0x00);  
      }
      triggerP1 = 0;
      xP1bullet = -10;
      yP1bullet = -10;
    }
  
    if(yP1bullet <= (enemy11y+28) && (xP1bullet+9)>enemy11x && (xP1bullet+1)<(enemy11x+28) && enemy11Life == 0){//Enemy #11
      enemy11Life = 1;
      tripleWave3++;
      if(tripleWave3 == 4){
        tripleWave3 = 0;
        enemy14Life = 0;
        enemy15Life = 0;
        enemy16Life = 0;
        enemy17Life = 0;  
      }
      LCD_Sprite(enemy11x, enemy11y, 30, 30, dead, 2, 1, 0, 0);
      LCD_Sprite(xEne11Bullet-10, yEne11Bullet-10, 30, 30, dead, 2, 0, 0, 0);
      delay(15);
      for(int i = 0; i<31; i++){
        H_line(enemy11x, enemy11y+i-1, 30, 0x00);
        H_line(xEne11Bullet-10, yEne11Bullet-11+i, 30, 0x00);
      }
      for(int i = 0; i <= 11; i++){
        V_line(xP1bullet+10-i, yP1bullet, 10, 0x00);  
      }
      triggerP1 = 0;
      xP1bullet = -10;
      yP1bullet = -10;
    }
  
    if(yP1bullet <= (enemy12y+28) && (xP1bullet+9)>enemy12x && (xP1bullet+1)<(enemy12x+28) && enemy12Life == 0){//Enemy #12
      enemy12Life = 1;
      tripleWave3++;
      if(tripleWave3 == 4){
        tripleWave3 = 0;
        enemy14Life = 0;
        enemy15Life = 0;
        enemy16Life = 0;
        enemy17Life = 0;  
      }
      LCD_Sprite(enemy12x, enemy12y, 30, 30, dead, 2, 1, 0, 0);
      LCD_Sprite(xEne12Bullet-10, yEne12Bullet-10, 30, 30, dead, 2, 0, 0, 0);
      delay(15);
      for(int i = 0; i<31; i++){
        H_line(enemy12x, enemy12y+i-1, 30, 0x00);
        H_line(xEne12Bullet-10, yEne12Bullet-11+i, 30, 0x00);
      }
      for(int i = 0; i <= 11; i++){
        V_line(xP1bullet+10-i, yP1bullet, 10, 0x00);  
      }
      triggerP1 = 0;
      xP1bullet = -10;
      yP1bullet = -10;
    }
  
    if(yP1bullet <= (enemy13y+28) && (xP1bullet+9)>enemy13x && (xP1bullet+1)<(enemy13x+28) && enemy13Life == 0){//Enemy #13
      enemy13Life = 1;
      tripleWave3++;
      if(tripleWave3 == 4){
        tripleWave3 = 0;
        enemy14Life = 0;
        enemy15Life = 0;
        enemy16Life = 0;
        enemy17Life = 0;  
      }
      LCD_Sprite(enemy13x, enemy13y, 30, 30, dead, 2, 1, 0, 0);
      LCD_Sprite(xEne13Bullet-10, yEne13Bullet-10, 30, 30, dead, 2, 0, 0, 0);
      delay(15);
      for(int i = 0; i<31; i++){
        H_line(enemy13x, enemy13y+i-1, 30, 0x00);
        H_line(xEne13Bullet-10, yEne13Bullet-11+i, 30, 0x00);
      }
      for(int i = 0; i <= 11; i++){
        V_line(xP1bullet+10-i, yP1bullet, 10, 0x00);  
      }
      triggerP1 = 0;
      xP1bullet = -10;
      yP1bullet = -10;
    }
  
    if(yP1bullet <= (enemy14y+28) && (xP1bullet+9)>enemy14x && (xP1bullet+1)<(enemy14x+28) && enemy14Life == 0){//Enemy #14
      enemy14Life = 1;
      tripleWave4++;
      if(tripleWave4 == 4){
        tripleWave4 = 0;
        enemy18Life = 0;
        enemy19Life = 0;
        enemy20Life = 0;
        enemy21Life = 0;
        enemy22Life = 0;
        enemy23Life = 0;
      }
      LCD_Sprite(enemy14x, enemy14y, 30, 30, dead, 2, 1, 0, 0);
      LCD_Sprite(xEne14Bullet-10, yEne14Bullet-10, 30, 30, dead, 2, 0, 0, 0);
      delay(15);
      for(int i = 0; i<31; i++){
        H_line(enemy14x, enemy14y+i-1, 30, 0x00);
        H_line(xEne14Bullet-10, yEne14Bullet-11+i, 30, 0x00);
      }
      for(int i = 0; i <= 11; i++){
        V_line(xP1bullet+10-i, yP1bullet, 10, 0x00);  
      }
      triggerP1 = 0;
      xP1bullet = -10;
      yP1bullet = -10;
    }
  
    if(yP1bullet <= (enemy15y+28) && (xP1bullet+9)>enemy15x && (xP1bullet+1)<(enemy15x+28) && enemy15Life == 0){//Enemy #15
      enemy15Life = 1;
      tripleWave4++;
      if(tripleWave4 == 4){
        tripleWave4 = 0;
        enemy18Life = 0;
        enemy19Life = 0;
        enemy20Life = 0;
        enemy21Life = 0;
        enemy22Life = 0;
        enemy23Life = 0;
      }
      LCD_Sprite(enemy15x, enemy15y, 30, 30, dead, 2, 1, 0, 0);
      LCD_Sprite(xEne15Bullet-10, yEne15Bullet-10, 30, 30, dead, 2, 0, 0, 0);
      delay(15);
      for(int i = 0; i<31; i++){
        H_line(enemy15x, enemy15y+i-1, 30, 0x00);
        H_line(xEne15Bullet-10, yEne15Bullet-11+i, 30, 0x00);
      }
      for(int i = 0; i <= 11; i++){
        V_line(xP1bullet+10-i, yP1bullet, 10, 0x00);  
      }
      triggerP1 = 0;
      xP1bullet = -10;
      yP1bullet = -10;
    }
  
    if(yP1bullet <= (enemy16y+28) && (xP1bullet+9)>enemy16x && (xP1bullet+1)<(enemy16x+28) && enemy16Life == 0){//Enemy #16
      enemy16Life = 1;
      tripleWave4++;
      if(tripleWave4 == 4){
        tripleWave4 = 0;
        enemy18Life = 0;
        enemy19Life = 0;
        enemy20Life = 0;
        enemy21Life = 0;
        enemy22Life = 0;
        enemy23Life = 0;
      }
      LCD_Sprite(enemy16x, enemy16y, 30, 30, dead, 2, 1, 0, 0);
      LCD_Sprite(xEne16Bullet-10, yEne16Bullet-10, 30, 30, dead, 2, 0, 0, 0);
      delay(15);
      for(int i = 0; i<31; i++){
        H_line(enemy16x, enemy16y+i-1, 30, 0x00);
        H_line(xEne16Bullet-10, yEne16Bullet-11+i, 30, 0x00);
      }
      for(int i = 0; i <= 11; i++){
        V_line(xP1bullet+10-i, yP1bullet, 10, 0x00);  
      }
      triggerP1 = 0;
      xP1bullet = -10;
      yP1bullet = -10;
    }
  
    if(yP1bullet <= (enemy17y+28) && (xP1bullet+9)>enemy17x && (xP1bullet+1)<(enemy17x+28) && enemy17Life == 0){//Enemy #17
      enemy17Life = 1;
      tripleWave4++;
      if(tripleWave4 == 4){
        tripleWave4 = 0;
        enemy18Life = 0;
        enemy19Life = 0;
        enemy20Life = 0;
        enemy21Life = 0;
        enemy22Life = 0;
        enemy23Life = 0;
      }
      LCD_Sprite(enemy17x, enemy17y, 30, 30, dead, 2, 1, 0, 0);
      LCD_Sprite(xEne17Bullet-10, yEne17Bullet-10, 30, 30, dead, 2, 0, 0, 0);
      delay(15);
      for(int i = 0; i<31; i++){
        H_line(enemy17x, enemy17y+i-1, 30, 0x00);
        H_line(xEne17Bullet-10, yEne17Bullet-11+i, 30, 0x00);
      }
      for(int i = 0; i <= 11; i++){
        V_line(xP1bullet+10-i, yP1bullet, 10, 0x00);  
      }
      triggerP1 = 0;
      xP1bullet = -10;
      yP1bullet = -10;
    }
  
    if(yP1bullet <= (enemy18y+28) && (xP1bullet+9)>enemy18x && (xP1bullet+1)<(enemy18x+28) && enemy18Life == 0){//Enemy #18
      enemy18Life = 1;
      tripleWave5++;
      LCD_Sprite(enemy18x, enemy18y, 30, 30, dead, 2, 1, 0, 0);
      LCD_Sprite(xEne18Bullet-10, yEne18Bullet-10, 30, 30, dead, 2, 0, 0, 0);
      delay(15);
      for(int i = 0; i<31; i++){
        H_line(enemy18x, enemy18y+i-1, 30, 0x00);
        H_line(xEne18Bullet-10, yEne18Bullet-11+i, 30, 0x00);
      }
      for(int i = 0; i <= 11; i++){
        V_line(xP1bullet+10-i, yP1bullet, 10, 0x00);  
      }
      triggerP1 = 0;
      xP1bullet = -10;
      yP1bullet = -10;
      if(tripleWave5 == 6){
        tripleWave5 = 0;
        LCD_Print(text1, 105, 100, 2, 0xffff, 0x00);
        delay(3000);
        LCD_Clear(0x00);
        enemy1Life = 0;
        enemy1y = 0;
        enemy2y = 0;
        enemy3y = 0;
        enemy4y = 0;
        enemy5y = 0;
        enemy6y = 0;
        enemy7y = 0;
        enemy8y = 0;
        enemy9y = 0;
        enemy10y = 0;
        enemy11y = 0;
        enemy12y = 0;
        enemy13y = 0;
        enemy14y = 0;
        enemy15y = 0;
        enemy16y = 0;
        enemy17y = 0;
        enemy18y = 0;
        enemy19y = 0;
        enemy20y = 0;
        enemy21y = 0;
        enemy22y = 0;
        enemy23y = 0;
        P1Life = 0;
        xP1 = 0;
        yP1 = 209;
        state = 0;
        turn = 0;
        mapeo_SD("menu.txt", 32, 25, 256);
      }
    }
  
    if(yP1bullet <= (enemy19y+28) && (xP1bullet+9)>enemy19x && (xP1bullet+1)<(enemy19x+28) && enemy19Life == 0){//Enemy #19
      enemy19Life = 1;
      tripleWave5++;
      LCD_Sprite(enemy19x, enemy19y, 30, 30, dead, 2, 1, 0, 0);
      LCD_Sprite(xEne19Bullet-10, yEne19Bullet-10, 30, 30, dead, 2, 0, 0, 0);
      delay(15);
      for(int i = 0; i<31; i++){
        H_line(enemy19x, enemy19y+i-1, 30, 0x00);
        H_line(xEne19Bullet-10, yEne19Bullet-11+i, 30, 0x00);
      }
      for(int i = 0; i <= 11; i++){
        V_line(xP1bullet+10-i, yP1bullet, 10, 0x00);  
      }
      triggerP1 = 0;
      xP1bullet = -10;
      yP1bullet = -10;
      if(tripleWave5 == 6){
        tripleWave5 = 0;
        LCD_Print(text1, 105, 100, 2, 0xffff, 0x00);
        delay(3000);
        LCD_Clear(0x00);
        enemy1Life = 0;
        enemy1y = 0;
        enemy2y = 0;
        enemy3y = 0;
        enemy4y = 0;
        enemy5y = 0;
        enemy6y = 0;
        enemy7y = 0;
        enemy8y = 0;
        enemy9y = 0;
        enemy10y = 0;
        enemy11y = 0;
        enemy12y = 0;
        enemy13y = 0;
        enemy14y = 0;
        enemy15y = 0;
        enemy16y = 0;
        enemy17y = 0;
        enemy18y = 0;
        enemy19y = 0;
        enemy20y = 0;
        enemy21y = 0;
        enemy22y = 0;
        enemy23y = 0;
        P1Life = 0;
        xP1 = 0;
        yP1 = 209;
        state = 0;
        turn = 0;
        mapeo_SD("menu.txt", 32, 25, 256);
      }
    }
  
    if(yP1bullet <= (enemy20y+28) && (xP1bullet+9)>enemy20x && (xP1bullet+1)<(enemy20x+28) && enemy20Life == 0){//Enemy #20
      enemy20Life = 1;
      tripleWave5++;
      LCD_Sprite(enemy20x, enemy20y, 30, 30, dead, 2, 1, 0, 0);
      LCD_Sprite(xEne20Bullet-10, yEne20Bullet-10, 30, 30, dead, 2, 0, 0, 0);
      delay(15);
      for(int i = 0; i<31; i++){
        H_line(enemy20x, enemy20y+i-1, 30, 0x00);
        H_line(xEne20Bullet-10, yEne20Bullet-11+i, 30, 0x00);
      }
      for(int i = 0; i <= 11; i++){
        V_line(xP1bullet+10-i, yP1bullet, 10, 0x00);  
      }
      triggerP1 = 0;
      xP1bullet = -10;
      yP1bullet = -10;
      if(tripleWave5 == 6){
        tripleWave5 = 0;
        LCD_Print(text1, 105, 100, 2, 0xffff, 0x00);
        delay(3000);
        LCD_Clear(0x00);
        enemy1Life = 0;
        enemy1y = 0;
        enemy2y = 0;
        enemy3y = 0;
        enemy4y = 0;
        enemy5y = 0;
        enemy6y = 0;
        enemy7y = 0;
        enemy8y = 0;
        enemy9y = 0;
        enemy10y = 0;
        enemy11y = 0;
        enemy12y = 0;
        enemy13y = 0;
        enemy14y = 0;
        enemy15y = 0;
        enemy16y = 0;
        enemy17y = 0;
        enemy18y = 0;
        enemy19y = 0;
        enemy20y = 0;
        enemy21y = 0;
        enemy22y = 0;
        enemy23y = 0;
        P1Life = 0;
        xP1 = 0;
        yP1 = 209;
        state = 0;
        turn = 0;
        mapeo_SD("menu.txt", 32, 25, 256);
      }
    }
  
    if(yP1bullet <= (enemy21y+28) && (xP1bullet+9)>enemy21x && (xP1bullet+1)<(enemy21x+28) && enemy21Life == 0){//Enemy #21
      enemy21Life = 1;
      tripleWave5++;
      LCD_Sprite(enemy21x, enemy21y, 30, 30, dead, 2, 1, 0, 0);
      LCD_Sprite(xEne21Bullet-10, yEne21Bullet-10, 30, 30, dead, 2, 0, 0, 0);
      delay(15);
      for(int i = 0; i<31; i++){
        H_line(enemy21x, enemy21y+i-1, 30, 0x00);
        H_line(xEne21Bullet-10, yEne21Bullet-11+i, 30, 0x00);
      }
      for(int i = 0; i <= 11; i++){
        V_line(xP1bullet+10-i, yP1bullet, 10, 0x00);  
      }
      triggerP1 = 0;
      xP1bullet = -10;
      yP1bullet = -10;
      if(tripleWave5 == 6){
        tripleWave5 = 0;
        LCD_Print(text1, 105, 100, 2, 0xffff, 0x00);
        delay(3000);
        LCD_Clear(0x00);
        enemy1Life = 0;
        enemy1y = 0;
        enemy2y = 0;
        enemy3y = 0;
        enemy4y = 0;
        enemy5y = 0;
        enemy6y = 0;
        enemy7y = 0;
        enemy8y = 0;
        enemy9y = 0;
        enemy10y = 0;
        enemy11y = 0;
        enemy12y = 0;
        enemy13y = 0;
        enemy14y = 0;
        enemy15y = 0;
        enemy16y = 0;
        enemy17y = 0;
        enemy18y = 0;
        enemy19y = 0;
        enemy20y = 0;
        enemy21y = 0;
        enemy22y = 0;
        enemy23y = 0;
        P1Life = 0;
        xP1 = 0;
        yP1 = 209;
        state = 0;
        turn = 0;
        mapeo_SD("menu.txt", 32, 25, 256);
      }
    }
  
    if(yP1bullet <= (enemy22y+28) && (xP1bullet+9)>enemy22x && (xP1bullet+1)<(enemy22x+28) && enemy22Life == 0){//Enemy #22
      enemy22Life = 1;
      tripleWave5++;
      LCD_Sprite(enemy22x, enemy22y, 30, 30, dead, 2, 1, 0, 0);
      LCD_Sprite(xEne22Bullet-10, yEne22Bullet-10, 30, 30, dead, 2, 0, 0, 0);
      delay(15);
      for(int i = 0; i<31; i++){
        H_line(enemy22x, enemy22y+i-1, 30, 0x00);
        H_line(xEne22Bullet-10, yEne22Bullet-11+i, 30, 0x00);
      }
      for(int i = 0; i <= 11; i++){
        V_line(xP1bullet+10-i, yP1bullet, 10, 0x00);  
      }
      triggerP1 = 0;
      xP1bullet = -10;
      yP1bullet = -10;
      if(tripleWave5 == 6){
        tripleWave5 = 0;
        LCD_Print(text1, 105, 100, 2, 0xffff, 0x00);
        delay(3000);
        LCD_Clear(0x00);
        enemy1Life = 0;
        enemy1y = 0;
        enemy2y = 0;
        enemy3y = 0;
        enemy4y = 0;
        enemy5y = 0;
        enemy6y = 0;
        enemy7y = 0;
        enemy8y = 0;
        enemy9y = 0;
        enemy10y = 0;
        enemy11y = 0;
        enemy12y = 0;
        enemy13y = 0;
        enemy14y = 0;
        enemy15y = 0;
        enemy16y = 0;
        enemy17y = 0;
        enemy18y = 0;
        enemy19y = 0;
        enemy20y = 0;
        enemy21y = 0;
        enemy22y = 0;
        enemy23y = 0;
        P1Life = 0;
        xP1 = 0;
        yP1 = 209;
        state = 0;
        turn = 0;
        mapeo_SD("menu.txt", 32, 25, 256);
      }
    }
  
    if(yP1bullet <= (enemy23y+28) && (xP1bullet+9)>enemy23x && (xP1bullet+1)<(enemy23x+28) && enemy23Life == 0){//Enemy #23
      enemy23Life = 1;
      tripleWave5++;
      LCD_Sprite(enemy23x, enemy23y, 30, 30, dead, 2, 1, 0, 0);
      LCD_Sprite(xEne23Bullet-10, yEne23Bullet-10, 30, 30, dead, 2, 0, 0, 0);
      delay(15);
      for(int i = 0; i<31; i++){
        H_line(enemy23x, enemy23y+i-1, 30, 0x00);
        H_line(xEne23Bullet-10, yEne23Bullet-11+i, 30, 0x00);
      }
      for(int i = 0; i <= 11; i++){
        V_line(xP1bullet+10-i, yP1bullet, 10, 0x00);  
      }
      triggerP1 = 0;
      xP1bullet = -10;
      yP1bullet = -10;
      if(tripleWave5 == 6){
        tripleWave5 = 0;
        LCD_Print(text1, 105, 100, 2, 0xffff, 0x00);
        delay(3000);
        LCD_Clear(0x00);
        enemy1Life = 0;
        enemy1y = 0;
        enemy2y = 0;
        enemy3y = 0;
        enemy4y = 0;
        enemy5y = 0;
        enemy6y = 0;
        enemy7y = 0;
        enemy8y = 0;
        enemy9y = 0;
        enemy10y = 0;
        enemy11y = 0;
        enemy12y = 0;
        enemy13y = 0;
        enemy14y = 0;
        enemy15y = 0;
        enemy16y = 0;
        enemy17y = 0;
        enemy18y = 0;
        enemy19y = 0;
        enemy20y = 0;
        enemy21y = 0;
        enemy22y = 0;
        enemy23y = 0;
        P1Life = 0;
        xP1 = 0;
        yP1 = 209;
        state = 0;
        turn = 0;
        mapeo_SD("menu.txt", 32, 25, 256);
      }
    }
  //**********************************************************************************************************************************
  //Player #1 bullet collision
  //**********************************************************************************************************************************


  //**********************************************************************************************************************************
  //Player #2 bullet collision
  //**********************************************************************************************************************************  
    if(yP2bullet <= (enemy1y+28) && (xP2bullet+9)>enemy1x && (xP2bullet+1)<(enemy1x+28) && enemy1Life == 0){//Enemy #1
      enemy1Life = 1;
      enemy2Life = 0;
      LCD_Sprite(enemy1x, enemy1y, 30, 30, dead, 2, 1, 0, 0);
      LCD_Sprite(xEne1Bullet-10, yEne1Bullet-10, 30, 30, dead, 2, 0, 0, 0);
      delay(15);
      for(int i = 0; i<31; i++){
        H_line(enemy1x, enemy1y+i-1, 30, 0x00);
        H_line(xEne1Bullet-10, yEne1Bullet-11+i, 30, 0x00);
      }
      for(int i = 0; i <= 11; i++){
        V_line(xP2bullet+10-i, yP2bullet, 10, 0x00);  
      }
      triggerP2 = 0;
      xP2bullet = -10;
      yP2bullet = -10;
    }
  
    if(yP2bullet <= (enemy2y+28) && (xP2bullet+9)>enemy2x && (xP2bullet+1)<(enemy2x+28) && enemy2Life == 0){//Enemy #2
      enemy2Life = 1;
      enemy3Life = 0;
      LCD_Sprite(enemy2x, enemy2y, 30, 30, dead, 2, 1, 0, 0);
      LCD_Sprite(xEne2Bullet-10, yEne2Bullet-10, 30, 30, dead, 2, 0, 0, 0);
      delay(15);
      for(int i = 0; i<31; i++){
        H_line(enemy2x, enemy2y+i-1, 30, 0x00);
        H_line(xEne2Bullet-10, yEne2Bullet-11+i, 30, 0x00);
      }
      for(int i = 0; i <= 11; i++){
        V_line(xP2bullet+10-i, yP2bullet, 10, 0x00);  
      }
      triggerP2 = 0;
      xP2bullet = -10;
      yP2bullet = -10;
    }
  
    if(yP2bullet <= (enemy3y+28) && (xP2bullet+9)>enemy3x && (xP2bullet+1)<(enemy3x+28) && enemy3Life == 0){//Enemy #3
      enemy3Life = 1;
      enemy4Life = 0;
      enemy5Life = 0;
      enemy6Life = 0;
      LCD_Sprite(enemy3x, enemy3y, 30, 30, dead, 2, 1, 0, 0);
      LCD_Sprite(xEne3Bullet-10, yEne3Bullet-10, 30, 30, dead, 2, 0, 0, 0);
      delay(15);
      for(int i = 0; i<31; i++){
        H_line(enemy3x, enemy3y+i-1, 30, 0x00);
        H_line(xEne3Bullet-10, yEne3Bullet-11+i, 30, 0x00);
      }
      for(int i = 0; i <= 11; i++){
        V_line(xP2bullet+10-i, yP2bullet, 10, 0x00);  
      }
      triggerP2 = 0;
      xP2bullet = -10;
      yP2bullet = -10;
    }
  
    if(yP2bullet <= (enemy4y+28) && (xP2bullet+9)>enemy4x && (xP2bullet+1)<(enemy4x+28) && enemy4Life == 0){//Enemy #4
      enemy4Life = 1;
      tripleWave1++;
      if(tripleWave1 == 3){
        tripleWave1 = 0;
        enemy7Life = 0;
        enemy8Life = 0;
        enemy9Life = 0;
      }
      LCD_Sprite(enemy4x, enemy4y, 30, 30, dead, 2, 1, 0, 0);
      LCD_Sprite(xEne4Bullet-10, yEne4Bullet-10, 30, 30, dead, 2, 0, 0, 0);
      delay(15);
      for(int i = 0; i<31; i++){
        H_line(enemy4x, enemy4y+i-1, 30, 0x00);
        H_line(xEne4Bullet-10, yEne4Bullet-11+i, 30, 0x00);
      }
      for(int i = 0; i <= 11; i++){
        V_line(xP2bullet+10-i, yP2bullet, 10, 0x00);  
      }
      triggerP2 = 0;
      xP2bullet = -10;
      yP2bullet = -10;
    }
  
    if(yP2bullet <= (enemy5y+28) && (xP2bullet+9)>enemy5x && (xP2bullet+1)<(enemy5x+28) && enemy5Life == 0){//Enemy #5
      enemy5Life = 1;
      tripleWave1++;
      if(tripleWave1 == 3){
        tripleWave1 = 0;
        enemy7Life = 0;
        enemy8Life = 0;
        enemy9Life = 0;
      }
      LCD_Sprite(enemy5x, enemy5y, 30, 30, dead, 2, 1, 0, 0);
      LCD_Sprite(xEne5Bullet-10, yEne5Bullet-10, 30, 30, dead, 2, 0, 0, 0);
      delay(15);
      for(int i = 0; i<31; i++){
        H_line(enemy5x, enemy5y+i-1, 30, 0x00);
        H_line(xEne5Bullet-10, yEne5Bullet-11+i, 30, 0x00);
      }
      for(int i = 0; i <= 11; i++){
        V_line(xP2bullet+10-i, yP2bullet, 10, 0x00);  
      }
      triggerP2 = 0;
      xP2bullet = -10;
      yP2bullet = -10;
    }
  
    if(yP2bullet <= (enemy6y+28) && (xP2bullet+9)>enemy6x && (xP2bullet+1)<(enemy6x+28) && enemy6Life == 0){//Enemy #6
      enemy6Life = 1;
      tripleWave1++;
      if(tripleWave1 == 3){
        tripleWave1 = 0;
        enemy7Life = 0;
        enemy8Life = 0;
        enemy9Life = 0;
      }
      LCD_Sprite(enemy6x, enemy6y, 30, 30, dead, 2, 1, 0, 0);
      LCD_Sprite(xEne6Bullet-10, yEne6Bullet-10, 30, 30, dead, 2, 0, 0, 0);
      delay(15);
      for(int i = 0; i<31; i++){
        H_line(enemy6x, enemy6y+i-1, 30, 0x00);
        H_line(xEne6Bullet-10, yEne6Bullet-11+i, 30, 0x00);
      }
      for(int i = 0; i <= 11; i++){
        V_line(xP2bullet+10-i, yP2bullet, 10, 0x00);  
      }
      triggerP2 = 0;
      xP2bullet = -10;
      yP2bullet = -10;
    }
  
    if(yP2bullet <= (enemy7y+28) && (xP2bullet+9)>enemy7x && (xP2bullet+1)<(enemy7x+28) && enemy7Life == 0){//Enemy #7
      enemy7Life = 1;
      tripleWave2++;
      if(tripleWave2 == 3){
        tripleWave2 = 0;
        enemy10Life = 0;
        enemy11Life = 0;
        enemy12Life = 0;
        enemy13Life = 0;  
      }
      LCD_Sprite(enemy7x, enemy7y, 30, 30, dead, 2, 1, 0, 0);
      LCD_Sprite(xEne7Bullet-10, yEne7Bullet-10, 30, 30, dead, 2, 0, 0, 0);
      delay(15);
      for(int i = 0; i<31; i++){
        H_line(enemy7x, enemy7y+i-1, 30, 0x00);
        H_line(xEne7Bullet-10, yEne7Bullet-11+i, 30, 0x00);
      }
      for(int i = 0; i <= 11; i++){
        V_line(xP2bullet+10-i, yP2bullet, 10, 0x00);  
      }
      triggerP2 = 0;
      xP2bullet = -10;
      yP2bullet = -10;
    }
  
    if(yP2bullet <= (enemy8y+28) && (xP2bullet+9)>enemy8x && (xP2bullet+1)<(enemy8x+28) && enemy8Life == 0){//Enemy #8
      enemy8Life = 1;
      tripleWave2++;
      if(tripleWave2 == 3){
        tripleWave2 = 0;
        enemy10Life = 0;
        enemy11Life = 0;
        enemy12Life = 0;
        enemy13Life = 0;  
      }
      LCD_Sprite(enemy8x, enemy8y, 30, 30, dead, 2, 1, 0, 0);
      LCD_Sprite(xEne8Bullet-10, yEne8Bullet-10, 30, 30, dead, 2, 0, 0, 0);
      delay(15);
      for(int i = 0; i<31; i++){
        H_line(enemy8x, enemy8y+i-1, 30, 0x00);
        H_line(xEne8Bullet-10, yEne8Bullet-11+i, 30, 0x00);
      }
      for(int i = 0; i <= 11; i++){
        V_line(xP2bullet+10-i, yP2bullet, 10, 0x00);  
      }
      triggerP2 = 0;
      xP2bullet = -10;
      yP2bullet = -10;
    }
  
    if(yP2bullet <= (enemy9y+28) && (xP2bullet+9)>enemy9x && (xP2bullet+1)<(enemy9x+28) && enemy9Life == 0){//Enemy #9
      enemy9Life = 1;
      tripleWave2++;
      if(tripleWave2 == 3){
        tripleWave2 = 0;
        enemy10Life = 0;
        enemy11Life = 0;
        enemy12Life = 0;
        enemy13Life = 0;  
      }
      LCD_Sprite(enemy9x, enemy9y, 30, 30, dead, 2, 1, 0, 0);
      LCD_Sprite(xEne9Bullet-10, yEne9Bullet-10, 30, 30, dead, 2, 0, 0, 0);
      delay(15);
      for(int i = 0; i<31; i++){
        H_line(enemy9x, enemy9y+i-1, 30, 0x00);
        H_line(xEne9Bullet-10, yEne9Bullet-11+i, 30, 0x00);
      }
      for(int i = 0; i <= 11; i++){
        V_line(xP2bullet+10-i, yP2bullet, 10, 0x00);  
      }
      triggerP2 = 0;
      xP2bullet = -10;
      yP2bullet = -10;
    }
  
    if(yP2bullet <= (enemy10y+28) && (xP2bullet+9)>enemy10x && (xP2bullet+1)<(enemy10x+28) && enemy10Life == 0){//Enemy #10
      enemy10Life = 1;
      tripleWave3++;
      if(tripleWave3 == 4){
        tripleWave3 = 0;
        enemy14Life = 0;
        enemy15Life = 0;
        enemy16Life = 0;
        enemy17Life = 0;  
      }
      LCD_Sprite(enemy10x, enemy10y, 30, 30, dead, 2, 1, 0, 0);
      LCD_Sprite(xEne10Bullet-10, yEne10Bullet-10, 30, 30, dead, 2, 0, 0, 0);
      delay(15);
      for(int i = 0; i<31; i++){
        H_line(enemy10x, enemy10y+i-1, 30, 0x00);
        H_line(xEne10Bullet-10, yEne10Bullet-11+i, 30, 0x00);
      }
      for(int i = 0; i <= 11; i++){
        V_line(xP2bullet+10-i, yP2bullet, 10, 0x00);  
      }
      triggerP2 = 0;
      xP2bullet = -10;
      yP2bullet = -10;
    }
  
    if(yP2bullet <= (enemy11y+28) && (xP2bullet+9)>enemy11x && (xP2bullet+1)<(enemy11x+28) && enemy11Life == 0){//Enemy #11
      enemy11Life = 1;
      tripleWave3++;
      if(tripleWave3 == 4){
        tripleWave3 = 0;
        enemy14Life = 0;
        enemy15Life = 0;
        enemy16Life = 0;
        enemy17Life = 0;  
      }
      LCD_Sprite(enemy11x, enemy11y, 30, 30, dead, 2, 1, 0, 0);
      LCD_Sprite(xEne11Bullet-10, yEne11Bullet-10, 30, 30, dead, 2, 0, 0, 0);
      delay(15);
      for(int i = 0; i<31; i++){
        H_line(enemy11x, enemy11y+i-1, 30, 0x00);
        H_line(xEne11Bullet-10, yEne11Bullet-11+i, 30, 0x00);
      }
      for(int i = 0; i <= 11; i++){
        V_line(xP2bullet+10-i, yP2bullet, 10, 0x00);  
      }
      triggerP2 = 0;
      xP2bullet = -10;
      yP2bullet = -10;
    }
  
    if(yP2bullet <= (enemy12y+28) && (xP2bullet+9)>enemy12x && (xP2bullet+1)<(enemy12x+28) && enemy12Life == 0){//Enemy #12
      enemy12Life = 1;
      tripleWave3++;
      if(tripleWave3 == 4){
        tripleWave3 = 0;
        enemy14Life = 0;
        enemy15Life = 0;
        enemy16Life = 0;
        enemy17Life = 0;  
      }
      LCD_Sprite(enemy12x, enemy12y, 30, 30, dead, 2, 1, 0, 0);
      LCD_Sprite(xEne12Bullet-10, yEne12Bullet-10, 30, 30, dead, 2, 0, 0, 0);
      delay(15);
      for(int i = 0; i<31; i++){
        H_line(enemy12x, enemy12y+i-1, 30, 0x00);
        H_line(xEne12Bullet-10, yEne12Bullet-11+i, 30, 0x00);
      }
      for(int i = 0; i <= 11; i++){
        V_line(xP2bullet+10-i, yP2bullet, 10, 0x00);  
      }
      triggerP2 = 0;
      xP2bullet = -10;
      yP2bullet = -10;
    }
  
    if(yP2bullet <= (enemy13y+28) && (xP2bullet+9)>enemy13x && (xP2bullet+1)<(enemy13x+28) && enemy13Life == 0){//Enemy #13
      enemy13Life = 1;
      tripleWave3++;
      if(tripleWave3 == 4){
        tripleWave3 = 0;
        enemy14Life = 0;
        enemy15Life = 0;
        enemy16Life = 0;
        enemy17Life = 0;  
      }
      LCD_Sprite(enemy13x, enemy13y, 30, 30, dead, 2, 1, 0, 0);
      LCD_Sprite(xEne13Bullet-10, yEne13Bullet-10, 30, 30, dead, 2, 0, 0, 0);
      delay(15);
      for(int i = 0; i<31; i++){
        H_line(enemy13x, enemy13y+i-1, 30, 0x00);
        H_line(xEne13Bullet-10, yEne13Bullet-11+i, 30, 0x00);
      }
      for(int i = 0; i <= 11; i++){
        V_line(xP2bullet+10-i, yP2bullet, 10, 0x00);  
      }
      triggerP2 = 0;
      xP2bullet = -10;
      yP2bullet = -10;
    }
  
    if(yP2bullet <= (enemy14y+28) && (xP2bullet+9)>enemy14x && (xP2bullet+1)<(enemy14x+28) && enemy14Life == 0){//Enemy #14
      enemy14Life = 1;
      tripleWave4++;
      if(tripleWave4 == 4){
        tripleWave4 = 0;
        enemy18Life = 0;
        enemy19Life = 0;
        enemy20Life = 0;
        enemy21Life = 0;
        enemy22Life = 0;
        enemy23Life = 0;
      }
      LCD_Sprite(enemy14x, enemy14y, 30, 30, dead, 2, 1, 0, 0);
      LCD_Sprite(xEne14Bullet-10, yEne14Bullet-10, 30, 30, dead, 2, 0, 0, 0);
      delay(15);
      for(int i = 0; i<31; i++){
        H_line(enemy14x, enemy14y+i-1, 30, 0x00);
        H_line(xEne14Bullet-10, yEne14Bullet-11+i, 30, 0x00);
      }
      for(int i = 0; i <= 11; i++){
        V_line(xP2bullet+10-i, yP2bullet, 10, 0x00);  
      }
      triggerP2 = 0;
      xP2bullet = -10;
      yP2bullet = -10;
    }
  
    if(yP2bullet <= (enemy15y+28) && (xP2bullet+9)>enemy15x && (xP2bullet+1)<(enemy15x+28) && enemy15Life == 0){//Enemy #15
      enemy15Life = 1;
      tripleWave4++;
      if(tripleWave4 == 4){
        tripleWave4 = 0;
        enemy18Life = 0;
        enemy19Life = 0;
        enemy20Life = 0;
        enemy21Life = 0;
        enemy22Life = 0;
        enemy23Life = 0;
      }
      LCD_Sprite(enemy15x, enemy15y, 30, 30, dead, 2, 1, 0, 0);
      LCD_Sprite(xEne15Bullet-10, yEne15Bullet-10, 30, 30, dead, 2, 0, 0, 0);
      delay(15);
      for(int i = 0; i<31; i++){
        H_line(enemy15x, enemy15y+i-1, 30, 0x00);
        H_line(xEne15Bullet-10, yEne15Bullet-11+i, 30, 0x00);
      }
      for(int i = 0; i <= 11; i++){
        V_line(xP2bullet+10-i, yP2bullet, 10, 0x00);  
      }
      triggerP2 = 0;
      xP2bullet = -10;
      yP2bullet = -10;
    }
  
    if(yP2bullet <= (enemy16y+28) && (xP2bullet+9)>enemy16x && (xP2bullet+1)<(enemy16x+28) && enemy16Life == 0){//Enemy #16
      enemy16Life = 1;
      tripleWave4++;
      if(tripleWave4 == 4){
        tripleWave4 = 0;
        enemy18Life = 0;
        enemy19Life = 0;
        enemy20Life = 0;
        enemy21Life = 0;
        enemy22Life = 0;
        enemy23Life = 0;
      }
      LCD_Sprite(enemy16x, enemy16y, 30, 30, dead, 2, 1, 0, 0);
      LCD_Sprite(xEne16Bullet-10, yEne16Bullet-10, 30, 30, dead, 2, 0, 0, 0);
      delay(15);
      for(int i = 0; i<31; i++){
        H_line(enemy16x, enemy16y+i-1, 30, 0x00);
        H_line(xEne16Bullet-10, yEne16Bullet-11+i, 30, 0x00);
      }
      for(int i = 0; i <= 11; i++){
        V_line(xP2bullet+10-i, yP2bullet, 10, 0x00);  
      }
      triggerP2 = 0;
      xP2bullet = -10;
      yP2bullet = -10;
    }
  
    if(yP2bullet <= (enemy17y+28) && (xP2bullet+9)>enemy17x && (xP2bullet+1)<(enemy17x+28) && enemy17Life == 0){//Enemy #17
      enemy17Life = 1;
      tripleWave4++;
      if(tripleWave4 == 4){
        tripleWave4 = 0;
        enemy18Life = 0;
        enemy19Life = 0;
        enemy20Life = 0;
        enemy21Life = 0;
        enemy22Life = 0;
        enemy23Life = 0;
      }
      LCD_Sprite(enemy17x, enemy17y, 30, 30, dead, 2, 1, 0, 0);
      LCD_Sprite(xEne17Bullet-10, yEne17Bullet-10, 30, 30, dead, 2, 0, 0, 0);
      delay(15);
      for(int i = 0; i<31; i++){
        H_line(enemy17x, enemy17y+i-1, 30, 0x00);
        H_line(xEne17Bullet-10, yEne17Bullet-11+i, 30, 0x00);
      }
      for(int i = 0; i <= 11; i++){
        V_line(xP2bullet+10-i, yP2bullet, 10, 0x00);  
      }
      triggerP2 = 0;
      xP2bullet = -10;
      yP2bullet = -10;
    }
  
    if(yP2bullet <= (enemy18y+28) && (xP2bullet+9)>enemy18x && (xP2bullet+1)<(enemy18x+28) && enemy18Life == 0){//Enemy #18
      enemy18Life = 1;
      tripleWave5++;
      LCD_Sprite(enemy18x, enemy18y, 30, 30, dead, 2, 1, 0, 0);
      LCD_Sprite(xEne18Bullet-10, yEne18Bullet-10, 30, 30, dead, 2, 0, 0, 0);
      delay(15);
      for(int i = 0; i<31; i++){
        H_line(enemy18x, enemy18y+i-1, 30, 0x00);
        H_line(xEne18Bullet-10, yEne18Bullet-11+i, 30, 0x00);
      }
      for(int i = 0; i <= 11; i++){
        V_line(xP2bullet+10-i, yP2bullet, 10, 0x00);  
      }
      triggerP2 = 0;
      xP2bullet = -10;
      yP2bullet = -10;
      if(tripleWave5 == 6){
        tripleWave5 = 0;
        LCD_Print(text1, 105, 100, 2, 0xffff, 0x00);
        delay(3000);
        LCD_Clear(0x00);
        enemy1Life = 0;
        enemy1y = 0;
        enemy2y = 0;
        enemy3y = 0;
        enemy4y = 0;
        enemy5y = 0;
        enemy6y = 0;
        enemy7y = 0;
        enemy8y = 0;
        enemy9y = 0;
        enemy10y = 0;
        enemy11y = 0;
        enemy12y = 0;
        enemy13y = 0;
        enemy14y = 0;
        enemy15y = 0;
        enemy16y = 0;
        enemy17y = 0;
        enemy18y = 0;
        enemy19y = 0;
        enemy20y = 0;
        enemy21y = 0;
        enemy22y = 0;
        enemy23y = 0;
        P1Life = 0;
        P2Life = 0;
        xP1 = 0;
        yP1 = 209;
        xP2 = 290;
        yP2 = 209;
        state = 0;
        turn = 0;
        mapeo_SD("menu.txt", 32, 25, 256);
      }
    }
  
    if(yP2bullet <= (enemy19y+28) && (xP2bullet+9)>enemy19x && (xP2bullet+1)<(enemy19x+28) && enemy19Life == 0){//Enemy #19
      enemy19Life = 1;
      tripleWave5++;
      LCD_Sprite(enemy19x, enemy19y, 30, 30, dead, 2, 1, 0, 0);
      LCD_Sprite(xEne19Bullet-10, yEne19Bullet-10, 30, 30, dead, 2, 0, 0, 0);
      delay(15);
      for(int i = 0; i<31; i++){
        H_line(enemy19x, enemy19y+i-1, 30, 0x00);
        H_line(xEne19Bullet-10, yEne19Bullet-11+i, 30, 0x00);
      }
      for(int i = 0; i <= 11; i++){
        V_line(xP2bullet+10-i, yP2bullet, 10, 0x00);  
      }
      triggerP2 = 0;
      xP2bullet = -10;
      yP2bullet = -10;
      if(tripleWave5 == 6){
        tripleWave5 = 0;
        LCD_Print(text1, 105, 100, 2, 0xffff, 0x00);
        delay(3000);
        LCD_Clear(0x00);
        enemy1Life = 0;
        enemy1y = 0;
        enemy2y = 0;
        enemy3y = 0;
        enemy4y = 0;
        enemy5y = 0;
        enemy6y = 0;
        enemy7y = 0;
        enemy8y = 0;
        enemy9y = 0;
        enemy10y = 0;
        enemy11y = 0;
        enemy12y = 0;
        enemy13y = 0;
        enemy14y = 0;
        enemy15y = 0;
        enemy16y = 0;
        enemy17y = 0;
        enemy18y = 0;
        enemy19y = 0;
        enemy20y = 0;
        enemy21y = 0;
        enemy22y = 0;
        enemy23y = 0;
        P1Life = 0;
        P2Life = 0;
        xP1 = 0;
        yP1 = 209;
        xP2 = 290;
        yP2 = 209;
        state = 0;
        turn = 0;
        mapeo_SD("menu.txt", 32, 25, 256);
      }
    }
  
    if(yP2bullet <= (enemy20y+28) && (xP2bullet+9)>enemy20x && (xP2bullet+1)<(enemy20x+28) && enemy20Life == 0){//Enemy #20
      enemy20Life = 1;
      tripleWave5++;
      LCD_Sprite(enemy20x, enemy20y, 30, 30, dead, 2, 1, 0, 0);
      LCD_Sprite(xEne20Bullet-10, yEne20Bullet-10, 30, 30, dead, 2, 0, 0, 0);
      delay(15);
      for(int i = 0; i<31; i++){
        H_line(enemy20x, enemy20y+i-1, 30, 0x00);
        H_line(xEne20Bullet-10, yEne20Bullet-11+i, 30, 0x00);
      }
      for(int i = 0; i <= 11; i++){
        V_line(xP2bullet+10-i, yP2bullet, 10, 0x00);  
      }
      triggerP2 = 0;
      xP2bullet = -10;
      yP2bullet = -10;
      if(tripleWave5 == 6){
        tripleWave5 = 0;
        LCD_Print(text1, 105, 100, 2, 0xffff, 0x00);
        delay(3000);
        LCD_Clear(0x00);
        enemy1Life = 0;
        enemy1y = 0;
        enemy2y = 0;
        enemy3y = 0;
        enemy4y = 0;
        enemy5y = 0;
        enemy6y = 0;
        enemy7y = 0;
        enemy8y = 0;
        enemy9y = 0;
        enemy10y = 0;
        enemy11y = 0;
        enemy12y = 0;
        enemy13y = 0;
        enemy14y = 0;
        enemy15y = 0;
        enemy16y = 0;
        enemy17y = 0;
        enemy18y = 0;
        enemy19y = 0;
        enemy20y = 0;
        enemy21y = 0;
        enemy22y = 0;
        enemy23y = 0;
        P1Life = 0;
        P2Life = 0;
        xP1 = 0;
        yP1 = 209;
        xP2 = 290;
        yP2 = 209;
        state = 0;
        turn = 0;
        mapeo_SD("menu.txt", 32, 25, 256);
      }
    }
  
    if(yP2bullet <= (enemy21y+28) && (xP2bullet+9)>enemy21x && (xP2bullet+1)<(enemy21x+28) && enemy21Life == 0){//Enemy #21
      enemy21Life = 1;
      tripleWave5++;
      LCD_Sprite(enemy21x, enemy21y, 30, 30, dead, 2, 1, 0, 0);
      LCD_Sprite(xEne21Bullet-10, yEne21Bullet-10, 30, 30, dead, 2, 0, 0, 0);
      delay(15);
      for(int i = 0; i<31; i++){
        H_line(enemy21x, enemy21y+i-1, 30, 0x00);
        H_line(xEne21Bullet-10, yEne21Bullet-11+i, 30, 0x00);
      }
      for(int i = 0; i <= 11; i++){
        V_line(xP2bullet+10-i, yP2bullet, 10, 0x00);  
      }
      triggerP2 = 0;
      xP2bullet = -10;
      yP2bullet = -10;
      if(tripleWave5 == 6){
        tripleWave5 = 0;
        LCD_Print(text1, 105, 100, 2, 0xffff, 0x00);
        delay(3000);
        LCD_Clear(0x00);
        enemy1Life = 0;
        enemy1y = 0;
        enemy2y = 0;
        enemy3y = 0;
        enemy4y = 0;
        enemy5y = 0;
        enemy6y = 0;
        enemy7y = 0;
        enemy8y = 0;
        enemy9y = 0;
        enemy10y = 0;
        enemy11y = 0;
        enemy12y = 0;
        enemy13y = 0;
        enemy14y = 0;
        enemy15y = 0;
        enemy16y = 0;
        enemy17y = 0;
        enemy18y = 0;
        enemy19y = 0;
        enemy20y = 0;
        enemy21y = 0;
        enemy22y = 0;
        enemy23y = 0;
        P1Life = 0;
        P2Life = 0;
        xP1 = 0;
        yP1 = 209;
        xP2 = 290;
        yP2 = 209;
        state = 0;
        turn = 0;
        mapeo_SD("menu.txt", 32, 25, 256);
      }
    }
  
    if(yP2bullet <= (enemy22y+28) && (xP2bullet+9)>enemy22x && (xP2bullet+1)<(enemy22x+28) && enemy22Life == 0){//Enemy #22
      enemy22Life = 1;
      tripleWave5++;
      LCD_Sprite(enemy22x, enemy22y, 30, 30, dead, 2, 1, 0, 0);
      LCD_Sprite(xEne22Bullet-10, yEne22Bullet-10, 30, 30, dead, 2, 0, 0, 0);
      delay(15);
      for(int i = 0; i<31; i++){
        H_line(enemy22x, enemy22y+i-1, 30, 0x00);
        H_line(xEne22Bullet-10, yEne22Bullet-11+i, 30, 0x00);
      }
      for(int i = 0; i <= 11; i++){
        V_line(xP2bullet+10-i, yP2bullet, 10, 0x00);  
      }
      triggerP2 = 0;
      xP2bullet = -10;
      yP2bullet = -10;
      if(tripleWave5 == 6){
        tripleWave5 = 0;
        LCD_Print(text1, 105, 100, 2, 0xffff, 0x00);
        delay(3000);
        LCD_Clear(0x00);
        enemy1Life = 0;
        enemy1y = 0;
        enemy2y = 0;
        enemy3y = 0;
        enemy4y = 0;
        enemy5y = 0;
        enemy6y = 0;
        enemy7y = 0;
        enemy8y = 0;
        enemy9y = 0;
        enemy10y = 0;
        enemy11y = 0;
        enemy12y = 0;
        enemy13y = 0;
        enemy14y = 0;
        enemy15y = 0;
        enemy16y = 0;
        enemy17y = 0;
        enemy18y = 0;
        enemy19y = 0;
        enemy20y = 0;
        enemy21y = 0;
        enemy22y = 0;
        enemy23y = 0;
        P1Life = 0;
        P2Life = 0;
        xP1 = 0;
        yP1 = 209;
        xP2 = 290;
        yP2 = 209;
        state = 0;
        turn = 0;
        mapeo_SD("menu.txt", 32, 25, 256);
      }
    }
  
    if(yP2bullet <= (enemy23y+28) && (xP2bullet+9)>enemy23x && (xP2bullet+1)<(enemy23x+28) && enemy23Life == 0){//Enemy #23
      enemy23Life = 1;
      tripleWave5++;
      LCD_Sprite(enemy23x, enemy23y, 30, 30, dead, 2, 1, 0, 0);
      LCD_Sprite(xEne23Bullet-10, yEne23Bullet-10, 30, 30, dead, 2, 0, 0, 0);
      delay(15);
      for(int i = 0; i<31; i++){
        H_line(enemy23x, enemy23y+i-1, 30, 0x00);
        H_line(xEne23Bullet-10, yEne23Bullet-11+i, 30, 0x00);
      }
      for(int i = 0; i <= 11; i++){
        V_line(xP2bullet+10-i, yP2bullet, 10, 0x00);  
      }
      triggerP2 = 0;
      xP2bullet = -10;
      yP2bullet = -10;
      if(tripleWave5 == 6){
        tripleWave5 = 0;
        LCD_Print(text1, 105, 100, 2, 0xffff, 0x00);
        delay(3000);
        LCD_Clear(0x00);
        enemy1Life = 0;
        enemy1y = 0;
        enemy2y = 0;
        enemy3y = 0;
        enemy4y = 0;
        enemy5y = 0;
        enemy6y = 0;
        enemy7y = 0;
        enemy8y = 0;
        enemy9y = 0;
        enemy10y = 0;
        enemy11y = 0;
        enemy12y = 0;
        enemy13y = 0;
        enemy14y = 0;
        enemy15y = 0;
        enemy16y = 0;
        enemy17y = 0;
        enemy18y = 0;
        enemy19y = 0;
        enemy20y = 0;
        enemy21y = 0;
        enemy22y = 0;
        enemy23y = 0;
        P1Life = 0;
        P2Life = 0;
        xP1 = 0;
        yP1 = 209;
        xP2 = 290;
        yP2 = 209;
        state = 0;
        turn = 0;
        mapeo_SD("menu.txt", 32, 25, 256);
      }
    }
  //**********************************************************************************************************************************
  //Player #1 bullet collision
  //**********************************************************************************************************************************
  
  //**********************************************************************************************************************************
  //Player #1 bullet direction
  //**********************************************************************************************************************************  
    if(triggerP1==1 && P1bullDirection == 1){//P1 bullet moving up
      LCD_Bitmap(xP1bullet, yP1bullet, 10, 10, bulletUp);
      H_line(xP1bullet, yP1bullet+10, 10, 0x00);
      yP1bullet--;
      LCD_Bitmap(xP1bullet, yP1bullet, 10, 10, bulletUp);
      H_line(xP1bullet, yP1bullet+10, 10, 0x00);
      yP1bullet--;
      if(yP1bullet <= 0){
        for(int i = 10; i >= 0; i--){
          H_line(xP1bullet, yP1bullet + i, 10, 0x00);  
        }
        triggerP1 = 0;
        xP1bullet = -10;
        yP1bullet = -10;
      }
    }else if(triggerP1==1 && P1bullDirection == 2){//P1 bullet moving right
      LCD_Bitmap(xP1bullet, yP1bullet, 10, 10, bulletRight);
      V_line(xP1bullet-1, yP1bullet, 10, 0x00);
      xP1bullet++;
      LCD_Bitmap(xP1bullet, yP1bullet, 10, 10, bulletRight);
      V_line(xP1bullet-1, yP1bullet, 10, 0x00);
      xP1bullet++;
      
      if((xP1bullet+10) >= 320){
        for(int i = 0; i <= 11; i++){
          V_line(xP1bullet+9-i, yP1bullet, 10, 0x00);  
        }
        triggerP1 = 0;
        xP1bullet = -10;
        yP1bullet = -10;
      }
    }else if(triggerP1==1 && P1bullDirection == 3){//P1 bullet moving left
      LCD_Bitmap(xP1bullet, yP1bullet, 10, 10, bulletLeft);
      V_line(xP1bullet+10, yP1bullet, 10, 0x00);
      xP1bullet--;
      LCD_Bitmap(xP1bullet, yP1bullet, 10, 10, bulletLeft);
      V_line(xP1bullet+10, yP1bullet, 10, 0x00);
      xP1bullet--;
      
      if(xP1bullet <= 0){
        for(int i = 0; i <= 11; i++){
          V_line(xP1bullet+i, yP1bullet, 10, 0x00);  
        }
        triggerP1 = 0;
        xP1bullet = -10;
        yP1bullet = -10;
      }
    }else{
      triggerP1 = 0;  
      xP1bullet = -10;
      yP1bullet = -10;
    }
  //**********************************************************************************************************************************
  //Player #1 bullet direction
  //**********************************************************************************************************************************

  //**********************************************************************************************************************************
  //Player #2 bullet direction
  //**********************************************************************************************************************************  
    if(triggerP2==1 && P2bullDirection == 1){//P2 bullet moving up
      LCD_Bitmap(xP2bullet, yP2bullet, 10, 10, bulletUp);
      H_line(xP2bullet, yP2bullet+10, 10, 0x00);
      yP2bullet--;
      LCD_Bitmap(xP2bullet, yP2bullet, 10, 10, bulletUp);
      H_line(xP2bullet, yP2bullet+10, 10, 0x00);
      yP2bullet--;
      if(yP2bullet <= 0){
        for(int i = 10; i >= 0; i--){
          H_line(xP2bullet, yP2bullet + i, 10, 0x00);  
        }
        triggerP2 = 0;
        xP2bullet = -10;
        yP2bullet = -10;
      }
    }else if(triggerP2==1 && P2bullDirection == 2){//P2 bullet moving right
      LCD_Bitmap(xP2bullet, yP2bullet, 10, 10, bulletRight);
      V_line(xP2bullet-1, yP2bullet, 10, 0x00);
      xP2bullet++;
      LCD_Bitmap(xP2bullet, yP2bullet, 10, 10, bulletRight);
      V_line(xP2bullet-1, yP2bullet, 10, 0x00);
      xP2bullet++;
      
      if((xP2bullet+10) >= 320){
        for(int i = 0; i <= 11; i++){
          V_line(xP2bullet+9-i, yP2bullet, 10, 0x00);  
        }
        triggerP2 = 0;
        xP2bullet = -10;
        yP2bullet = -10;
      }
    }else if(triggerP2==1 && P2bullDirection == 3){//P2 bullet moving left
      LCD_Bitmap(xP2bullet, yP2bullet, 10, 10, bulletLeft);
      V_line(xP2bullet+10, yP2bullet, 10, 0x00);
      xP2bullet--;
      LCD_Bitmap(xP2bullet, yP2bullet, 10, 10, bulletLeft);
      V_line(xP2bullet+10, yP2bullet, 10, 0x00);
      xP2bullet--;
      
      if(xP2bullet <= 0){
        for(int i = 0; i <= 11; i++){
          V_line(xP2bullet+i, yP2bullet, 10, 0x00);  
        }
        triggerP2 = 0;
        xP2bullet = -10;
        yP2bullet = -10;
      }
    }else{
      triggerP2 = 0;  
      xP2bullet = -10;
      yP2bullet = -10;
    }
  //**********************************************************************************************************************************
  //Player #2 bullet direction
  //**********************************************************************************************************************************
    delay(10);
  }
}
//***************************************************************************************************************************************
// Función para inicializar LCD
//***************************************************************************************************************************************
void LCD_Init(void) {
  pinMode(LCD_RST, OUTPUT);
  pinMode(LCD_CS, OUTPUT);
  pinMode(LCD_RS, OUTPUT);
  pinMode(LCD_WR, OUTPUT);
  pinMode(LCD_RD, OUTPUT);
  for (uint8_t i = 0; i < 8; i++){
    pinMode(DPINS[i], OUTPUT);
  }
  //****************************************
  // Secuencia de Inicialización
  //****************************************
  digitalWrite(LCD_CS, HIGH);
  digitalWrite(LCD_RS, HIGH);
  digitalWrite(LCD_WR, HIGH);
  digitalWrite(LCD_RD, HIGH);
  digitalWrite(LCD_RST, HIGH);
  delay(5);
  digitalWrite(LCD_RST, LOW);
  delay(20);
  digitalWrite(LCD_RST, HIGH);
  delay(150);
  digitalWrite(LCD_CS, LOW);
  //****************************************
  LCD_CMD(0xE9);  // SETPANELRELATED
  LCD_DATA(0x20);
  //****************************************
  LCD_CMD(0x11); // Exit Sleep SLEEP OUT (SLPOUT)
  delay(100);
  //****************************************
  LCD_CMD(0xD1);    // (SETVCOM)
  LCD_DATA(0x00);
  LCD_DATA(0x71);
  LCD_DATA(0x19);
  //****************************************
  LCD_CMD(0xD0);   // (SETPOWER) 
  LCD_DATA(0x07);
  LCD_DATA(0x01);
  LCD_DATA(0x08);
  //****************************************
  LCD_CMD(0x36);  // (MEMORYACCESS)
  LCD_DATA(0x40|0x80|0x20|0x08); // LCD_DATA(0x19);
  //****************************************
  LCD_CMD(0x3A); // Set_pixel_format (PIXELFORMAT)
  LCD_DATA(0x05); // color setings, 05h - 16bit pixel, 11h - 3bit pixel
  //****************************************
  LCD_CMD(0xC1);    // (POWERCONTROL2)
  LCD_DATA(0x10);
  LCD_DATA(0x10);
  LCD_DATA(0x02);
  LCD_DATA(0x02);
  //****************************************
  LCD_CMD(0xC0); // Set Default Gamma (POWERCONTROL1)
  LCD_DATA(0x00);
  LCD_DATA(0x35);
  LCD_DATA(0x00);
  LCD_DATA(0x00);
  LCD_DATA(0x01);
  LCD_DATA(0x02);
  //****************************************
  LCD_CMD(0xC5); // Set Frame Rate (VCOMCONTROL1)
  LCD_DATA(0x04); // 72Hz
  //****************************************
  LCD_CMD(0xD2); // Power Settings  (SETPWRNORMAL)
  LCD_DATA(0x01);
  LCD_DATA(0x44);
  //****************************************
  LCD_CMD(0xC8); //Set Gamma  (GAMMASET)
  LCD_DATA(0x04);
  LCD_DATA(0x67);
  LCD_DATA(0x35);
  LCD_DATA(0x04);
  LCD_DATA(0x08);
  LCD_DATA(0x06);
  LCD_DATA(0x24);
  LCD_DATA(0x01);
  LCD_DATA(0x37);
  LCD_DATA(0x40);
  LCD_DATA(0x03);
  LCD_DATA(0x10);
  LCD_DATA(0x08);
  LCD_DATA(0x80);
  LCD_DATA(0x00);
  //****************************************
  LCD_CMD(0x2A); // Set_column_address 320px (CASET)
  LCD_DATA(0x00);
  LCD_DATA(0x00);
  LCD_DATA(0x01);
  LCD_DATA(0x3F);
  //****************************************
  LCD_CMD(0x2B); // Set_page_address 480px (PASET)
  LCD_DATA(0x00);
  LCD_DATA(0x00);
  LCD_DATA(0x01);
  LCD_DATA(0xE0);
//  LCD_DATA(0x8F);
  LCD_CMD(0x29); //display on 
  LCD_CMD(0x2C); //display on

  LCD_CMD(ILI9341_INVOFF); //Invert Off
  delay(120);
  LCD_CMD(ILI9341_SLPOUT);    //Exit Sleep
  delay(120);
  LCD_CMD(ILI9341_DISPON);    //Display on
  digitalWrite(LCD_CS, HIGH);
}
//***************************************************************************************************************************************
// Función para enviar comandos a la LCD - parámetro (comando)
//***************************************************************************************************************************************
void LCD_CMD(uint8_t cmd) {
  digitalWrite(LCD_RS, LOW);
  digitalWrite(LCD_WR, LOW);
  GPIO_PORTB_DATA_R = cmd;
  digitalWrite(LCD_WR, HIGH);
}
//***************************************************************************************************************************************
// Función para enviar datos a la LCD - parámetro (dato)
//***************************************************************************************************************************************
void LCD_DATA(uint8_t data) {
  digitalWrite(LCD_RS, HIGH);
  digitalWrite(LCD_WR, LOW);
  GPIO_PORTB_DATA_R = data;
  digitalWrite(LCD_WR, HIGH);
}
//***************************************************************************************************************************************
// Función para definir rango de direcciones de memoria con las cuales se trabajara (se define una ventana)
//***************************************************************************************************************************************
void SetWindows(unsigned int x1, unsigned int y1, unsigned int x2, unsigned int y2) {
  LCD_CMD(0x2a); // Set_column_address 4 parameters
  LCD_DATA(x1 >> 8);
  LCD_DATA(x1);   
  LCD_DATA(x2 >> 8);
  LCD_DATA(x2);   
  LCD_CMD(0x2b); // Set_page_address 4 parameters
  LCD_DATA(y1 >> 8);
  LCD_DATA(y1);   
  LCD_DATA(y2 >> 8);
  LCD_DATA(y2);   
  LCD_CMD(0x2c); // Write_memory_start
}
//***************************************************************************************************************************************
// Función para borrar la pantalla - parámetros (color)
//***************************************************************************************************************************************
void LCD_Clear(unsigned int c){  
  unsigned int x, y;
  LCD_CMD(0x02c); // write_memory_start
  digitalWrite(LCD_RS, HIGH);
  digitalWrite(LCD_CS, LOW);   
  SetWindows(0, 0, 319, 239); // 479, 319);
  for (x = 0; x < 320; x++)
    for (y = 0; y < 240; y++) {
      LCD_DATA(c >> 8); 
      LCD_DATA(c); 
    }
  digitalWrite(LCD_CS, HIGH);
} 
//***************************************************************************************************************************************
// Función para dibujar una línea horizontal - parámetros ( coordenada x, cordenada y, longitud, color)
//*************************************************************************************************************************************** 
void H_line(unsigned int x, unsigned int y, unsigned int l, unsigned int c) {  
  unsigned int i, j;
  LCD_CMD(0x02c); //write_memory_start
  digitalWrite(LCD_RS, HIGH);
  digitalWrite(LCD_CS, LOW);
  l = l + x;
  SetWindows(x, y, l, y);
  j = l;// * 2;
  for (i = 0; i < l; i++) {
      LCD_DATA(c >> 8); 
      LCD_DATA(c); 
  }
  digitalWrite(LCD_CS, HIGH);
}
//***************************************************************************************************************************************
// Función para dibujar una línea vertical - parámetros ( coordenada x, cordenada y, longitud, color)
//*************************************************************************************************************************************** 
void V_line(unsigned int x, unsigned int y, unsigned int l, unsigned int c) {  
  unsigned int i,j;
  LCD_CMD(0x02c); //write_memory_start
  digitalWrite(LCD_RS, HIGH);
  digitalWrite(LCD_CS, LOW);
  l = l + y;
  SetWindows(x, y, x, l);
  j = l; //* 2;
  for (i = 1; i <= j; i++) {
    LCD_DATA(c >> 8); 
    LCD_DATA(c);
  }
  digitalWrite(LCD_CS, HIGH);  
}
//***************************************************************************************************************************************
// Función para dibujar un rectángulo - parámetros ( coordenada x, cordenada y, ancho, alto, color)
//***************************************************************************************************************************************
void Rect(unsigned int x, unsigned int y, unsigned int w, unsigned int h, unsigned int c) {
  H_line(x  , y  , w, c);
  H_line(x  , y+h, w, c);
  V_line(x  , y  , h, c);
  V_line(x+w, y  , h, c);
}
//***************************************************************************************************************************************
// Función para dibujar un rectángulo relleno - parámetros ( coordenada x, cordenada y, ancho, alto, color)
//***************************************************************************************************************************************
/*void FillRect(unsigned int x, unsigned int y, unsigned int w, unsigned int h, unsigned int c) {
  unsigned int i;
  for (i = 0; i < h; i++) {
    H_line(x  , y  , w, c);
    H_line(x  , y+i, w, c);
  }
}
*/

void FillRect(unsigned int x, unsigned int y, unsigned int w, unsigned int h, unsigned int c) {
  LCD_CMD(0x02c); // write_memory_start
  digitalWrite(LCD_RS, HIGH);
  digitalWrite(LCD_CS, LOW); 
  
  unsigned int x2, y2;
  x2 = x+w;
  y2 = y+h;
  SetWindows(x, y, x2-1, y2-1);
  unsigned int k = w*h*2-1;
  unsigned int i, j;
  for (int i = 0; i < w; i++) {
    for (int j = 0; j < h; j++) {
      LCD_DATA(c >> 8);
      LCD_DATA(c);
      
      //LCD_DATA(bitmap[k]);    
      k = k - 2;
     } 
  }
  digitalWrite(LCD_CS, HIGH);
}
//***************************************************************************************************************************************
// Función para dibujar texto - parámetros ( texto, coordenada x, cordenada y, color, background) 
//***************************************************************************************************************************************
void LCD_Print(String text, int x, int y, int fontSize, int color, int background) {
  int fontXSize ;
  int fontYSize ;
  
  if(fontSize == 1){
    fontXSize = fontXSizeSmal ;
    fontYSize = fontYSizeSmal ;
  }
  if(fontSize == 2){
    fontXSize = fontXSizeBig ;
    fontYSize = fontYSizeBig ;
  }
  
  char charInput ;
  int cLength = text.length();
  Serial.println(cLength,DEC);
  int charDec ;
  int c ;
  int charHex ;
  char char_array[cLength+1];
  text.toCharArray(char_array, cLength+1) ;
  for (int i = 0; i < cLength ; i++) {
    charInput = char_array[i];
    Serial.println(char_array[i]);
    charDec = int(charInput);
    digitalWrite(LCD_CS, LOW);
    SetWindows(x + (i * fontXSize), y, x + (i * fontXSize) + fontXSize - 1, y + fontYSize );
    long charHex1 ;
    for ( int n = 0 ; n < fontYSize ; n++ ) {
      if (fontSize == 1){
        charHex1 = pgm_read_word_near(smallFont + ((charDec - 32) * fontYSize) + n);
      }
      if (fontSize == 2){
        charHex1 = pgm_read_word_near(bigFont + ((charDec - 32) * fontYSize) + n);
      }
      for (int t = 1; t < fontXSize + 1 ; t++) {
        if (( charHex1 & (1 << (fontXSize - t))) > 0 ) {
          c = color ;
        } else {
          c = background ;
        }
        LCD_DATA(c >> 8);
        LCD_DATA(c);
      }
    }
    digitalWrite(LCD_CS, HIGH);
  }
}
//***************************************************************************************************************************************
// Función para dibujar una imagen a partir de un arreglo de colores (Bitmap) Formato (Color 16bit R 5bits G 6bits B 5bits)
//***************************************************************************************************************************************
void LCD_Bitmap(unsigned int x, unsigned int y, unsigned int width, unsigned int height, unsigned char bitmap[]){  
  LCD_CMD(0x02c); // write_memory_start
  digitalWrite(LCD_RS, HIGH);
  digitalWrite(LCD_CS, LOW); 
  
  unsigned int x2, y2;
  x2 = x+width;
  y2 = y+height;
  SetWindows(x, y, x2-1, y2-1);
  unsigned int k = 0;
  unsigned int i, j;

  for (int i = 0; i < width; i++) {
    for (int j = 0; j < height; j++) {
      LCD_DATA(bitmap[k]);
      LCD_DATA(bitmap[k+1]);
      //LCD_DATA(bitmap[k]);    
      k = k + 2;
     } 
  }
  digitalWrite(LCD_CS, HIGH);
}
//***************************************************************************************************************************************
// Función para dibujar una imagen sprite - los parámetros columns = número de imagenes en el sprite, index = cual desplegar, flip = darle vuelta
//***************************************************************************************************************************************
void LCD_Sprite(int x, int y, int width, int height, unsigned char bitmap[],int columns, int index, char flip, char offset){
  LCD_CMD(0x02c); // write_memory_start
  digitalWrite(LCD_RS, HIGH);
  digitalWrite(LCD_CS, LOW); 

  unsigned int x2, y2;
  x2 =   x+width;
  y2=    y+height;
  SetWindows(x, y, x2-1, y2-1);
  int k = 0;
  int ancho = ((width*columns));
  if(flip){
  for (int j = 0; j < height; j++){
      k = (j*(ancho) + index*width -1 - offset)*2;
      k = k+width*2;
     for (int i = 0; i < width; i++){
      LCD_DATA(bitmap[k]);
      LCD_DATA(bitmap[k+1]);
      k = k - 2;
     } 
  }
  }else{
     for (int j = 0; j < height; j++){
      k = (j*(ancho) + index*width + 1 + offset)*2;
     for (int i = 0; i < width; i++){
      LCD_DATA(bitmap[k]);
      LCD_DATA(bitmap[k+1]);
      k = k + 2;
     } 
  }
    
    
    }
  digitalWrite(LCD_CS, HIGH);
}

int ascii2hex(int a){
  switch(a){
    case (48):
      return 0;
    case (49):
      return 1;
    case (50):
      return 2;
    case (51):
      return 3;
    case (52):
      return 4;
    case (53):
      return 5;
    case (54):
      return 6;
    case (55):
      return 7;
    case (56):
      return 8;
    case (57):
      return 9;
    case (97):
      return 10;
    case (98):
      return 11;
    case (99):
      return 12;
    case (100):
      return 13;
    case (101):
      return 14;
    case (102):
      return 15;
  }
}

void mapeo_SD(char doc[], int x, int y, int w){
  myFile = SD.open(doc, FILE_READ);
  int hex1 = 0;
  int val1 = 0;  
  int val2 = 0;
  int mapear = 0;
  int vertical = 0;
  unsigned char maps[w*2];

  if(myFile){
    while(myFile.available()){
      mapear = 0;
      while(mapear < w*2){
        hex1 = myFile.read();
        if(hex1 == 120){
          val1 = myFile.read();
          val2 = myFile.read();
          val1 = ascii2hex(val1);
          val2 = ascii2hex(val2);
          maps[mapear] = val1*16 + val2;
          mapear++;  
        }  
      } 
      LCD_Bitmap(x, vertical+y, w, 1, maps);
      vertical++; 
    }  
    myFile.close();
  }else{
    Serial.println("No se puedo abrir la imagen");
    myFile.close();  
  }
}
