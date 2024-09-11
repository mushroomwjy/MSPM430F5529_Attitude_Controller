
#include "parameter_type.h"
#include "mpu6050.h"
#include "gpio.h"

void ByteWrite6050(uint8 REG_Address,uint8 REG_data);
uint8 ByteRead6050(uint8 REG_Address);
int Get6050Data(uint8 REG_Address);
void InitMPU6050();
float Mpu6050AccelAngle(int8 dir);
float Mpu6050GyroAngle(int8 dir);

#define    SCL_HIGH    P8OUT|=BIT1
#define    SCL_LOW     P8OUT&=~BIT1
#define    SDA_HIGH    P8OUT|=BIT2
#define    SDA_LOW     P8OUT&=~BIT2
#define    SDA_OUT     P8DIR|=BIT2
#define    SDA_IN      P8DIR&=~BIT2
#define    SCL_OUT     P8DIR|=BIT1
#define    SDA         P8IN&BIT2


void IIC_start();//
void IIC_stop();//
void SendACK(unsigned char ack);
void IIC_writebyte(unsigned char IIC_byte);///
unsigned char IIC_testACK();///
unsigned char IIC_readebyte();///



///�����ź�
void IIC_start()
{
    SDA_OUT;
    SCL_OUT;
    SCL_HIGH;
    SDA_HIGH;
    __delay_cycles(5);
    SDA_LOW;
    __delay_cycles(5);
    SCL_LOW;
}
///ֹͣ�ź�
void IIC_stop()
{
    SDA_OUT;
    SCL_OUT;
    SDA_LOW;
    SCL_HIGH;
    __delay_cycles(5);
    SDA_HIGH;
    __delay_cycles(5);
}


//����Ӧ���ź�(MCU=>||)
void SendACK(unsigned char ack)
{
    SDA_OUT;
    SCL_OUT;
if(ack==1)
{
    SDA_HIGH;
}
else if(ack==0)
{
    SDA_LOW;
}
else
    return;
SCL_HIGH;
__delay_cycles(5);
SCL_LOW;
__delay_cycles(5);
}
////����Ӧ���źţ�||=>MCU��
unsigned char IIC_testACK()
{
    unsigned char a;
    SCL_OUT;
    SDA_IN;
//GPIO_setAsInputPinWithPullUpResistor (GPIO_PORT_P8, GPIO_PIN2);
    SCL_HIGH;
    __delay_cycles(5);
    if(GPIO_getInputPinValue (GPIO_PORT_P8, GPIO_PIN2)==1)
    {
        a=1;
    }
    else
    {
        a=0;
    }

    SCL_LOW;
    __delay_cycles(5);
    SDA_OUT;
    return a;
}

///��IIC���߷������ݣ�MCU=>||��
void IIC_writebyte(unsigned char IIC_byte)
{
    unsigned char i;
    SDA_OUT;
    SCL_OUT;
//    SCL_LOW;
        for (i=0; i<8; i++)         //8λ������
            {
                if((IIC_byte<<i)&0x80)
                {
                    SDA_HIGH;
                }
                else
                {
                    SDA_LOW;
                }

        __delay_cycles(5);
        SCL_HIGH;
        __delay_cycles(5);
        SCL_LOW;
//        __delay_cycles(10);
//        IIC_byte<<=1;
    }

    IIC_testACK();
}


////IIC����һ���ֽ�(||����>MCU)
unsigned char IIC_readebyte()
{
    unsigned char i,k=0;
    SDA_IN;
    GPIO_setAsInputPinWithPullUpResistor (GPIO_PORT_P8, GPIO_PIN2);
    SCL_OUT;
    SCL_LOW;
    __delay_cycles(50);
    for(i=0;i<8;i++)
    {
        SCL_HIGH;
        k=k<<1;
        if(SDA)
            k|=1;
        SCL_LOW;
        __delay_cycles(50);
    }
    SDA_OUT;
    __delay_cycles(50);
    return k;
}

void ByteWrite6050(uint8 REG_Address,uint8 REG_data)
{
    IIC_start();                  //��ʼ�ź�
    IIC_writebyte(SlaveAddress);   //�����豸��ַ+д�ź�
    IIC_writebyte(REG_Address);    //�ڲ��Ĵ�����ַ��
    IIC_writebyte(REG_data);        //�ڲ��Ĵ������ݣ�
    IIC_stop();                   //����ֹͣ�ź�
}

uint8 ByteRead6050(uint8 REG_Address)
{
    uint8 REG_data;
    IIC_start();                   //��ʼ�ź�
    IIC_writebyte(SlaveAddress);     //�����豸��ַ+д�ź�
    IIC_writebyte(REG_Address);      //���ʹ洢��Ԫ��ַ����0��ʼ
    IIC_start();                   //��ʼ�ź�
    IIC_writebyte(SlaveAddress+1);  //�����豸��ַ+���ź�
    REG_data=IIC_readebyte();       //�����Ĵ�������
    SendACK(1);                //����Ӧ���ź�
    IIC_stop();                    //ֹͣ�ź�
    return REG_data;
}

int Get6050Data(uint8 REG_Address)
{
    char H,L;
    H=ByteRead6050(REG_Address);
    L=ByteRead6050(REG_Address+1);
    return (H<<8)+L;   //�ϳ�����
}

void InitMPU6050()
{
    ByteWrite6050(PWR_MGMT_1, 0x00);  // �������״̬
    ByteWrite6050(SMPLRT_DIV, 0x07);  // �����ǲ��������ã�125HZ��
    ByteWrite6050(CONFIG, 0x06);      // ��ͨ�˲������ã�5HZƵ�ʣ�
    ByteWrite6050(GYRO_CONFIG, 0x18); // �������Լ켰��ⷶΧ����(���Լ�,16.4LSB/DBS/S)
    ByteWrite6050(ACCEL_CONFIG, 0x01); // ���Լ죬����2g
}

float Mpu6050AccelAngle(int8 dir)
{
    float accel_agle; // ���������ֵ
    float result; // ����ֵ�������
    result = (float)Get6050Data(dir); // ������ǰ����ļ��ٶ�ֵ,ת��Ϊ������
    accel_agle = (result + MPU6050_ZERO_ACCELL)/16384; // ȥ�����ƫ��,����õ��Ƕȣ����ȣ�
    accel_agle = accel_agle*1.2*180/3.14;     //����ת��Ϊ��

    return accel_agle; // ���ز���ֵ
}

float Mpu6050GyroAngle(int8 dir)
{
    float gyro_angle;
    gyro_angle = (float)Get6050Data(dir);   // ��������ǵĵ�ǰֵ
    gyro_angle = -(gyro_angle + MPU6050_ZERO_GYRO)/16.4;    //ȥ�����ƫ�ƣ�������ٶ�ֵ,����Ϊ������

    return gyro_angle; // ���ز���ֵ
}

