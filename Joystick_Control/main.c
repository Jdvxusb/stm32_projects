#include "main.h"
#include "stm32f4xx_hal.h"
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "stm32f4xx_hal_tim.h"


UART_HandleTypeDef huart2;

void SystemClock_Config(void);
static void MX_GPIO_Init(void);
static void MX_USART2_UART_Init(void);
static void MX_TIM3_Init(void); // Timer initialization function
void Error_Handler(void);
void Motor_SetDirection(uint8_t direction);
void Motor_SetSpeed(uint16_t speed);


int main(void) {
    // Initialize the HAL Library
    HAL_Init();
    // Configure the system clock
    SystemClock_Config();
    // Initialize all configured peripherals
    MX_USART2_UART_Init();




    uint8_t received_char;               // Variable to store received character
    uint8_t idx = 0;
    uint8_t xy[2][4]; // Array to hold x and y values
    uint8_t indx = 0, indy = 0;

    while (1) {
        // Receive one character at a time
        if (HAL_UART_Receive(&huart2, &received_char, 1, HAL_MAX_DELAY) == HAL_OK) {
            // Check if received character is newline (Enter key)
            if (received_char == '\r') { // If Enter is pressed, check for Carriage Return
                xy[1][indy] = '\0';      // Null terminate y string

                // Convert received string to integers
                int x = atoi((char*)xy[0]);
                int y = atoi((char*)xy[1]);

                // Prepare the result message to transmit
                char result_message[50];
                int length = snprintf(result_message, sizeof(result_message), "%d %d\r\n", x, y);

                // Transmit the result back
                HAL_UART_Transmit(&huart2, (uint8_t*)result_message, length, HAL_MAX_DELAY);

                // Reset indices for the next message
                idx = 0;
                indx = 0;
                indy = 0;
            } else if (idx <  4) {  // Check buffer overflow
                if (received_char == ' ') { // If space, switch to the next array
                    indx++;
                    xy[0][indy] = '\0'; // Null terminate the x string
                    indy = 0;
                }
                xy[indx][indy++] = received_char; // Store received character
            }
        }
    }
}

// USART2 initialization function
static void MX_USART2_UART_Init(void) {
    huart2.Instance = USART2;
    huart2.Init.BaudRate = 115200;  // Make sure this matches the terminal setting
    huart2.Init.WordLength = UART_WORDLENGTH_8B;
    huart2.Init.StopBits = UART_STOPBITS_1;
    huart2.Init.Parity = UART_PARITY_NONE;
    huart2.Init.Mode = UART_MODE_TX_RX;
    huart2.Init.HwFlowCtl = UART_HWCONTROL_NONE;
    huart2.Init.OverSampling = UART_OVERSAMPLING_16;

    if (HAL_UART_Init(&huart2) != HAL_OK) {
        Error_Handler();  // Handle errors in UART initialization
    }
}

// GPIO initialization function



// Clock configuration function
void SystemClock_Config(void) {
    RCC_OscInitTypeDef RCC_OscInitStruct = {0};
    RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};

    // Configure the main internal regulator output voltage
    __HAL_RCC_PWR_CLK_ENABLE();
    __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE1);

    // Initialize the RCC Oscillators according to your clock requirements
    RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI;
    RCC_OscInitStruct.HSIState = RCC_HSI_ON;
    RCC_OscInitStruct.HSICalibrationValue = RCC_HSICALIBRATION_DEFAULT;
    RCC_OscInitStruct.PLL.PLLState = RCC_PLL_NONE;  // No PLL for HSI
    HAL_RCC_OscConfig(&RCC_OscInitStruct);

    // Select the PLL as system clock source and configure the HCLK, PCLK1, and PCLK2 clocks dividers
    RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK | RCC_CLOCKTYPE_SYSCLK |
                                  RCC_CLOCKTYPE_PCLK1 | RCC_CLOCKTYPE_PCLK2;
    RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_HSI;
    RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
    RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV4;
    RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV2;

    // Initialize the CPU, AHB and APB buses clocks
    HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_0);
}

// Error handler function
void Error_Handler(void) {
    // Stay in an infinite loop if an error occurs
    while (1) {
        // Optionally toggle an LED or send a message
    }
}