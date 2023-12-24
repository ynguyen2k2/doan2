#include "main.h"
#include <math.h>
#include "matrix.c"
#include "graphics.h"
#include "i2c-lcd.h"

I2C_HandleTypeDef hi2c1;
RTC_HandleTypeDef hrtc;

UART_HandleTypeDef huart1;

TIM_HandleTypeDef htim2;
TIM_HandleTypeDef htim3;

void SystemClock_Config(void);
static void MX_GPIO_Init(void);
static void MX_TIM2_Init(void);
static void MX_TIM3_Init(void);
static void MX_RTC_Init(void);
static void MX_I2C1_Init(void);
static void MX_USART1_UART_Init(void);

void dongho_kim();

int32_t dem1, dem2, dem3;
int32_t dem6;

char buffer[17];
char rx_data[15];
int i=0;

char gpg[8];
char ntn[6];

char gio1[2];
char a[1];

float radians, goc;

// RTC
RTC_TimeTypeDef sTime;
RTC_DateTypeDef sDate;

unsigned char ena_chinh= 0;
unsigned char chon_dh=0;// chon dong ho

int8_t gio, phut, giay, ngay, thang, nam, thu;

int8_t gio_ht, phut_ht, giay_ht,  thu_ht, ngay_ht, thang_ht, nam_ht;// ht hien thi
 
int8_t gio_esp, phut_esp, giay_esp,  thu_esp, ngay_esp, thang_esp, nam_esp;

int8_t TT_wifi;

uint8_t chinh_phut ;
uint8_t chinh_giay ;
uint8_t chinh_gio ;

uint8_t chinh_ngay ;
uint8_t chinh_thang ;
uint8_t chinh_nam ;

uint8_t chinh_thu ;

void set_alarm(void);

void set_time(void);

void Update_time_esp()
{
		if (giay_esp	!= sTime.Seconds)
		{
			sTime.Seconds = giay_esp;
		}
		if (phut_esp != sTime.Minutes)
		{
			sTime.Minutes = phut_esp;
		}
		if(gio_esp != sTime.Hours)
		{
			sTime.Hours = gio_esp;
		}
		if (ngay_esp!=sDate.Date)
		{
			sDate.Date=ngay_esp;
		}
		if (thang_esp!=sDate.Month)
		{
			sDate.Month=thang_esp;
		}
		if (nam_esp!=sDate.Year)
		{
			sDate.Year=nam_esp;
		}
		if (thu_esp!=sDate.WeekDay)
		{
			sDate.WeekDay=thu_esp;
		}
		if (HAL_RTC_SetTime(&hrtc, &sTime, RTC_FORMAT_BIN) != HAL_OK)
			{
				Error_Handler();
			}

		if (HAL_RTC_SetDate(&hrtc, &sDate, RTC_FORMAT_BIN) != HAL_OK)
			{
				Error_Handler();
			}
}

void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart)
{
    if (rx_data[0] != 13)
    {
        buffer[i++] = rx_data[0];
    }
    else
    {
      i = 0;
			gio_esp = (buffer[1] - '0') * 10 + (buffer[2] - '0');
			phut_esp = (buffer[3] - '0') * 10 + (buffer[4] - '0');
			giay_esp = (buffer[5] - '0') * 10 + (buffer[6] - '0');
			
			ngay_esp = (buffer[7] - '0') * 10 + (buffer[8] - '0');
			thang_esp = (buffer[9] - '0') * 10 + (buffer[10] - '0');
			nam_esp = (buffer[13] - '0')*10 + (buffer[14] - '0');
			thu_esp = (buffer[15] - '0');
			
			TT_wifi=buffer[16]-'0';
			if (buffer[16] == '0')
			{	
					Update_time_esp();

			}
        for (int cnt = 0; cnt < sizeof(buffer); cnt++)
        {
            buffer[cnt] = '\0';
        }
				for (int cnt = 0; cnt < sizeof(rx_data); cnt++)
        {
            rx_data[cnt] = '\0';
        }
    }
	
    HAL_UART_Receive_IT(&huart1, (uint8_t *)rx_data, 1);
}
void xuatmaled(void)
{
		HAL_GPIO_WritePin(GPIOE,OE,GPIO_PIN_SET);
		for(int i=0;i<128;i++)
		{	
			GPIOA->ODR=bufferGPIO[vitri_bit][hang_led][i];
			HAL_GPIO_WritePin(GPIOE,CLK,GPIO_PIN_SET);
			HAL_GPIO_WritePin(GPIOE,CLK,GPIO_PIN_RESET);
			scan16S(scan_data[hang_led]);
		}
		HAL_GPIO_WritePin(GPIOE,LAT,GPIO_PIN_SET);
		HAL_GPIO_WritePin(GPIOE,LAT,GPIO_PIN_RESET);
		HAL_GPIO_WritePin(GPIOE,OE,GPIO_PIN_RESET);
		
		vitri_bit++;
		
		if(vitri_bit==5)
		{
			vitri_bit=0;
			hang_led++;
			if(hang_led==16)
			{
				hang_led=0;
			}
		}		
}

