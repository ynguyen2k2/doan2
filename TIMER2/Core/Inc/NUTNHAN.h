if(GPIO_Pin==GPIO_PIN_0)
	{
		ena_chinh++;
		if(ena_chinh>6)
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
		}
		else if(ena_chinh==2)
		{
			chinh_giay=0;
			chinh_phut=1;
			chinh_gio=0;
			
			chinh_ngay=0;
			chinh_thang=0;
			chinh_nam=0;
		}
		else if(ena_chinh==3)
		{
			chinh_giay=0;
			chinh_phut=0;
			chinh_gio=1;
			
			chinh_ngay=0;
			chinh_thang=0;
			chinh_nam=0;
		}
		else if(ena_chinh==4)
		{
			chinh_giay=0;
			chinh_phut=0;
			chinh_gio=0;
			
			chinh_ngay=1;
			chinh_thang=0;
			chinh_nam=0;
		}
		else if(ena_chinh==5)
		{
			chinh_giay=0;
			chinh_phut=0;
			chinh_gio=0;
			
			chinh_ngay=0;
			chinh_thang=1;
			chinh_nam=0;
		}
		else if(ena_chinh==6)
		{
			chinh_giay=0;
			chinh_phut=0;
			chinh_gio=0;
			
			chinh_ngay=0;
			chinh_thang=0;
			chinh_nam=1;
		}
	}
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
				ngay=0;
			}
		}
		else if(chinh_thang==1)
		{
			thang++;
			if(thang>12)
			{
				thang=0;
			}
		}
		else if(chinh_nam==1)
		{
			nam++;
		}
	}
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
	}