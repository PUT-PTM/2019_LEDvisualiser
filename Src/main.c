
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
  ******************************************************************************
  ** This notice applies to any and all portions of this file
  * that are not between comment pairs USER CODE BEGIN and
  * USER CODE END. Other portions of this file, whether 
  * inserted by the user or by software development tools
  * are owned by their respective copyright owners.
  *
  * COPYRIGHT(c) 2019 STMicroelectronics
  *
  * Redistribution and use in source and binary forms, with or without modification,
  * are permitted provided that the following conditions are met:
  *   1. Redistributions of source code must retain the above copyright notice,
  *      this list of conditions and the following disclaimer.
  *   2. Redistributions in binary form must reproduce the above copyright notice,
  *      this list of conditions and the following disclaimer in the documentation
  *      and/or other materials provided with the distribution.
  *   3. Neither the name of STMicroelectronics nor the names of its contributors
  *      may be used to endorse or promote products derived from this software
  *      without specific prior written permission.
  *
  * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
  * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
  * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
  * DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE
  * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
  * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
  * SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
  * CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
  * OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
  * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
  *
  ******************************************************************************
  */
/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "stm32f4xx_hal.h"
#include <math.h>
/* USER CODE BEGIN Includes */
#include "max_matrix_stm32.h"
/* USER CODE END Includes */

/* Private variables ---------------------------------------------------------*/

