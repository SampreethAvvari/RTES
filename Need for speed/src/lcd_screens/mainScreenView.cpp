#include <LCD.h>
#include <GYRO_SENSOR.h>

//Gyroscope value below this will be discarded for calculating average velocity
#define ZERO_RATE_NOISE 0.280000

SPI spi(PF_9, PF_8, PF_7);
DigitalOut cs(PC_1);

Ticker tick;
Gyroscope gyroscope(&spi, PC_1);

float speed = 0;
float avg_speed = 0;
float gyroX;
float gyroY;
float gyroZ;
float gyroX1;
bool debug_flag = false;


// Initialize the ticker called every one second;
// Displays the time elapsed, the speed and the distance travelled;
void mainScreenTickerFunc() {
    debug_flag = true;
    speed = avg_speed * ARC_length;
    distanceTravelled += speed;

    float printSpeed = speed * 3.6;

    lcd.SetFont(&Font24);
    // time elapsed; seconds
    sprintf((char*)text1, "%d", secondsCount);
    lcd.DisplayStringAt(0, 26, (uint8_t *)&text1, CENTER_MODE);
    // speed; km/hr
    sprintf((char*)text2, "    %0.2f    ", printSpeed);
    lcd.DisplayStringAt(0, 124, (uint8_t *)&text2, CENTER_MODE);
    // distance; meters
    sprintf((char*)text3, "%0.2f", distanceTravelled);
    lcd.DisplayStringAt(0, 224, (uint8_t *)&text3, CENTER_MODE); 

    secondsCount++;
}


// Main screen UI used to display the time elapsed, speed,
// and the distance travelled by the user;
// Gyroscope is read in this functio at a 100Hz sampling rate;
// Speed and distance displayed using a ticker function
// Ticker attached to mainScreenTickerFunc;
void mainScreenView() {
    speed = 0;
    distanceTravelled = 0;
    avg_speed = 0;
    secondsCount = 1;
    
    lcd.Clear(LCD_COLOR_WHITE);
    lcd.SetBackColor(LCD_COLOR_WHITE);


    lcd.SetTextColor(LCD_COLOR_RED);
    lcd.SetFont(&Font16);
    lcd.DisplayStringAt(0, 6, (uint8_t *)"TIME", CENTER_MODE);
    lcd.SetFont(&Font16);
    lcd.DisplayStringAt(0, 48, (uint8_t *)"seconds", CENTER_MODE);
        
    // lcd.SetTextColor(LCD_COLOR_DARKGREEN);
    // lcd.FillRect((getScreenWidth() / 4) - 20, 80, (getScreenWidth() / 2) + 40, (getScreenWidth() / 4) + 20);
    lcd.SetTextColor(LCD_COLOR_RED);
    lcd.SetFont(&Font16);
    lcd.DisplayStringAt(0, 100, (uint8_t *)"SPEED", CENTER_MODE);
    lcd.SetFont(&Font16);
    lcd.DisplayStringAt(0, 148, (uint8_t *)"KM / H", CENTER_MODE);
        
    // lcd.SetTextColor(LCD_COLOR_DARKCYAN);
    // lcd.FillEllipse(getScreenWidth() / 2, 200, getScreenWidth() / 4, getScreenWidth() / 8);
    lcd.SetTextColor(LCD_COLOR_RED);
    lcd.SetFont(&Font20);
    lcd.DisplayStringAt(0, 200, (uint8_t *)"DISTANCE", CENTER_MODE);
    lcd.SetFont(&Font16);
    lcd.DisplayStringAt(0, 248, (uint8_t *)"METERS", CENTER_MODE);
        
    lcd.SetTextColor(LCD_COLOR_BLACK);
    lcd.SetFont(&Font16);
    lcd.DisplayStringAt(0, getScreenHeight() - 32, (uint8_t *)"Tap to End", CENTER_MODE);

    
    int err = gyroscope.init();

    int16_t w_dps[3];
    
    wait();

    ARC_length = inputUserHeight / 200;
    tick.attach(&mainScreenTickerFunc, 1.0);
    
    while(1) {
        wait_us(100000);
        gyroscope.read_data_16(w_dps);
        
        gyroX = w_dps[0] * 0.00875f; // Calculate average linear velocity and distance;
        gyroY = w_dps[1] * 0.00875f; // Unused;
        gyroZ = w_dps[2] * 0.00875f; // Unused;

        gyroX = gyroX + ZERO_RATE_NOISE;
        if (gyroX < 0.30) {
            gyroX = 0;  
        }

        gyroX1 = gyroX * 0.0174533f;
        
        // Circular queue to calculate the average velocity;
        avg_speed = gyroscope.average_Velocity(gyroX1);

        if(debug_flag){
            debug_flag = false;
            printf("%0.2f  ",avg_speed*ARC_length);
        }
        //printf("wx:%f  wx1:%f  temp:%f\n", gyroX, gyroX1, avg_speed);

        // touchscreen stuff
        ts.GetState(&TS_State);
        touchX = TS_State.X;
        touchY = TS_State.Y;

        // printf("x - %d, y - %d\n", touchX, touchY);

        if (TS_State.TouchDetected) {
            activeScreen = RESULT_SCREEN;

            if (touchY < 40) {
                break;
            }
        }

        if (secondsCount>=20)
        {
            activeScreen = RESULT_SCREEN;
            break;
        }
    }

    tick.detach();
}
