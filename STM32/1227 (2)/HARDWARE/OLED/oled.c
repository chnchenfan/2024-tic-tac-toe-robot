#include "oled.h"
#include "stdlib.h"
#include "oledfont.h"
#include "delay.h"

u8 OLED_GRAM[144][8];

void delay(u32 t)
{
    while (t--);
}


void OLED_ColorTurn(u8 i)
{
    if (i == 0)
    {
        OLED_WR_Byte(0xA6, OLED_CMD); 
    }
    if (i == 1)
    {
        OLED_WR_Byte(0xA7, OLED_CMD); 
    }
}


void OLED_DisplayTurn(u8 i)
{
    if (i == 0)
    {
        OLED_WR_Byte(0xC8, OLED_CMD); 
        OLED_WR_Byte(0xA1, OLED_CMD);
    }
    if (i == 1)
    {
        OLED_WR_Byte(0xC0, OLED_CMD); 
        OLED_WR_Byte(0xA0, OLED_CMD);
    }
}


void I2C_Start(void)
{
    OLED_SDIN_Set();
    delay(1);
    OLED_SCLK_Set();
    delay(1);
    OLED_SDIN_Clr();
    delay(1);
    OLED_SCLK_Clr();
    delay(1);
}


void I2C_Stop(void)
{
    OLED_SCLK_Set();
    delay(1);
    OLED_SDIN_Clr();
    delay(1);
    OLED_SDIN_Set();
    delay(1);
}


void I2C_WaitAck(void) 
{
    OLED_SCLK_Set();
    delay(1);
    OLED_SCLK_Clr();
    delay(1);
}


void Send_Byte(u8 dat)
{
    u8 i;
    for (i = 0; i < 8; i++)
    {
        OLED_SCLK_Clr();
        delay(1);
        if (dat & 0x80) 
        {
            OLED_SDIN_Set();
            delay(1);
        }
        else
        {
            OLED_SDIN_Clr();
            delay(1);
        }
        OLED_SCLK_Set();
        delay(1);
        OLED_SCLK_Clr();
        delay(1);
        dat <<= 1;
    }
}




void OLED_WR_Byte(u8 dat, u8 mode)
{
    I2C_Start();
    Send_Byte(0x78);
    I2C_WaitAck();
    if (mode)
    {
        Send_Byte(0x40);
    }
    else
    {
        Send_Byte(0x00);
    }
    I2C_WaitAck();
    Send_Byte(dat);
    I2C_WaitAck();
    I2C_Stop();
}


void OLED_DisPlay_On(void)
{
    OLED_WR_Byte(0x8D, OLED_CMD); 
    OLED_WR_Byte(0x14, OLED_CMD); 
    OLED_WR_Byte(0xAF, OLED_CMD); 
}


void OLED_DisPlay_Off(void)
{
    OLED_WR_Byte(0x8D, OLED_CMD); 
    OLED_WR_Byte(0x10, OLED_CMD); 
    OLED_WR_Byte(0xAF, OLED_CMD); 
}


void OLED_Refresh(void)
{
    u8 i, n;
    for (i = 0; i < 8; i++)
    {
        OLED_WR_Byte(0xb0 + i, OLED_CMD); 
        OLED_WR_Byte(0x00, OLED_CMD);  
        OLED_WR_Byte(0x10, OLED_CMD);  
        for (n = 0; n < 128; n++)
            OLED_WR_Byte(OLED_GRAM[n][i], OLED_DATA);
    }
}

void OLED_Clear(void)
{
    u8 i, n;
    for (i = 0; i < 8; i++)
    {
        for (n = 0; n < 128; n++)
        {
            OLED_GRAM[n][i] = 0; 
        }
    }
    OLED_Refresh();
}




void OLED_DrawPoint(u8 x, u8 y)
{
    u8 i, m, n;
    i = y / 8;
    m = y % 8;
    n = 1 << m;
    OLED_GRAM[x][i] |= n;
}




