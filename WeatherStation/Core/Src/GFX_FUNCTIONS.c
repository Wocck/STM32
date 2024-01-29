/**
 * @file       	GFX_FUNCTIONS.c
 * @brief      	This file contains graphics functions to draw and manipulate various shapes and text
 *              on the ST7735 TFT LCD display.
 * @date		30-Oct-2020
 * @author    	Controllerstech

 * It includes basic drawing functions for pixels, lines,
 * rectangles, circles, triangles, and more complex shapes like rounded rectangles and filled shapes.
 * The file defines utility functions for screen manipulation such as filling the screen, drawing
 * gradients, and displaying text. Additionally, there are functions for creating animated values,
 * testing graphic primitives, and rendering a techy interface with temperature and humidity updates.
 * The functions leverage the ST7735 TFT library for low-level display operations. Color definitions,
 * global variables for display dimensions, and helper macros for calculations are also included.
 */

/** @defgroup DisplayGraphics
 *  This group contains files related to display and graphics functionalities.
 *  @{
 */

#include <GFX_Functions.h>
#include <stdio.h>
#include <ST7735_Tft.h>
#include <stdlib.h>

#define RECT_WIDTH 140
#define RECT_HEIGHT 50
#define ANIMATION_DELAY 1 		/**< Time in milliseconds */

extern int16_t _width;
extern int16_t _height;
extern int16_t cursor_x;
extern int16_t cursor_y;
extern uint8_t rotation;
extern uint8_t _colstart;
extern uint8_t _rowstart;
extern uint8_t _xstart;
extern uint8_t _ystart;

void drawPixel(int16_t x, int16_t y, uint16_t color)
{
	ST7735_DrawPixel(x, y, color);
}

void fillRect(int16_t x, int16_t y, int16_t w, int16_t h, uint16_t color)
{
	ST7735_FillRectangle(x, y, w, h, color);
}


#define _swap_int16_t(a, b)                                                    \
  {                                                                            \
    int16_t t = a;                                                             \
    a = b;                                                                     \
    b = t;                                                                     \
  }

#define min(a, b) (((a) < (b)) ? (a) : (b))


void writePixel(int16_t x, int16_t y, uint16_t color)
{
    drawPixel(x, y, color);
}

void writeLine(int16_t x0, int16_t y0, int16_t x1, int16_t y1, uint16_t color)
{
    int16_t steep = abs(y1 - y0) > abs(x1 - x0);
    if (steep) {
        _swap_int16_t(x0, y0);
        _swap_int16_t(x1, y1);
    }

    if (x0 > x1) {
        _swap_int16_t(x0, x1);
        _swap_int16_t(y0, y1);
    }

    int16_t dx, dy;
    dx = x1 - x0;
    dy = abs(y1 - y0);

    int16_t err = dx / 2;
    int16_t ystep;

    if (y0 < y1) {
        ystep = 1;
    } else {
        ystep = -1;
    }

    for (; x0<=x1; x0++) {
        if (steep) {
            writePixel(y0, x0, color);
        } else {
            writePixel(x0, y0, color);
        }
        err -= dy;
        if (err < 0) {
            y0 += ystep;
            err += dx;
        }
    }
}

void  drawFastVLine(int16_t x, int16_t y, int16_t h, uint16_t color)
{
	writeLine(x, y, x, y + h - 1, color);
}
void  drawFastHLine(int16_t x, int16_t y, int16_t w, uint16_t color)
{
	writeLine(x, y, x + w - 1, y, color);
}

void drawLine(int16_t x0, int16_t y0, int16_t x1, int16_t y1, uint16_t color)
{
    if(x0 == x1){
        if(y0 > y1) _swap_int16_t(y0, y1);
        drawFastVLine(x0, y0, y1 - y0 + 1, color);
    } else if(y0 == y1){
        if(x0 > x1) _swap_int16_t(x0, x1);
        drawFastHLine(x0, y0, x1 - x0 + 1, color);
    } else {
        writeLine(x0, y0, x1, y1, color);
    }
}

