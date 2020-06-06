/*
  Erste Tests mit einem 2D Array und einer Seriellen ausgabe
*/


int myNumbers[10][15];
bool newBlock = true;
int xLength = 10;
int yLength = 15;
int xBlock = 5;
int yBlock = 0;
int blockColor = 3;



// the setup function runs once when you press reset or power the board
void setup() {
  // initialize digital pin LED_BUILTIN as an output.
  Serial.begin(9600);

}

// the loop function runs over and over again forever
void loop() {

  /* if (newBlock == true) {
     myNumbers[xBlock][yBlock] = 1;
    }

  */

  //überprüfen, ob der Block in der nächsten Position an einem anderen ankommt, oder am Boden
  if (yBlock + 1 >= yLength || myNumbers[xBlock][yBlock+1] > 1) {
    Serial.println("Fix Block");

    
    //Aktuellen Block fix setzen
    for (byte i = 0; i < xLength; i = i + 1) {

      for (byte j = 0; j < yLength; j = j + 1) {

        if (myNumbers[i][j] == 1) {

          myNumbers[i][j] = blockColor;


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
    myNumbers[xBlock][yBlock] = 1;
  }















  for (byte i = 0; i < xLength; i = i + 1) {

    for (byte j = 0; j < yLength; j = j + 1) {
      Serial.print(myNumbers[i][j]);
      Serial.print(":");
    }
    Serial.println("");
  }





  Serial.println("-----");

  delay(1000);
}



void newblock() {

  blockColor++;

  yBlock = 0;
  newBlock = true;


}