//*************************dong ho kim
void ve_so_donghokim()
{
	const int CENTRE_X = 30;
	const int CENTRE_Y = 29;
  int j = 1;
  for (int i = 5; i < 60; i+=5) {
    goc =  180 - i * 6;
    radians = (float)goc * (3.14159265f / 180.0f); //
		int x0 = CENTRE_X + 25 * sin(radians);
		int y0 = CENTRE_Y + 25 * cos(radians);
		sprintf(a,"%d",j++);
		glcd_text45(x0, y0, a, 1, 0x00,0x1f,0x1f);
		glcd_text45(29, 4, "12", 1, 0x00,0x1f,0x1f);
  }
}
void ve_kim_giay(float goc, int x0, int y0, int w, int c0, int c1, int z1) {
      int x[3];
      int y[3];
			float radians;
			//radians = radians(angle+z1);
	
			radians = (float)goc * (3.14159265f / 180.0f);
      x[0] = x0 + w * sin(radians);
      y[0] = y0 + w * cos(radians);
	
			radians = (float)goc * (3.14159265f / 180.0f)+z1;
      
      x[1] = x0 + 6 * sin(radians);
      y[1] = y0 + 6 * cos(radians);
	
      radians = (float)goc * (3.14159265f / 180.0f)-z1;;
      x[2] = x0 + 6 * sin(radians);
      y[2] = y0 + 6 * cos(radians);
      
			GrTriangleDrawFill(x0, y0, x[0], y[0], x[1], y[1],0x08, 0x00, 0x04 );
			GrTriangleDrawFill(x0, y0, x[0], y[0], x[2], y[2],0x08, 0x00, 0x04 );
			glcd_line(x[0], y[0], x[1], y[1],0x08, 0x00, 0x04);
			glcd_line(x[0], y[0], x[2], y[2], 0x08, 0x00, 0x04);
}

void ve_kim_phut(float goc, int x0, int y0, int w, int c0, int c1, int z1) {
      int x[3];
      int y[3];
			float radians;
			//radians = radians(angle+z1);
	
			radians = (float)goc * (3.14159265f / 180.0f);
      x[0] = x0 + w * sin(radians);
      y[0] = y0 + w * cos(radians);
	
			radians = (float)goc * (3.14159265f / 180.0f)+z1;
      x[1] = x0 + 6 * sin(radians);
      y[1] = y0 + 6 * cos(radians);
	
      radians = (float)goc * (3.14159265f / 180.0f)-z1;;
      x[2] = x0 + 6 * sin(radians);
      y[2] = y0 + 6 * cos(radians);
    
			GrTriangleDrawFill(x0, y0, x[0], y[0], x[1], y[1],0x00, 0x1f, 0x1f );
			GrTriangleDrawFill(x0, y0, x[0], y[0], x[2], y[2],0x00, 0x1f, 0x1f );
			glcd_line(x[0], y[0], x[1], y[1], 0x00, 0x1f, 0x1f);
			glcd_line(x[0], y[0], x[2], y[2], 0x00, 0x1f, 0x1f);
}

void ve_kim_gio(float goc, int x0, int y0, int w, int c0, int c1, int z1) {
      int x[3];
      int y[3];
			float radians;
			//radians = radians(angle+z1);
			radians = (float)goc * (3.14159265f / 180.0f);
      x[0] = x0 + w * sin(radians);
      y[0] = y0 + w * cos(radians);
	
			radians = (float)goc * (3.14159265f / 180.0f)+z1;    
      x[1] = x0 + 6 * sin(radians);
      y[1] = y0 + 6 * cos(radians);
	
      radians = (float)goc * (3.14159265f / 180.0f)-z1;;
      x[2] = x0 + 6 * sin(radians);
      y[2] = y0 + 6 * cos(radians);
  
			GrTriangleDrawFill(x0, y0, x[0], y[0], x[1], y[1],0x0e, 0x0e, 0x0f );
			GrTriangleDrawFill(x0, y0, x[0], y[0], x[2], y[2],0x0e, 0x0e, 0x0f );
			glcd_line(x[0], y[0], x[1], y[1], 0x0e, 0x0e, 0x0f);
			glcd_line(x[0], y[0], x[2], y[2], 0x0e, 0x0e, 0x0f);
}