void drawCircle(int16_t x0, int16_t y0, int16_t r, uint16_t color)
{
    int16_t f = 1 - r;
    int16_t ddF_x = 1;
    int16_t ddF_y = -2 * r;
    int16_t x = 0;
    int16_t y = r;

    writePixel(x0  , y0+r, color);
    writePixel(x0  , y0-r, color);
    writePixel(x0+r, y0  , color);
    writePixel(x0-r, y0  , color);

    while (x<y) {
        if (f >= 0) {
            y--;
            ddF_y += 2;
            f += ddF_y;
        }
        x++;
        ddF_x += 2;
        f += ddF_x;

        writePixel(x0 + x, y0 + y, color);
        writePixel(x0 - x, y0 + y, color);
        writePixel(x0 + x, y0 - y, color);
        writePixel(x0 - x, y0 - y, color);
        writePixel(x0 + y, y0 + x, color);
        writePixel(x0 - y, y0 + x, color);
        writePixel(x0 + y, y0 - x, color);
        writePixel(x0 - y, y0 - x, color);
    }
}

void drawCircleHelper( int16_t x0, int16_t y0, int16_t r, uint8_t cornername, uint16_t color)
{
    int16_t f     = 1 - r;
    int16_t ddF_x = 1;
    int16_t ddF_y = -2 * r;
    int16_t x     = 0;
    int16_t y     = r;

    while (x<y) {
        if (f >= 0) {
            y--;
            ddF_y += 2;
            f     += ddF_y;
        }
        x++;
        ddF_x += 2;
        f     += ddF_x;
        if (cornername & 0x4) {
            writePixel(x0 + x, y0 + y, color);
            writePixel(x0 + y, y0 + x, color);
        }
        if (cornername & 0x2) {
            writePixel(x0 + x, y0 - y, color);
            writePixel(x0 + y, y0 - x, color);
        }
        if (cornername & 0x8) {
            writePixel(x0 - y, y0 + x, color);
            writePixel(x0 - x, y0 + y, color);
        }
        if (cornername & 0x1) {
            writePixel(x0 - y, y0 - x, color);
            writePixel(x0 - x, y0 - y, color);
        }
    }
}

void fillCircleHelper(int16_t x0, int16_t y0, int16_t r, uint8_t corners, int16_t delta, uint16_t color)
{

    int16_t f     = 1 - r;
    int16_t ddF_x = 1;
    int16_t ddF_y = -2 * r;
    int16_t x     = 0;
    int16_t y     = r;
    int16_t px    = x;
    int16_t py    = y;

    delta++; // Avoid some +1's in the loop

    while(x < y) {
        if (f >= 0) {
            y--;
            ddF_y += 2;
            f     += ddF_y;
        }
        x++;
        ddF_x += 2;
        f     += ddF_x;
        // These checks avoid double-drawing certain lines, important
        // for the SSD1306 library which has an INVERT drawing mode.
        if(x < (y + 1)) {
            if(corners & 1) drawFastVLine(x0+x, y0-y, 2*y+delta, color);
            if(corners & 2) drawFastVLine(x0-x, y0-y, 2*y+delta, color);
        }
        if(y != py) {
            if(corners & 1) drawFastVLine(x0+py, y0-px, 2*px+delta, color);
            if(corners & 2) drawFastVLine(x0-py, y0-px, 2*px+delta, color);
            py = y;
        }
        px = x;
    }
}

void fillCircle(int16_t x0, int16_t y0, int16_t r, uint16_t color)
{
    drawFastVLine(x0, y0-r, 2*r+1, color);
    fillCircleHelper(x0, y0, r, 3, 0, color);
}



void drawRect(int16_t x, int16_t y, int16_t w, int16_t h, uint16_t color)
{
    drawFastHLine(x, y, w, color);
    drawFastHLine(x, y+h-1, w, color);
    drawFastVLine(x, y, h, color);
    drawFastVLine(x+w-1, y, h, color);
}

void drawRoundRect(int16_t x, int16_t y, int16_t w, int16_t h, int16_t r, uint16_t color)
{
    int16_t max_radius = ((w < h) ? w : h) / 2; // 1/2 minor axis
    if(r > max_radius) r = max_radius;
    // smarter version
    drawFastHLine(x+r  , y    , w-2*r, color); // Top
    drawFastHLine(x+r  , y+h-1, w-2*r, color); // Bottom
    drawFastVLine(x    , y+r  , h-2*r, color); // Left
    drawFastVLine(x+w-1, y+r  , h-2*r, color); // Right
    // draw four corners
    drawCircleHelper(x+r    , y+r    , r, 1, color);
    drawCircleHelper(x+w-r-1, y+r    , r, 2, color);
    drawCircleHelper(x+w-r-1, y+h-r-1, r, 4, color);
    drawCircleHelper(x+r    , y+h-r-1, r, 8, color);
}


