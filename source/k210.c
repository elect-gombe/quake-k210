#ifndef PC
/* Copyright 2018 Canaan Inc.
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#include <stdio.h>
#include "fpioa.h"
#include "lcd.h"
#include "sysctl.h"
#include "nt35310.h"
#include "board_config.h"
#include <unistd.h>
#include <bsp.h>
#include "gpiohs.h"
#include "sdcard.h"
#include "ff.h"
#include "dualshock2.h"

//uint32_t g_lcd_gram[LCD_X_MAX * LCD_Y_MAX / 2] __attribute__((aligned(128)));

static void io_set_power(void)
{
#if BOARD_LICHEEDAN
    sysctl_set_power_mode(SYSCTL_POWER_BANK6, SYSCTL_POWER_V18);
    sysctl_set_power_mode(SYSCTL_POWER_BANK7, SYSCTL_POWER_V18);
#else
    sysctl_set_power_mode(SYSCTL_POWER_BANK1, SYSCTL_POWER_V18);
#endif
}

// overclock and voltageboost suported XD
//use  to configure core voltage.
#define CORE_VOLTAGE_GPIONUM (6)
int set_cpu_freq(uint32_t f){//MHz
  if(f<600){
    gpiohs_set_drive_mode(CORE_VOLTAGE_GPIONUM, GPIO_DM_INPUT);
    gpiohs_set_pin(CORE_VOLTAGE_GPIONUM, GPIO_PV_LOW);
  }else{
    gpiohs_set_drive_mode(CORE_VOLTAGE_GPIONUM, GPIO_DM_INPUT);
  }
  //Wait for voltage setting done.
  for(volatile int i=0;i<1000;i++);
#define MHz *1000000
  return sysctl_cpu_set_freq(f MHz)/1000000;
#undef MHz  
}

static void io_mux_init(void)
{
#if BOARD_LICHEEDAN
    fpioa_set_function(27, FUNC_SPI1_SCLK);
    fpioa_set_function(28, FUNC_SPI1_D0);
    fpioa_set_function(26, FUNC_SPI1_D1);
    fpioa_set_function(29, FUNC_GPIOHS7);

    fpioa_set_function(11, FUNC_GPIOHS0 + CORE_VOLTAGE_GPIONUM);
    fpioa_set_function(38, FUNC_GPIOHS0 + DCX_GPIONUM);
    fpioa_set_function(36, FUNC_SPI0_SS3);
    fpioa_set_function(39, FUNC_SPI0_SCLK);
    fpioa_set_function(37, FUNC_GPIOHS0 + RST_GPIONUM);
    sysctl_set_spi0_dvp_data(1);

    fpioa_set_function(18, FUNC_GPIOHS0 + DS2_CS);   //Dualshock2 ss
    fpioa_set_function(19, FUNC_GPIOHS0 + DS2_CLK); //clk
    fpioa_set_function(21, FUNC_GPIOHS0 + DS2_MOSI); //mosi--DO/CMD
    fpioa_set_function(20, FUNC_GPIOHS0 + DS2_MISO); //miso--DI/DAT
#else
#error todo
    fpioa_set_function(8, FUNC_GPIOHS0 + DCX_GPIONUM);
    fpioa_set_function(6, FUNC_SPI0_SS3);
    fpioa_set_function(7, FUNC_SPI0_SCLK);
    sysctl_set_spi0_dvp_data(1);
#endif
}

uint64_t get_time(void)
{
    uint64_t v_cycle = read_cycle();
    return v_cycle * 1000000 / sysctl_clock_get_freq(SYSCTL_CLOCK_CPU);
}

void quake_main(int argc,char **argv);

extern char _heap_end[];
extern char _tp1[];
extern char _tp0[];
extern char _sp1[];
extern char _sp0[];

void ds2readcore(void){// dualshock 2 controller handler
  //reading only XD
  printf("read task begin\n");
  while(1){
  }
}

int main(void)
{  
#define PLL1_OUTPUT_FREQ 400000000UL
  /* sysctl_pll_set_freq(SYSCTL_PLL0, PLL0_OUTPUT_FREQ); */
  sysctl_pll_set_freq(SYSCTL_PLL1, PLL1_OUTPUT_FREQ);
  sysctl_clock_enable(SYSCTL_CLOCK_AI);
  FATFS sdfs;
    io_mux_init();
#if BOARD_LICHEEDAN
 #if defined(OVER_VOLTAGE)
    gpiohs_set_drive_mode(CORE_VOLTAGE_GPIONUM, GPIO_DM_INPUT);
    gpiohs_set_pin(CORE_VOLTAGE_GPIONUM, GPIO_PV_LOW);
  //overclock settings, edit if you want
  #define MHz *1000000
    usleep(100000);
    sysctl_cpu_set_freq(600 MHz);
    usleep(100000);
  #undef MHz
 #endif
    io_set_power();
    lcd_init();
    lcd_set_direction(DIR_YX_RLDU);
#else
    lcd_set_direction(DIR_YX_RLUD);
#endif

    PS2X_confg_io(DS2_CS,DS2_CLK,DS2_MOSI,DS2_MISO);
    PS2X_config_gamepad(0,0);
    register_core1(ds2readcore, 0);

    /* SD card init */
    if (sd_init())
      {
        printf("Fail to init SD card\n");
        return -1;
      }

    /* mount file system to SD card */
    if (f_mount(&sdfs, _T("0:"), 1))
      {
        printf("Fail to mount file system\n");
        return -1;
      }

    /* system start */
    printf("heap end:%p\n",_heap_end);
    {
      int n;
      printf("current stack :%p\n",&n);
    }
    printf("stack start:%p\n",_tp0);
    printf("stack end:%p\n",_tp1);
    printf("stack start:%p\n",_sp0);
    printf("stack end:%p\n",_sp1);
    printf("system start\n");
    quake_main(0,0);
    while(1);
}

#endif
