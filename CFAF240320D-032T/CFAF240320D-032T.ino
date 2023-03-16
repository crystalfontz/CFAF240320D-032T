//==============================================================================
//
//  CRYSTALFONTZ

//  The controller is a Sitronix ST7789V
//    https://www.crystalfontz.com/controllers/Sitronix/ST7789V/
//
//  Seeeduino v4.2, an open-source 3.3v capable Arduino clone.
//    https://www.seeedstudio.com/Seeeduino-V4.2-p-2517.html
//    https://github.com/SeeedDocument/SeeeduinoV4/raw/master/resources/Seeeduino_v4.2_sch.pdf
//
//==============================================================================
// This is free and unencumbered software released into the public domain.
//
// Anyone is free to copy, modify, publish, use, compile, sell, or
// distribute this software, either in source code form or as a compiled
// binary, for any purpose, commercial or non-commercial, and by any
// means.
//
// In jurisdictions that recognize copyright laws, the author or authors
// of this software dedicate any and all copyright interest in the
// software to the public domain. We make this dedication for the benefit
// of the public at large and to the detriment of our heirs and
// successors. We intend this dedication to be an overt act of
// relinquishment in perpetuity of all present and future rights to this
// software under copyright law.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
// EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
// MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.
// IN NO EVENT SHALL THE AUTHORS BE LIABLE FOR ANY CLAIM, DAMAGES OR
// OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE,
// ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR
// OTHER DEALINGS IN THE SOFTWARE.
//
// For more information, please refer to <http://unlicense.org/>
//==============================================================================
// LCD 16-bit parallel pins and control lines on Seeeduino:
//  ARD      | Port  | Display pin |  Function - 8080 Parallel                |
//-----------+-------+-------------+------------------------------------------+
// 3.3V      |       |             |  POWER 3.3V                              |
// GND       |       |             |  GROUND                                  |
//-----------+-------+-------------+------------------------------------------+
// D8        | PORTB |  05         | Reset                           (Reset)  |
// D9        | PORTB |  29         | Write                           (WR)     |
// D10       | PORTB |  30         | Register select (data/command)  (RS)     |
// D11       | PORTB |  34         | Chip select                     (CS)     |
//-----------+-------+-------------+------------------------------------------+
// Data Lines
//-----------+-------+-------------+------------------------------------------+
// D0        | PORTD |  26, 17     |  DATA BUS LINE (DB1, DB10)               |
// D1        | PORTD |  25, 16     |  DATA BUS LINE (DB2, DB11)               |
// D2        | PORTD |  24, 15     |  DATA BUS LINE (DB3, DB12)               |
// D3        | PORTD |  23, 14     |  DATA BUS LINE (DB4, DB13)               |
// D4        | PORTD |  22, 13     |  DATA BUS LINE (DB5, DB14)               |
// D5        | PORTD |  21, 12     |  DATA BUS LINE (DB6, DB15)               |
// D6        | PORTD |  20, 11     |  DATA BUS LINE (DB7, DB16)               |
// D7        | PORTD |  19, 10     |  DATA BUS LINE (DB8, DB17)               |
//-----------+-------+-------------+------------------------------------------+
// Interface Selection
// IM1 |  Interface mode  |
//-----+------------------+
// 0   |  16-bit parallel |
// 1   |  18-bit parallel |
//-----+------------------+

#include <Arduino.h>

// define pin connections
#define RESET (8)
#define WR (9)
#define RS (10)
#define CS (11)
// we will be operating in pseudo 8-bit mode for simplicity
#define datapins (PORTD)

// define set and clear operations for each pin
#define RESET_MASK (0x01)
#define CLR_RESET (PORTB &= ~(RESET_MASK))
#define SET_RESET (PORTB |= (RESET_MASK))
#define WR_MASK (0x02)
#define CLR_WR (PORTB &= ~(WR_MASK))
#define SET_WR (PORTB |= (WR_MASK))
#define RS_MASK (0x04)
#define CLR_RS (PORTB &= ~(RS_MASK))
#define SET_RS (PORTB |= (RS_MASK))
#define CS_MASK (0x08)
#define CLR_CS (PORTB &= ~(CS_MASK))
#define SET_CS (PORTB |= (CS_MASK))

// ST7789V defines
#define ST7789V_PGAMCTRL (0xE0)		
#define ST7789V_NGAMCTRL (0xE1)		
#define ST7789V_PCTRL1 (0xD0)		
#define ST7789V_VRHS (0xC3)			
#define ST7789V_VDVS (0xC4)			
#define ST7789V_FRCTRL2 (0xC6)		
#define ST7789V_MADCTL (0x36)		
#define ST7789V_COLMOD (0x3A)		
#define ST7789V_PORCTR (0xB2)		
#define ST7789V_GCTRL (0xB7)		
#define ST7789V_VCOMS (0xBB)		
#define ST7789V_CASET (0x2A)		
#define ST7789V_RASET (0x2B)		
#define ST7789V_SLPIN (0x10)		
#define ST7789V_SLPOUT (0x11)		
#define ST7789V_DISPOFF (0x28)		
#define ST7789V_DISPON (0x29)		
#define ST7789V_RAMWR (0x2C)		

