/* 
主程式
	請見本檔案的header檔案以得到更多關於本檔案的說明
*/
/*||||| 程式所include之函式庫的標頭檔 | Included Library Headers |||||*/
	/* Standard io library definition? */
		#include <stdio.h>
	
	/* for random number generator */
		#include <stdlib.h>
		
	/* Definitions of 8052 register constants
		 　如果您要直接存取而非透過 Project_configurations/Ports_and_control_signals_* 來存取硬體的話需引入此檔案
		#include <REG52.h> */

	/* Project_configurations */
		#include "Project_configurations/Common_definitions.h"

	/* Hardware_configurations */
		#include "Hardware_configurations/NTOUCS_Home_Network_51AVR.h"

	/* LED */
		#include "LED/LED.h"
	
	/* ADC */
		#include "ADC/ADC.h"
		
	/* Button_switch */
		#include "Button_switch/Button_switch.h"
	
	/* DIP_switch */
		#include "DIP_switch/DIP_switch.h"
		
	/* Flow_of_control */
		#include "Flow_of_control/Flow_of_control.h"

	/* Seven_segment_display */
		#include "Seven_segment_display/Seven_segment_display.h"
		
	/* Timer_or_counter */
		#include "Timer_or_counter/Timer_or_counter.h"
	
	/* LCD */
		#include "LCD/LCD.h"
		
/*||||| 常數與巨集 | Constants & Macros |||||*/

/*||||| Definition of data type, enumeration, data structure and class |||||*/

/*||||| 函式雛型 | Function Prototypes |||||*/
	/* 中斷服務子程式 */
		void cbExtInt1(void);
		
	void changeLedLight(
		/* 修改個別 LED 亮度子程式 */
		unsigned char light[]
			/* 儲存 LED 亮度的陣列 */, 
		unsigned char which
			/* 欲修改亮度的 LED (1~8) */);
		
	void decreaseLedLight(
		/* 將所有 LED 亮度減半 */
		unsigned char led_light[]
			/* 儲存 LED 亮度的陣列 */);

	void increaseLedLight(
		/* 將所有 LED 亮度加倍 */
		unsigned char led_light[]
			/* 儲存 LED 亮度的陣列 */);
			
/*||||| 全域變數 | Global Variables |||||*/
	/* 保存各 LED 的亮度 */
		unsigned char led_light[9] = {0};	
	
