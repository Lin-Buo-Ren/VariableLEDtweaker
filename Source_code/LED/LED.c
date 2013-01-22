/* 
LED.c
	請見本檔案的 header 檔案以得到更多關於本檔案的說明
*/
/*||||| 程式所 include 之函式庫的標頭檔 | Included Library Headers |||||*/
	#include "LED.h"
		/* led constant definition */
	#include "../Project_configurations/Common_definitions.h"
		/* for C true */
	/* Target device configurations */
		#include "../Hardware_configurations/NTOUCS_Home_Network_51AVR.h"
	/* Flow_of_control */
		#include "../Flow_of_control/Flow_of_control.h"
		
/*||||| 常數與巨集 | Constants & Macros |||||*/

/*||||| Definition of data type, enumeration, data structure and class |||||*/

/*||||| 函式雛型 | Function Prototypes |||||*/

/*||||| 全域變數 | Global Variables |||||*/
	code unsigned char led_position[8] = {
		LED0, LED1, LED2, LED3, LED4, LED5, LED6, LED7
	};

/*||||| 主要程式碼 | Main Code |||||*/
void ledDisplayValue(unsigned char value){
	if(LED_LIT == LOGIC_HIGH){
		led = value;
	}else{
		/* 目標電路的 LED 於邏輯 LOW 時發光，所以將值反向之後再填進去 */
		led = ~value;
	}
	return;
}

void ledDisable(
	/* 關閉 LED 顯示 */
	void){
	ledDisplayValue(LED_NONE);
	return;
}

void ledBlinkSecond(
	/* 讓 LED 亮一下再熄掉 */
	bit timer
		/* 用來計時的計時器 */, 
	unsigned char interval
		/* 亮一下的時間 */){
	ledDisplayValue(LED_ALL);
	delaySecond(timer, interval);
	ledDisplayValue(LED_NONE);
	return;
}
