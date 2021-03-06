#include <Arduino.h>

//LED Matrix Libary einbinden
#include <RGBmatrixPanel.h>

//Libarys für Flash speicher
#include <SPI.h>
#include "SdFat.h"
#include "Adafruit_SPIFlash.h"

//Pins für die LED Matrix
#define CLK A4 // USE THIS ON METRO M4 (not M0)
#define OE 9
#define LAT 10
#define A A0
#define B A1
#define C A2
#define D A3

//Pins Definieren
#define buttonLeft 0
#define buttonRight 1
#define led 8
#define buttonDown 11
#define buttonTurn 12
#define analogRandomSeed A5

//grösse Spielfeld 1 = 2 Pixel
#define spielfeldX 10
#define spielfeldY 22

//Rand Spielfeld 1 = 1 Pixel
#define randLinks 6
#define randOben 20

//geschwindigkeit
#define speed 300
#define speedFast 25

RGBmatrixPanel matrix(A, B, C, D, CLK, LAT, OE, false, 64);

//Filesystem auf dem Falsh Speicher erstellen
#if defined(EXTERNAL_FLASH_USE_QSPI)
Adafruit_FlashTransport_QSPI flashTransport;

#elif defined(EXTERNAL_FLASH_USE_SPI)
Adafruit_FlashTransport_SPI flashTransport(EXTERNAL_FLASH_USE_CS, EXTERNAL_FLASH_USE_SPI);

#else
#error No QSPI/SPI flash are defined on your board variant.h !
#endif

Adafruit_SPIFlash flash(&flashTransport);

// file system object from SdFat
FatFileSystem fatfs;

File myFile;

//Globale Variablen

bool newBlock = true;
int xLength = spielfeldX;
int yLength = spielfeldY;
int myNumbers[spielfeldX][spielfeldY];
int xBlock;
int yBlock;
int blockOrientation;
int blockColor;
int nextBlockOrientation;
int nextBlockColor;
//int color[] = {matrix.Color333(0, 7, 0), matrix.Color333(0, 0, 7), matrix.Color333(7, 0, 0), matrix.Color333(7, 0, 7), matrix.Color333(0, 7, 7), matrix.Color333(7, 5, 0), matrix.Color333(7, 7, 7)};
int color[] = {matrix.Color333(0, 2, 0), matrix.Color333(0, 0, 2), matrix.Color333(2, 0, 0), matrix.Color333(2, 0, 2), matrix.Color333(0, 2, 2), matrix.Color333(2, 1, 0), matrix.Color333(2, 2, 2)};
int moveBlock = 0;
int rotateBlock = 0;
int score = 0;
int highScore = 0;
int actualSpeed = speed;

unsigned long lastDebounceTime = 0;
unsigned long debounceDelay = 75;
unsigned long debounceDelayTurn = 200;

//Methoden deklarieren
void newblock();
void gameOver();
void panelPrintSpielfeld();
void checkLine();
void checkMoveBlock();
void aktualisiereBlock();
bool chekNextBlockPosition(int dir);
void interruptLeft();
void interruptRight();
void interruptRotate();
int readHighScore();
void writeHighsSore(int highScore);
int getActiveBlock(int a, int b, int c, int color);

void setup()
{ // the setup function runs once when you press reset or power the board


  //Interrupt für alle Buttons initialisieren.
  pinMode(buttonLeft, INPUT_PULLUP);
  attachInterrupt(digitalPinToInterrupt(buttonLeft), interruptLeft, FALLING);

  pinMode(buttonRight, INPUT_PULLUP);
  attachInterrupt(digitalPinToInterrupt(buttonRight), interruptRight, FALLING);

  pinMode(buttonTurn, INPUT_PULLUP);
  attachInterrupt(digitalPinToInterrupt(buttonTurn), interruptRotate, FALLING);

  pinMode(buttonDown, INPUT_PULLUP);

  pinMode(led, OUTPUT);
  digitalWrite(led, HIGH);

  //Freien analogen Pin lesen, für einen besseren Zufall
  pinMode(analogRandomSeed, INPUT);
  randomSeed(analogRead(analogRandomSeed));

  // Init external flash
  flash.begin();

  // Init file system on the flash
  fatfs.begin(&flash);

  //Highscore reseten
  //fatfs.remove("score.txt");

  matrix.begin();
  matrix.setRotation(3);

  newblock();

  highScore = readHighScore();
}

