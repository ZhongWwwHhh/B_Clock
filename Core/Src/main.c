/* USER CODE BEGIN Header */
/**
 ******************************************************************************
 * @file           : main.c
 * @brief          : Main program body
 ******************************************************************************
 * @attention
 *
 * Copyright (c) 2023 STMicroelectronics.
 * All rights reserved.
 *
 * This software is licensed under terms that can be found in the LICENSE file
 * in the root directory of this software component.
 * If no LICENSE file comes with this software, it is provided AS-IS.
 *
 ******************************************************************************
 */
/* USER CODE END Header */
/* Includes ------------------------------------------------------------------*/
#include "main.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include <stdlib.h>
#include "struct_define.h"
#include "time_control.h"
#include "oled.h"
#include "error.h"
#include "LM75A.h"
#include "screen_content.h"
#include "encoder.h"
// #include "oledfont.h"
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */

/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/
I2C_HandleTypeDef hi2c1;

RTC_HandleTypeDef hrtc;

TIM_HandleTypeDef htim2;
TIM_HandleTypeDef htim3;
TIM_HandleTypeDef htim4;
TIM_HandleTypeDef htim5;

UART_HandleTypeDef huart1;

/* USER CODE BEGIN PV */

// 定义来自 struct_define.h 的结构体
// 记录时间
Times time_now;
// 记录设置
Alarm_Setting alarm_setting;
// 记录屏幕内容
Screen screen;
// LED pwm 控制
Led_Control led_control = {0, 1}; // LED 呼吸灯控制
// 编码器状态
Encoder_State encoder_state = {0, 0}; // 初始时没有左右旋

// 记录温度
extern TempDataStruct Tempdata;

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
static void MX_GPIO_Init(void);
static void MX_I2C1_Init(void);
static void MX_RTC_Init(void);
static void MX_USART1_UART_Init(void);
static void MX_TIM3_Init(void);
static void MX_TIM5_Init(void);
static void MX_TIM2_Init(void);
static void MX_TIM4_Init(void);
/* USER CODE BEGIN PFP */

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */

void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin)
{
  if (GPIO_Pin == KEY_Pin)
  {
    // 按键蜂鸣提示
    alarm_setting.alarming_time = 50;

    // 按界面分情况
    switch (screen.screen_display_num)
    {
    case 0: // 开机屏幕，忽略
      break;

    case 1: // 主界面，进设置
      screen.screen_display_num = 2;
      screen.screen_display_choose = 0;
      screen.clean_display = 1;
      break;

    case 2: // 设置界面，看选择
      switch (screen.screen_display_choose)
      {
      case 0: // 回主界面
        screen.screen_display_num = 1;
        screen.screen_display_choose = -1;
        screen.clean_display = 1;
        break;
      case 1: // 进入时间设定界面
        screen.screen_display_num = 4;
        screen.screen_display_choose = 0;
        screen.clean_display = 1;
        break;
      case 2: // 进入闹钟设定界面
        screen.screen_display_num = 5;
        screen.screen_display_choose = 0;
        screen.clean_display = 1;
        break;
      case 3: // 进入蓝牙界面
        screen.screen_display_num = 3;
        screen.screen_display_choose = -1;
        screen.clean_display = 1;
        break;
      }
      break;

    case 3: // 蓝牙界面，回主界面
      screen.screen_display_num = 1;
      screen.screen_display_choose = -1;
      screen.clean_display = 1;
      break;

    case 4: // 时间设定界面，进入下一项，直到结束
      screen.screen_display_choose++;
      break;

    case 5: // 闹钟设定界面，进入下一项，直到结束
      screen.screen_display_choose++;
      break;

    case 10: // 闹钟提示界面，按下停止，回主界面
      screen.screen_display_num = 1;
      screen.screen_display_choose = -1;
      screen.clean_display = 1;
      // 清空延时响铃
      alarm_setting.is_alarm_delayed = 0;
      break;
    }
    return;
  }
}