void fillRoundRect(int16_t x, int16_t y, int16_t w, int16_t h, int16_t r, uint16_t color)
{
    int16_t max_radius = ((w < h) ? w : h) / 2; // 1/2 minor axis
    if(r > max_radius) r = max_radius;
    // smarter version
    fillRect(x+r, y, w-2*r, h, color);
    // draw four corners
    fillCircleHelper(x+w-r-1, y+r, r, 1, h-2*r-1, color);
    fillCircleHelper(x+r    , y+r, r, 2, h-2*r-1, color);
}


void drawTriangle(int16_t x0, int16_t y0, int16_t x1, int16_t y1, int16_t x2, int16_t y2, uint16_t color)
{
    drawLine(x0, y0, x1, y1, color);
    drawLine(x1, y1, x2, y2, color);
    drawLine(x2, y2, x0, y0, color);
}


void fillTriangle(int16_t x0, int16_t y0, int16_t x1, int16_t y1, int16_t x2, int16_t y2, uint16_t color)
{

    int16_t a, b, y, last;

    // Sort coordinates by Y order (y2 >= y1 >= y0)
    if (y0 > y1) {
        _swap_int16_t(y0, y1); _swap_int16_t(x0, x1);
    }
    if (y1 > y2) {
        _swap_int16_t(y2, y1); _swap_int16_t(x2, x1);
    }
    if (y0 > y1) {
        _swap_int16_t(y0, y1); _swap_int16_t(x0, x1);
    }

    if(y0 == y2) { // Handle awkward all-on-same-line case as its own thing
        a = b = x0;
        if(x1 < a)      a = x1;
        else if(x1 > b) b = x1;
        if(x2 < a)      a = x2;
        else if(x2 > b) b = x2;
        drawFastHLine(a, y0, b-a+1, color);
        return;
    }

    int16_t
    dx01 = x1 - x0,
    dy01 = y1 - y0,
    dx02 = x2 - x0,
    dy02 = y2 - y0,
    dx12 = x2 - x1,
    dy12 = y2 - y1;
    int32_t
    sa   = 0,
    sb   = 0;

    // For upper part of triangle, find scanline crossings for segments
    // 0-1 and 0-2.  If y1=y2 (flat-bottomed triangle), the scanline y1
    // is included here (and second loop will be skipped, avoiding a /0
    // error there), otherwise scanline y1 is skipped here and handled
    // in the second loop...which also avoids a /0 error here if y0=y1
    // (flat-topped triangle).
    if(y1 == y2) last = y1;   // Include y1 scanline
    else         last = y1-1; // Skip it

    for(y=y0; y<=last; y++) {
        a   = x0 + sa / dy01;
        b   = x0 + sb / dy02;
        sa += dx01;
        sb += dx02;
        /* longhand:
        a = x0 + (x1 - x0) * (y - y0) / (y1 - y0);
        b = x0 + (x2 - x0) * (y - y0) / (y2 - y0);
        */
        if(a > b) _swap_int16_t(a,b);
        drawFastHLine(a, y, b-a+1, color);
    }

    // For lower part of triangle, find scanline crossings for segments
    // 0-2 and 1-2.  This loop is skipped if y1=y2.
    sa = (int32_t)dx12 * (y - y1);
    sb = (int32_t)dx02 * (y - y0);
    for(; y<=y2; y++) {
        a   = x1 + sa / dy12;
        b   = x0 + sb / dy02;
        sa += dx12;
        sb += dx02;
        /* longhand:
        a = x1 + (x2 - x1) * (y - y1) / (y2 - y1);
        b = x0 + (x2 - x0) * (y - y0) / (y2 - y0);
        */
        if(a > b) _swap_int16_t(a,b);
        drawFastHLine(a, y, b-a+1, color);
    }
}

void fillScreen(uint16_t color) {
    fillRect(0, 0, _width, _height, color);
}

uint16_t interpolateColor(uint16_t colorStart, uint16_t colorEnd, float t) {
    // Assuming 5 bits for red, 6 bits for green, and 5 bits for blue
    int redStart = (colorStart >> 11) & 0x1F;
    int greenStart = (colorStart >> 5) & 0x3F;
    int blueStart = colorStart & 0x1F;

    int redEnd = (colorEnd >> 11) & 0x1F;
    int greenEnd = (colorEnd >> 5) & 0x3F;
    int blueEnd = colorEnd & 0x1F;

    int red = redStart + (int)((redEnd - redStart) * t);
    int green = greenStart + (int)((greenEnd - greenStart) * t);
    int blue = blueStart + (int)((blueEnd - blueStart) * t);

    return (uint16_t)((red << 11) | (green << 5) | blue);
}

