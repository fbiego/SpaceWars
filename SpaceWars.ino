#include <Arduino_GFX_Library.h>
#include <Tone32.h>
#include <Ps3Controller.h>
#include "rocket.h"
#include "brod1.h"
#include "bulet.h"
#include "ebullet.h"
#include "life.h"
#include "rover.h"
#include "earth.h"
#include "ex.h"
#include "ex2.h"
#include "ricon.h"
#include "back2.h"
#include "sens.h"
#include "buum.h"
#include "gameOver.h"

#define BUZZER_PIN 27
#define BUZZER_CHANNEL 0

#define GREY 0x5AEB
#define lightblue 0x2D18
#define orange 0xFB60
#define purple 0xFB9B

Arduino_DataBus *bus = new Arduino_HWSPI(2 /* DC */, 15 /* CS */);
Arduino_GFX *gfx = new Arduino_GC9A01(bus, 4, 0 /* rotation */, true /* IPS */);

int brojac = 0; // Invoke custom library
float buletX[10] = { -20, -20, -20, -20, -20, -20, -20, -20, -20, -20};
float buletY[10] = { -20, -20, -20, -20, -20, -20, -20, -20, -20, -20};

float EbuletX[10] = { -20, -20, -20, -20, -20, -20, -20, -20, -20, -20};
float EbuletY[10] = { -20, -20, -20, -20, -20, -20, -20, -20, -20, -20};

float rocketX[10] = { -20, -20, -20, -20, -20, -20, -20, -20, -20, -20};
float rocketY[10] = { -20, -20, -20, -20, -20, -20, -20, -20, -20, -20};
float rocketSpeed = 0.22;
int rockets = 3;

int counter = 0;
int rcounter = 0;
int Ecounter = 0;
int level = 1;
float x = 10;
float y = 20;

float ey = 18;
float ex = 170;

float es = 0.1;

float bx = -50;
float by = 0;

int pom = 0; //pressdebounce for fire
int pom2 = 0; //pressdebounce for rockets
float sped = 0.42;
int blinkTime = 0;
int eHealth = 50;
int mHealth = eHealth;
int lives = 4;
int ly[4] = {0, 0, 0, 0};
int ri[3] = {0, 0, 0};
int fireTime = 100;
int fireCount = 0;
float EbulletSpeed = 0.42;
int rDamage = 8; //rocket damage
int tr = 0;

int pom3 = 0;
bool sound = 1; //sound on or off

int fase = 0; //fase 0=start screen,//fase 1=playing fase //fase 3=game over

float spaceX[30];
float spaceY[30];

uint8_t mac[] = {1, 2, 3, 4, 5, 6};
int st = 0;

void notify() {
  st = Ps3.data.button.circle;
}

void setup() {
  Serial.begin(115200);
  Ps3.attach(notify);
  //Ps3.attachOnConnect(onConnect);
  Ps3.begin(mac);
  gfx->begin();
  gfx->fillScreen(BLACK);

  gfx->draw16bitRGBBitmap(0, 50, back2, 240, 135);


  for (int i = 0; i < 30; i++) {
    spaceX[i] = random(5, 235);
    spaceY[i] = random(68, 182);
  }

  while (st == 0) { // wait until button a is pressed.............
    Serial.print("");
  }

}

void restart() {
  counter = 0;
  rcounter = 0;
  Ecounter = 0;
  level = 1;
  x = 10;
  y = 20;
  ey = 18;
  ex = 170;
  es = 0.1;
  bx = -50;
  by = 0;

  rockets = 3;
  rDamage = 8;
  lives = 4;
  brojac = 0;
  ri[0] = 0; ri[1] = 0; ri[2] = 0;
  ly[0] = 0; ly[1] = 0; ly[2] = 0; ly[3] = 0;
  ey = 44;
  sped = 0.42;
  eHealth = 50;
  mHealth = eHealth;
  EbulletSpeed = 0.42;
  rocketSpeed = 0.22;


  for (int i = 0; i < 10; i++) {
    buletX[i] = -20;
    EbuletX[i] = -20;
    rocketX[i] = -20;

  }


}