void ve_3kim() 
{
	int8_t minute=30;
	int8_t second=10;
	int8_t hour=12;;
  if (hour > 12) hour -= 12;
  //float angle = 180 - (30 * hour) - (minute / 2);
	float angle = 180 - (30 * gio_ht) - (phut_ht/ 2);
	ve_kim_gio(angle, 31, 31, 20, 31, 31, 25);
  //angle = 180 - (6 * minute);
	angle = 180 - (6 *phut_ht);
	ve_kim_phut(angle, 31, 31, 24, 31, 31, 25);
	// angle =  180 - (second * 6);
	 angle =  180 - (giay_ht * 6);
	ve_kim_giay(angle, 31, 31, 28, 31, 31, 25);
}
void manhinh_chinhsua()
{
	glcd_text57(5, 1,"Time edit", 1, 0x1f, 0x1f, 0x1f);
	glcd_text57(5, 11,"H:", 1, 0x1f, 0x1f, 0x1f);
	glcd_text57(5, 21,"M:", 1, 0x1f, 0x1f, 0x1f);
	glcd_text57(5, 31,"S:", 1, 0x1f, 0x1f, 0x1f);
	glcd_text57(32, 11,"D:", 1, 0x1f, 0x1f, 0x1f);
	glcd_text57(32, 21,"M:", 1, 0x1f, 0x1f, 0x1f);
	glcd_text57(32, 31,"Y:", 1, 0x1f, 0x1f, 0x1f);
	
	sprintf(gpg, "%02d", sTime.Hours);
	glcd_text45(18, 12,gpg, 1, 0x1f, 0x00, 0x00);
	sprintf(gpg, "%02d", sTime.Minutes);
	glcd_text45(18, 22,gpg, 1, 0x1f, 0x00, 0x00);
	sprintf(gpg, "%02d", sTime.Seconds);
	glcd_text45(18, 32,gpg, 1, 0x1f, 0x00, 0x00);
	
	sprintf(ntn, "%02d", sDate.Date);
	glcd_text45(45, 12,ntn, 1, 0x1f, 0x00, 0x00);
	sprintf(ntn, "%02d", sDate.Month);
	glcd_text45(45, 22,ntn, 1, 0x1f, 0x00, 0x00);
	glcd_text45(45, 32,"2", 1, 0x1f, 0x00, 0x00);
	sprintf(ntn, "%03d", sDate.Year);
	glcd_text45(49, 32,ntn, 1, 0x1f, 0x00, 0x00);
	if(ena_chinh==1)
	{
			//giay
		glcd_circle(3, 34, 1, 1, 0x1f, 0x00, 0x00);
		glcd_line(0, 34,3, 34 , 0x1f, 0x00, 0x00);
	}
	else if (ena_chinh==2)
	{
		//phut
		glcd_circle(3, 24, 1, 1, 0x1f, 0x00, 0x00);
		glcd_line(0, 24,3, 24 , 0x1f, 0x00, 0x00);
	}
	else if (ena_chinh==3)
	{
		//gio
		glcd_circle(3, 14, 1, 1, 0x1f, 0x00, 0x00);
		glcd_line(0, 14,3, 14 , 0x1f, 0x00, 0x00);
	}
	else if (ena_chinh==4)
	{
		//ngay
	glcd_circle(30, 14, 1, 1, 0x1f, 0x00, 0x00);
	glcd_line(27, 14,30, 14 , 0x1f, 0x00, 0x00);
	}
	else if (ena_chinh==5)
	{
		//thang
	glcd_circle(30, 24, 1, 1, 0x1f, 0x00, 0x00);
	glcd_line(27, 24,30, 24 , 0x1f, 0x00, 0x00);
	}
	else if (ena_chinh==6)
	{
		//nam
	glcd_circle(30, 34, 1, 1, 0x1f, 0x00, 0x00);
	glcd_line(27, 34,30, 34 , 0x1f, 0x00, 0x00);
	}
	else if (ena_chinh==7)
	{
		//thu
	glcd_circle(3, 44, 1, 1, 0x1f, 0x00, 0x00);
	glcd_line(0, 44,3, 44 , 0x1f, 0x00, 0x00);
	}
	
	glcd_text57(5, 41,"W:", 1, 0x1f, 0x1f, 0x1f);
	const char *mang_thu[] = {"SUNDAY","MONDAY", "TUESDAY", "WEDNESDAY", "THURSDAY", "FRIDAY", "SATURDAY"};
		char a[10];
		 if (thu_ht >= 1 && thu_ht <= 7) {
				
        sprintf(a, mang_thu[thu_ht - 1]);
        glcd_text57(15, 41, a, 1, 0x1f, 0x00, 0x00);
    }
}
void dongho_kim()
{
		ve_3kim();
		glcd_circle(31, 31, 30, 0, 0x1f, 0x1f, 0x00);
		glcd_circle(31, 31, 29, 0, 0x1f, 0x00, 0x00);
		glcd_circle(31, 31, 31, 0, 0x00, 0x1f, 0x1f);
		//	glcd_circle(31, 31, 0, 0, 0x1f, 0x00, 0x00);
		glcd_circle(32, 31, 1, 0, 0x1f, 0x00, 0x00);
		sprintf(ntn, "%02d-%02d-", ngay_ht, thang_ht);
		glcd_text45(11, 36,ntn, 1, 0x1f, 0x1f, 0x1f);
		glcd_text45(36, 36,"2 ", 1, 0x1f, 0x1f, 0x1f);
		sprintf(ntn, "%03d", nam_ht);
		glcd_text45(39, 36,ntn, 1, 0x1f, 0x1f, 0x1f);
	/*
		sprintf(ntn, "%d", thu_ht);
		glcd_text45(31, 18,ntn, 1, 0x1f, 0x00, 120);
		glcd_rect(29, 16,35, 24, 0,0x1f, 0x1f, 0x1f );
	*/
		//glcd_text45(24, 40,"-", 1, 0x1f, 0x1f, 0x1f);
		//glcd_text57(24, 40, "-", 1,0x1f, 0x1f, 0x1f );
		
		ve_so_donghokim();
}
//*****************end dong ho kim

