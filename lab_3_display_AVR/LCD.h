#ifndef _LCD_H
#define _LCD_H

//void LCD_Init(void);
void LCD_Init(uint8_t * in_PORT, uint8_t * in_DDR);
void LCD_SendChar(uint8_t data);
void LCD_SendString(char * data, uint8_t Length);
void LCD_GotoXY(uint8_t X, uint8_t Y);
void LCD_Clear(void);
void LCD_NewItem(void);

#endif /* _LCD_H */



	