void newLevel() {
  level++;
  sped = sped + 0.05;
  EbulletSpeed = EbulletSpeed + 0.1;
  eHealth = 50 + (level * 5);
  mHealth = eHealth;
  es = 0.05 + (0.035 * level);

  rockets = 3;
  rDamage = 8 + (level * 2);
  rocketSpeed = 0.22 + (level * 0.02);
  ri[0] = 0; ri[1] = 0; ri[2] = 0;

  ey = 44;

  for (int i = 0; i < 10; i++) {
    buletX[i] = -20;
    EbuletX[i] = -20;
    rocketX[i] = -20;

  }
  draw();

}

void draw() {
  gfx->fillScreen(BLACK);
  gfx->setCursor(30, 72);
  gfx->setTextSize(2);
  gfx->print("Level " + String(level));
  gfx->setCursor(0, 92);
  gfx->setTextSize(1);
  gfx->println("   Enemy speed : " + String(es));
  gfx->println("   Enemy health : " + String(eHealth));
  gfx->println("   Enemy bullet speed : " + String(EbulletSpeed));
  gfx->println("   Remaining lives: " + String(lives));
  gfx->println("   My speed : " + String(sped));
  gfx->println("   Rocket damage : " + String(rDamage));
  gfx->println("   Rocket speed : " + String(rocketSpeed));

  gfx->draw16bitRGBBitmap(170, 55, earth[level - 1],  55, 54);
  gfx->draw16bitRGBBitmap(170, 111, sens,  72, 72);
  delay(1000);


  while (st == 0) { // wait until button a is pressed.............
    Serial.print("");
  }
  gfx->fillScreen(BLACK);


  gfx->drawLine(0, 66, 240, 66, lightblue);
  gfx->drawLine(0, 184, 240, 184, lightblue);

  gfx->setCursor(200, 50);
  gfx->print(brojac);

  gfx->fillRect(120, 53, 70, 57, GREEN);
  gfx->drawRect(119, 52, 72, 59, GREY);
}



