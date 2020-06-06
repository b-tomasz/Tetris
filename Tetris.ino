/*
  Erste Tests mit einem 2D Array und einer Seriellen ausgabe
*/

//LED Matrix Libary einbinden
#include <RGBmatrixPanel.h>


//Pins für die LED Matrix
#define CLK A4 // USE THIS ON METRO M4 (not M0)
#define OE   9
#define LAT 10
#define A   A0
#define B   A1
#define C   A2
#define D   A3

//Test
#define buttonLeft 0
#define buttonRight 1
#define led 8
#define buttonDown 11
#define buttonTurn 12

RGBmatrixPanel matrix(A, B, C, D, CLK, LAT, OE, false, 64);


//Globale Variablen

bool newBlock = true;
int xLength = 16;
int yLength = 32;
int myNumbers[16][32];
int xBlock = xLength / 2;
int yBlock = 0;
int blockOrientation = 0;
int blockColor = 3;
int color[] = {matrix.Color333(0, 7, 0), matrix.Color333(0, 0, 7), matrix.Color333(7, 0, 0), matrix.Color333(7, 0, 7), matrix.Color333(0, 7, 7), matrix.Color333(7, 5, 0), matrix.Color333(7, 7, 7)};
int moveBlock = 0;



int block1[4][4][4] = {
  {
    {0, 1, 0, 0},
    {1, 1, 0, 0},
    {1, 0, 0, 0},
    {0, 0, 0, 0}
  },
  {
    {1, 1, 0, 0},
    {0, 1, 1, 0},
    {0, 0, 0, 0},
    {0, 0, 0, 0}
  },
  {
    {0, 1, 0, 0},
    {1, 1, 0, 0},
    {1, 0, 0, 0},
    {0, 0, 0, 0}
  },
  {
    {1, 1, 0, 0},
    {0, 1, 1, 0},
    {0, 0, 0, 0},
    {0, 0, 0, 0}
  },
};




int block2[4][4][4] = {
  {
    {1, 0, 0, 0},
    {1, 1, 0, 0},
    {0, 1, 0, 0},
    {0, 0, 0, 0}
  },
  {
    {0, 1, 1, 0},
    {1, 1, 0, 0},
    {0, 0, 0, 0},
    {0, 0, 0, 0}
  },
  {
    {1, 0, 0, 0},
    {1, 1, 0, 0},
    {0, 1, 0, 0},
    {0, 0, 0, 0}
  },
  {
    {0, 1, 1, 0},
    {1, 1, 0, 0},
    {0, 0, 0, 0},
    {0, 0, 0, 0}
  },
};




int block3[4][4][4] = {
  {
    {1, 0, 0, 0},
    {1, 0, 0, 0},
    {1, 1, 0, 0},
    {0, 0, 0, 0}
  },
  {
    {1, 1, 1, 0},
    {1, 0, 0, 0},
    {0, 0, 0, 0},
    {0, 0, 0, 0}
  },
  {
    {1, 1, 0, 0},
    {0, 1, 0, 0},
    {0, 1, 0, 0},
    {0, 0, 0, 0}
  },
  {
    {0, 0, 1, 0},
    {1, 1, 1, 0},
    {0, 0, 0, 0},
    {0, 0, 0, 0}
  },
};




int block4[4][4][4] = {
  {
    {0, 1, 0, 0},
    {0, 1, 0, 0},
    {1, 1, 0, 0},
    {0, 0, 0, 0}
  },
  {
    {1, 0, 0, 0},
    {1, 1, 1, 0},
    {0, 0, 0, 0},
    {0, 0, 0, 0}
  },
  {
    {1, 1, 0, 0},
    {1, 0, 0, 0},
    {1, 0, 0, 0},
    {0, 0, 0, 0}
  },
  {
    {1, 1, 1, 0},
    {0, 0, 1, 0},
    {0, 0, 0, 0},
    {0, 0, 0, 0}
  },
};




int block5[4][4][4] = {
  {
    {1, 1, 1, 0},
    {0, 1, 0, 0},
    {0, 0, 0, 0},
    {0, 0, 0, 0}
  },
  {
    {0, 1, 0, 0},
    {1, 1, 0, 0},
    {0, 1, 0, 0},
    {0, 0, 0, 0}
  },
  {
    {0, 1, 0, 0},
    {1, 1, 1, 0},
    {0, 0, 0, 0},
    {0, 0, 0, 0}
  },
  {
    {1, 0, 0, 0},
    {1, 1, 0, 0},
    {1, 0, 0, 0},
    {0, 0, 0, 0}
  },
};