void loop()
{ // the loop function runs over and over again forever

  checkMoveBlock();

  panelPrintSpielfeld();

  int stateButtonDown = digitalRead(buttonDown);
  if (stateButtonDown == 0)
  {
    delay(speedFast);
  }
  else
  {
    delay(actualSpeed);
  }
}

//------------------------  Methoden   ---------------------

void newblock() //Einen Neuen Block setzen
{

  yBlock = 0;
  xBlock = (xLength / 2) - 1;
  newBlock = true;
  blockColor = nextBlockColor;
  nextBlockColor = random(7); // Eine Random Zahl zwischen 0-6
  checkLine();
  blockOrientation = nextBlockOrientation;
  nextBlockOrientation = random(4);

  if (!chekNextBlockPosition(0))
  {
    gameOver();
  }
}

void gameOver() //Das Spielfeld zurücksetzen und neu starten
{


  if (score > highScore)
  {
    highScore = score;
    writeHighsSore(score);
  }
  
  

  actualSpeed = speed;

  for (byte i = 0; i < 46; i++)
  {
    matrix.fillScreen(matrix.Color333(0, 0, 0));
    matrix.setTextColor(matrix.Color333(3, 0, 0));

    matrix.setCursor(4, i);
    matrix.println("GAME");

    matrix.setCursor(4, i + 10);
    matrix.println("OVER");
    delay(2);

    matrix.setTextColor(matrix.Color333(7, 0, 0));

    matrix.setCursor(4, i);
    matrix.println("GAME");

    matrix.setCursor(4, i + 10);
    matrix.println("OVER");

    delay(40);

    matrix.setTextColor(matrix.Color333(3, 0, 0));

    matrix.setCursor(4, i);
    matrix.println("GAME");

    matrix.setCursor(4, i + 10);
    matrix.println("OVER");

    delay(2);
  }

  //Print Score

  matrix.setTextColor(matrix.Color333(0, 0, 3));

  matrix.setCursor(2, 2);
  matrix.println("Score");

  matrix.setCursor(2, 11);
  matrix.println(String(score));

  //Print Highscore

  matrix.setTextColor(matrix.Color333(0, 3, 0));

  matrix.setCursor(2, 20);
  matrix.println("Best");

  matrix.setCursor(2, 29);
  matrix.println(String(highScore));

  delay(10000);

  score = 0;

  for (byte i = 0; i < 5; i++)
  {
    matrix.fillScreen(matrix.Color333(1, 1, 1));
    delay(50);
    matrix.fillScreen(matrix.Color333(0, 0, 0));
    delay(50);
  }

  for (byte i = 0; i < xLength; i = i + 1)
  {
    for (byte j = 0; j < yLength; j = j + 1)
    {

      myNumbers[i][j] = 0;
    }
  }

  panelPrintSpielfeld();
}

void panelPrintSpielfeld() //Spielfeld auf das Panel übertragen
{

  matrix.fillScreen(matrix.Color333(0, 0, 0));

  for (byte i = 0; i < xLength; i = i + 1)
  {

    for (byte j = 0; j < yLength; j = j + 1)
    {

      if (myNumbers[i][j] == 1)
      {

        //matrix.drawPixel(i, j, matrix.Color333(7, 0, 0));

        matrix.fillRect((i * 2) + randLinks, (j * 2) + randOben, 2, 2, color[blockColor]);
      }
      else if (myNumbers[i][j] > 1)
      {
        matrix.fillRect((i * 2) + randLinks, (j * 2) + randOben, 2, 2, color[myNumbers[i][j] - 2]);
      }
    }
  }
  // Print next Block

  for (byte i = 0; i < 4; i = i + 1)
  {

    for (byte j = 0; j < 4; j = j + 1)
    {

      if (getActiveBlock(nextBlockOrientation, i, j, nextBlockColor) == 1)
      {
        matrix.fillRect((i * 2) + 14, (j * 2) + 11, 2, 2, color[nextBlockColor]);
      }
    }
  }

  //Print Score
  matrix.setTextColor(matrix.Color333(3, 0, 0));

  matrix.setCursor(2, 1);
  matrix.println(String(score));

  //Print Lines

  matrix.drawLine(randLinks - 1, randOben - 1, randLinks - 1, randOben - 1 + (yLength * 2), matrix.Color333(1, 1, 1));
  matrix.drawLine(randLinks + (xLength * 2), randOben - 1, randLinks + (xLength * 2), randOben - 1 + (yLength * 2), matrix.Color333(1, 1, 1));
  matrix.drawRect(0, 0, 32, 9, matrix.Color333(1, 1, 1));
}

