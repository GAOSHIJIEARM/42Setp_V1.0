#include <M5Stack.h>

int Dir = 0 ,Speed = 0,Distance = 0;
char flag = 0;
#define DEV_ID 0Xaa
#define Set 1
#define Reset 0

char Sign;//步进电机行进位置

int Send_Data[10];//ID 方向 速度 距离 复位   给步进电机的参数
int Receive_Data[10];//发送给电脑的参数 验证接收是否正确

void setup() 
{
  M5.begin(true, false, true);
  Serial.begin(4800);
  // Serial2.begin(unsigned long baud, uint32_t config, int8_t rxPin, int8_t txPin, bool invert)
  Serial2.begin(4800, SERIAL_8N1, 16, 17);
  M5.Lcd.setTextSize(2);
  M5.Lcd.setCursor(140, 135);
  M5.Lcd.print("SEND");
  M5.Lcd.setTextSize(1);
  M5.Lcd.setCursor(0, 180);
  M5.Lcd.print("Dir:");
  M5.Lcd.setCursor(100, 180);
  M5.Lcd.print("Speed:");
  M5.Lcd.setCursor(200, 180);
  M5.Lcd.print("Distance:");
  
  M5.Lcd.setTextColor(YELLOW);
  M5.Lcd.setTextSize(2);
  M5.Lcd.setCursor(58, 210);
  M5.Lcd.print("+");
  M5.Lcd.setCursor(145, 210);
  M5.Lcd.print("Y/N");
  M5.Lcd.setCursor(248, 210);
  M5.Lcd.print("-");
  M5.Lcd.drawRect(40, 200, 50, 30, BLUE);
  M5.Lcd.drawRect(135, 200, 50, 30, BLUE);
  M5.Lcd.drawRect(230, 200, 50, 30, BLUE);
  
  M5.Lcd.setTextColor(WHITE);
  M5.Lcd.setTextSize(1);
  Display();
}
void Display()
{
  M5.Lcd.fillRect(155,155,20,10,BLACK);
  M5.Lcd.setCursor(30,180);
  M5.Lcd.fillRect(25,175,50,20,BLACK);
  M5.Lcd.print(Dir);//方向
  M5.Lcd.setCursor(150, 180);
  M5.Lcd.fillRect(145,175,50,20,BLACK);
  M5.Lcd.print(Speed);//速度
  M5.Lcd.setCursor(260, 180);
  M5.Lcd.fillRect(255,175,50,20,BLACK);//下次更新数据时 消除之前显示
  M5.Lcd.print(Distance);
  
  M5.Lcd.drawLine(40,50,250,50,WHITE);
  M5.Lcd.drawLine(40,45,40,50,WHITE);
  M5.Lcd.drawLine(70,45,70,50,WHITE);
  M5.Lcd.drawLine(100,45,100,50,WHITE);
  M5.Lcd.drawLine(130,45,130,50,WHITE);
  M5.Lcd.drawLine(160,45,160,50,WHITE);
  M5.Lcd.drawLine(190,45,190,50,WHITE);
  M5.Lcd.drawLine(220,45,220,50,WHITE);
  M5.Lcd.drawLine(250,45,250,50,WHITE);
  M5.Lcd.drawTriangle(40,55,35,60,45,60,WHITE);
  
  switch(Sign)
  {
	case 0:{M5.Lcd.fillRect(35,55,300,6,BLACK);M5.Lcd.drawTriangle(40+(Sign*10),55,35+(Sign*10),60,45+(Sign*10),60,WHITE); }break;
	case 1:{M5.Lcd.fillRect(35,55,300,6,BLACK);M5.Lcd.drawTriangle(40+(Sign*10),55,35+(Sign*10),60,45+(Sign*10),60,WHITE); }break;
	case 2:{M5.Lcd.fillRect(35,55,300,6,BLACK);M5.Lcd.drawTriangle(40+(Sign*10),55,35+(Sign*10),60,45+(Sign*10),60,WHITE); }break;
	case 3:{M5.Lcd.fillRect(35,55,300,6,BLACK);M5.Lcd.drawTriangle(40+(Sign*10),55,35+(Sign*10),60,45+(Sign*10),60,WHITE); }break;
	case 4:{M5.Lcd.fillRect(35,55,300,6,BLACK);M5.Lcd.drawTriangle(40+(Sign*10),55,35+(Sign*10),60,45+(Sign*10),60,WHITE); }break;
	case 5:{M5.Lcd.fillRect(35,55,300,6,BLACK);M5.Lcd.drawTriangle(40+(Sign*10),55,35+(Sign*10),60,45+(Sign*10),60,WHITE); }break;
	case 6:{M5.Lcd.fillRect(35,55,300,6,BLACK);M5.Lcd.drawTriangle(40+(Sign*10),55,35+(Sign*10),60,45+(Sign*10),60,WHITE); }break;
	case 7:{M5.Lcd.fillRect(35,55,300,6,BLACK);M5.Lcd.drawTriangle(40+(Sign*10),55,35+(Sign*10),60,45+(Sign*10),60,WHITE); }break;
	case 8:{M5.Lcd.fillRect(35,55,300,6,BLACK);M5.Lcd.drawTriangle(40+(Sign*10),55,35+(Sign*10),60,45+(Sign*10),60,WHITE); }break;
	case 9:{M5.Lcd.fillRect(35,55,300,6,BLACK);M5.Lcd.drawTriangle(40+(Sign*10),55,35+(Sign*10),60,45+(Sign*10),60,WHITE); }break;
	case 10:{M5.Lcd.fillRect(35,55,300,6,BLACK);M5.Lcd.drawTriangle(40+(Sign*10),55,35+(Sign*10),60,45+(Sign*10),60,WHITE); }break;
	case 11:{M5.Lcd.fillRect(35,55,300,6,BLACK);M5.Lcd.drawTriangle(40+(Sign*10),55,35+(Sign*10),60,45+(Sign*10),60,WHITE); }break;
	case 12:{M5.Lcd.fillRect(35,55,300,6,BLACK);M5.Lcd.drawTriangle(40+(Sign*10),55,35+(Sign*10),60,45+(Sign*10),60,WHITE); }break;
	case 13:{M5.Lcd.fillRect(35,55,300,6,BLACK);M5.Lcd.drawTriangle(40+(Sign*10),55,35+(Sign*10),60,45+(Sign*10),60,WHITE); }break;
	case 14:{M5.Lcd.fillRect(35,55,300,6,BLACK);M5.Lcd.drawTriangle(40+(Sign*10),55,35+(Sign*10),60,45+(Sign*10),60,WHITE); }break;
	case 15:{M5.Lcd.fillRect(35,55,300,6,BLACK);M5.Lcd.drawTriangle(40+(Sign*10),55,35+(Sign*10),60,45+(Sign*10),60,WHITE); }break;
	case 16:{M5.Lcd.fillRect(35,55,300,6,BLACK);M5.Lcd.drawTriangle(40+(Sign*10),55,35+(Sign*10),60,45+(Sign*10),60,WHITE); }break;
	case 17:{M5.Lcd.fillRect(35,55,300,6,BLACK);M5.Lcd.drawTriangle(40+(Sign*10),55,35+(Sign*10),60,45+(Sign*10),60,WHITE); }break;
	case 18:{M5.Lcd.fillRect(35,55,300,6,BLACK);M5.Lcd.drawTriangle(40+(Sign*10),55,35+(Sign*10),60,45+(Sign*10),60,WHITE); }break;
	case 19:{M5.Lcd.fillRect(35,55,300,6,BLACK);M5.Lcd.drawTriangle(40+(Sign*10),55,35+(Sign*10),60,45+(Sign*10),60,WHITE); }break;
	case 20:{M5.Lcd.fillRect(35,55,300,6,BLACK);M5.Lcd.drawTriangle(40+(Sign*10),55,35+(Sign*10),60,45+(Sign*10),60,WHITE); }break;
	case 21:{M5.Lcd.fillRect(35,55,300,6,BLACK);M5.Lcd.drawTriangle(40+(Sign*10),55,35+(Sign*10),60,45+(Sign*10),60,WHITE); }break;
//	case 22:{M5.Lcd.fillRect(35,55,300,6,BLACK);M5.Lcd.drawTriangle(40+(Sign*10),55,35+(Sign*10),60,45+(Sign*10),60,WHITE); }break;
//	case 23:{M5.Lcd.fillRect(35,55,300,6,BLACK);M5.Lcd.drawTriangle(40+(Sign*10),55,35+(Sign*10),60,45+(Sign*10),60,WHITE); }break;
//	case 24:{M5.Lcd.fillRect(35,55,300,6,BLACK);M5.Lcd.drawTriangle(40+(Sign*10),55,35+(Sign*10),60,45+(Sign*10),60,WHITE); }break;
	default : break;
  }
  
  M5.Lcd.setCursor(0,0);//2.26新加
  
  switch(flag)//标注更改值
  {
    case 0:{M5.Lcd.drawTriangle(160,155,155,160,165,160,WHITE);M5.Lcd.fillTriangle(160,155,155,160,165,160,WHITE);}break;
    case 1:{M5.Lcd.drawRect(25, 175, 50, 20, BLUE);}break;
    case 2:{M5.Lcd.drawRect(145, 175, 50, 20, BLUE);}break;
    case 3:{M5.Lcd.drawRect(255, 175, 50, 20, BLUE);}break;
    default : break;
    }
  }