// define the command and data write operations. it is possible to operate
// the 16-bit display via 8-bits as commands typically use the first 8 data
// bits whereas the remaining 8 are "don't care" (and can be set to any value)
void writeCommand(uint8_t command)
{
	CLR_RS;				// set operation as command
	CLR_CS;				// select the LCD controller
	datapins = command; // write the command to the data pins
	CLR_WR;
	SET_WR; // toggle write to transmit the command
	SET_CS; // de-select the LCD controller
}

void writeData(uint8_t data)
{
	SET_RS;			 // set operation as data
	CLR_CS;			 // select the LCD controller
	datapins = data; // write the data to the data pins
	CLR_WR;
	SET_WR; // toggle write to transmit the data
	SET_CS; // de-select the LCD controller
}

// initialize display function
void initDisplay(void)
{
	// perform a hardware reset on the LCD
	SET_RESET;
	delay(1);
	CLR_RESET;
	delay(10);
	SET_RESET;
	delay(120);

	writeCommand(ST7789V_PORCTR); // porch setting
	writeData(0x0C);
	writeData(0x0C);
	writeData(0x00);
	writeData(0x33);
	writeData(0x33);

	writeCommand(ST7789V_GCTRL); // gate control
	writeData(0x35);

	writeCommand(ST7789V_COLMOD); // interface pixel format
	writeData(0x55);			  // 65K of RGB interface - 16bit/pixel

	writeCommand(ST7789V_MADCTL); // memory data access control
	writeData(0x00);			  // top-bottom, left-right, normal mode, RGB format

	writeCommand(ST7789V_VCOMS); // VCOMs setting
	writeData(0x2A);

	writeCommand(ST7789V_VRHS); // VRH set
	writeData(0x0A);
	writeCommand(ST7789V_VRHS);
	writeData(0x0A);

	writeCommand(ST7789V_VDVS); // VDV set
	writeData(0x20);

	writeCommand(ST7789V_FRCTRL2); // frame rate control in normal mode
	writeData(0x0F);

	writeCommand(ST7789V_PCTRL1); // power control 1
	writeData(0xA4);
	writeData(0xA2);
	writeCommand(ST7789V_PCTRL1);
	writeData(0xA4);
	writeData(0xA2);

	writeCommand(ST7789V_PGAMCTRL); // positive voltage gamma control
	writeData(0xD0);
	writeData(0x00);
	writeData(0x02);
	writeData(0x07);
	writeData(0x0A);
	writeData(0x28);
	writeData(0x32);
	writeData(0x44);
	writeData(0x42);
	writeData(0x06);
	writeData(0x0E);
	writeData(0x12);
	writeData(0x14);
	writeData(0x17);

	writeCommand(ST7789V_NGAMCTRL); // negative voltage gamma control
	writeData(0xD0);
	writeData(0x00);
	writeData(0x02);
	writeData(0x07);
	writeData(0x0A);
	writeData(0x28);
	writeData(0x31);
	writeData(0x54);
	writeData(0x47);
	writeData(0x0E);
	writeData(0x1C);
	writeData(0x17);
	writeData(0x1B);
	writeData(0x1E);

	// set the display window to the full size of the display
	setDisplayWindow(0x0000, 0x0000, 0x00EF, 0x013F);

	exitSleep();
}

// enter sleep function
void enterSleep(void)
{
	writeCommand(ST7789V_DISPOFF); // display off
	delay(20);
	writeCommand(ST7789V_SLPIN); // enter sleep mode (low power mode)
}

// exit sleep function
void exitSleep(void)
{
	writeCommand(ST7789V_SLPOUT); // exit sleep mode
	delay(120);					  // it is required to wait 120ms before sending another command
	writeCommand(ST7789V_DISPON); // display on

	writeCommand(ST7789V_RAMWR); // memory write
								 // when this command is accepted, the column register and the page
								 // register are reset to the start column/start page positions
}

// set display window function
void setDisplayWindow(int x0, int y0, int x1, int y1)
{
	writeCommand(ST7789V_CASET); // column address set
	writeData(x0 >> 8);
	writeData(x0);
	writeData(x1 >> 8);
	writeData(x1);

	writeCommand(ST7789V_RASET); // row address set
	writeData(y0 >> 8);
	writeData(y0);
	writeData(y1 >> 8);
	writeData(y1);
}

// set write position function
void setPosition(uint16_t x, uint16_t y)
{
	writeCommand(ST7789V_CASET); // column address set
	writeData(x >> 8);
	writeData(x & 0x00FF); // the x axis will increment according to the variable passed through
	writeData(0x0000);
	writeData(0x00EF);

	writeCommand(ST7789V_RASET); // page address set function
	writeData(y >> 8);
	writeData(y & 0x00FF); // the y axis will increment according to the variable passed through
	writeData(0x01);
	writeData(0x3F);

	writeCommand(ST7789V_RAMWR); // write the current position to RAM
}