void checkLine() // überprüfen, ob eine Zeile Voll ist und die Vollen Zeilen entfernen.
{

  for (byte j = 0; j < yLength; j = j + 1)
  {
    int fullLine = 0;

    for (byte i = 0; i < xLength; i = i + 1)
    {
      if (myNumbers[i][j] > 0)
      {
        fullLine++;
      }
    }

    if (fullLine >= xLength)
    {
      //Remove Line
      score++;
      actualSpeed -= 5;

      for (byte k = j; k > 0; k = k - 1)
      {

        for (byte i = 0; i < xLength; i = i + 1)
        {
          myNumbers[i][k] = myNumbers[i][k - 1];
        }
      }
    }
  }
}

void checkMoveBlock() //überprüfen, ob der Block nach links oder rechts beget werden soll, und ob dies möglich ist.
{

  //überprüfen ob der Block über den Rand bewegt wird
  while (moveBlock != 0)
  {

    if (moveBlock < 0)
    {
      if (!chekNextBlockPosition(2))
      {
        moveBlock = 0;
      }
      else
      {

        xBlock--;
        moveBlock++;
      }
    }
    else if (moveBlock > 0)
    {

      if (!chekNextBlockPosition(1))
      {
        moveBlock = 0;
      }
      else
      {

        xBlock++;
        moveBlock--;
      }
    }
  }

  while (rotateBlock > 0)
  {
    if (!chekNextBlockPosition(3))
    {
      rotateBlock = 0;
    }
    else
    {
      blockOrientation++;
      blockOrientation = blockOrientation % 4;
      rotateBlock--;
    }
  }

  aktualisiereBlock();

  //überprüfen, ob der Block in der nächsten Position an einem anderen ankommt, oder am Boden
  if (!chekNextBlockPosition(0))
  {
    //Aktuellen Block fix setzen
    for (byte i = 0; i < xLength; i = i + 1)
    {

      for (byte j = 0; j < yLength; j = j + 1)
      {

        if (myNumbers[i][j] == 1)
        {

          myNumbers[i][j] = blockColor + 2;
        }
      }
    }

    newblock();
  }
  else
  {

    //Block um eine Position nach unten verschieben
    yBlock++;

    aktualisiereBlock();
  }
}

void aktualisiereBlock() // Block auf die aktuelle Position setzen
{
  //Aktuellen Block Löschen
  for (byte i = 0; i < xLength; i = i + 1)
  {

    for (byte j = 0; j < yLength; j = j + 1)
    {
      if (myNumbers[i][j] == 1)
      {

        myNumbers[i][j] = 0;
      }
    }
  }

  //Aktuellen Block an neuer Position setzen
  //myNumbers[xBlock][yBlock] = 1;

  for (byte i = 0; i < 4; i = i + 1)
  {

    for (byte j = 0; j < 4; j = j + 1)
    {

      if (getActiveBlock(blockOrientation, i, j, blockColor) == 1)
      {
        myNumbers[i + xBlock][j + yBlock] = getActiveBlock(blockOrientation, i, j, blockColor);
      }
    }
  }
}