void HAL_RTC_AlarmAEventCallback(RTC_HandleTypeDef *hrtc)
{
  /* Prevent unused argument(s) compilation warning */
  UNUSED(hrtc);

  // 每秒时钟增一单位
  time_add(&time_now);

  // 检测是否鸣响
  if (time_now.hour == alarm_setting.time_alart.hour && time_now.minute == alarm_setting.time_alart.minute && time_now.second == alarm_setting.time_alart.second)
  {
    alarm_setting.alarming_time = 60000; // 响铃60秒
    // 进入鸣响提示界面
    screen.screen_display_num = 10;
    screen.clean_display = 1;
    screen.screen_display_choose = -1;
    // 重置延时响铃
    alarm_setting.is_alarm_delayed = 0;
    alarm_setting.time_alart_real.hour = alarm_setting.time_alart.hour;
    alarm_setting.time_alart_real.minute = alarm_setting.time_alart.minute;
    alarm_setting.time_alart_real.second = alarm_setting.time_alart.second;
  }

  // 检测延时响铃，最多三次
  if (alarm_setting.is_alarm_delayed > 0 && alarm_setting.is_alarm_delayed < 4)
  {
    if (time_now.hour == alarm_setting.time_alart_real.hour && time_now.minute == alarm_setting.time_alart_real.minute && time_now.second == alarm_setting.time_alart_real.second)
    {
      alarm_setting.alarming_time = 60000; // 响铃60秒
      // 进入鸣响提示界面
      screen.screen_display_num = 10;
      screen.clean_display = 1;
      screen.screen_display_choose = -1;
    }
  }
  else
  {
    // 已经延时三次了，重置
    alarm_setting.is_alarm_delayed = 0;
  }
  return;
}

void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{
  /* Prevent unused argument(s) compilation warning */
  UNUSED(htim);

  // 确认是tim2 10hz中断
  if (htim == (&htim2))
  {
    // 获取温度
    LM75A_GetTemperature();

    // 获取encoder值
    encoder_get_state();

    // 根据界面决定encoder值更改位置
    if (encoder_state.Left || encoder_state.Right)
    {
      switch (screen.screen_display_num)
      {
      case 0: // 开机界面，丢弃
        break;

      case 1: // 主界面，丢弃
        break;

      case 2: // 设置界面，选择条目
        screen.screen_display_choose += encoder_state.Right - encoder_state.Left;
        break;

      case 3: // 蓝牙界面，丢弃
        break;

      case 4: // 时间设定界面，调整对应值
        switch (screen.screen_display_choose)
        {
        case 0:
          time_now.hour += encoder_state.Right - encoder_state.Left;
          if (time_now.hour > 23)
          {
            time_now.hour = 0;
          }
          if (time_now.hour < 0)
          {
            time_now.hour = 23;
          }
          break;
        case 1:
          time_now.minute += encoder_state.Right - encoder_state.Left;
          if (time_now.minute > 59)
          {
            time_now.minute = 0;
          }
          if (time_now.minute < 0)
          {
            time_now.minute = 59;
          }
          break;

        case 2:
          time_now.second += encoder_state.Right - encoder_state.Left;
          if (time_now.second > 59)
          {
            time_now.second = 0;
          }
          if (time_now.second < 0)
          {
            time_now.second = 59;
          }
          break;
        }
        break;

      case 5: // 闹钟设定界面，调整对应值
        switch (screen.screen_display_choose)
        {
        case 0:
          alarm_setting.time_alart.hour += encoder_state.Right - encoder_state.Left;
          if (alarm_setting.time_alart.hour > 23)
          {
            alarm_setting.time_alart.hour = 0;
          }
          if (alarm_setting.time_alart.hour < 0)
          {
            alarm_setting.time_alart.hour = 23;
          }
          break;

        case 1:
          alarm_setting.time_alart.minute += encoder_state.Right - encoder_state.Left;
          if (alarm_setting.time_alart.minute > 59)
          {
            alarm_setting.time_alart.minute = 0;
          }
          if (alarm_setting.time_alart.minute < 0)
          {
            alarm_setting.time_alart.minute = 59;
          }
          break;

        case 2:
          alarm_setting.time_alart.second += encoder_state.Right - encoder_state.Left;
          if (alarm_setting.time_alart.second > 59)
          {
            alarm_setting.time_alart.second = 0;
          }
          if (alarm_setting.time_alart.second < 0)
          {
            alarm_setting.time_alart.second = 59;
          }
          break;
        case 3:
          alarm_setting.alarm_frequency += encoder_state.Right - encoder_state.Left;
          if (alarm_setting.alarm_frequency > 10)
          {
            alarm_setting.alarm_frequency = 0;
          }
          if (alarm_setting.alarm_frequency < 1)
          {
            alarm_setting.alarm_frequency = 10;
          }
          break;
        }
        break;

      case 10: // 闹钟提示界面，回主界面，延时响铃
        screen.screen_display_num = 1;
        screen.screen_display_choose = -1;
        screen.clean_display = 1;
        // 停止响铃
        alarm_setting.alarming_time = 0;
        // 延时加一次
        alarm_setting.is_alarm_delayed++;
        time_alarm_delay(&alarm_setting);
        break;
      }
    }

    // 丢弃
    encoder_state.Left = 0;
    encoder_state.Right = 0;

    // 屏显
    screen_show(&screen.screen_display_num, &screen.clean_display);
  }
  // 确认是tim4 100hz中断
  if (htim == (&htim4))
  {
    // 闲时LED保持呼吸，不响铃
    if (!alarm_setting.alarming_time)
    {
      // 不响铃
      HAL_GPIO_WritePin(BEEP_GPIO_Port, BEEP_Pin, GPIO_PIN_SET);
      // 呼吸
      if (led_control.LedpwmVal_Dir)
        led_control.LedpwmVal += 2;
      else
        led_control.LedpwmVal -= 2;

      if (led_control.LedpwmVal >= 98)
        led_control.LedpwmVal_Dir = 0; // 切换为PWM值减

      if (led_control.LedpwmVal <= 1)
        led_control.LedpwmVal_Dir = 1; // 切换为PWM值增
    }
    else
    {
      // 处理alarm
      if ((alarm_setting.alarming_time / (500 / alarm_setting.alarm_frequency)) % 2 == 0)
      {
        HAL_GPIO_WritePin(BEEP_GPIO_Port, BEEP_Pin, GPIO_PIN_RESET);
        led_control.LedpwmVal = 0; // 占空比100%
        led_control.LedpwmVal_Dir = 1;
      }
      else
      {
        HAL_GPIO_WritePin(BEEP_GPIO_Port, BEEP_Pin, GPIO_PIN_SET);
        led_control.LedpwmVal = 100; // 占空比0%
        led_control.LedpwmVal_Dir = 0;
      }

      alarm_setting.alarming_time -= 10; // 对alarm剩余时间减一单位
    }
    // 更新led pwm配置
    __HAL_TIM_SET_COMPARE(&htim5, TIM_CHANNEL_2, led_control.LedpwmVal);
  }
}