void dongho_so()
{
		xoa_2so(2, 7, 2);
		sprintf(gpg, "%02d", gio_ht);
		glcd_text57(7, 2,gpg, 2, 0x1f, 0x1f, 0x1f);
		
		glcd_text57(27, 2,":", 2, 0x1f, 0x00, 0x1f);
		xoa_2so(2, 36, 2);
		sprintf(gpg, "%02d", phut_ht);
		glcd_text57(36, 2,gpg, 2, 0x1f, 0x00, 0x00);
		
		xoa_2so(17, 20, 2);
		sprintf(gpg, "%02d", giay_ht);
		glcd_text57(20, 17,gpg, 2, 0x00, 0x1f, 0x00);
		xoa_2so(35, 0, 2);
		xoa_2so(35, 21, 2);
		xoa_2so(35, 42, 2);
		//sprintf(ntn, "%02d", sDate.WeekDay+1);
		const char *mang_thu[] = {"SUNDAY","MONDAY", "TUESDAY", "WEDNESDAY", "THURSDAY", "FRIDAY", "SATURDAY"};
		char a[10];
		 if (thu_ht >= 1 && thu_ht <= 7) {
        sprintf(a, mang_thu[thu_ht - 1]);
        glcd_text57(9, 35, a, 1, 0x00, 0x00, 0x0f);
    }
		sprintf(ntn, "%02d-%02d-", ngay_ht, thang_ht);
		glcd_text45(11, 46,ntn, 1, 0x1f, 0x1f, 0x1f);
		glcd_text45(36, 46,"2 ", 1, 0x1f, 0x1f, 0x1f);
		sprintf(ntn, "%03d", nam_ht);
		glcd_text45(40, 46,ntn, 1, 0x1f, 0x1f, 0x1f);
}


uint32_t last_interrupt_time = 0;
const uint32_t DEBOUNCE_DELAY = 200;
uint8_t buttonState = 0;

