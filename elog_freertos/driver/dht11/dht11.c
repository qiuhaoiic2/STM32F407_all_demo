#include "dht11.h"


void dht11_out(dht11_t *dht11)
{

    GPIO_InitTypeDef GPIO_InitStructure;
    memset(&GPIO_InitStructure, 0, sizeof(GPIO_InitStructure));
    GPIO_InitStructure.GPIO_Mode    = GPIO_Mode_OUT;
    GPIO_InitStructure.GPIO_Pin     = dht11->pin;
    GPIO_InitStructure.GPIO_Speed   = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_OType   = GPIO_OType_PP;
    GPIO_InitStructure.GPIO_PuPd    = GPIO_PuPd_UP;
    GPIO_Init(dht11->gpiox, &GPIO_InitStructure);
}

void dht11_in(dht11_t *dht11)
{
    GPIO_InitTypeDef GPIO_InitStructure;
    memset(&GPIO_InitStructure, 0, sizeof(GPIO_InitStructure));
    GPIO_InitStructure.GPIO_Mode    = GPIO_Mode_IN;
    GPIO_InitStructure.GPIO_Pin     = dht11->pin;
    GPIO_InitStructure.GPIO_Speed   = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_PuPd    = GPIO_PuPd_UP;
    GPIO_Init(dht11->gpiox, &GPIO_InitStructure);
}



int8_t dht11_res(dht11_t * dht11)
{
    dht11_out(dht11);
    DHT11_GPIO_LOW(dht11);
    delay_ms(20);
    DHT11_GPIO_HIGH(dht11);
    delay_us(30);
    return 0;


}
int8_t dht11_check(dht11_t *dht11)
{
    uint16_t retry = 0;
    dht11_in(dht11);
    while (DHT11_GPIO_READ(dht11) && (retry < 100))
    {
        retry ++;
        delay_us(1);
    }
    if (retry >= 100)
    {
        return -1;
    }
    else
        retry = 0;
    while (DHT11_GPIO_READ(dht11) == RESET && (retry < 100))
    {
        retry ++;
        delay_us(1);
    }
    if (retry >= 100)
    {
        return -2;
    }
    return 0;
}

uint8_t dht11_readBit(dht11_t *dht11)
{
    uint16_t retry = 0;
    while (DHT11_GPIO_READ(dht11) && retry < 100)//µÈ´ý±äµÍ
    {
        retry ++;
        delay_us(1);
    }
    retry = 0;
    while (!DHT11_GPIO_READ(dht11) && retry < 100)
    {
        retry++;
        delay_us(1);
    }
    delay_us(40);
    if (DHT11_GPIO_READ(dht11) == SET)
        return 1;
    else
        return 0;
}

uint8_t dht11_readbyte(dht11_t *dht11)
{
    uint8_t i = 0;
    uint8_t data = 0;
    for (i = 0;i < 8;i++)
    {
        data <<= 1;
        data |= dht11_readBit(dht11);
    }
    return data;
}

int16_t  dht11_readdata(dht11_t  *dht11)
{
    if (dht11 == NULL || dht11->init == false)
        return -1;
    uint8_t buff[5];
    uint8_t i = 0;
    dht11_res(dht11);
    if (dht11_check(dht11) == 0)
    {
       for (i = 0;i < 5;i++)
       {
           buff[i] = dht11_readbyte(dht11);

       }
       if (buff[0] + buff[1] + buff[2] + buff[3] == buff[4])
       {
           dht11->humidity   = buff[0];
           dht11->temperature = buff[2];
           dht11->is_valid   = true;
       }
       else
           dht11->is_valid   = false;
    }

		return 0;


}




uint8_t dht11_init(dht11_t *dht11,GPIO_TypeDef * gpiox,uint32_t pin)
{
    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB, ENABLE);
    dht11->gpiox = gpiox;
    dht11->pin = pin;
    dht11->temperature = 0;
    dht11->humidity = 0;
    dht11->is_valid = false;
    dht11->init = false;
    dht11_out(dht11);
    dht11_res(dht11);
    if (dht11_check(dht11) == 0)
        dht11->init = 1;
    return 0;
}