void OLED_ClearPoint(u8 x, u8 y)
{
    u8 i, m, n;
    i = y / 8;
    m = y % 8;
    n = 1 << m;
    OLED_GRAM[x][i] = ~OLED_GRAM[x][i];
    OLED_GRAM[x][i] |= n;
    OLED_GRAM[x][i] = ~OLED_GRAM[x][i];
}




void OLED_DrawCircle(u8 x, u8 y, u8 r)
{
    int a, b, num;
    a = 0;
    b = r;
    while (2 * b * b >= r * r)
    {
        OLED_DrawPoint(x + a, y - b);
        OLED_DrawPoint(x - a, y - b);
        OLED_DrawPoint(x - a, y + b);
        OLED_DrawPoint(x + a, y + b);

        OLED_DrawPoint(x + b, y + a);
        OLED_DrawPoint(x + b, y - a);
        OLED_DrawPoint(x - b, y - a);
        OLED_DrawPoint(x - b, y + a);

        a++;
        num = (a * a + b * b) - r * r; 
        if (num > 0)
        {
            b--;
            a--;
        }
    }
}








void OLED_ShowChar(u8 x, u8 y, u8 chr, u8 size1)
{
    u8 i, m, temp, size2, chr1;
    u8 y0 = y;
    size2 = (size1 / 8 + ((size1 % 8) ? 1 : 0)) * (size1 / 2); 
    chr1 = chr - ' '; 
    for (i = 0; i < size2; i++)
    {
        if (size1 == 12)
        {
            temp = asc2_1206[chr1][i];   
        }
        else if (size1 == 16)
        {
            temp = asc2_1608[chr1][i];   
        }
        else if (size1 == 24)
        {
            temp = asc2_2412[chr1][i];   
        }
        else return;
        for (m = 0; m < 8; m++)    
        {
            if (temp & 0x80)OLED_DrawPoint(x, y);
            else OLED_ClearPoint(x, y);
            temp <<= 1;
            y++;
            if ((y - y0) == size1)
            {
                y = y0;
                x++;
                break;
            }
        }
    }
}






void OLED_ShowString(u8 x, u8 y, u8 *chr, u8 size1)
{
    while ((*chr >= ' ') && (*chr <= '~')) 
    {
        OLED_ShowChar(x, y, *chr, size1);
        x += size1 / 2;
        if (x > 128 - size1) 
        {
            x = 0;
            y += 2;
        }
        chr++;
    }
}


u32 OLED_Pow(u8 m, u8 n)
{
    u32 result = 1;
    while (n--)
    {
        result *= m;
    }
    return result;
}





void OLED_ShowNum(u8 x, u8 y, u32 num, u8 len, u8 size1)
{
    u8 t, temp;
    for (t = 0; t < len; t++)
    {
        temp = (num / OLED_Pow(10, len - t - 1)) % 10;
        if (temp == 0)
        {
            OLED_ShowChar(x + (size1 / 2)*t, y, '0', size1);
        }
        else
        {
            OLED_ShowChar(x + (size1 / 2)*t, y, temp + '0', size1);
        }
    }
}





void OLED_ShowChinese(u8 x, u8 y, u8 num, u8 size1)
{
    u8 i, m, n = 0, temp, chr1;
    u8 x0 = x, y0 = y;
    u8 size3 = size1 / 8;
    while (size3--)
    {
        chr1 = num * size1 / 8 + n;
        n++;
        for (i = 0; i < size1; i++)
        {
            if (size1 == 16)
            {
                temp = Hzk1[chr1][i];   
            }
            else if (size1 == 24)
            {
                temp = Hzk2[chr1][i];   
            }
            else if (size1 == 32)
            {
                temp = Hzk3[chr1][i];   
            }
            else if (size1 == 64)
            {
                temp = Hzk4[chr1][i];   
            }
            else return;

            for (m = 0; m < 8; m++)
            {
                if (temp & 0x01)OLED_DrawPoint(x, y);
                else OLED_ClearPoint(x, y);
                temp >>= 1;
                y++;
            }
            x++;
            if ((x - x0) == size1)
            {
                x = x0;
                y0 = y0 + 8;
            }
            y = y0;
        }
    }
}