void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin)
{
	giay=sTime.Seconds;
	phut=sTime.Minutes;
	gio=sTime.Hours;
	
	ngay=sDate.Date;
	thang=sDate.Month;
	nam=sDate.Year;
	thu=sDate.WeekDay;
	
	 uint32_t current_time = HAL_GetTick();
    if (current_time - last_interrupt_time < DEBOUNCE_DELAY) {
        return;
    }
    
    last_interrupt_time = current_time;
	if(GPIO_Pin==GPIO_PIN_2)
	{
		for(int i=0; i<50000;i++);
		if(GPIO_Pin==GPIO_PIN_2)
	{
		chon_dh++;
		if(chon_dh==3)
		chon_dh=0;
	}
	}
	if(GPIO_Pin==GPIO_PIN_0)
		{
			for(int i=0; i<50000;i++);
		if(GPIO_Pin==GPIO_PIN_0)
	{
			//while(GPIO_Pin==GPIO_PIN_2);
			ena_chinh++;
			if(ena_chinh>7)
			{
				ena_chinh=0;
				if(HAL_RTCEx_BKUPRead(&hrtc, RTC_BKP_DR1)!=0x32F2)// cap nhat lai thoi gian
				{
					set_time();
				}
			}
			if(ena_chinh==1)
			{
				chinh_giay=1;
				chinh_phut=0;
				chinh_gio=0;
				
				chinh_ngay=0;
				chinh_thang=0;
				chinh_nam=0;
				
				chinh_thu=0;
			}
			else if(ena_chinh==2)
			{
				chinh_giay=0;
				chinh_phut=1;
				chinh_gio=0;
				
				chinh_ngay=0;
				chinh_thang=0;
				chinh_nam=0;
				
				chinh_thu=0;
			}
			else if(ena_chinh==3)
			{
				chinh_giay=0;
				chinh_phut=0;
				chinh_gio=1;
				
				chinh_ngay=0;
				chinh_thang=0;
				chinh_nam=0;
				
				chinh_thu=0;
			}
			else if(ena_chinh==4)
			{
				chinh_giay=0;
				chinh_phut=0;
				chinh_gio=0;
				
				chinh_ngay=1;
				chinh_thang=0;
				chinh_nam=0;
				
				chinh_thu=0;
			}
			else if(ena_chinh==5)
			{
				chinh_giay=0;
				chinh_phut=0;
				chinh_gio=0;
				
				chinh_ngay=0;
				chinh_thang=1;
				chinh_nam=0;
				
				chinh_thu=0;
			}
			else if(ena_chinh==6)
			{
				chinh_giay=0;
				chinh_phut=0;
				chinh_gio=0;
				
				chinh_ngay=0;
				chinh_thang=0;
				chinh_nam=1;
				
				chinh_thu=0;
			}
			else if(ena_chinh==7)
			{
				chinh_giay=0;
				chinh_phut=0;
				chinh_gio=0;
				
				chinh_ngay=0;
				chinh_thang=0;
				chinh_nam=0;
				
				chinh_thu=1;
			}
		}
	}
		if(GPIO_Pin==GPIO_PIN_3)
		{
			for(int i=0; i<50000;i++);
		if(GPIO_Pin==GPIO_PIN_3)
	{
			if(chinh_giay==1)
			{
				giay++;
				if(giay>59)
				{
					giay=0;
				}
			}
			else if(chinh_phut==1)
			{
				phut++;
				if(phut>59)
				{
					phut=0;
				}
			}
			else if(chinh_gio==1)
			{
				gio++;
				if(gio>23)
				{
					gio=0;
				}
			}
			else if(chinh_ngay==1)
			{
				ngay++;
				if(ngay>31)
				{
					ngay=1;
				}
			}
			else if(chinh_thang==1)
			{
				thang++;
				if(thang>12)
				{
					thang=1;
				}
			}
			else if(chinh_nam==1)
			{
				nam++;
			}
			else if(chinh_thu==1)
			{
				thu++;
				if(thu>7)
				{
					thu=1;
				}
			}
		
		}
	}
		if(GPIO_Pin==GPIO_PIN_4)
		{
			for(int i=0; i<50000;i++);
		if(GPIO_Pin==GPIO_PIN_4)
	{
			if(chinh_giay==1)
			{
				giay--;
				if(giay<0)
				{
					giay=59;
				}
			}
			else if(chinh_phut==1)
			{
				phut--;
				if(phut<0)
				{
					phut=59;
				}
			}
			else if(chinh_gio==1)
			{
				gio--;
				if(gio<0)
				{
					gio=23;
				}
			}
			else if(chinh_ngay==1)
			{
				ngay--;
				if(ngay<0)
				{
					ngay=31;
				}
			}
			else if(chinh_thang==1)
			{
				thang--;
				if(thang<0)
				{
					thang=12;
				}
			}
			else if(chinh_nam==1)
			{
				nam--;
			}
			else if(chinh_thu==1)
			{
				thu--;
				if(thu<1)
				{
					thu=7;
				}
			}
	}
		}
}