/* USER CODE BEGIN PV */
/* Private variables ---------------------------------------------------------*/
uint8_t buffer[80];
uint8_t buffer_row[80];
uint8_t CH[] = {
	0, 0, 0, 0, 0, 0, 0, 0,  // 0 - 'Empty Cell'
  5, 0x3e, 0x5b, 0x4f, 0x5b, 0x3e, 0, 0,  // 1 - 'Sad Smiley'
  5, 0x3e, 0x6b, 0x4f, 0x6b, 0x3e, 0, 0,  // 2 - 'Happy Smiley'
  5, 0x1c, 0x3e, 0x7c, 0x3e, 0x1c, 0, 0,  // 3 - 'Heart'
  5, 0x18, 0x3c, 0x7e, 0x3c, 0x18, 0, 0,  // 4 - 'Diamond'
  5, 0x1c, 0x57, 0x7d, 0x57, 0x1c, 0, 0,  // 5 - 'Clubs'
  5, 0x1c, 0x5e, 0x7f, 0x5e, 0x1c, 0, 0,  // 6 - 'Spades'
  4, 0x00, 0x18, 0x3c, 0x18, 0, 0, 0,  // 7 - 'Bullet Point'
  5, 0xff, 0xe7, 0xc3, 0xe7, 0xff, 0, 0,  // 8 - 'Rev Bullet Point'
  4, 0x00, 0x18, 0x24, 0x18, 0, 0, 0,  // 9 - 'Hollow Bullet Point'
  5, 0xff, 0xe7, 0xdb, 0xe7, 0xff, 0, 0,  // 10 - 'Rev Hollow BP'
  5, 0x30, 0x48, 0x3a, 0x06, 0x0e, 0, 0,  // 11 - 'Male'
  5, 0x26, 0x29, 0x79, 0x29, 0x26, 0, 0,  // 12 - 'Female'
  5, 0x40, 0x7f, 0x05, 0x05, 0x07, 0, 0,  // 13 - 'Music Note 1'
  5, 0x40, 0x7f, 0x05, 0x25, 0x3f, 0, 0,  // 14 - 'Music Note 2'
  5, 0x5a, 0x3c, 0xe7, 0x3c, 0x5a, 0, 0,  // 15 - 'Snowflake'
  5, 0x7f, 0x3e, 0x1c, 0x1c, 0x08, 0, 0,  // 16 - 'Right Pointer'
  5, 0x08, 0x1c, 0x1c, 0x3e, 0x7f, 0, 0,  // 17 - 'Left Pointer'
  5, 0x14, 0x22, 0x7f, 0x22, 0x14, 0, 0,  // 18 - 'UpDown Arrows'
  5, 0x5f, 0x5f, 0x00, 0x5f, 0x5f, 0, 0,  // 19 - 'Double Exclamation'
  5, 0x06, 0x09, 0x7f, 0x01, 0x7f, 0, 0,  // 20 - 'Paragraph Mark'
  4, 0x66, 0x89, 0x95, 0x6a, 0, 0, 0,  // 21 - 'Section Mark'
  5, 0x60, 0x60, 0x60, 0x60, 0x60, 0, 0,  // 22 - 'Double Underline'
  5, 0x94, 0xa2, 0xff, 0xa2, 0x94, 0, 0,  // 23 - 'UpDown Underlined'
  5, 0x08, 0x04, 0x7e, 0x04, 0x08, 0, 0,  // 24 - 'Up Arrow'
  5, 0x10, 0x20, 0x7e, 0x20, 0x10, 0, 0,  // 25 - 'Down Arrow'
  5, 0x08, 0x08, 0x2a, 0x1c, 0x08, 0, 0,  // 26 - 'Right Arrow'
  5, 0x08, 0x1c, 0x2a, 0x08, 0x08, 0, 0,  // 27 - 'Left Arrow'
  5, 0x1e, 0x10, 0x10, 0x10, 0x10, 0, 0,  // 28 - 'Angled'
  5, 0x0c, 0x1e, 0x0c, 0x1e, 0x0c, 0, 0,  // 29 - 'Squashed #'
  5, 0x30, 0x38, 0x3e, 0x38, 0x30, 0, 0,  // 30 - 'Up Pointer'
  5, 0x06, 0x0e, 0x3e, 0x0e, 0x06, 0, 0,  // 31 - 'Down Pointer'
  3, 0x00, 0x00, 0x00, 0, 0, 0, 0, // 32 - 'Space'
  1, 0x5f, 0, 0, 0, 0, 0, 0, // 33 - '!'
  3, 0x07, 0x00, 0x07, 0, 0, 0, 0, // 34 - '"'
  5, 0x14, 0x7f, 0x14, 0x7f, 0x14, 0, 0,   // 35 - '#'
  5, 0x24, 0x2a, 0x7f, 0x2a, 0x12, 0, 0,  // 36 - '$'
  5, 0x23, 0x13, 0x08, 0x64, 0x62, 0, 0,  // 37 - '%'
  5, 0x36, 0x49, 0x56, 0x20, 0x50, 0, 0,  // 38 - '&'
  3, 0x08, 0x07, 0x03, 0, 0, 0, 0,  // 39 - '''
  3, 0x1c, 0x22, 0x41, 0, 0, 0, 0,  // 40 - '('
  3, 0x41, 0x22, 0x1c, 0, 0, 0, 0,  // 41 - ')'
  5, 0x2a, 0x1c, 0x7f, 0x1c, 0x2a, 0, 0, // 42 - '*'
  5, 0x08, 0x08, 0x3e, 0x08, 0x08, 0, 0,  // 43 - '+'
  3, 0x80, 0x70, 0x30, 0, 0, 0, 0,  // 44 - ','
  5, 0x08, 0x08, 0x08, 0x08, 0x08, 0, 0,  // 45 - '-'
  2, 0x60, 0x60, 0, 0, 0, 0, 0,  // 46 - '.'
  5, 0x20, 0x10, 0x08, 0x04, 0x02, 0, 0,  // 47 - '/'
  5, 0x3e, 0x51, 0x49, 0x45, 0x3e, 0, 0,  // 48 - '0'
  3, 0x42, 0x7f, 0x40, 0, 0, 0, 0,  // 49 - '1'
  5, 0x72, 0x49, 0x49, 0x49, 0x46, 0, 0,  // 50 - '2'
  5, 0x21, 0x41, 0x49, 0x4d, 0x33, 0, 0,  // 51 - '3'
  5, 0x18, 0x14, 0x12, 0x7f, 0x10, 0, 0,  // 52 - '4'
  5, 0x27, 0x45, 0x45, 0x45, 0x39, 0, 0,  // 53 - '5'
  5, 0x3c, 0x4a, 0x49, 0x49, 0x31, 0, 0,  // 54 - '6'
  5, 0x41, 0x21, 0x11, 0x09, 0x07, 0, 0,  // 55 - '7'
  5, 0x36, 0x49, 0x49, 0x49, 0x36, 0, 0,  // 56 - '8'
  5, 0x46, 0x49, 0x49, 0x29, 0x1e, 0, 0,  // 57 - '9'
  1, 0x14, 0, 0, 0, 0, 0, 0,  // 58 - ':'
  2, 0x80, 0x68, 0, 0, 0, 0, 0,  // 59 - ';'
  4, 0x08, 0x14, 0x22, 0x41, 0, 0, 0,  // 60 - '<'
  5, 0x14, 0x14, 0x14, 0x14, 0x14, 0, 0,  // 61 - '='
  4, 0x41, 0x22, 0x14, 0x08, 0, 0, 0,  // 62 - '>'
  5, 0x02, 0x01, 0x59, 0x09, 0x06, 0, 0,  // 63 - '?'
  5, 0x3e, 0x41, 0x5d, 0x59, 0x4e, 0, 0,  // 64 - '@'
  5, 0x7c, 0x12, 0x11, 0x12, 0x7c, 0, 0,// 65 - 'A'
  5, 0x7f, 0x49, 0x49, 0x49, 0x36, 0, 0,// 66 - 'B'
  5, 0x3e, 0x41, 0x41, 0x41, 0x22, 0, 0, // 67 - 'C'
  5, 0x7f, 0x41, 0x41, 0x41, 0x3e, 0, 0, // 68 - 'D'
  5, 0x7f, 0x49, 0x49, 0x49, 0x41, 0, 0, // 69 - 'E'
  5, 0x7f, 0x09, 0x09, 0x09, 0x01, 0, 0, // 70 - 'F'
  5, 0x3e, 0x41, 0x41, 0x51, 0x73, 0, 0, // 71 - 'G'
  5, 0x7f, 0x08, 0x08, 0x08, 0x7f, 0, 0,// 72 - 'H'
  3, 0x41, 0x7f, 0x41, 0, 0, 0, 0,// 73 - 'I'
  5, 0x20, 0x40, 0x41, 0x3f, 0x01, 0, 0,// 74 - 'J'
  5, 0x7f, 0x08, 0x14, 0x22, 0x41, 0, 0,// 75 - 'K'
  5, 0x7f, 0x40, 0x40, 0x40, 0x40, 0, 0,// 76 - 'L'
  5, 0x7f, 0x02, 0x1c, 0x02, 0x7f, 0, 0,// 77 - 'M'
  5, 0x7f, 0x04, 0x08, 0x10, 0x7f, 0, 0,// 78 - 'N'
  5, 0x3e, 0x41, 0x41, 0x41, 0x3e, 0, 0,// 79 - 'O'
  5, 0x7f, 0x09, 0x09, 0x09, 0x06, 0, 0,// 80 - 'P'
  5, 0x3e, 0x41, 0x51, 0x21, 0x5e, 0, 0,// 81 - 'Q'
  5, 0x7f, 0x09, 0x19, 0x29, 0x46, 0, 0,// 82 - 'R'
  5, 0x26, 0x49, 0x49, 0x49, 0x32, 0, 0,// 83 - 'S'
  5, 0x03, 0x01, 0x7f, 0x01, 0x03, 0, 0,// 84 - 'T'
  5, 0x3f, 0x40, 0x40, 0x40, 0x3f, 0, 0,// 85 - 'U'
  5, 0x1f, 0x20, 0x40, 0x20, 0x1f, 0, 0,// 86 - 'V'
  5, 0x3f, 0x40, 0x38, 0x40, 0x3f, 0, 0,// 87 - 'W'
  5, 0x63, 0x14, 0x08, 0x14, 0x63, 0, 0,// 88 - 'X'
  5, 0x03, 0x04, 0x78, 0x04, 0x03, 0, 0,// 89 - 'Y'
  5, 0x61, 0x59, 0x49, 0x4d, 0x43, 0, 0,// 90 - 'Z'
  3, 0x7f, 0x41, 0x41, 0, 0, 0, 0,  // 91 - '['
  5, 0x02, 0x04, 0x08, 0x10, 0x20, 0, 0,  // 92 - '\'
  3, 0x41, 0x41, 0x7f, 0, 0, 0, 0,  // 93 - ']'
  5, 0x04, 0x02, 0x01, 0x02, 0x04, 0, 0,  // 94 - '^'
  5, 0x40, 0x40, 0x40, 0x40, 0x40, 0, 0,  // 95 - '_'
  3, 0x03, 0x07, 0x08, 0, 0, 0, 0,  // 96 - '`'
  5, 0x20, 0x54, 0x54, 0x78, 0x40, 0, 0,  // 97 - 'a'
  5, 0x7f, 0x28, 0x44, 0x44, 0x38, 0, 0,  // 98 - 'b'
  5, 0x38, 0x44, 0x44, 0x44, 0x00, 0, 0,  // 99 - 'c'
  5, 0x38, 0x44, 0x44, 0x28, 0x7f, 0, 0,  // 100 - 'd'
  5, 0x38, 0x54, 0x54, 0x54, 0x18, 0, 0,  // 101 - 'e'
  4, 0x08, 0x7e, 0x09, 0x02, 0, 0, 0,  // 102 - 'f'
  5, 0x18, 0xa4, 0xa4, 0x9c, 0x78, 0, 0,  // 103 - 'g'
  5, 0x7f, 0x08, 0x04, 0x04, 0x78, 0, 0,  // 104 - 'h'
  3, 0x44, 0x7d, 0x40, 0, 0, 0, 0,  // 105 - 'i'
  4, 0x40, 0x80, 0x80, 0x7a, 0, 0, 0,  // 106 - 'j'
  4, 0x7f, 0x10, 0x28, 0x44, 0, 0, 0,  // 107 - 'k'
  3, 0x41, 0x7f, 0x40, 0, 0, 0, 0,  // 108 - 'l'
  5, 0x7c, 0x04, 0x78, 0x04, 0x78, 0, 0,  // 109 - 'm'
  5, 0x7c, 0x08, 0x04, 0x04, 0x78, 0, 0,  // 110 - 'n'
  5, 0x38, 0x44, 0x44, 0x44, 0x38, 0, 0,  // 111 - 'o'
  5, 0xfc, 0x18, 0x24, 0x24, 0x18, 0, 0,  // 112 - 'p'
  5, 0x18, 0x24, 0x24, 0x18, 0xfc, 0, 0,  // 113 - 'q'
  5, 0x7c, 0x08, 0x04, 0x04, 0x08, 0, 0,  // 114 - 'r'
  5, 0x48, 0x54, 0x54, 0x54, 0x24, 0, 0,  // 115 - 's'
  4, 0x04, 0x3f, 0x44, 0x24, 0, 0, 0,  // 116 - 't'
  5, 0x3c, 0x40, 0x40, 0x20, 0x7c, 0, 0,  // 117 - 'u'
  5, 0x1c, 0x20, 0x40, 0x20, 0x1c, 0, 0,  // 118 - 'v'
  5, 0x3c, 0x40, 0x30, 0x40, 0x3c, 0, 0,  // 119 - 'w'
  5, 0x44, 0x28, 0x10, 0x28, 0x44, 0, 0,  // 120 - 'x'
  5, 0x4c, 0x90, 0x90, 0x90, 0x7c, 0, 0,  // 121 - 'y'
  5, 0x44, 0x64, 0x54, 0x4c, 0x44, 0, 0,  // 122 - 'z'
  3, 0x08, 0x36, 0x41, 0, 0, 0, 0,  // 123 - '{'
  1, 0x77, 0, 0, 0, 0, 0, 0,  // 124 - '|'
  3, 0x41, 0x36, 0x08, 0, 0, 0, 0,  // 125 - '}'
  5, 0x02, 0x01, 0x02, 0x04, 0x02, 0, 0,  // 126 - '~'
};
/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
static void MX_GPIO_Init(void);