bool chekNextBlockPosition(int dir) //int dir  0= down 1= left 2= right 3= rotate -> false = block an nächster position
{
  switch (dir)
  {
  case 0: //down

    for (byte i = 0; i < 4; i = i + 1)
    {

      for (byte j = 0; j < 4; j = j + 1)
      {

        if (getActiveBlock(blockOrientation, i, j, blockColor) == 1)
        {
          if (myNumbers[i + xBlock][j + yBlock + 1] > 1 || j + yBlock + 1 >= yLength)
          {
            return false;
          }
        }
      }
    }

    break;
  case 1: //left

    for (byte i = 0; i < 4; i = i + 1)
    {

      for (byte j = 0; j < 4; j = j + 1)
      {

        if (getActiveBlock(blockOrientation, i, j, blockColor) == 1)
        {
          if (myNumbers[i + xBlock + 1][j + yBlock] > 1 || i + xBlock + 1 >= xLength)
          {
            return false;
          }
        }
      }
    }

    break;
  case 2: //right

    for (byte i = 0; i < 4; i = i + 1)
    {

      for (byte j = 0; j < 4; j = j + 1)
      {

        if (getActiveBlock(blockOrientation, i, j, blockColor) == 1)
        {
          if (myNumbers[i + xBlock - 1][j + yBlock] > 1 || i + xBlock - 1 < 0)
          {
            return false;
          }
        }
      }
    }

    break;

  case 3: //rotate

    for (byte i = 0; i < 4; i = i + 1)
    {

      for (byte j = 0; j < 4; j = j + 1)
      {

        if (getActiveBlock((blockOrientation + 1) % 4, i, j, blockColor) == 1)
        {
          if (myNumbers[i + xBlock][j + yBlock] > 1 || i + xBlock >= xLength || i + xBlock < 0 || j + yBlock >= yLength)
          {
            return false;
          }
        }
      }
    }

    break;

    break;
  default:
    return true;
    break; // Wird nicht benötigt, wenn Statement(s) vorhanden sind
  }
  return true;
}

void interruptLeft() // Interrupt methode für button left
{
  if (millis() - lastDebounceTime > debounceDelay)
  {

    moveBlock++;

    lastDebounceTime = millis();
  }
}

void interruptRight() // Interrupt methode für button right
{
  if (millis() - lastDebounceTime > debounceDelay)
  {

    moveBlock--;

    lastDebounceTime = millis();
  }
}

void interruptRotate() // Interrupt methode für button rotate
{
  if (millis() - lastDebounceTime > debounceDelayTurn)
  {

    rotateBlock++;

    lastDebounceTime = millis();
  }
}

int readHighScore() //Higscore vom Flash auslesen
{
  myFile = fatfs.open("score.txt");
  if (myFile)
  {
    String scoreRead;
    // read from the file until there's nothing else in it:
   
    scoreRead=myFile.readStringUntil('\n');
    // close the file:
    myFile.close();

    return scoreRead.toInt();
  }
  else
  {
    
    return 0;
  }
}

void writeHighsSore(int highScore) //Highscore in den Flash schreiben
{
  fatfs.remove("score.txt");
  myFile = fatfs.open("score.txt", FILE_WRITE);

  // if the file opened okay, write to it:
  if (myFile)
  {
    myFile.println(String(highScore));
    // close the file:
    myFile.close();
  }
 
}