void Update_time(void)
{
  if (giay	!= sTime.Seconds)
  {
    sTime.Seconds = giay;
  }
	else if (phut != sTime.Minutes)
  {
    sTime.Minutes = phut;
  }
	else if (gio != sTime.Hours)
  {
    sTime.Hours = gio;
  }
	else if (ngay!=sDate.Date)
	{
		sDate.Date=ngay;
	}
	else if (thang!=sDate.Month)
	{
		sDate.Month=thang;
	}
	else if (nam!=sDate.Year)
	{
		sDate.Year=nam;
	}
	else if (thu!=sDate.WeekDay)
	{
		sDate.WeekDay=thu;
	}
  if (HAL_RTC_SetTime(&hrtc, &sTime, RTC_FORMAT_BIN) != HAL_OK)
    {
      Error_Handler();
    }
	if (HAL_RTC_SetDate(&hrtc, &sDate, RTC_FORMAT_BIN) != HAL_OK)
    {
      Error_Handler();
    }
}
void set_time(void)
{
	RTC_TimeTypeDef sTime = {0};
  RTC_DateTypeDef sDate = {0};
	sTime.Hours = 0x10;
  sTime.Minutes = 0x58;
  sTime.Seconds = 0x30;
  sTime.DayLightSaving = RTC_DAYLIGHTSAVING_NONE;
  sTime.StoreOperation = RTC_STOREOPERATION_RESET;
  if (HAL_RTC_SetTime(&hrtc, &sTime, RTC_FORMAT_BCD) != HAL_OK)
  {
    Error_Handler();
  }
  sDate.WeekDay = RTC_WEEKDAY_THURSDAY;
  sDate.Month = RTC_MONTH_JUNE;
  sDate.Date = 0x11;
  sDate.Year = 0x23;

  if (HAL_RTC_SetDate(&hrtc, &sDate, RTC_FORMAT_BCD) != HAL_OK)
  {
    Error_Handler();
  }
}

void set_alarm(void)
{
  RTC_AlarmTypeDef sAlarm = {0};
	sAlarm.AlarmTime.Hours = 0x0;
  sAlarm.AlarmTime.Minutes = 0x0;
  sAlarm.AlarmTime.Seconds = 0x20;
  sAlarm.AlarmTime.SubSeconds = 0x0;
  sAlarm.AlarmTime.DayLightSaving = RTC_DAYLIGHTSAVING_NONE;
  sAlarm.AlarmTime.StoreOperation = RTC_STOREOPERATION_RESET;
  sAlarm.AlarmMask = RTC_ALARMMASK_NONE;
  sAlarm.AlarmSubSecondMask = RTC_ALARMSUBSECONDMASK_ALL;
  sAlarm.AlarmDateWeekDaySel = RTC_ALARMDATEWEEKDAYSEL_DATE;
  sAlarm.AlarmDateWeekDay = 0x10;
  sAlarm.Alarm = RTC_ALARM_A;
  if (HAL_RTC_SetAlarm_IT(&hrtc, &sAlarm, RTC_FORMAT_BCD) != HAL_OK)
  {
    Error_Handler();
  }
}
//RTC
void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{
    if (htim->Instance == TIM2)
    { 
					xuatmaled();	
    }
}

void Mode_dh()
{
	khoitaoRGB();
	if(chon_dh==0)
	{
		manhinh_chinhsua();
	}
	else if(chon_dh==1)
	{
		dongho_so();
	}
	else
	{
		dongho_kim();
	}
}