int block6[4][4][4] = {
  {
    {1, 0, 0, 0},
    {1, 0, 0, 0},
    {1, 0, 0, 0},
    {1, 0, 0, 0}
  },
  {
    {1, 1, 1, 1},
    {0, 0, 0, 0},
    {0, 0, 0, 0},
    {0, 0, 0, 0}
  },
  {
    {1, 0, 0, 0},
    {1, 0, 0, 0},
    {1, 0, 0, 0},
    {1, 0, 0, 0}
  },
  {
    {1, 1, 1, 1},
    {0, 0, 0, 0},
    {0, 0, 0, 0},
    {0, 0, 0, 0}
  },
};




int block7[4][4][4] = {
  {
    {1, 1, 0, 0},
    {1, 1, 0, 0},
    {0, 0, 0, 0},
    {0, 0, 0, 0}
  },
  {
    {1, 1, 0, 0},
    {1, 1, 0, 0},
    {0, 0, 0, 0},
    {0, 0, 0, 0}
  },
  {
    {1, 1, 0, 0},
    {1, 1, 0, 0},
    {0, 0, 0, 0},
    {0, 0, 0, 0}
  },
  {
    {1, 1, 0, 0},
    {1, 1, 0, 0},
    {0, 0, 0, 0},
    {0, 0, 0, 0}
  },
};



unsigned long lastDebounceTime = 0;
unsigned long debounceDelay = 200;



// the setup function runs once when you press reset or power the board
void setup() {
  // initialize digital pin LED_BUILTIN as an output.
  Serial.begin(9600);

  pinMode(buttonLeft, INPUT_PULLUP);
  attachInterrupt(digitalPinToInterrupt(buttonLeft), interruptLeft, FALLING);


  pinMode(buttonRight, INPUT_PULLUP);
  attachInterrupt(digitalPinToInterrupt(buttonRight), interruptRight, FALLING);

  pinMode(buttonTurn, INPUT_PULLUP);
  attachInterrupt(digitalPinToInterrupt(buttonTurn), interruptRotate, FALLING);


  pinMode(buttonDown, INPUT_PULLUP);


  pinMode(led, OUTPUT);
  digitalWrite(led, HIGH);



  matrix.begin();





}

// the loop function runs over and over again forever
void loop() {


  checkMoveBlock();


  //Spielfeld über die Serielle schnittstelle ausgeben
  serialPrintSpielfeld();

  panelPrintSpielfeld();


  int stateButtonDown = digitalRead (buttonDown);
  if (stateButtonDown == 0) {
    delay(25);
  }
  else {
    delay(200);
  }
}




//------------------------  Methoden   ---------------------


void newblock() {//Einen Neuen Block setzen

  yBlock = 0;
  xBlock = xLength / 2;
  newBlock = true;
  blockColor = random(7);  // Eine Random Zahl zwischen 0-6
  checkLine();
  blockOrientation = 0;

}

void serialPrintSpielfeld() { //Spielfeld über die Serielle schnittstelle ausgeben

  for (byte i = 0; i < xLength; i = i + 1) {

    for (byte j = 0; j < yLength; j = j + 1) {
      Serial.print(myNumbers[i][j]);
      Serial.print(":");
    }
    Serial.println("");
  }

  Serial.println("-----");
  Serial.println(moveBlock);
}


void panelPrintSpielfeld() { //Spielfeld über die Serielle schnittstelle ausgeben

  matrix.fillScreen(matrix.Color333(0, 0, 0));

  for (byte i = 0; i < xLength; i = i + 1) {

    for (byte j = 0; j < yLength; j = j + 1) {

      if (myNumbers[i][j] == 1) {

        //matrix.drawPixel(i, j, matrix.Color333(7, 0, 0));

        matrix.fillRect(j * 2, i * 2, 2, 2, color[blockColor]);

      } else if (myNumbers[i][j] > 1) {
        matrix.fillRect(j * 2, i * 2, 2, 2, color[myNumbers[i][j] - 2]);
      }

    }

  }

}