/* USER CODE END 0 */

/**
 * @brief  The application entry point.
 * @retval int
 */
int main(void)
{
  /* USER CODE BEGIN 1 */

  /* USER CODE END 1 */

  /* MCU Configuration--------------------------------------------------------*/

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
  MX_I2C1_Init();
  MX_RTC_Init();
  MX_USART1_UART_Init();
  MX_TIM3_Init();
  MX_TIM5_Init();
  MX_TIM2_Init();
  MX_TIM4_Init();
  /* USER CODE BEGIN 2 */

  // 初始化状态检测
  HAL_StatusTypeDef status_test;

  // 初始化屏幕
  OLED_Init();

  // 启用编码器计数
  HAL_TIM_Encoder_Start(&htim3, TIM_CHANNEL_ALL);

  // 启用tim2 10hz
  HAL_TIM_Base_Start_IT(&htim2);
  // 启用tim4 100hz
  HAL_TIM_Base_Start_IT(&htim4);

  // 启动LED的PWM
  HAL_TIM_PWM_Start(&htim5, TIM_CHANNEL_2);

  // 鸣响频率 1~10
  alarm_setting.alarm_frequency = 5;
  // 初始化为不响铃
  alarm_setting.time_alart.hour = 0;
  alarm_setting.time_alart.minute = 0;
  alarm_setting.time_alart.second = 0;
  alarm_setting.time_alart_real.hour = 0;
  alarm_setting.time_alart_real.minute = 0;
  alarm_setting.time_alart_real.second = 0;
  alarm_setting.is_alarm_delayed = 0;

  // 不在响铃状态
  alarm_setting.alarming_time = 0;

  // 开启oled
  OLED_DisPlay_On();

  // 0.显示开机画面
  screen.screen_display_num = 0;
  screen.screen_display_choose = -1;
  screen.clean_display = 1;
  screen_show(&screen.screen_display_num, &screen.clean_display);
  HAL_Delay(3000);

  // 1.显示主界面
  screen.screen_display_num = 1;
  screen.screen_display_choose = -1;
  screen.clean_display = 1;

  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
    /* USER CODE END WHILE */

    /* USER CODE BEGIN 3 */

    // while 函数不用执行太快
    HAL_Delay(7);
  }
  /* USER CODE END 3 */
}