int main(void)
{
	dem1=0;
	dem2=0;

  HAL_Init();
  SystemClock_Config();

  MX_GPIO_Init();
  MX_TIM2_Init();
	MX_TIM3_Init();
	MX_I2C1_Init();
  MX_RTC_Init();
  MX_USART1_UART_Init();
	HAL_UART_Receive_IT(&huart1, (uint8_t *)rx_data, 1);
	HAL_TIM_Base_Start_IT(&htim2);
	HAL_TIM_PWM_Start(&htim3,TIM_CHANNEL_1);
	lcd_init();
  while (1)
  {
		HAL_RTC_GetTime(&hrtc, &sTime, RTC_FORMAT_BIN);
		HAL_RTC_GetDate(&hrtc, &sDate, RTC_FORMAT_BIN);
		if(TT_wifi==1)
		{
			gio_ht=gio_esp;
			phut_ht=phut_esp;
			giay_ht=giay_esp;
			
			ngay_ht=ngay_esp;
			thang_ht=thang_esp;
			nam_ht= nam_esp;
			
			thu_ht=thu_esp;
		}
		else
		{
			 					
			gio_ht = sTime.Hours;
			phut_ht = sTime.Minutes;
			giay_ht = sTime.Seconds;
			
			ngay_ht = sDate.Date;
			thang_ht = sDate.Month;
			nam_ht = sDate.Year;
			
			thu_ht = sDate.WeekDay;
		}
		 if (ena_chinh>0)
    {
      Update_time(); 
    }
		Mode_dh();
		update();
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
  __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE1);

  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI|RCC_OSCILLATORTYPE_LSI;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.HSICalibrationValue = RCC_HSICALIBRATION_DEFAULT;
  RCC_OscInitStruct.LSIState = RCC_LSI_ON;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSI;
  RCC_OscInitStruct.PLL.PLLM = 8;
  RCC_OscInitStruct.PLL.PLLN = 168;
  RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV2;
  RCC_OscInitStruct.PLL.PLLQ = 4;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }

  /** Initializes the CPU, AHB and APB buses clocks
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV4;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV2;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_5) != HAL_OK)
  {
    Error_Handler();
  }
}

/**
  * @brief I2C1 Initialization Function
  * @param None
  * @retval None
  */

/**
  * @brief RTC Initialization Function
  * @param None
  * @retval None
  */