/* USER CODE BEGIN PFP */
/* Private function prototypes -----------------------------------------------*/
void write_byte (uint8_t byte)
{
	for (int i =0; i<8; i++)
	{
		HAL_GPIO_WritePin (maxport, clock_Pin, 0);  // pull the clock pin low
		HAL_GPIO_WritePin (maxport, data_Pin, byte&0x80);  // write the MS0b bit to the data pin
		byte = byte<<1;  // shift left
		HAL_GPIO_WritePin (maxport, clock_Pin, 1);  // pull the clock pin HIGH
	}
}


void write_max_cmd (uint8_t address, uint8_t cmd)
{
	HAL_GPIO_WritePin (maxport, cs_Pin, 0);  // pull the CS pin LOW
	for (int i=0;i<num; i++)
	{
		write_byte (address);
		write_byte (cmd);
	}
	HAL_GPIO_WritePin (maxport, cs_Pin, 0);  // pull the CS pin LOW
	HAL_GPIO_WritePin (maxport, cs_Pin, 1);  // pull the CS pin HIGH
}


void setled(uint8_t row, uint8_t col, uint8_t value)
{
  bitWrite(buffer[col], row, value);

	int n = col / 8;
	int c = col % 8;
	HAL_GPIO_WritePin (maxport, cs_Pin, 0);  // pull the CS pin LOW
	for (int i=0; i<num; i++)
	{
		if (i == (num-(n+1)))
		{
			write_byte (((c+1)));
			write_byte (buffer[col]);
		}
		else
		{
			write_byte (0);
			write_byte (0);
		}
	}
	HAL_GPIO_WritePin (maxport, cs_Pin, 0);  // pull the CS pin LOW
	HAL_GPIO_WritePin (maxport, cs_Pin, 1);  // pull the CS pin HIGH
}