void displayText(int16_t x, int16_t y, const char* text, uint16_t color, uint16_t bgcolor) {
    // Assuming ST7735_WriteString is the function to draw text
    ST7735_WriteString(x, y, text, Font_11x18, color, bgcolor);
}

void drawGradientBackground() {
    for (int i = 0; i < _height+3; i++) {
        uint16_t color = interpolateColor(BLACK, BLUE, (float)i / _height+1);
        drawFastHLine(1, i, _width, color);
    }
}


void drawAnimatedValue(uint16_t x, uint16_t y, char* label, float value, uint16_t color) {
    char buffer[32];
    sprintf(buffer, "%s: %.1f", label, value);

    for (int w = 0; w <= RECT_WIDTH; w += 10) {
        fillRoundRect(x, y, w, RECT_HEIGHT, 10, color);
    }

    displayText(x + 10, y + 10, buffer, WHITE, color);
}


void testLines(uint16_t color)
{
    int           x1, y1, x2, y2,
                  w = _width,
                  h = _height;

    fillScreen(BLACK);

    x1 = y1 = 0;
    y2    = h - 1;
    for (x2 = 0; x2 < w; x2 += 6) drawLine(x1, y1, x2, y2, color);
    x2    = w - 1;
    for (y2 = 0; y2 < h; y2 += 6) drawLine(x1, y1, x2, y2, color);

    fillScreen(BLACK);

    x1    = w - 1;
    y1    = 0;
    y2    = h - 1;
    for (x2 = 0; x2 < w; x2 += 6) drawLine(x1, y1, x2, y2, color);
    x2    = 0;
    for (y2 = 0; y2 < h; y2 += 6) drawLine(x1, y1, x2, y2, color);

    fillScreen(BLACK);

    x1    = 0;
    y1    = h - 1;
    y2    = 0;
    for (x2 = 0; x2 < w; x2 += 6) drawLine(x1, y1, x2, y2, color);
    x2    = w - 1;
    for (y2 = 0; y2 < h; y2 += 6) drawLine(x1, y1, x2, y2, color);

    fillScreen(BLACK);

    x1    = w - 1;
    y1    = h - 1;
    y2    = 0;
    for (x2 = 0; x2 < w; x2 += 6) drawLine(x1, y1, x2, y2, color);
    x2    = 0;
    for (y2 = 0; y2 < h; y2 += 6) drawLine(x1, y1, x2, y2, color);

}

void testFastLines(uint16_t color1, uint16_t color2)
{
    int           x, y, w = _width, h = _height;

    fillScreen(BLACK);
    for (y = 0; y < h; y += 5) drawFastHLine(0, y, w, color1);
    for (x = 0; x < w; x += 5) drawFastVLine(x, 0, h, color2);
}

void testRects(uint16_t color)
{
    int           n, i, i2,
                  cx = _width  / 2,
                  cy = _height / 2;

    fillScreen(BLACK);
    n     = min(_width, _height);
    for (i = 2; i < n; i += 6) {
        i2 = i / 2;
        drawRect(cx - i2, cy - i2, i, i, color);
    }

}

void testFilledRects(uint16_t color1, uint16_t color2)
{
    int           n, i, i2,
                  cx = _width  / 2 - 1,
                  cy = _height / 2 - 1;

    fillScreen(BLACK);
    n = min(_width, _height);
    for (i = n; i > 0; i -= 6) {
        i2    = i / 2;

        fillRect(cx - i2, cy - i2, i, i, color1);

        drawRect(cx - i2, cy - i2, i, i, color2);
    }
}

void testFilledCircles(uint8_t radius, uint16_t color)
{
    int x, y, w = _width, h = _height, r2 = radius * 2;

    fillScreen(BLACK);
    for (x = radius; x < w; x += r2) {
        for (y = radius; y < h; y += r2) {
            fillCircle(x, y, radius, color);
        }
    }

}

void testCircles(uint8_t radius, uint16_t color)
{
    int           x, y, r2 = radius * 2,
                        w = _width  + radius,
                        h = _height + radius;

    // Screen is not cleared for this one -- this is
    // intentional and does not affect the reported time.
    for (x = 0; x < w; x += r2) {
        for (y = 0; y < h; y += r2) {
            drawCircle(x, y, radius, color);
        }
    }

}

void testTriangles()
{
    int           n, i, cx = _width  / 2 - 1,
                        cy = _height / 2 - 1;

    fillScreen(BLACK);
    n     = min(cx, cy);
    for (i = 0; i < n; i += 5) {
        drawTriangle(
            cx    , cy - i, // peak
            cx - i, cy + i, // bottom left
            cx + i, cy + i, // bottom right
            color565(0, 0, i));
    }

}