int getActiveBlock(int a, int b, int c, int color) // Gibt vom aktuellen Block die aktuelle Position zurück
{
  int block1[4][4][4] = {
      {{0, 1, 0, 0},
       {1, 1, 0, 0},
       {1, 0, 0, 0},
       {0, 0, 0, 0}},
      {{1, 1, 0, 0},
       {0, 1, 1, 0},
       {0, 0, 0, 0},
       {0, 0, 0, 0}},
      {{0, 1, 0, 0},
       {1, 1, 0, 0},
       {1, 0, 0, 0},
       {0, 0, 0, 0}},
      {{1, 1, 0, 0},
       {0, 1, 1, 0},
       {0, 0, 0, 0},
       {0, 0, 0, 0}},
  };

  int block2[4][4][4] = {
      {{1, 0, 0, 0},
       {1, 1, 0, 0},
       {0, 1, 0, 0},
       {0, 0, 0, 0}},
      {{0, 1, 1, 0},
       {1, 1, 0, 0},
       {0, 0, 0, 0},
       {0, 0, 0, 0}},
      {{1, 0, 0, 0},
       {1, 1, 0, 0},
       {0, 1, 0, 0},
       {0, 0, 0, 0}},
      {{0, 1, 1, 0},
       {1, 1, 0, 0},
       {0, 0, 0, 0},
       {0, 0, 0, 0}},
  };

  int block3[4][4][4] = {
      {{1, 0, 0, 0},
       {1, 0, 0, 0},
       {1, 1, 0, 0},
       {0, 0, 0, 0}},
      {{0, 0, 1, 0},
       {1, 1, 1, 0},
       {0, 0, 0, 0},
       {0, 0, 0, 0}},
      {{1, 1, 0, 0},
       {0, 1, 0, 0},
       {0, 1, 0, 0},
       {0, 0, 0, 0}},
      {{1, 1, 1, 0},
       {1, 0, 0, 0},
       {0, 0, 0, 0},
       {0, 0, 0, 0}},
  };

  int block4[4][4][4] = {
      {{0, 1, 0, 0},
       {0, 1, 0, 0},
       {1, 1, 0, 0},
       {0, 0, 0, 0}},
      {{1, 1, 1, 0},
       {0, 0, 1, 0},
       {0, 0, 0, 0},
       {0, 0, 0, 0}},
      {{1, 1, 0, 0},
       {1, 0, 0, 0},
       {1, 0, 0, 0},
       {0, 0, 0, 0}},
      {{1, 0, 0, 0},
       {1, 1, 1, 0},
       {0, 0, 0, 0},
       {0, 0, 0, 0}},
  };

  int block5[4][4][4] = {
      {{1, 1, 1, 0},
       {0, 1, 0, 0},
       {0, 0, 0, 0},
       {0, 0, 0, 0}},
      {{1, 0, 0, 0},
       {1, 1, 0, 0},
       {1, 0, 0, 0},
       {0, 0, 0, 0}},
      {{0, 1, 0, 0},
       {1, 1, 1, 0},
       {0, 0, 0, 0},
       {0, 0, 0, 0}},
      {{0, 1, 0, 0},
       {1, 1, 0, 0},
       {0, 1, 0, 0},
       {0, 0, 0, 0}},
  };

  int block6[4][4][4] = {
      {{0, 1, 0, 0},
       {0, 1, 0, 0},
       {0, 1, 0, 0},
       {0, 1, 0, 0}},
      {{0, 0, 0, 0},
       {1, 1, 1, 1},
       {0, 0, 0, 0},
       {0, 0, 0, 0}},
      {{0, 1, 0, 0},
       {0, 1, 0, 0},
       {0, 1, 0, 0},
       {0, 1, 0, 0}},
      {{0, 0, 0, 0},
       {1, 1, 1, 1},
       {0, 0, 0, 0},
       {0, 0, 0, 0}},
  };

  int block7[4][4][4] = {
      {{1, 1, 0, 0},
       {1, 1, 0, 0},
       {0, 0, 0, 0},
       {0, 0, 0, 0}},
      {{1, 1, 0, 0},
       {1, 1, 0, 0},
       {0, 0, 0, 0},
       {0, 0, 0, 0}},
      {{1, 1, 0, 0},
       {1, 1, 0, 0},
       {0, 0, 0, 0},
       {0, 0, 0, 0}},
      {{1, 1, 0, 0},
       {1, 1, 0, 0},
       {0, 0, 0, 0},
       {0, 0, 0, 0}},
  };

  switch (color)
  {
  case 0:
    return block1[a][b][c];
    break;
  case 1:
    return block2[a][b][c];
    break;
  case 2:
    return block3[a][b][c];
    break;
  case 3:
    return block4[a][b][c];
    break;
  case 4:
    return block5[a][b][c];
    break;
  case 5:
    return block6[a][b][c];
    break;
  case 6:
    return block7[a][b][c];
    break;
  default:
    return 0;
    break; // Wird nicht benötigt, wenn Statement(s) vorhanden sind
  }
}