static void MX_RTC_Init(void)
{

  /* USER CODE BEGIN RTC_Init 0 */

  /* USER CODE END RTC_Init 0 */

/*  RTC_TimeTypeDef sTime = {0};
  RTC_DateTypeDef sDate = {0};
  RTC_AlarmTypeDef sAlarm = {0};
*/
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
	
HAL_RTCEx_BKUPWrite(&hrtc, RTC_BKP_DR1, 0x32F2);
	if (HAL_RTCEx_BKUPRead(&hrtc, RTC_BKP_DR1) != 0x32F2)
	{
			__HAL_RTC_WRITEPROTECTION_DISABLE(&hrtc); // Disable write protection
			HAL_RTCEx_EnableBypassShadow(&hrtc); // Enable bypass shadow register
			__HAL_RTC_WRITEPROTECTION_ENABLE(&hrtc); // Enable write protection
			set_time(); // Set the initial time
	}
  /* USER CODE BEGIN Check_RTC_BKUP */

  /* USER CODE END Check_RTC_BKUP */

  /** Initialize RTC and set the Time and Date
  */
  

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
  htim2.Init.Prescaler = 20;
  htim2.Init.CounterMode = TIM_COUNTERMODE_UP;
  htim2.Init.Period = 599;
  htim2.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
  htim2.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_DISABLE;
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
static void MX_TIM3_Init(void)
{

  /* USER CODE BEGIN TIM3_Init 0 */

  /* USER CODE END TIM3_Init 0 */

  TIM_ClockConfigTypeDef sClockSourceConfig = {0};
  TIM_MasterConfigTypeDef sMasterConfig = {0};
  TIM_OC_InitTypeDef sConfigOC = {0};

  /* USER CODE BEGIN TIM3_Init 1 */

  /* USER CODE END TIM3_Init 1 */
  htim3.Instance = TIM3;
  htim3.Init.Prescaler = 83;
  htim3.Init.CounterMode = TIM_COUNTERMODE_UP;
  htim3.Init.Period = 999;
  htim3.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
  htim3.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_DISABLE;
  if (HAL_TIM_Base_Init(&htim3) != HAL_OK)
  {
    Error_Handler();
  }
  sClockSourceConfig.ClockSource = TIM_CLOCKSOURCE_INTERNAL;
  if (HAL_TIM_ConfigClockSource(&htim3, &sClockSourceConfig) != HAL_OK)
  {
    Error_Handler();
  }
  if (HAL_TIM_PWM_Init(&htim3) != HAL_OK)
  {
    Error_Handler();
  }
  sMasterConfig.MasterOutputTrigger = TIM_TRGO_RESET;
  sMasterConfig.MasterSlaveMode = TIM_MASTERSLAVEMODE_DISABLE;
  if (HAL_TIMEx_MasterConfigSynchronization(&htim3, &sMasterConfig) != HAL_OK)
  {
    Error_Handler();
  }
  sConfigOC.OCMode = TIM_OCMODE_PWM1;
  sConfigOC.Pulse = 0;
  sConfigOC.OCPolarity = TIM_OCPOLARITY_HIGH;
  sConfigOC.OCFastMode = TIM_OCFAST_DISABLE;
  if (HAL_TIM_PWM_ConfigChannel(&htim3, &sConfigOC, TIM_CHANNEL_1) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN TIM3_Init 2 */

  /* USER CHAL_TIM_MspPostInitODE END TIM3_Init 2 */
  HAL_TIM_MspPostInit(&htim3);

}


/**
  * @brief GPIO Initialization Function
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
  huart1.Init.BaudRate = 115200;
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

static void MX_GPIO_Init(void)
{
  GPIO_InitTypeDef GPIO_InitStruct = {0};
/* USER CODE BEGIN MX_GPIO_Init_1 */
/* USER CODE END MX_GPIO_Init_1 */

  /* GPIO Ports Clock Enable */
  __HAL_RCC_GPIOE_CLK_ENABLE();
  __HAL_RCC_GPIOH_CLK_ENABLE();
  __HAL_RCC_GPIOB_CLK_ENABLE();
  __HAL_RCC_GPIOA_CLK_ENABLE();
	__HAL_RCC_GPIOC_CLK_ENABLE();

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOB, GPIO_PIN_12, GPIO_PIN_RESET);

  /*Configure GPIO pins : PE3 PE4 */
  GPIO_InitStruct.Pin = GPIO_PIN_3|GPIO_PIN_4|GPIO_PIN_2;
  GPIO_InitStruct.Mode = GPIO_MODE_IT_FALLING;
  GPIO_InitStruct.Pull = GPIO_PULLUP;
  HAL_GPIO_Init(GPIOE, &GPIO_InitStruct);

	GPIO_InitStruct.Pin = GPIO_PIN_0;
  GPIO_InitStruct.Mode = GPIO_MODE_IT_RISING;
  GPIO_InitStruct.Pull = GPIO_PULLDOWN;
  HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);
	
  /*Configure GPIO pin : PB12 */
  GPIO_InitStruct.Pin = GPIO_PIN_12;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);
	
	// matrix
	/*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOA, GPIO_PIN_1|GPIO_PIN_2|GPIO_PIN_3|GPIO_PIN_4|GPIO_PIN_5|GPIO_PIN_7, GPIO_PIN_RESET);

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOE, GPIO_PIN_7|GPIO_PIN_8|GPIO_PIN_9|GPIO_PIN_10|GPIO_PIN_11|GPIO_PIN_12|GPIO_PIN_15, GPIO_PIN_RESET);

  /*Configure GPIO pins : PA1 PA2 PA3 PA4
                           PA5 PA8 */
  GPIO_InitStruct.Pin = GPIO_PIN_1|GPIO_PIN_2|GPIO_PIN_3|GPIO_PIN_4|GPIO_PIN_5|GPIO_PIN_7;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
  HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

  /*Configure GPIO pins : PE7 PE8 PE9 PE10
                           PE11 PE12 PE13 PE14 */
  GPIO_InitStruct.Pin = GPIO_PIN_7|GPIO_PIN_8|GPIO_PIN_9|GPIO_PIN_10|GPIO_PIN_11|GPIO_PIN_12|GPIO_PIN_15;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
  HAL_GPIO_Init(GPIOE, &GPIO_InitStruct);
/*Configure GPIO pin : PB11 */
  GPIO_InitStruct.Pin = GPIO_PIN_11;
  GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
  GPIO_InitStruct.Alternate = GPIO_AF1_TIM2;
  HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);
	
	GPIO_InitStruct.Pin = GPIO_PIN_0;
  GPIO_InitStruct.Mode = GPIO_MODE_IT_RISING;
  GPIO_InitStruct.Pull = GPIO_PULLDOWN;
  HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);
  /* EXTI interrupt init*/
  HAL_NVIC_SetPriority(EXTI3_IRQn, 2, 0);
  HAL_NVIC_EnableIRQ(EXTI3_IRQn);

  HAL_NVIC_SetPriority(EXTI4_IRQn, 2, 0);
  HAL_NVIC_EnableIRQ(EXTI4_IRQn);

	HAL_NVIC_SetPriority(EXTI0_IRQn, 2, 0);
  HAL_NVIC_EnableIRQ(EXTI0_IRQn);
	
	HAL_NVIC_SetPriority(EXTI2_IRQn, 2, 0);
  HAL_NVIC_EnableIRQ(EXTI2_IRQn);

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

#ifdef  USE_FULL_ASSERT
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
