#include "../../Inc/main.h"
#include "LCD44780_I2C.h"
#include <stdlib.h>
//-----------------------------------------------------------------------------------------------------------
// *** Obs�uga wy�wietlaczy alfanumerycznych zgodnych z HD44780 ***
//
// - Sterowanie: tryb 4-bitowy
// - Dowolne przypisanie ka�dego sygna�u steruj�cego do dowolnego pinu mikrokontrolera
// - Praca z pinem RW pod��czonym do GND lub do mikrokontrolera (sprawdzanie BusyFLAG - szybkie operacje LCD)
//
// Pliki             : lcd44780.c , lcd44780.h
// Mikrokontrolery     : STM32
// Kompilator         : arm-none-eabi-gcc 
// Framework          : stm32cube
// �r�d�o             : http://www.atnel.pl
// Data             : marzec 2010-2013
// Autor             : Miros�aw Karda�
// modyfikacje        : shoc z www.forum.atnel.pl
// port stm32           : detmot5
//-----------------------------------------------------------------------------------------------------------


#define SET_RS()     mpxLCD |= (1<<LCD_RS);     SEND_I2C()   
#define CLR_RS()     mpxLCD &= ~(1<<LCD_RS);    SEND_I2C()    

#define SET_RW()     mpxLCD |= (1<<LCD_RW);     SEND_I2C()   
#define CLR_RW()     mpxLCD &= ~(1<<LCD_RW);    SEND_I2C()   

#define SET_E()     mpxLCD |= (1<<LCD_E);      SEND_I2C()   
#define CLR_E()     mpxLCD &= ~(1<<LCD_E);     SEND_I2C()    




static I2C_HandleTypeDef* hi2c;
uint8_t    mpxLCD;

#define SEND_I2C()  HAL_I2C_Master_Transmit (hi2c, PCF8574_LCD_ADDR, &mpxLCD, 1, 100)

//********************* FUNKCJE WEWN�TRZNE *********************

//----------------------------------------------------------------------------------------
//
//         Ustawienie wszystkich 4 linii danych jako WYj�cia
//
//----------------------------------------------------------------------------------------
static inline void data_dir_out(void)
{
    //Zerowanie zmiennych danych (D4..D7)
    mpxLCD    &= ~(1<<LCD_D7);
    mpxLCD    &= ~(1<<LCD_D6);
    mpxLCD    &= ~(1<<LCD_D5);
    mpxLCD    &= ~(1<<LCD_D4);
    SEND_I2C();
}

//----------------------------------------------------------------------------------------
//
//         Wys�anie po��wki bajtu do LCD (D4..D7)
//
//----------------------------------------------------------------------------------------
static inline void lcd_sendHalf(uint8_t data)
{
    if (data&(1<<0)) mpxLCD |= (1<<LCD_D4); else mpxLCD &= ~(1<<LCD_D4);
    if (data&(1<<1)) mpxLCD |= (1<<LCD_D5); else mpxLCD &= ~(1<<LCD_D5);
    if (data&(1<<2)) mpxLCD |= (1<<LCD_D6); else mpxLCD &= ~(1<<LCD_D6);
    if (data&(1<<3)) mpxLCD |= (1<<LCD_D7); else mpxLCD &= ~(1<<LCD_D7);
    SEND_I2C();
}

//----------------------------------------------------------------------------------------
//
//         Zapis bajtu do wy�wietlacza LCD
//
//----------------------------------------------------------------------------------------
static void _lcd_write_byte(unsigned char _data) {

    // Ustawienie pin�w portu LCD D4..D7 jako wyj�cia
    data_dir_out();

    SET_E();
    lcd_sendHalf(_data >> 4);            // wys�anie starszej cz�ci bajtu danych D7..D4
    CLR_E();

    SET_E();
    lcd_sendHalf(_data);                // wys�anie m�odszej cz�ci bajtu danych D3..D0
    CLR_E();

}