void setrow(uint8_t row, uint8_t value)
{
	int n = row / 8;
	int r = row % 8;

	uint8_t store = value;
	for (int i=0; i<num; i++)
	{
		if (i == ((n)))
		{
//			for (int col=0+(8*n); col<8+(8*n); col++)  // uncomment this if the character looks inverted about X axis
		for (int col=(7-0)+(8*n); col>=0+(8*n); col--)
			{
				bool b = value&0x80;
				setled (r, col, b);
				value<<=1;
			}
		}
		else
		{
			write_byte (0);
			write_byte (0);
		}
	}
	buffer_row[row] = store;
}



void write_char (char c, uint8_t max)
{
	int width = CH[8*c];
	int start= (8*c)+1;
	int row =6;
	for (int j=start; j<(start+width); j++)
//	for (int j=start+7; j>=(start); j--)  // uncomment this if the character looks inverted about Y axis
	{
		setrow (row+(8*(max-1)), CH[j]);
		row--;
	}
}



void scroll_char (char c,uint32_t speed, char direction)
{
	int width = CH[8*c];
	int start= (8*c)+1;
	int row =7;
		switch (direction)
			{
				case ('L') :
					for (int j=start; j<(start+width+1); j++)
					{
						setrow (row, CH[j]);
						row--;
					}
					for (int i=0;i<(num*8);i++)
					{
						shiftleft();
						HAL_Delay (speed);
					}
						break;
				case ('R') :
					for (int j=start; j<(start+width+1); j++)
					{
						setrow (row+((num-1)*8), CH[j]);
						row--;
					}
					for (int i=0;i<(num*8);i++)
					{
						shiftright();
						HAL_Delay (speed);
					}
					break;

				default :
					break;
			}
}