void checkLine() {

  for (byte j = 0; j < yLength; j = j + 1) {
    int fullLine = 0;

    for (byte i = 0; i < xLength; i = i + 1) {
      if (myNumbers[i][j] > 0) {
        fullLine++;
        Serial.println(fullLine);
      }
    }

    if (fullLine >= xLength) {
      //Remove Line
      Serial.println("Clear Line");

      for (byte k = j; k > 0; k = k - 1) {

        for (byte i = 0; i < xLength; i = i + 1) {
          myNumbers[i][k] = myNumbers[i][k - 1];
        }




      }




    }

  }


}
void checkMoveBlock() {


  //überprüfen ob der Block über den Rand bewegt wird
  while (moveBlock != 0) {

  if(moveBlock < 0){
    if (!chekNextBlockPosition(2)) {
      moveBlock = 0;

    }else{

      xBlock--;
      moveBlock++;
    }

  }else if (moveBlock > 0){

    if (!chekNextBlockPosition(1)) {
      moveBlock = 0;
    }else {

      xBlock++;
      moveBlock--;
    }
  }

   
  }

  //überprüfen, ob der Block in der nächsten Position an einem anderen ankommt, oder am Boden
  if (yBlock + 1 >= yLength || myNumbers[xBlock][yBlock + 1] > 1 || !chekNextBlockPosition(0)) {
    Serial.println("Fix Block");


    //Aktuellen Block fix setzen
    for (byte i = 0; i < xLength; i = i + 1) {

      for (byte j = 0; j < yLength; j = j + 1) {

        if (myNumbers[i][j] == 1) {

          myNumbers[i][j] = blockColor + 2;


        }
      }

    }

    newblock();
  } else {

    //Aktuellen Block Löschen
    for (byte i = 0; i < xLength; i = i + 1) {

      for (byte j = 0; j < yLength; j = j + 1) {
        if (myNumbers[i][j] == 1) {

          myNumbers[i][j] = 0;
        }
      }

    }

    //Block um eine Position nach unten verschieben
    yBlock++;

    //Aktuellen Block an neuer Position setzen
    //myNumbers[xBlock][yBlock] = 1;


    for (byte i = 0; i < 4; i = i + 1) {

      for (byte j = 0; j < 4; j = j + 1) {

        if (block1[blockOrientation][i][j] == 1) {
          myNumbers[i + xBlock][j + yBlock] = block1[blockOrientation][i][j];
        }
      }

    }




  }


}

bool chekNextBlockPosition(int dir) { //int dir  0= down 1= left 2= right
  switch (dir) {
    case 0:   //down

      for (byte i = 0; i < 4; i = i + 1) {

        for (byte j = 0; j < 4; j = j + 1) {

          if (block1[blockOrientation][i][j] == 1) {
            if (myNumbers[i + xBlock][j + yBlock + 1] > 1 || j + yBlock + 1 >= yLength) {
              return false;
            }
          }
        }

      }



      break;
    case 1:   //left

      for (byte i = 0; i < 4; i = i + 1) {

        for (byte j = 0; j < 4; j = j + 1) {

          if (block1[blockOrientation][i][j] == 1) {
            if (myNumbers[i + xBlock][j + yBlock + 1] > 1 || i + xBlock + 1 >= xLength) {
              return false;
            }
          }
        }

      }


      break;
    case 2:   //right

      for (byte i = 0; i < 4; i = i + 1) {

        for (byte j = 0; j < 4; j = j + 1) {

          if (block1[blockOrientation][i][j] == 1) {
            if (myNumbers[i + xBlock][j + yBlock + 1] > 1 || i + xBlock - 1 < 0) {
              return false;
            }
          }
        }

      }


      break;
    default:
      return true;
      break; // Wird nicht benötigt, wenn Statement(s) vorhanden sind
  }
  return true;


}

void interruptLeft() {
  if (millis() - lastDebounceTime > debounceDelay) {

    moveBlock++;

    lastDebounceTime = millis();
  }

}

void interruptRight() {
  if (millis() - lastDebounceTime > debounceDelay) {

    moveBlock--;

    lastDebounceTime = millis();
  }

}

void interruptRotate() {
  if (millis() - lastDebounceTime > debounceDelay) {

    blockOrientation++;

    blockOrientation = blockOrientation % 4;

    lastDebounceTime = millis();
  }

}