void OLED_ScrollDisplay(u8 num, u8 space)
{
    u8 i, n, t = 0, m = 0, r;
    while (1)
    {
        if (m == 0)
        {
            OLED_ShowChinese(128, 24, t, 16); 
            t++;
        }
        if (t == num)
        {
            for (r = 0; r < 16 * space; r++) 
            {
                for (i = 0; i < 144; i++)
                {
                    for (n = 0; n < 8; n++)
                    {
                        OLED_GRAM[i - 1][n] = OLED_GRAM[i][n];
                    }
                }
                OLED_Refresh();
            }
            t = 0;
        }
        m++;
        if (m == 16)
        {
            m = 0;
        }
        for (i = 0; i < 144; i++) 
        {
            for (n = 0; n < 8; n++)
            {
                OLED_GRAM[i - 1][n] = OLED_GRAM[i][n];
            }
        }
        OLED_Refresh();
    }
}


void OLED_WR_BP(u8 x, u8 y)
{
    OLED_WR_Byte(0xb0 + y, OLED_CMD); 
    OLED_WR_Byte(((x & 0xf0) >> 4) | 0x10, OLED_CMD);
    OLED_WR_Byte((x & 0x0f), OLED_CMD);
}




void OLED_ShowPicture(u8 x0, u8 y0, u8 x1, u8 y1, u8 BMP[])
{
    u32 j = 0;
    u8 x = 0, y = 0;
    if (y % 8 == 0)y = 0;
    else y += 1;
    for (y = y0; y < y1; y++)
    {
        OLED_WR_BP(x0, y);
        for (x = x0; x < x1; x++)
        {
            OLED_WR_Byte(BMP[j], OLED_DATA);
            j++;
        }
    }
}

void OLED_Init(void)
{
    GPIO_InitTypeDef  GPIO_InitStructure;

    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOD, ENABLE); 

    
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_1 | GPIO_Pin_0 ;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
    GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
    GPIO_Init(GPIOD, &GPIO_InitStructure);


    delay_ms(200);

    OLED_WR_Byte(0xAE, OLED_CMD); 
    OLED_WR_Byte(0x00, OLED_CMD); 
    OLED_WR_Byte(0x10, OLED_CMD); 
    OLED_WR_Byte(0x40, OLED_CMD); 
    OLED_WR_Byte(0x81, OLED_CMD); 
    OLED_WR_Byte(0xCF, OLED_CMD); 
    OLED_WR_Byte(0xA1, OLED_CMD); 
    OLED_WR_Byte(0xC8, OLED_CMD); 
    OLED_WR_Byte(0xA6, OLED_CMD); 
    OLED_WR_Byte(0xA8, OLED_CMD); 
    OLED_WR_Byte(0x3f, OLED_CMD); 
    OLED_WR_Byte(0xD3, OLED_CMD); 
    OLED_WR_Byte(0x00, OLED_CMD); 
    OLED_WR_Byte(0xd5, OLED_CMD); 
    OLED_WR_Byte(0x80, OLED_CMD); 
    OLED_WR_Byte(0xD9, OLED_CMD); 
    OLED_WR_Byte(0xF1, OLED_CMD); 
    OLED_WR_Byte(0xDA, OLED_CMD); 
    OLED_WR_Byte(0x12, OLED_CMD);
    OLED_WR_Byte(0xDB, OLED_CMD); 
    OLED_WR_Byte(0x40, OLED_CMD); 
    OLED_WR_Byte(0x20, OLED_CMD); 
    OLED_WR_Byte(0x02, OLED_CMD); 
    OLED_WR_Byte(0x8D, OLED_CMD); 
    OLED_WR_Byte(0x14, OLED_CMD); 
    OLED_WR_Byte(0xA4, OLED_CMD); 
    OLED_WR_Byte(0xA6, OLED_CMD); 
    OLED_WR_Byte(0xAF, OLED_CMD);
    OLED_Clear();
}
