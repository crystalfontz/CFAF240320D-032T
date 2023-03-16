# CFAF240320D-032T Demonstration Code
This is Arduino sample code for the CFAF240320D-032T. This is a 3.2" TFT which uses the [Sitronix ST7789V](https://www.crystalfontz.com/controllers/Sitronix/ST7789V/) LCD controller. This LCD controller is capable of 8/9/16/18 bit parallel and 3 or 4-Wire SPI. In order to interface with the display, a 40 pin ZIF to header breakout board or the [CFA10102 breakout board](https://www.crystalfontz.com/product/cfa10102-oled-breakout-board) will be required.

## Connection Guide
```
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
```
## Display information
Here is a link to our active display:\
[CFAF240320D-032T](https://www.crystalfontz.com/product/cfaf240320d032t-240x320-rgb-tft-graphic)

For more information about other TFT offerings, please see our full list [here](https://www.crystalfontz.com/c/tft-lcd-displays/25).