void testFilledTriangles() {
    int           i, cx = _width  / 2 - 1,
                     cy = _height / 2 - 1;

    fillScreen(BLACK);
    for (i = min(cx, cy); i > 10; i -= 5) {
    	fillTriangle(cx, cy - i, cx - i, cy + i, cx + i, cy + i,
    	                         color565(0, i, i));
    	drawTriangle(cx, cy - i, cx - i, cy + i, cx + i, cy + i,
    	                         color565(i, i, 0));
    }
}

void testRoundRects() {
    int           w, i, i2, red, step,
                  cx = _width  / 2 - 1,
                  cy = _height / 2 - 1;

    fillScreen(BLACK);
    w     = min(_width, _height);
    red = 0;
    step = (256 * 6) / w;
    for (i = 0; i < w; i += 6) {
        i2 = i / 2;
        red += step;
        drawRoundRect(cx - i2, cy - i2, i, i, i / 8, color565(red, 0, 0));
    }

}

void testFilledRoundRects() {
    int           i, i2, green, step,
                  cx = _width  / 2 - 1,
                  cy = _height / 2 - 1;

    fillScreen(BLACK);
    green = 256;
    step = (256 * 6) / min(_width, _height);
    for (i = min(_width, _height); i > 20; i -= 6) {
        i2 = i / 2;
        green -= step;
        fillRoundRect(cx - i2, cy - i2, i, i, i / 8, color565(0, green, 0));
    }

}
void testFillScreen()
{
    fillScreen(BLACK);
    fillScreen(RED);
    fillScreen(GREEN);
    fillScreen(BLUE);
    fillScreen(BLACK);
}

void testAll (void)
{
	testFillScreen();
	testLines(CYAN);
	testFastLines(RED, BLUE);
	testRects(GREEN);
	testFilledRects(YELLOW, MAGENTA);
	testFilledCircles(10, MAGENTA);
	testCircles(10, WHITE);
	testTriangles();
	testFilledTriangles();
	testRoundRects();
	testFilledRoundRects();
}

/**
 * @brief  Draw initial interface in which data is displayed
 * @retval None
 */
void drawTechyInterface() {
	fillScreen(CYAN);
	fillScreen(BLACK);
    // Draw some static lines for a techy look
    drawFastHLine(0, 20, _width, CYAN);
    drawFastHLine(0, _height - 20, _width, CYAN);
    drawFastVLine(_width / 2, 20, _height - 80, CYAN);
    drawFastHLine(0, _height - 60, _width, CYAN);

    // Additional design elements
    drawCircle(10, 10, 8, YELLOW);
    drawCircle(_width - 10, 10, 8, YELLOW);
    drawCircle(10, _height - 10, 8, YELLOW);
    drawCircle(_width - 10, _height - 10, 8, YELLOW);

    // Display "Temp:" and "Humid:" labels
    int tempX = 10;
	int humidX = _width / 2 + 10;
	int labelY = 30;
    ST7735_WriteString(tempX, labelY, "Temp:", Font_7x10, GREEN, BLACK);
	ST7735_WriteString(humidX, labelY, "Humid:", Font_7x10, GREEN, BLACK);
}

/**
 * @brief  Updates the temperature and humidity display values.
 * @param  temp: The new temperature value to display.
 * @param  humid: The new humidity value to display.
 * @retval None
 */
void updateTemperatureAndHumidity(float temp, float humid) {
    char buffer[32]; // Buffer to hold text for temperature and humidity
    // Coordinates and size for the temperature and humidity update areas
    int labelY = 30; // Y-coordinate for labels
    int valueY = labelY + 20; // Y-coordinate for values, adjusted to be below labels
    int tempX = 10;
    int humidX = _width / 2 + 10;

    // Update Temperature value
    ST7735_WriteString(tempX, valueY, "       ", Font_7x10, WHITE, BLACK);
    sprintf(buffer, "%.2fC", temp);
    ST7735_WriteString(tempX, valueY, buffer, Font_7x10, WHITE, BLACK);

    // Update Humidity value
    ST7735_WriteString(humidX, valueY, "       ", Font_7x10, WHITE, BLACK);
    sprintf(buffer, "%.2f%%", humid);
    ST7735_WriteString(humidX, valueY, buffer, Font_7x10, WHITE, BLACK);
}

/** @} */