/*||||| 主要程式碼 | Main Code |||||*/
	/* 程式進入點 | Program entry point
		 　因為嵌入式系統開機之後就會執行到電源關閉，故不需要回傳值*/
	void main(void){
		unsigned char dip_sw_value;
		unsigned char i = 0;
			
		initializeSystem();
		adcStartConvert();
		/* 設定偽亂數產生器
			 　因為 51AVR 沒有 RTC 時鐘可以取隨機值
		   　我們這裡用 ADC 讀取到的數值
			 　來當作 srand 的 seed */{
			srand(adcGetValue());
		}
		
		/* 啟動 S4 中斷 */{
			ext_int1_type = LOGIC_HIGH;
			int_enable_all = LOGIC_HIGH;
			int_ext1_enable = LOGIC_HIGH;
		}
		
		/* showTitle() */{
			lcdOn(FALSE, FALSE);
			lcdPrintString("Var. LED Tweaker");
			lcdSetPositionCursur(LCD_INST_POSITION_ROW2);
			lcdPrintString("Press S4 2 Cont.");
			ledDisplayValue(LED_ODD);
			while(!button_swIsPressed(BTN_SW4)){
				led = ~led;
				delay(12345);
			}
			delay(BTN_SW_CLICK_DELAY);
			ledDisable();
		}
		
		lcdClear();
		lcdPrintString("S4=Randomize");
		lcdSetPositionCursur(LCD_INST_POSITION_ROW2);
		lcdPrintString("S5=Dec. S6=Inc.");
		
		/* main loop */
		while(TRUE){
			dip_swEnable();
			dip_sw_value = dip_swGetValue();
			dip_swDisable();
			if(dip_sw_value != 0/* 如果 dip 按鈕有至少一個有開 */){
				/* 判定開的按鈕，並變更其對應 LED 亮度 */{
					unsigned char bit_count;
					unsigned char on_count;
					unsigned char on_switch;
					
					on_count = 0;
					for(bit_count = 1; bit_count <= 8; ++bit_count){
		
						/* 掃描每個 bit 判斷是哪一個按鈕為開 */
						if(dip_sw_value % 2 == 1){
							on_count++;
							on_switch = bit_count;
						}
						dip_sw_value >>= 1;
					}
					
					if(on_count == 1){
						/* 如果只有按一個按鍵，就進入設定亮度子程序 */
						changeLedLight(led_light, on_switch);
						lcdClear();
						lcdPrintString("S4=Randomize");
						lcdSetPositionCursur(LCD_INST_POSITION_ROW2);
						lcdPrintString("S5=Dec. S6=Inc.");						
					}
					
				}
			}
			
			/* 依照 led_light[] 中保存的亮度發光 */
				led0 = ((i < led_light[1]) ? LED_LIT : !LED_LIT);
				led1 = ((i < led_light[2]) ? LED_LIT : !LED_LIT);
				led2 = ((i < led_light[3]) ? LED_LIT : !LED_LIT);
				led3 = ((i < led_light[4]) ? LED_LIT : !LED_LIT);
				led4 = ((i < led_light[5]) ? LED_LIT : !LED_LIT);
				led5 = ((i < led_light[6]) ? LED_LIT : !LED_LIT);
				led6 = ((i < led_light[7]) ? LED_LIT : !LED_LIT);
				led7 = ((i < led_light[8]) ? LED_LIT : !LED_LIT);
			
			if(i == 255){
				i = 0;
			}else{
				++i;
			}
			
#if 1
			if(button_swIsPressed(BTN_SW5)){
				ledDisplayValue(LED_NONE);
				decreaseLedLight(led_light);
				delay(BTN_SW_CLICK_DELAY);
			}
			
			if(button_swIsPressed(BTN_SW6)){
				ledDisplayValue(LED_NONE);
				increaseLedLight(led_light);
				delay(BTN_SW_CLICK_DELAY);
			}
#endif
		}
		return;
	}

		void cbExtInt1(void) interrupt 2{
			unsigned char i;
			
			int_ext1_enable = LOGIC_LOW;
			for(i = 1; i <= 8; ++i){
				led_light[i] = rand() % 256;
			}
			int_ext1_enable = LOGIC_HIGH;
			
			return;
		}
		
	void changeLedLight(
		/* 修改個別 LED 亮度子程式 */
		unsigned char light[], 
		unsigned char which){
		unsigned char result;
		unsigned char dip_sw_value;
			
		ledDisplayValue(led_position[which - 1]);
		lcdClear();
		lcdPrintString("Turn the switch ");
		lcdSetPositionCursur(LCD_INST_POSITION_ROW2);
		lcdPrintString("2 ctrl lightness");
			
		dip_swEnable();
		dip_sw_value = dip_swGetValue();
		dip_swDisable();
		while(dip_sw_value != 0){
			seven_segmentDisplayDecimal(result = adcGetValue());
			dip_swEnable();
			dip_sw_value = dip_swGetValue();
			dip_swDisable();			
		}
		seven_segmentDisable();
		
		light[which] = result;
		ledDisplayValue(LED_NONE);
		lcdOff();
		return;		
	}
	
	void decreaseLedLight(
		/* 將所有 LED 亮度減半 */
		unsigned char led_light[]
			/* 儲存 LED 亮度的陣列 */){
		unsigned char i;
				
		for(i = 1; i <= 8; ++i){
			led_light[i] = led_light[i] / 2;

		}
		return;
	}

	void increaseLedLight(
		/* 將所有 LED 亮度減半 */
		unsigned char led_light[]
			/* 儲存 LED 亮度的陣列 */){
		unsigned char i;
				
		for(i = 1; i <= 8; ++i){
			if(led_light[i] == 0){
				led_light[i] = 1;
			}else if(led_light[i] * 2 <= 255){
				led_light[i] = led_light[i] * 2;
			}else{
				led_light[i] = 255;
			}
			
		}
		return;
	}