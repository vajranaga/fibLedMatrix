#include <Arduino.h>
#include <stdio.h>
#include <Arduino_LED_Matrix.h>

#define ROWS 8
#define COLS 12

uint8_t matrix[ROWS][COLS] = {0};
uint8_t binary[2][8] = {0};
uint8_t inc = 1;

ArduinoLEDMatrix LED_Matrix;

void convertToBinByte(uint16_t num)
{
  for (int i = 0; i < 16; i++)
  {
    int bit = (num >> (15 - i)) & 1;
    binary[i / 8][i % 8] = bit;
  }
}

uint16_t fibSeries(uint16_t n)
{
  if (n <= 1)
    return n;
  else
  {
    uint32_t currFib = fibSeries(n - 1) + fibSeries(n - 2);
    if (currFib > 65535)
    {
      return 65535;
    }
  }
}
// debug prints
void printMatrix()
{
  for (uint8_t i = 0; i < ROWS; i++)
  {
    for (uint8_t j = 0; j < COLS; j++)
    {
      Serial.print(matrix[i][j] + " ");
    }
    Serial.print("\n");
  }
}
// debug prints
void printBinaryNum()
{
  for (uint8_t i = 0; i < 2; i++)
  {
    for (uint8_t j = 0; j < 8; j++)
    {
      Serial.print(binary[i][j] + " ");
    }
    Serial.print("\n");
  }
}

void copyBinaryToMatrixFront(uint16_t fibNum)
{
  if (fibNum > 0 && fibNum <= 255)
  {
    for (uint8_t i = 0; i < ROWS; i++)
    {
      matrix[i][0] = binary[1][i];
    }
    return;
  }
  else if (fibNum > 255)
  {
    for (uint8_t j = 0; j < 2; j++)
    {
      for (uint8_t i = 0; i < ROWS; i++)
      {
        matrix[i][j] = binary[j][i];
      }
    }
    return;
  }
}

void pushMatrixRight(uint16_t fibNum)
{
  uint8_t shiftAmt = (fibNum > 0 && fibNum <= 255) ? 1 : 2;
  uint8_t tempMatrix[ROWS][COLS] = {0};

  for (uint8_t i = 0; i < ROWS; i++)
  {
    for (uint8_t j = 0; j < COLS; j++)
    {
      if (j + shiftAmt < COLS)
      {
        tempMatrix[i][j + shiftAmt] = matrix[i][j];
      }
    }
  }
  for (uint8_t i = 0; i < ROWS; i++)
  {
    for (uint8_t j = 0; j < COLS; j++)
    {
      matrix[i][j] = tempMatrix[i][j];
    }
  }
}

void setup()
{
  Serial.begin(115200);
  Serial.println("Starting setup");
  Serial.println("Setting up LED Matrix");
  LED_Matrix.begin();
  LED_Matrix.loadPixels((uint8_t *)matrix, sizeof(matrix));
}

void loop()
{
  Serial.println("Starting loop");

  uint16_t currentFib = fibSeries(inc);
  if (currentFib >= 65535)
  {
    Serial.println("Fibonacci number is greater than 65535");
    // break;
  }
  Serial.print(inc);
  Serial.print(" Fibonacci number: ");
  Serial.println(currentFib);

  convertToBinByte(currentFib);

  // printBinaryNum();

  pushMatrixRight(currentFib);

  copyBinaryToMatrixFront(currentFib);

  // printMatrix();
  LED_Matrix.loadPixels((uint8_t *)matrix, sizeof(matrix));

  if (inc == 24)
  {
    inc = 1;
  }
  else
  {
    inc++;
  }

  delay(500);
  // Serial.println("Press any key to continue...");
  // while (Serial.available() == 0)
  // {
  //   // Wait for user input
  // }
  // Serial.read(); // Clear the input buffer
}
