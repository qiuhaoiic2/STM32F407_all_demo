#ifndef _DRIVER_DHT11_H
#define _DRIVER_DHT11_H



#include "main.h"


typedef struct dht11_t
{
    GPIO_TypeDef * gpiox;
    uint32_t  pin;
    uint8_t temperature;
    uint8_t humidity;
    uint8_t    is_valid;
    uint8_t    init;

}dht11_t;
#define  DHT11_GPIO_HIGH(DHT11)   GPIO_SetBits(DHT11->gpiox,DHT11->pin)
#define  DHT11_GPIO_LOW(DHT11)    GPIO_ResetBits(DHT11->gpiox,DHT11->pin)
#define  DHT11_GPIO_READ(DHT11)    GPIO_ReadInputDataBit(DHT11->gpiox,DHT11->pin)




uint8_t dht11_init(dht11_t *dht11,GPIO_TypeDef * gpiox,uint32_t pin);
int16_t  dht11_readdata(dht11_t  *dht11);
#endif /* _DRIVER_DHT11_H */
