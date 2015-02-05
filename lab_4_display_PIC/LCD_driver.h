#ifndef _LCD_H
#define _LCD_H
//#define uint8_t int8

void LCD_Init(void);
void LCD_SendChar(uint8_t data);
void LCD_SendString(char *data, uint8_t Length);
void LCD_GotoXY(uint8_t X, uint8_t Y);
void LCD_Clear(void);
void LCD_NewItem(void);

#endif /* _LCD_H */

