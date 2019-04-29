
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
  ******************************************************************************
  * This notice applies to any and all portions of this file
  * that are not between comment pairs USER CODE BEGIN and
  * USER CODE END. Other portions of this file, whether 
  * inserted by the user or by software development tools
  * are owned by their respective copyright owners.
  *
  * Copyright (c) 2019 STMicroelectronics International N.V. 
  * All rights reserved.
  *
  * Redistribution and use in source and binary forms, with or without 
  * modification, are permitted, provided that the following conditions are met:
  *
  * 1. Redistribution of source code must retain the above copyright notice, 
  *    this list of conditions and the following disclaimer.
  * 2. Redistributions in binary form must reproduce the above copyright notice,
  *    this list of conditions and the following disclaimer in the documentation
  *    and/or other materials provided with the distribution.
  * 3. Neither the name of STMicroelectronics nor the names of other 
  *    contributors to this software may be used to endorse or promote products 
  *    derived from this software without specific written permission.
  * 4. This software, including modifications and/or derivative works of this 
  *    software, must execute solely and exclusively on microcontroller or
  *    microprocessor devices manufactured by or for STMicroelectronics.
  * 5. Redistribution and use of this software other than as permitted under 
  *    this license is void and will automatically terminate your rights under 
  *    this license. 
  *
  * THIS SOFTWARE IS PROVIDED BY STMICROELECTRONICS AND CONTRIBUTORS "AS IS" 
  * AND ANY EXPRESS, IMPLIED OR STATUTORY WARRANTIES, INCLUDING, BUT NOT 
  * LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY, FITNESS FOR A 
  * PARTICULAR PURPOSE AND NON-INFRINGEMENT OF THIRD PARTY INTELLECTUAL PROPERTY
  * RIGHTS ARE DISCLAIMED TO THE FULLEST EXTENT PERMITTED BY LAW. IN NO EVENT 
  * SHALL STMICROELECTRONICS OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT,
  * INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
  * LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, 
  * OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF 
  * LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING 
  * NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE,
  * EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
  *
  ******************************************************************************
  */
/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "stm32f4xx_hal.h"
#include "usb_device.h"
#include "max_driver.h"
#include "math.h"

/* USER CODE BEGIN Includes */
#include "max_matrix_stm32.h"

extern uint8_t CDC_Transmit_FS(uint8_t* Buf, uint16_t Len);
/* USER CODE END Includes */

/* Private variables ---------------------------------------------------------*/

/* USER CODE BEGIN PV */
/* Private variables ---------------------------------------------------------*/
uint8_t buffer[80];
uint8_t buffer_row[80];

uint8_t LED_state[32]={0};
uint8_t ReceivedData[40]; // Array used to store received data from USB port
uint8_t ReceivedDataFlag = 0; // Flag, that informs about receiving a data

uint8_t DataToSend[40]; // Array, that contains data to be sent through USB port
uint8_t MessageLength = 0; // Variable, which stores information about length of data to be sent

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
static void MX_GPIO_Init(void);

/* USER CODE BEGIN PFP */
/* Private function prototypes -----------------------------------------------*/
void write_byte (uint8_t byte) // Function used for writing single byte of data to display
{
	for (int i =0; i<8; i++)
	{
		HAL_GPIO_WritePin (maxport, clock_Pin, 0);  // Set CLK pin - LOW
		HAL_GPIO_WritePin (maxport, data_Pin, byte&0x80);  // Write the MS0b bit to the data pin
		byte = byte<<1;  // Shift the byte to the left
		HAL_GPIO_WritePin (maxport, clock_Pin, 1);  // Set CLK pin - HIGH
	}
}


void write_max_cmd (uint8_t address, uint8_t cmd) // Function that runs a command (writes data to registers)
{
	HAL_GPIO_WritePin(maxport, cs_Pin, 0);  // CS pin - LOW
	for (int i=0;i<num; i++)
	{
		write_byte(address); // First write the address...
		write_byte(cmd); // ...Then write the data
	}
	HAL_GPIO_WritePin(maxport, cs_Pin, 0);  // Set CS pin - LOW
	HAL_GPIO_WritePin(maxport, cs_Pin, 1);  // Set CS pin - HIGH
}