/**
 * @brief System Clock Configuration
 * @retval None
 */
void SystemClock_Config(void)
{
  RCC_OscInitTypeDef RCC_OscInitStruct = {0};
  RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};

  /** Configure the main internal regulator output voltage
   */
  __HAL_RCC_PWR_CLK_ENABLE();
  __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE2);

  /** Initializes the RCC Oscillators according to the specified parameters
   * in the RCC_OscInitTypeDef structure.
   */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE | RCC_OSCILLATORTYPE_LSE;
  RCC_OscInitStruct.HSEState = RCC_HSE_ON;
  RCC_OscInitStruct.LSEState = RCC_LSE_ON;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
  RCC_OscInitStruct.PLL.PLLM = 25;
  RCC_OscInitStruct.PLL.PLLN = 144;
  RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV2;
  RCC_OscInitStruct.PLL.PLLQ = 4;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }

  /** Initializes the CPU, AHB and APB buses clocks
   */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK | RCC_CLOCKTYPE_SYSCLK | RCC_CLOCKTYPE_PCLK1 | RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV2;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_2) != HAL_OK)
  {
    Error_Handler();
  }

  /** Enables the Clock Security System
   */
  HAL_RCC_EnableCSS();
}

/**
 * @brief I2C1 Initialization Function
 * @param None
 * @retval None
 */