void shift_char (char c, uint32_t speed, char direction)
{
	int width = CH[8*c];
	int start= (8*c)+1;

		switch (direction)
			{
				case ('L') :
					for (int j=start; j<(start+width+1); j++)
					{
						setrow (0, CH[j]);
						shiftleft();
						HAL_Delay (speed);
					}
						break;

				case ('R') :
					for (int j=start+width+1; j>=(start); j--)
					{
						setrow ((num*8)-1, CH[j]);
						shiftright();
						HAL_Delay (speed);
					}
					break;

				default :
					break;
			}
}



void scroll_string (uint8_t *string, uint32_t speed, char direction)
{
	while (*string != 0)
	{
		shift_char (*string, speed, direction);
		string++;
  }
}



void max_clear(void)
{
	for (int i=0; i<num*8; i++)
		setrow(i,0);

	for (int i=0; i<80; i++)
	{
		buffer[i] = 0;
	  buffer_row[i] = 0;
	}
}



void shiftleft(void)
{
	int last = num*8-1;
	uint8_t old = buffer_row[last];
	int i;
	for (i=0; i<num*8; i++)
	{
		setrow(i, buffer_row[i]);
	}
	for (i=79; i>0; i--)
	{
 		buffer_row[i] = buffer_row[i-1];
	}

	buffer_row[0] = old;
}


void shiftright(void)
{
	int last = num*8-1;
	uint8_t old = buffer_row[0];

	for (int i=last; i>=0; i--)
	{
		setrow (i, buffer_row[i]);
	}

	for (int i=0; i<80; i++)
	{
		buffer_row[i] = buffer_row[i+1];
	}

	buffer_row[num*8-1] = old;
}



void max_init (uint8_t brightness)
{
	write_max_cmd(0x09, 0x00);       //  no decoding
	write_max_cmd(0x0b, 0x07);       //  scan limit = 8 LEDs
	write_max_cmd(0x0c, 0x01);       //  power down =0,normal mode = 1
	write_max_cmd(0x0f, 0x00);       //  no test display

	max_clear ();

	write_max_cmd(0x0a, brightness);       //  brightness intensity
}

void setBelt(int n, int h){
	if(n<8) n+=24;
	else if(n<16) n+=8;
	else if(n<24) n-=8;
	else n-=24;
	if(h>1) h=pow(2,h)-1;
	setrow(n,h);
}
void desetBelt(int n, int h){
	if(n<8) n+=24;
	else if(n<16) n+=8;
	else if(n<24) n-=8;
	else n-=24;
	setrow(n,0x00);
}
/* USER CODE END PFP */