void loop() 
{
  char i=0;
  M5.update();
  if (M5.BtnA.wasReleased()) 
  {
    switch(flag)
    {
      case 1:{Dir++;if(Dir>1) Dir = 1;}break;
      case 2:{Speed++;}break;
      case 3:{Distance++;Distance = Distance%22;}break;
      default : break;
    }
    Display();
  } 
  else if (M5.BtnB.wasReleased()) 
  {
    flag = flag + 1;
    flag = flag%4;
    Display();
  } 
  else if (M5.BtnC.wasReleased()) 
  {
    switch(flag)
    {
      case 1:{Dir--;if(Dir<0) Dir = 0;}break;
      case 2:{Speed--;}break;
      case 3:{Distance--;if(Distance<0) Distance = 0;}break;
      default : break;
    }
    Display();
  }
  else if (M5.BtnB.wasReleasefor(700)) 
  {
    Send_Data[0] = DEV_ID;
    Send_Data[1] = Dir;
    Send_Data[2] = Speed;
    Send_Data[3] = Distance;
    Send_Data[4] = Dir+Speed+Distance;
    Send_Data[5] = 0xff;
	
	if(Dir>0)
	{
		Sign = Sign+Distance;
	}
	else
	{
		Sign = Sign-Distance;
	}
	
    for(i=0;i<6;i++)
    {
      Serial2.write(Send_Data[i]);
      delayMicroseconds(20);
    }
    Dir = 0;
    Speed = 0;
    Distance = 0;
    flag = 0;
	
    Display();
  }
	for(i=0;i<6;i++)
	{
		Send_Data[i] = 0;
	}
	
  if(Serial2.available())
  {
    for(i=0;i<3;i++)
    {
     Receive_Data[i] = Serial2.read();
     Serial.write(Receive_Data[i]);
    }
    }
}
