#include <Arduino.h>
#include <SPI.h>
#include <ssd1351.h>


const int numChars = 16000;
char receivedChars[numChars];   // an array to store the received data
char e[1];

boolean newData = false;

typedef ssd1351::HighColor Color;

auto display = ssd1351::SSD1351<Color, ssd1351::SingleBuffer, 128, 128>();

void setup() {
  Serial.begin(9600);
  display.begin();
}

void loop() {
  recvWithEndMarker();
  showNewData();
}

void recvWithEndMarker() {
  static int ndx = 0;
  char endMarker = '\n';
  char rc;

  while (Serial.available() > 0 && newData == false) {
    rc = Serial.read();

    if (rc != endMarker) {
      receivedChars[ndx] = rc;
      ndx++;
      if (ndx >= numChars) {
        ndx = numChars - 1;
      }
    }
    else {
      receivedChars[ndx] = '\0'; // terminate the string
      ndx = 0;
      newData = true;
    }
  }

}

void showNewData() {
  Color color = ssd1351::RGB(0, 0, 0);

  int x = 0;
  int y = 0;

  for (int x = 0; x < 128; x++) {
    for (int y = 0; y < 128; y++) {
      display.drawPixel(x, y, color);
    }
  }

  if (newData == true) {
    char data[numChars]; // copy chars (num_size)
    for (int i = 0; i < numChars; i++) {
      data[i] = receivedChars[i];
      Serial.print(data[i]);
    }

    String str_total = ""; // total amount of pixels eg 20b -> 20
    int rle_total = 0;// convert to int

    for (int i = 0; i < numChars; i++) { // iterate through rle compressed data
      char current_char = data[i];

      if (isDigit(current_char)) { // handles digits
        str_total.concat(current_char);

      }

      else { // handles color string before digit
        rle_total = str_total.toInt();

        for (int pixel = 0; pixel < rle_total; pixel++) {
         
          if (current_char == 'b') {
            Color color = ssd1351::RGB(0,0,0);
            display.drawPixel(x, y, color); // draw pixel
          }
          else {
            //Color color = ssd1351::RGB(70, 70, 70); //                             BRIGHTNESS DETECTION
            Color color = ssd1351::RGB(60,60,60);
                        
            display.drawPixel(x, y, color); // draw pixel
          }

          if (x >= 127) {
            x = 0;
            y += 1;
          } else {
            x += 1;
          }
        }   

        rle_total = 0;
        str_total = "";
        
      }
      
    }
    display.updateScreen();

    newData = false;
  }
}