//----------------------------------------------------------------------------------------
//
//         Zapis komendy do wy�wietlacza LCD
//
//----------------------------------------------------------------------------------------
static void lcd_write_cmd(uint8_t cmd) {

    CLR_RS();
    _lcd_write_byte(cmd);
}

//----------------------------------------------------------------------------------------
//
//         Zapis danych do wy�wietlacza LCD
//
//----------------------------------------------------------------------------------------
static void lcd_write_data(uint8_t data) {

    SET_RS();
    _lcd_write_byte(data);
}




//**************************  FUNKCJE PRZEZNACZONE TAK�E DLA INNYCH MODU��W  ******************

#if USE_LCD_CHAR == 1
//----------------------------------------------------------------------------------------
//
//         Wys�anie pojedynczego znaku do wy�wietlacza LCD w postaci argumentu
//
//         8 w�asnych znak�w zdefiniowanych w CGRAM
//         wysy�amy za pomoc� kod�w 0x80 do 0x87 zamiast 0x00 do 0x07
//
//----------------------------------------------------------------------------------------
void lcd_char(char c) {

    lcd_write_data( ( c>=0x80 && c<=0x87 ) ? (c & 0x07) : c);
}
#endif

//----------------------------------------------------------------------------------------
//
//         Wys�anie stringa do wy�wietlacza LCD z pami�ci RAM
//
//----------------------------------------------------------------------------------------
void lcd_str(char * str) {

    register char c;
    while ( (c=*(str++)) ) lcd_char( c );

}


#if USE_LCD_INT == 1
//----------------------------------------------------------------------------------------
//
//         Wy�wietla liczb� dziesi�tn� na wy�wietlaczu LCD
//
//----------------------------------------------------------------------------------------
void lcd_int(int val) {

    char buffer[17];
    lcd_str( itoa(val, buffer, 10) );
}
#endif

#if USE_LCD_HEX == 1
//----------------------------------------------------------------------------------------
//
//         Wy�wietla liczb� szestnastkow� HEX na wy�wietlaczu LCD
//
//----------------------------------------------------------------------------------------
void lcd_hex(uint32_t val) {

    char buffer[17];
    lcd_str( itoa(val, buffer, 16) );
}
#endif

#if USE_LCD_DEFCHAR == 1
//----------------------------------------------------------------------------------------
//
//        Definicja w�asnego znaku na LCD z pami�ci RAM
//
//        argumenty:
//        nr:         - kod znaku w pami�ci CGRAM od 0x80 do 0x87
//        *def_znak:    - wska�nik do tablicy 7 bajt�w definiuj�cych znak
//
//----------------------------------------------------------------------------------------
void lcd_defchar(uint8_t nr, uint8_t *def_char) {

    register uint8_t i,c;
    lcd_write_cmd( 64+((nr&0x07)*8) );
    for(i=0;i<8;i++) {
        c = *(def_char++);
        lcd_write_data(c);
    }
}
#endif



#if USE_LCD_LOCATE == 1
//----------------------------------------------------------------------------------------
//
//        Ustawienie kursora w pozycji Y-wiersz, X-kolumna
//
//         Y = od 0 do 3
//         X = od 0 do n
//
//        funkcja dostosowuje automatycznie adresy DDRAM
//        w zale�no�ci od rodzaju wy�wietlacza (ile posiada wierszy)
//
//----------------------------------------------------------------------------------------
void lcd_locate(uint8_t y, uint8_t x) {

    switch(y) {
        case 0: y = LCD_LINE1; break;

#if (LCD_ROWS>1)
        case 1: y = LCD_LINE2; break; // adres 1 znaku 2 wiersza
#endif
#if (LCD_ROWS>2)
        case 2: y = LCD_LINE3; break; // adres 1 znaku 3 wiersza
#endif
#if (LCD_ROWS>3)
        case 3: y = LCD_LINE4; break; // adres 1 znaku 4 wiersza
#endif
    }

    lcd_write_cmd( (0x80 + y + x) );
}
#endif