/* USER CODE BEGIN 0 */

/* USER CODE END 0 */

/**
  * @brief  The application entry point.
  *
  * @retval None
  */
int main(void)
{
  /* USER CODE BEGIN 1 */

  /* USER CODE END 1 */

  /* MCU Configuration----------------------------------------------------------*/

  /* Reset of all peripherals, Initializes the Flash interface and the Systick. */
  HAL_Init();

  /* USER CODE BEGIN Init */

  /* USER CODE END Init */

  /* Configure the system clock */
  SystemClock_Config();

  /* USER CODE BEGIN SysInit */

  /* USER CODE END SysInit */

  /* Initialize all configured peripherals */
  MX_GPIO_Init();
  /* USER CODE BEGIN 2 */
  max_init (0x02);

  //setrow(16,0x09);

  /* USER CODE END 2 */
  int n=0;
  int h=0;
  int flag=0;


  setBelt(5,3);
  setBelt(14,7);
  setBelt(14,3);
  setBelt(21,6);
  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
	  //scroll_string ((uint8_t *) "ControllersTech   ", 150, left);
  /* USER CODE END WHILE */

  /* USER CODE BEGIN 3 */
	  if(n>31) n=0;
	  if(flag==0) h++;
	  if(flag==1) h--;
	  if(h==8) flag = 1;
	  if(h==0) flag = 0;
	  setBelt(n,h);
	  n++;
	  for(int i=0; i<10000000; i++);

  }
  /* USER CODE END 3 */

}

/**
  * @brief System Clock Configuration
  * @retval None
  */
void SystemClock_Config(void)
{

  RCC_OscInitTypeDef RCC_OscInitStruct;
  RCC_ClkInitTypeDef RCC_ClkInitStruct;

    /**Configure the main internal regulator output voltage 
    */
  __HAL_RCC_PWR_CLK_ENABLE();

  __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE1);

    /**Initializes the CPU, AHB and APB busses clocks 
    */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE;
  RCC_OscInitStruct.HSEState = RCC_HSE_ON;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
  RCC_OscInitStruct.PLL.PLLM = 4;
  RCC_OscInitStruct.PLL.PLLN = 168;
  RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV2;
  RCC_OscInitStruct.PLL.PLLQ = 4;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    _Error_Handler(__FILE__, __LINE__);
  }

    /**Initializes the CPU, AHB and APB busses clocks 
    */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV4;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV2;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_5) != HAL_OK)
  {
    _Error_Handler(__FILE__, __LINE__);
  }

    /**Configure the Systick interrupt time 
    */
  HAL_SYSTICK_Config(HAL_RCC_GetHCLKFreq()/1000);

    /**Configure the Systick 
    */
  HAL_SYSTICK_CLKSourceConfig(SYSTICK_CLKSOURCE_HCLK);

  /* SysTick_IRQn interrupt configuration */
  HAL_NVIC_SetPriority(SysTick_IRQn, 0, 0);
}

/** Configure pins as 
        * Analog 
        * Input 
        * Output
        * EVENT_OUT
        * EXTI
*/
static void MX_GPIO_Init(void)
{

  GPIO_InitTypeDef GPIO_InitStruct;

  /* GPIO Ports Clock Enable */
  __HAL_RCC_GPIOH_CLK_ENABLE();
  __HAL_RCC_GPIOA_CLK_ENABLE();

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOA, GPIO_PIN_2|GPIO_PIN_3|GPIO_PIN_4, GPIO_PIN_RESET);

  /*Configure GPIO pins : PA2 PA3 PA4 */
  GPIO_InitStruct.Pin = GPIO_PIN_2|GPIO_PIN_3|GPIO_PIN_4;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

}

/* USER CODE BEGIN 4 */

/* USER CODE END 4 */

/**
  * @brief  This function is executed in case of error occurrence.
  * @param  file: The file name as string.
  * @param  line: The line in file as a number.
  * @retval None
  */
void _Error_Handler(char *file, int line)
{
  /* USER CODE BEGIN Error_Handler_Debug */
  /* User can add his own implementation to report the HAL error return state */
  while(1)
  {
  }
  /* USER CODE END Error_Handler_Debug */
}

#ifdef  USE_FULL_ASSERT
/**
  * @brief  Reports the name of the source file and the source line number
  *         where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t* file, uint32_t line)
{ 
  /* USER CODE BEGIN 6 */
  /* User can add his own implementation to report the file name and line number,
     tex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */

/**
  * @}
  */

/**
  * @}
  */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
