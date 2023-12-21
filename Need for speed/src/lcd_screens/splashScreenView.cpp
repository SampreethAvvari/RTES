#include <LCD.h>

// Display the first screen UI on the LCD
// Once user taps the 'START' button, move to the next screen;
void splashScreenView() {
    lcd.Clear(LCD_COLOR_WHITE);
    lcd.SetBackColor(LCD_COLOR_WHITE);
    
    wait();

    lcd.SetFont(&Font20);
    lcd.SetTextColor(LCD_COLOR_ORANGE);
    lcd.DisplayStringAt(0, LINE(3), (uint8_t *)"The Need For", CENTER_MODE);
    lcd.DisplayStringAt(0, LINE(4), (uint8_t *)"Speed", CENTER_MODE);

    wait();

    lcd.SetTextColor(LCD_COLOR_ORANGE);
    lcd.DrawLine(getScreenWidth() / 4, 134, (getScreenWidth() / 4) * 3, 134);
    wait();
    lcd.SetFont(&Font16);
    lcd.SetTextColor(LCD_COLOR_BLUE);
    lcd.DisplayStringAt(0, LINE(9), (uint8_t *)"Team 37", CENTER_MODE);

    wait();
    
    // lcd.SetTextColor(LCD_COLOR_LIGHTBLUE);
    //lcd.SetBackColor(LCD_COLOR_LIGHTBLUE);
    //lcd.FillRect(screenMargin, getScreenHeight() - 52, getScreenWidth() - (2 * screenMargin), 44);
    //lcd.SetTextColor(LCD_COLOR_WHITE);
    //lcd.SetFont(&Font16);
    //lcd.DisplayStringAt(0, getScreenHeight() - 36, (uint8_t *)"START", CENTER_MODE);

    lcd.SetTextColor(LCD_COLOR_DARKGREEN);
    lcd.SetBackColor(LCD_COLOR_DARKGREEN);
    
    int circleRadius = 40;  // Adjust the radius based on your design
    lcd.FillCircle(getScreenWidth() / 2, getScreenHeight() - 60, circleRadius);

    lcd.SetTextColor(LCD_COLOR_WHITE);
    lcd.SetFont(&Font16);
    lcd.DisplayStringAt(0, getScreenHeight() - 64, (uint8_t *)" START", CENTER_MODE);

    wait();

    while(1) {
    
        ts.GetState(&TS_State);
        touchX = TS_State.X;
        touchY = TS_State.Y;

        if ((((TS_State.TouchDetected) && (touchY < 60)) && ((touchX > 80))) && (touchX < 160)){
            activeScreen = CONFIG_SCREEN;
            break;
        }
    
    }
}