//----------------------------------------------------------------------------------------
//
//        Kasowanie ekranu wy�wietlacza
//
//----------------------------------------------------------------------------------------
void lcd_cls(void) {

    lcd_write_cmd( LCDC_CLS );

    HAL_Delay(5);
}


#if USE_LCD_CURSOR_HOME == 1
//----------------------------------------------------------------------------------------
//
//        Powr�t kursora na pocz�tek
//
//----------------------------------------------------------------------------------------
void lcd_home(void) {

    lcd_write_cmd( LCDC_CLS|LCDC_HOME );

    HAL_Delay(5);
}
#endif

#if USE_LCD_CURSOR_ON == 1
//----------------------------------------------------------------------------------------
//
//        Wlaczenie kursora na LCD
//
//----------------------------------------------------------------------------------------
void lcd_cursor_on(void) {

    lcd_write_cmd( LCDC_ONOFF|LCDC_DISPLAYON|LCDC_CURSORON);
}

//----------------------------------------------------------------------------------------
//
//        Wy��czenie kursora na LCD
//
//----------------------------------------------------------------------------------------
void lcd_cursor_off(void) {

    lcd_write_cmd( LCDC_ONOFF|LCDC_DISPLAYON);
}
#endif


#if USE_LCD_CURSOR_BLINK == 1
//----------------------------------------------------------------------------------------
//
//        W��cza miganie kursora na LCD
//
//----------------------------------------------------------------------------------------
void lcd_blink_on(void) {

    lcd_write_cmd( LCDC_ONOFF|LCDC_DISPLAYON|LCDC_CURSORON|LCDC_BLINKON);
}

//----------------------------------------------------------------------------------------
//
//        WY��cza miganie kursora na LCD
//
//----------------------------------------------------------------------------------------
void lcd_blink_off(void) {

    lcd_write_cmd( LCDC_ONOFF|LCDC_DISPLAYON);
}
#endif




//----------------------------------------------------------------------------------------
//
//         ******* INICJALIZACJA WY�WIETLACZA LCD ********
//
//----------------------------------------------------------------------------------------
void lcd_init( I2C_HandleTypeDef* i2c_handler ) {

    hi2c = i2c_handler;

    
    mpxLCD = 0;
    
    lcd_LED(1);

    HAL_Delay(15);
    mpxLCD &= ~(1<<LCD_E);
    mpxLCD &= ~(1<<LCD_RS);
    SEND_I2C();



    // jeszcze nie mo�na u�ywa� Busy Flag
    SET_E();
    lcd_sendHalf(0x03);    // tryb 8-bitowy
    CLR_E();
    HAL_Delay(5);



    SET_E();
    lcd_sendHalf(0x03);    // tryb 8-bitowy
    CLR_E();
    HAL_Delay(1);



    SET_E();
    lcd_sendHalf(0x03);    // tryb 8-bitowy
    CLR_E();
    HAL_Delay(1);

    SET_E();
    lcd_sendHalf(0x02);// tryb 4-bitowy
    CLR_E();
    HAL_Delay(1);



    // ju� mo�na u�ywa� Busy Flag
    // tryb 4-bitowy, 2 wiersze, znak 5x7
    lcd_write_cmd( LCDC_FUNC|LCDC_FUNC4B|LCDC_FUNC2L|LCDC_FUNC5x7 );

    // wy��czenie kursora
    lcd_write_cmd( LCDC_ONOFF|LCDC_CURSOROFF );
    // w��czenie wy�wietlacza
    lcd_write_cmd( LCDC_ONOFF|LCDC_DISPLAYON );
    // przesuwanie kursora w prawo bez przesuwania zawarto�ci ekranu
    lcd_write_cmd( LCDC_ENTRY|LCDC_ENTRYR );



    // kasowanie ekranu
    lcd_cls();

}




void lcd_LED(uint8_t _on)
{
    if( _on ) mpxLCD |= (1<<LCD_LED);
    else mpxLCD &= ~(1<<LCD_LED);

    SEND_I2C();

}