// function to fill the screen with a color
void fillScreen(unsigned long color)
{
	setPosition(0, 0); // begin write position at 0,0
	unsigned int i, j;

	for (i = 0; i < 320; i++) // increment through y axis
	{
		for (j = 0; j < 240; j++) // increment through x axis
		{
			writeData(color);
		}
	}
}

// function to fill the screen with colored bars
void colorbars()
{

	unsigned int i, j;
	setPosition(0, 0);
	for (i = 0; i < 320; i++)
	{
		for (j = 0; j < 80; j++) // fill the display with the color for the desired height
		{
			writeData(0xF8F8);
		}
	}
	setPosition(0, 80); // increment the write position depending on the height of the colored bar
	for (i = 0; i < 320; i++)
	{
		for (j = 0; j < 80; j++)
		{
			writeData(0x2F2F);
		}
	}
	setPosition(0, 160);
	for (i = 0; i < 320; i++)
	{
		for (j = 0; j < 80; j++)
		{
			writeData(0x4747);
		}
	}
	setPosition(0, 240);
	for (i = 0; i < 320; i++)
	{
		for (j = 0; j < 80; j++)
		{
			writeData(0xC0C0);
		}
	}
}

// function to draw colored circles on the display
void LCD_circle(uint16_t x0, uint16_t y0, uint16_t radius, uint16_t color)
{
	//uint16_t colortemp = color;
	uint16_t x = radius;
	uint16_t y = 0;
	int16_t radiusError = 1 - (int16_t)x;

	while (x >= y)
	{
		// 11 o'clock
		placepixel(x0 - y, y0 + x, color);
		// 1 o'clock
		placepixel(x0 + y, y0 + x, color);
		// 10 o'clock
		placepixel(x0 - x, y0 + y, color);
		// 2 o'clock
		placepixel(x0 + x, y0 + y, color);
		// 8 o'clock
		placepixel(x0 - x, y0 - y, color);
		// 4 o'clock
		placepixel(x0 + x, y0 - y, color);
		// 7 o'clock
		placepixel(x0 - y, y0 - x, color);
		// 5 o'clock
		placepixel(x0 + y, y0 - x, color);

		y++;
		if (radiusError < 0)
			radiusError += (int16_t)(2 * y + 1);
		else
		{
			x--;
			radiusError += 2 * (((int16_t)y - (int16_t)x) + 1);
		}
	}
}

// function to place a pixel in the required position
void placepixel(uint16_t x, uint16_t y, uint16_t color)
{
	setPosition(x, y);
	// write the single pixel's worth of data
	writeData(color);
}

// function to draw a checkerboard on the display
void checkerboard(uint16_t color1, uint16_t color2)
{
	uint8_t i;
	uint16_t x, y, sub_x, sub_y;

	for (x = 0; x < (240 / 16); x++)
	{
		for (y = 0; y < (320 / 16); y++)
		{
			for (sub_x = 0; sub_x <= 15; sub_x++)
			{
				for (sub_y = 0; sub_y <= 15; sub_y++)
				{
					if (((x & 0x01) && !(y & 0x01)) || (!(x & 0x01) && (y & 0x01)))
					{
						placepixel((x << 4) + sub_x, (y << 4) + sub_y, color1);
					}
					else
					{
						placepixel((x << 4) + sub_x, (y << 4) + sub_y, color2);
					}
				}
			}
		}
	}
}

// setup function
void setup()
{
	// setup pin directions
	DDRB = 0x0F; // set pins
	DDRD = 0xFF;

	// set a suitable starting state for the pins
	SET_WR;
	SET_CS;
	CLR_RS;
	datapins = 0;

	// initialize the display
	initDisplay();
	delay(1);
}

// set the demo to 1 to try it out
#define fillscreen_demo 1
#define colorbars_demo 1
#define circles_demo 1
#define checkerboard_demo 1

// loop function
void loop()
{
#if fillscreen_demo
	fillScreen(0xE5E5);
	delay(1000);
	fillScreen(0xC0C0);
	delay(1000);
	fillScreen(0xF8F8);
	delay(1000);
#endif

#if colorbars_demo
	colorbars();
	delay(5000);
#endif

#ifdef circles_demo
	fillScreen(0x1515);
	delay(1000);
	LCD_circle(120, 120 + 40, 119, 0xE5E5);
	LCD_circle(120, 120 + 40, 40, 0xF1F1);
	LCD_circle(40, 120 + 40, 37, 0x4444);
	LCD_circle(200, 120 + 40, 37, 0x0000);
	LCD_circle(120, 200 + 40, 32, 0xFCFC);
	LCD_circle(120, 40 + 40, 28, 0xBABA);
	delay(1000);
#endif

#ifdef checkerboard_demo
	checkerboard(0x0000, 0xFFFF);
#endif
	while (1); // stop the loop
}