void loop() {

  if (fase == 0) {
    restart();
    gfx->fillScreen(BLACK);

    gfx->draw16bitRGBBitmap(0, 50, back2,  240, 135);
    while (st == 0) { // wait until button a is pressed.............
      Serial.print("");
    }
    draw();

    fase = 1;
  }

  if (fase == 1) { //playing fase
    if (Ps3.data.button.down == 1 and y < 94) { //Move down
      y = y + sped;
    }
    if (Ps3.data.button.up == 1 and y > 18) { //Move up
      y = y - sped;
    }
    if (Ps3.data.button.right == 1 and x < 125) { //Move right
      x = x + sped;
    }
    if (Ps3.data.button.left == 1 and  x > 0) { //Move left
      x = x - sped;
    }

    if (Ps3.data.button.circle == 1 ) { //fire button A button
      if (pom == 0) {
        pom = 1;

        buletX[counter] = x + 34;
        buletY[counter] = y + 15;
        counter = counter + 1;
      }
    } else {
      pom = 0;
    }


    if (Ps3.data.button.cross == 1 && rockets > 0) { //Rocket button B button
      if (pom2 == 0) {
        pom2 = 1;
        rockets--;
        rocketX[rcounter] = x + 34;
        rocketY[rcounter] = y + 14;
        rcounter = rcounter + 1;
        ri[rockets] = -100;
        gfx->fillRect(70 + (rockets * 14), 50, 8, 14, BLACK);
      }
    } else {
      pom2 = 0;
    }
    if (Ps3.data.button.triangle == 1) { //buton 35 , on and off sound
      if (pom3 == 0) {
        pom3 = 1;
        sound = !sound;

      }
    } else {
      pom3 = 0;
    }
    for (int i = 0; i < 30; i++) { //drawStars..........................................
      gfx->drawPixel(spaceX[i], spaceY[i], BLACK);
      spaceX[i] = spaceX[i] - 0.5;
      gfx->drawPixel(spaceX[i], spaceY[i], GREY);
      if (spaceX[i] < 0) {
        gfx->drawPixel(spaceX[i], spaceY[i], BLACK);

        spaceX[i] = 244;
      }
    }



    gfx->draw16bitRGBBitmap(x, y+50, brod1,  49, 40);
    gfx->draw16bitRGBBitmap(ex, ey+50, earth[level - 1],  55, 54);

    for (int i = 0; i < 10; i++) { //firing buletts
      if (buletX[i] > 0) {
        gfx->draw16bitRGBBitmap(buletX[i], buletY[i]+50, bulet,  8, 8);
        buletX[i] = buletX[i] + 0.6;
      }
      if (buletX[i] > 240) {
        buletX[i] = -30;
      }
    }

    for (int i = 0; i < 10; i++) { //firing rockets
      if (rocketX[i] > 0) {
        gfx->draw16bitRGBBitmap(rocketX[i], rocketY[i]+50, rocket,  24, 12);
        rocketX[i] = rocketX[i] + rocketSpeed;
      }
      if (rocketX[i] > 240) {
        rocketX[i] = -30;
      }
    }

    //delay(1);


    for (int j = 0; j < 10; j++) { //did my bulet hit enemy
      if (buletX[j] > ex + 20 && buletY[j] > ey + 2 && buletY[j] < ey + 52 ) {
        gfx->draw16bitRGBBitmap(buletX[j], buletY[j]+50, ex2,  12, 12);
        if (sound == 1) {
          tone(BUZZER_PIN, NOTE_C5, 12, BUZZER_CHANNEL);
          noTone(BUZZER_PIN, BUZZER_CHANNEL);
        } else {
          delay(12);
        }
        gfx->fillRect(buletX[j], buletY[j]+50, 12, 12, BLACK);
        buletX[j] = -50;
        brojac = brojac + 1;
        gfx->setCursor(200, 0);
        gfx->print(brojac);
        eHealth--;
        tr = map(eHealth, 0, mHealth, 0, 70);
        gfx->fillRect(120, 53, 70, 57, BLACK);
        gfx->fillRect(120, 53, tr, 57, GREEN);

        if (eHealth <= 0) {
          gfx->draw16bitRGBBitmap(ex, ey+50, buum,  55, 55);
          tone(BUZZER_PIN, NOTE_E4, 100, BUZZER_CHANNEL);
          tone(BUZZER_PIN, NOTE_D4, 80, BUZZER_CHANNEL);
          tone(BUZZER_PIN, NOTE_G5, 100, BUZZER_CHANNEL);
          tone(BUZZER_PIN, NOTE_C3, 80, BUZZER_CHANNEL);
          tone(BUZZER_PIN, NOTE_F4, 280, BUZZER_CHANNEL);
          noTone(BUZZER_PIN, BUZZER_CHANNEL);
          delay(700);
          newLevel();
        }
        //digitalWrite(25, 1);
        blinkTime = 1;



      }

    }


    for (int j = 0; j < 10; j++) { //did my ROCKET hit enemy
      if (rocketX[j] + 18 > ex && rocketY[j] > ey + 2 && rocketY[j] < ey + 52 ) {
        gfx->draw16bitRGBBitmap(rocketX[j], rocketY[j]+50, explosion,  24, 24);
        if (sound == 1) {
          tone(BUZZER_PIN, NOTE_C3, 40, BUZZER_CHANNEL);
          noTone(BUZZER_PIN, BUZZER_CHANNEL);
        } else {
          delay(40);
        }
        gfx->fillRect(rocketX[j], rocketY[j], 24, 24, BLACK);
        //delay(30);

        rocketX[j] = -50;
        brojac = brojac + 12;
        gfx->setCursor(200, 0);
        gfx->print(brojac);
        eHealth = eHealth - rDamage;
        tr = map(eHealth, 0, mHealth, 0, 70);
        gfx->fillRect(120, 3, 70, 7, BLACK);
        gfx->fillRect(120, 3, tr, 7, GREEN);

        if (eHealth <= 0) {
          gfx->draw16bitRGBBitmap(ex, ey, buum,  55, 55);
          tone(BUZZER_PIN, NOTE_E4, 100, BUZZER_CHANNEL);
          tone(BUZZER_PIN, NOTE_D4, 80, BUZZER_CHANNEL);
          tone(BUZZER_PIN, NOTE_G5, 100, BUZZER_CHANNEL);
          tone(BUZZER_PIN, NOTE_C3, 80, BUZZER_CHANNEL);
          tone(BUZZER_PIN, NOTE_F4, 280, BUZZER_CHANNEL);
          noTone(BUZZER_PIN, BUZZER_CHANNEL);
          delay(700);
          newLevel();
        }
        //digitalWrite(25, 0);
        blinkTime = 1;


      }

    }


    for (int j = 0; j < 10; j++) { //Am I hit
      if (EbuletX[j] < x + 30 && EbuletX[j] > x + 4 && EbuletY[j] > y + 4 && EbuletY[j] < y + 36 ) {
        EbuletX[j] = -50;
        ly[lives - 1] = -40;
        gfx->fillRect((lives - 1) * 14, 0, 14, 14, BLACK);
        lives--;
        if (lives == 0) {
          gfx->draw16bitRGBBitmap(x, y, buum,  55, 55);
          tone(BUZZER_PIN, NOTE_G4, 100, BUZZER_CHANNEL);
          tone(BUZZER_PIN, NOTE_B4, 80, BUZZER_CHANNEL);
          tone(BUZZER_PIN, NOTE_C5, 100, BUZZER_CHANNEL);
          tone(BUZZER_PIN, NOTE_A4, 80, BUZZER_CHANNEL);
          tone(BUZZER_PIN, NOTE_F4, 280, BUZZER_CHANNEL);
          noTone(BUZZER_PIN, BUZZER_CHANNEL);
          delay(500);
          gfx->fillScreen(BLACK);
          fase = 2;
        }


        //digitalWrite(33, 1);
        blinkTime = 1;
        if (sound == 1) {
          tone(BUZZER_PIN, NOTE_C6, 4, BUZZER_CHANNEL);
          noTone(BUZZER_PIN, BUZZER_CHANNEL);
        } else {
          delay(4);
        }

      }

    }

    ey = ey + es;
    if (ey > 80) {
      es = es * -1;
    }
    if (ey < 18) {
      es = es * -1;
    }
    if (blinkTime > 0) {
      blinkTime++;
    }
    if (blinkTime > 2) {
      //digitalWrite(25, 0);
      //digitalWrite(33, 0);
      blinkTime = 0;
    }

    for (int i = 0; i < 10; i++) { //enemy shoots
      if (EbuletX[i] > -10) {
        gfx->draw16bitRGBBitmap(EbuletX[i], EbuletY[i], ebullet,  7, 7);
        EbuletX[i] = EbuletX[i] - EbulletSpeed;
      }

    }

    for (int i = 0; i < 4; i++) { //draw lifes
      gfx->draw16bitRGBBitmap(i * 14, ly[i], life,  12, 11);
    }
    for (int i = 0; i < 3; i++) { //draw lifes
      gfx->draw16bitRGBBitmap(70 + (i * 14), ri[i], ricon ,  8, 14);
    }

    fireCount++;
    if (fireTime == fireCount) {
      EbuletX[Ecounter] = ex + 5;
      EbuletY[Ecounter] = ey + 24;
      fireCount = 0;
      fireTime = random(110 - (level * 15), 360 - (level * 30));
      Ecounter++;
    }
    if (counter == 9) {
      counter = 0;
    }
    if (rcounter == 3) {
      rcounter = 0;
    }

    if (Ecounter == 9) {
      Ecounter = 0;
    }
  }
  if (fase == 2) { //game over fase


    gfx->fillScreen(BLACK);
    gfx->draw16bitRGBBitmap(0, 0, gameOver,  240, 135);
    gfx->setCursor(24, 54);
    gfx->print("Score : " + String(brojac));
    gfx->setCursor(24, 69);
    gfx->print("Level : " + String(level));
    while (st == 0) { // wait until button a is pressed.............
      Serial.print("");
    }
    fase = 0;
  }
}
