#define ARR_MAX 200  //加减速步数
const unsigned short AccStep[ARR_MAX]PROGMEM=
{
1560,1555,1550,1546,1538,1534,1527,1522,1515,1508,
1499,1493,1484,1477,1468,1460,1449,1441,1431,1420,
1410,1399,1389,1377,1364,1353,1340,1326,1314,1300,
1285,1271,1256,1241,1225,1211,1193,1178,1160,1144,
1126,1109,1092,1073,1055,1036,1018,1000,981,962,
943,924,905,887,867,849,830,811,792,774,
756,737,720,702,684,668,651,634,618,602,
586,570,556,541,527,513,499,486,473,460,
448,436,425,414,403,393,383,373,364,355,
346,338,329,322,314,307,300,294,287,281,
275,270,264,259,254,249,245,240,236,232,
229,225,221,218,215,212,209,206,204,201,
199,196,194,192,190,188,186,185,183,181,
180,178,177,176,175,173,172,171,170,169,
168,167,167,166,165,164,164,163,162,162,
161,161,160,160,159,159,158,158,157,157,
157,156,156,156,156,155,155,155,155,154,
154,154,154,154,153,153,153,153,153,153,
152,152,152,152,152,152,152,152,152,152,
152,151,151,151,151,151,151,151,151,151,
} ;//生成的加减速数组
// PROGMEM

//LED: PB4/PB5/PC2/PC3
#define Set_Bit(val, bitn)    (val |=(1<<(bitn)))
#define Clr_Bit(val, bitn)     (val&=~(1<<(bitn)))
#define Get_Bit(val, bitn)    (val &(1<<(bitn)) )
//MS PD2/3/4
//SETP PD6,PWM接口
//DIR PD5
//EN PB0
//LIMIT PB1
#define DEV_ID 0x01

#define LedRed1 Set_Bit(PORTB,4);Set_Bit(PORTC,3)
#define LedRed0 Clr_Bit(PORTB,4);Clr_Bit(PORTC,3)
#define LedGreen1 Set_Bit(PORTB,5);Set_Bit(PORTC,2)
#define LedGreen0 Clr_Bit(PORTB,5);Clr_Bit(PORTC,2)

#define Limit (PINB & 0x02) == 0x02//0000 0010
#define DisableMotor Set_Bit(PORTB,0)
#define EnableMotor Clr_Bit(PORTB,0)

#define MotorP1 Set_Bit(PORTD,6)
#define MotorP0 Clr_Bit(PORTD,6)
#define MotorD1 Set_Bit(PORTD,5)
#define MotorD0 Clr_Bit(PORTD,5)
#define MS1_1 Set_Bit(PORTD,2)
#define MS1_0 Clr_Bit(PORTD,2)
#define MS2_1 Set_Bit(PORTD,3)
#define MS2_0 Clr_Bit(PORTD,3)
#define MS3_1 Set_Bit(PORTD,4)
#define MS3_0 Clr_Bit(PORTD,4)

#define ACCEL   1  //电机运行状态标志位
#define DECEL   2
#define RUN     3
#define STOP   0
unsigned long step_count;  //步数计数
int acc_count;    	//加速计数
int dcc_count;    	//减速计数
uint8_t flag = STOP;//开始时的状态

int Dir,Speed,Distance;

void serialEvent()//串口接收
{
	static int aa = 0,i = 0;
	int Rec,Val,Buff[10];
	while(Serial.available())
	{
		Rec = Serial.read();
		if(Rec == 0xaa)//接收第一个字节 起始位
			aa=0x80;
		if(aa==0x80)
			Buff[i++]=Rec;
		if((i==6)&&(Buff[5]==0xff))//校验终止位
		{
			i=0;
			aa=0;
			Val = Buff[1]+Buff[2]+Buff[3];
			if(Val == Buff[4])
			{
				Dir = Buff[1];
				Speed = Buff[2];
				Distance = Buff[3];
			}
		}
	}
	if(Distance!=0)
	{
		flag = ACCEL; //等到下一个数据进入后进入加速状态
		EnableMotor;//使能电机
		SetSteps(Dir,Distance);//设置电机方向和步数
	}
}
void SetSteps(int DIR,unsigned int DISTANCE)//运行步数
{
	if(DIR>0)
		MotorD0;      //设定电机运行方向
	else
		MotorD1;
	step_count = (DISTANCE*2000);
	
}

void Motor_Reset()
{
    EnableMotor;	
	MotorD1;
	while(Limit)
    {
    LedGreen0;
    LedRed1;
    MotorP1;
    delayMicroseconds(255);//频率越大速度越大
    MotorP0;
    delayMicroseconds(255);
	}
	DisableMotor;
	LedGreen1;
	LedRed0;
	
}
void setup()//初始化
{
  DDRC = 0xff;//c端口初始化为输出
  DDRB = 0xff;
  PORTB = 0xff;//b端口初始化为高电平
  LedGreen1;
  LedRed0;
  DisableMotor;//驱动失能
  MS1_0;
  MS2_1;
  MS3_0;//
  Serial.begin(4800);
  Motor_Reset();
}
int NowTime = 0;
void Pulse()//产生脉冲
{
  switch(flag)  //查询状态
  {
    case STOP  :
	{
      acc_count =0;
      step_count = 0;
      dcc_count = ARR_MAX-1;
	  DisableMotor;//电机失能
	  
	  LedGreen1;
	  LedRed0;//停机指示灯
    }break;
    
    case ACCEL :
	{
      acc_count++;
      step_count--;
	  
	  LedGreen0;
	  LedRed1;    //运行指示灯
	  
      MotorP1; //输出一个脉冲
	  delayMicroseconds(20);
      MotorP0;
	  
      NowTime =  pgm_read_word_near(&AccStep[acc_count])*5;//查表
      if(acc_count == ARR_MAX-1) 
		  flag = RUN;
    }break;
 
    case RUN   :
	{
      step_count--;
	  
	  LedGreen0;
	  LedRed1;//运行指示灯
    
      MotorP1; //输出一个脉冲
	  delayMicroseconds(20);
      MotorP0;
	  NowTime =  pgm_read_word_near(&AccStep[ARR_MAX-1])*5;//查表
      if(step_count == ARR_MAX-1) //减到进入减速步数范围
      {
        flag = DECEL;
      } 
    }break;
    case DECEL :
	{
      dcc_count--;
      step_count--;
	  
	  LedGreen0;
	  LedRed1;//运行指示灯	  
      
      MotorP1; //输出一个脉冲
	  delayMicroseconds(20);
      MotorP0;

      
      NowTime =  pgm_read_word_near(&AccStep[dcc_count])*5;
      if(acc_count == 0 || step_count==0)
	  {		  
		  flag = STOP;
	  }
    }break;
  }
}
void loop() 
{
	
	while(NowTime--);
	Pulse();//产生脉冲函数
}