void setled(uint8_t row, uint8_t col, uint8_t value)
{
  bitWrite(buffer[col], row, value);

	int n = col / 8;
	int c = col % 8;
	HAL_GPIO_WritePin(maxport, cs_Pin, 0);  // Set CS pin - LOW
	for (int i=0; i<num; i++)
	{
		if (i == (num-(n+1)))
		{
			write_byte(c+1);
			write_byte(buffer[col]);
		}
		else
		{
			write_byte(0);
			write_byte(0);
		}
	}
	HAL_GPIO_WritePin(maxport, cs_Pin, 0);  // Set CS pin - LOW
	HAL_GPIO_WritePin(maxport, cs_Pin, 1);  // Set CS pin - HIGH
}


void setrow(uint8_t row, uint8_t value) //Function that sets belt of LED's
{
	int n = row / 8;
	int r = row % 8;

	uint8_t store = value;
	for(int i=0; i<num; i++)
	{
		if(i == ((n)))
		{
		for(int col=(7-0)+(8*n); col>=0+(8*n); col--)
			{
				bool b = value&0x80;
				setled (r, col, b);
				value<<=1;
			}
		}
		else
		{
			write_byte(0);
			write_byte(0);
		}
	}
	buffer_row[row] = store;
}

void max_clear(void) // Function that clears whole LED display and LED_state table
{
	for(int i=0; i<num*8; i++)
	{
	LED_state[i]=0;
	setrow(i,0);
	}

	for(int i=0; i<80; i++)
	{
	  buffer[i] = 0;
	  buffer_row[i] = 0;
	}
}


void max_init (uint8_t brightness) // Function that sets up parameters of display
{
	write_max_cmd(0x09, 0x00);       //  No decoding
	write_max_cmd(0x0b, 0x07);       //  Scan limit = 8 LEDs
	write_max_cmd(0x0c, 0x01);       //  Turn on in normal mode
	write_max_cmd(0x0f, 0x00);       //  Display-test is switched off

	max_clear(); // Clear the LED display

	write_max_cmd(0x0a, brightness); // Set the brightness of display
}

void setBelt(int n, int h) // Function that helps to set appropriate number of LED's at desired belt (counting from the bottom)
{
	if(n<8) n+=24;
	else if(n<16) n+=8;
	else if(n<24) n-=8;
	else n-=24;
	if(h>1) h=pow(2,h)-1;
	setrow(n,h);
}
void desetBelt(int n, int h) // Function that resets chosen belt of LED's
{
	if(n<8) n+=24;
	else if(n<16) n+=8;
	else if(n<24) n-=8;
	else n-=24;
	setrow(n,0x00);
}

void max_refresh() //Function that sets LED's at display according to LED_state table
{
	for(int i=0;i<32;i++)
	{
		setBelt(i,LED_state[i]);
	}
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
  MX_USB_DEVICE_Init();
  /* USER CODE BEGIN 2 */
  max_init(0x02);

  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
  /* USER CODE END WHILE */

  /* USER CODE BEGIN 3 */

	  if(ReceivedDataFlag == 1) // When data has been received
	  {
	  	MessageLength = sprintf(DataToSend, "Received: %s\n\r", ReceivedData); // Combine message, which is going to be sent as an echo
	  	CDC_Transmit_FS(DataToSend, MessageLength); // Send Echo through USB port
	  	max_refresh(); // Refresh the LED display, while received data
	  	ReceivedDataFlag = 0; // Setting the flag value to 0, until another chunk of data will be received
	  }

  /* USER CODE END 3 */

  }

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
  RCC_OscInitStruct.PLL.PLLN = 72;
  RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV2;
  RCC_OscInitStruct.PLL.PLLQ = 3;
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
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV2;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_2) != HAL_OK)
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