static void MX_I2C1_Init(void)
{

  /* USER CODE BEGIN I2C1_Init 0 */

  /* USER CODE END I2C1_Init 0 */

  /* USER CODE BEGIN I2C1_Init 1 */

  /* USER CODE END I2C1_Init 1 */
  hi2c1.Instance = I2C1;
  hi2c1.Init.ClockSpeed = 100000;
  hi2c1.Init.DutyCycle = I2C_DUTYCYCLE_2;
  hi2c1.Init.OwnAddress1 = 0;
  hi2c1.Init.AddressingMode = I2C_ADDRESSINGMODE_7BIT;
  hi2c1.Init.DualAddressMode = I2C_DUALADDRESS_DISABLE;
  hi2c1.Init.OwnAddress2 = 0;
  hi2c1.Init.GeneralCallMode = I2C_GENERALCALL_DISABLE;
  hi2c1.Init.NoStretchMode = I2C_NOSTRETCH_DISABLE;
  if (HAL_I2C_Init(&hi2c1) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN I2C1_Init 2 */

  /* USER CODE END I2C1_Init 2 */
}

/**
 * @brief RTC Initialization Function
 * @param None
 * @retval None
 */
static void MX_RTC_Init(void)
{

  /* USER CODE BEGIN RTC_Init 0 */

  /* USER CODE END RTC_Init 0 */

  RTC_TimeTypeDef sTime = {0};
  RTC_DateTypeDef sDate = {0};
  RTC_AlarmTypeDef sAlarm = {0};

  /* USER CODE BEGIN RTC_Init 1 */

  /* USER CODE END RTC_Init 1 */

  /** Initialize RTC Only
   */
  hrtc.Instance = RTC;
  hrtc.Init.HourFormat = RTC_HOURFORMAT_24;
  hrtc.Init.AsynchPrediv = 127;
  hrtc.Init.SynchPrediv = 255;
  hrtc.Init.OutPut = RTC_OUTPUT_DISABLE;
  hrtc.Init.OutPutPolarity = RTC_OUTPUT_POLARITY_HIGH;
  hrtc.Init.OutPutType = RTC_OUTPUT_TYPE_OPENDRAIN;
  if (HAL_RTC_Init(&hrtc) != HAL_OK)
  {
    Error_Handler();
  }

  /* USER CODE BEGIN Check_RTC_BKUP */

  /* USER CODE END Check_RTC_BKUP */

  /** Initialize RTC and set the Time and Date
   */
  sTime.Hours = 0;
  sTime.Minutes = 0;
  sTime.Seconds = 0;
  sTime.DayLightSaving = RTC_DAYLIGHTSAVING_NONE;
  sTime.StoreOperation = RTC_STOREOPERATION_RESET;
  if (HAL_RTC_SetTime(&hrtc, &sTime, RTC_FORMAT_BIN) != HAL_OK)
  {
    Error_Handler();
  }
  sDate.WeekDay = RTC_WEEKDAY_MONDAY;
  sDate.Month = RTC_MONTH_JANUARY;
  sDate.Date = 1;
  sDate.Year = 0;

  if (HAL_RTC_SetDate(&hrtc, &sDate, RTC_FORMAT_BIN) != HAL_OK)
  {
    Error_Handler();
  }

  /** Enable the Alarm A
   */
  sAlarm.AlarmTime.Hours = 0;
  sAlarm.AlarmTime.Minutes = 0;
  sAlarm.AlarmTime.Seconds = 0;
  sAlarm.AlarmTime.SubSeconds = 0;
  sAlarm.AlarmTime.DayLightSaving = RTC_DAYLIGHTSAVING_NONE;
  sAlarm.AlarmTime.StoreOperation = RTC_STOREOPERATION_RESET;
  sAlarm.AlarmMask = RTC_ALARMMASK_ALL;
  sAlarm.AlarmSubSecondMask = RTC_ALARMSUBSECONDMASK_ALL;
  sAlarm.AlarmDateWeekDaySel = RTC_ALARMDATEWEEKDAYSEL_DATE;
  sAlarm.AlarmDateWeekDay = 1;
  sAlarm.Alarm = RTC_ALARM_A;
  if (HAL_RTC_SetAlarm_IT(&hrtc, &sAlarm, RTC_FORMAT_BIN) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN RTC_Init 2 */

  /* USER CODE END RTC_Init 2 */
}

/**
 * @brief TIM2 Initialization Function
 * @param None
 * @retval None
 */
static void MX_TIM2_Init(void)
{

  /* USER CODE BEGIN TIM2_Init 0 */

  /* USER CODE END TIM2_Init 0 */

  TIM_ClockConfigTypeDef sClockSourceConfig = {0};
  TIM_MasterConfigTypeDef sMasterConfig = {0};

  /* USER CODE BEGIN TIM2_Init 1 */

  /* USER CODE END TIM2_Init 1 */
  htim2.Instance = TIM2;
  htim2.Init.Prescaler = 7199;
  htim2.Init.CounterMode = TIM_COUNTERMODE_UP;
  htim2.Init.Period = 999;
  htim2.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
  htim2.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_ENABLE;
  if (HAL_TIM_Base_Init(&htim2) != HAL_OK)
  {
    Error_Handler();
  }
  sClockSourceConfig.ClockSource = TIM_CLOCKSOURCE_INTERNAL;
  if (HAL_TIM_ConfigClockSource(&htim2, &sClockSourceConfig) != HAL_OK)
  {
    Error_Handler();
  }
  sMasterConfig.MasterOutputTrigger = TIM_TRGO_RESET;
  sMasterConfig.MasterSlaveMode = TIM_MASTERSLAVEMODE_DISABLE;
  if (HAL_TIMEx_MasterConfigSynchronization(&htim2, &sMasterConfig) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN TIM2_Init 2 */

  /* USER CODE END TIM2_Init 2 */
}

/**
 * @brief TIM3 Initialization Function
 * @param None
 * @retval None
 */
static void MX_TIM3_Init(void)
{

  /* USER CODE BEGIN TIM3_Init 0 */

  /* USER CODE END TIM3_Init 0 */

  TIM_Encoder_InitTypeDef sConfig = {0};
  TIM_MasterConfigTypeDef sMasterConfig = {0};

  /* USER CODE BEGIN TIM3_Init 1 */

  /* USER CODE END TIM3_Init 1 */
  htim3.Instance = TIM3;
  htim3.Init.Prescaler = 0;
  htim3.Init.CounterMode = TIM_COUNTERMODE_UP;
  htim3.Init.Period = 65535;
  htim3.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
  htim3.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_DISABLE;
  sConfig.EncoderMode = TIM_ENCODERMODE_TI12;
  sConfig.IC1Polarity = TIM_ICPOLARITY_RISING;
  sConfig.IC1Selection = TIM_ICSELECTION_DIRECTTI;
  sConfig.IC1Prescaler = TIM_ICPSC_DIV1;
  sConfig.IC1Filter = 6;
  sConfig.IC2Polarity = TIM_ICPOLARITY_RISING;
  sConfig.IC2Selection = TIM_ICSELECTION_DIRECTTI;
  sConfig.IC2Prescaler = TIM_ICPSC_DIV1;
  sConfig.IC2Filter = 6;
  if (HAL_TIM_Encoder_Init(&htim3, &sConfig) != HAL_OK)
  {
    Error_Handler();
  }
  sMasterConfig.MasterOutputTrigger = TIM_TRGO_RESET;
  sMasterConfig.MasterSlaveMode = TIM_MASTERSLAVEMODE_DISABLE;
  if (HAL_TIMEx_MasterConfigSynchronization(&htim3, &sMasterConfig) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN TIM3_Init 2 */

  /* USER CODE END TIM3_Init 2 */
}

/**
 * @brief TIM4 Initialization Function
 * @param None
 * @retval None
 */
static void MX_TIM4_Init(void)
{

  /* USER CODE BEGIN TIM4_Init 0 */

  /* USER CODE END TIM4_Init 0 */

  TIM_ClockConfigTypeDef sClockSourceConfig = {0};
  TIM_MasterConfigTypeDef sMasterConfig = {0};

  /* USER CODE BEGIN TIM4_Init 1 */

  /* USER CODE END TIM4_Init 1 */
  htim4.Instance = TIM4;
  htim4.Init.Prescaler = 719;
  htim4.Init.CounterMode = TIM_COUNTERMODE_UP;
  htim4.Init.Period = 900;
  htim4.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
  htim4.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_ENABLE;
  if (HAL_TIM_Base_Init(&htim4) != HAL_OK)
  {
    Error_Handler();
  }
  sClockSourceConfig.ClockSource = TIM_CLOCKSOURCE_INTERNAL;
  if (HAL_TIM_ConfigClockSource(&htim4, &sClockSourceConfig) != HAL_OK)
  {
    Error_Handler();
  }
  sMasterConfig.MasterOutputTrigger = TIM_TRGO_RESET;
  sMasterConfig.MasterSlaveMode = TIM_MASTERSLAVEMODE_DISABLE;
  if (HAL_TIMEx_MasterConfigSynchronization(&htim4, &sMasterConfig) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN TIM4_Init 2 */

  /* USER CODE END TIM4_Init 2 */
}

/**
 * @brief TIM5 Initialization Function
 * @param None
 * @retval None
 */
static void MX_TIM5_Init(void)
{

  /* USER CODE BEGIN TIM5_Init 0 */

  /* USER CODE END TIM5_Init 0 */

  TIM_MasterConfigTypeDef sMasterConfig = {0};
  TIM_OC_InitTypeDef sConfigOC = {0};

  /* USER CODE BEGIN TIM5_Init 1 */

  /* USER CODE END TIM5_Init 1 */
  htim5.Instance = TIM5;
  htim5.Init.Prescaler = 7199;
  htim5.Init.CounterMode = TIM_COUNTERMODE_UP;
  htim5.Init.Period = 99;
  htim5.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
  htim5.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_DISABLE;
  if (HAL_TIM_PWM_Init(&htim5) != HAL_OK)
  {
    Error_Handler();
  }
  sMasterConfig.MasterOutputTrigger = TIM_TRGO_RESET;
  sMasterConfig.MasterSlaveMode = TIM_MASTERSLAVEMODE_DISABLE;
  if (HAL_TIMEx_MasterConfigSynchronization(&htim5, &sMasterConfig) != HAL_OK)
  {
    Error_Handler();
  }
  sConfigOC.OCMode = TIM_OCMODE_PWM1;
  sConfigOC.Pulse = 0;
  sConfigOC.OCPolarity = TIM_OCPOLARITY_HIGH;
  sConfigOC.OCFastMode = TIM_OCFAST_DISABLE;
  if (HAL_TIM_PWM_ConfigChannel(&htim5, &sConfigOC, TIM_CHANNEL_2) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN TIM5_Init 2 */

  /* USER CODE END TIM5_Init 2 */
  HAL_TIM_MspPostInit(&htim5);
}

/**
 * @brief USART1 Initialization Function
 * @param None
 * @retval None
 */
static void MX_USART1_UART_Init(void)
{

  /* USER CODE BEGIN USART1_Init 0 */

  /* USER CODE END USART1_Init 0 */

  /* USER CODE BEGIN USART1_Init 1 */

  /* USER CODE END USART1_Init 1 */
  huart1.Instance = USART1;
  huart1.Init.BaudRate = 9600;
  huart1.Init.WordLength = UART_WORDLENGTH_8B;
  huart1.Init.StopBits = UART_STOPBITS_1;
  huart1.Init.Parity = UART_PARITY_NONE;
  huart1.Init.Mode = UART_MODE_TX_RX;
  huart1.Init.HwFlowCtl = UART_HWCONTROL_NONE;
  huart1.Init.OverSampling = UART_OVERSAMPLING_16;
  if (HAL_UART_Init(&huart1) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN USART1_Init 2 */

  /* USER CODE END USART1_Init 2 */
}

/**
 * @brief GPIO Initialization Function
 * @param None
 * @retval None
 */
static void MX_GPIO_Init(void)
{
  GPIO_InitTypeDef GPIO_InitStruct = {0};
  /* USER CODE BEGIN MX_GPIO_Init_1 */
  /* USER CODE END MX_GPIO_Init_1 */

  /* GPIO Ports Clock Enable */
  __HAL_RCC_GPIOC_CLK_ENABLE();
  __HAL_RCC_GPIOH_CLK_ENABLE();
  __HAL_RCC_GPIOA_CLK_ENABLE();
  __HAL_RCC_GPIOB_CLK_ENABLE();

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOA, SPI_Screen_CS_Pin | SPI_SCK_Pin | SPI_Screen_RES_Pin | SPI_SDA_Pin, GPIO_PIN_RESET);

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(SPI_Screen_DC_GPIO_Port, SPI_Screen_DC_Pin, GPIO_PIN_RESET);

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(BEEP_GPIO_Port, BEEP_Pin, GPIO_PIN_SET);

  /*Configure GPIO pins : SPI_Screen_CS_Pin SPI_SCK_Pin SPI_Screen_RES_Pin SPI_SDA_Pin */
  GPIO_InitStruct.Pin = SPI_Screen_CS_Pin | SPI_SCK_Pin | SPI_Screen_RES_Pin | SPI_SDA_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_PULLUP;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
  HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

  /*Configure GPIO pin : SPI_Screen_DC_Pin */
  GPIO_InitStruct.Pin = SPI_Screen_DC_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_PULLUP;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
  HAL_GPIO_Init(SPI_Screen_DC_GPIO_Port, &GPIO_InitStruct);

  /*Configure GPIO pin : BEEP_Pin */
  GPIO_InitStruct.Pin = BEEP_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_OD;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(BEEP_GPIO_Port, &GPIO_InitStruct);

  /*Configure GPIO pin : KEY_Pin */
  GPIO_InitStruct.Pin = KEY_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_IT_FALLING;
  GPIO_InitStruct.Pull = GPIO_PULLUP;
  HAL_GPIO_Init(KEY_GPIO_Port, &GPIO_InitStruct);

  /* EXTI interrupt init*/
  HAL_NVIC_SetPriority(EXTI9_5_IRQn, 4, 0);
  HAL_NVIC_EnableIRQ(EXTI9_5_IRQn);

  /* USER CODE BEGIN MX_GPIO_Init_2 */
  /* USER CODE END MX_GPIO_Init_2 */
}

/* USER CODE BEGIN 4 */

/* USER CODE END 4 */

/**
 * @brief  This function is executed in case of error occurrence.
 * @retval None
 */
void Error_Handler(void)
{
  /* USER CODE BEGIN Error_Handler_Debug */
  /* User can add his own implementation to report the HAL error return state */
  __disable_irq();
  while (1)
  {
  }
  /* USER CODE END Error_Handler_Debug */
}

#ifdef USE_FULL_ASSERT
/**
 * @brief  Reports the name of the source file and the source line number
 *         where the assert_param error has occurred.
 * @param  file: pointer to the source file name
 * @param  line: assert_param error line source number
 * @retval None
 */
void assert_failed(uint8_t *file, uint32_t line)
{
  /* USER CODE BEGIN 6 */
  /* User can add his own implementation to report the file name and line number,
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */
