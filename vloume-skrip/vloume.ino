#include <HID-Project.h>
//#include <TrinketHidCombo.h>
#include <elapsedMillis.h>

elapsedMillis timeElapsed;
char sr;
int m;
int m2;
int c;
int d;
int e = 1;

// Rotary Encoder Inputs
#define inputCLK 1
#define inputDT 2

//Rotary Encoder variables
int counter = 0;
int counter2 = 0;
int counter3 = 0; 
int counter4 = 0;
int counter5 = 0;
int currentStateCLK;
int previousStateCLK;
//String encdir ="";

//buttonPin variables
const int buttonPin = 0;
int buttonState = 0; 

void setup() {

    //Set buttonPin as input
    pinMode(buttonPin,INPUT_PULLUP);

    // Set encoder pins as inputs  
    pinMode (inputCLK,INPUT);
    pinMode (inputDT,INPUT);

    // Setup Serial Monitor
    Serial.begin (9600);

    // Read the initial state of inputCLK
    // Assign to previousStateCLK variable
    previousStateCLK = digitalRead(inputCLK);

    //start Keyboard
    //TrinketHidCombo.begin();

    //TrinketHidCombo.pressMultimediaKey(MMKEY_VOL_DOWN);
    //TrinketHidCombo.pressMultimediaKey(MMKEY_VOL_UP);
    //TrinketHidCombo.pressKey(0, KEYCODE_ARROW_RIGHT);
    //TrinketHidCombo.pressKey(0, 0);
    //TrinketHidCombo.pressKey(0, KEYCODE_ARROW_LEFT);
    //TrinketHidCombo.pressKey(0, 0);
}

void button3() {
    buttonState = digitalRead(buttonPin);
    if(buttonState == 1) {
        srswitch();
    }
    else {
        button3();
    }
    
}

void button2() {
    buttonState = digitalRead(buttonPin);
    if(buttonState == 0){
       m2 = timeElapsed - m;
       if(m2 == 300) {
           delay(300);
           buttonState = digitalRead(buttonPin);
           if (buttonState == 0){
               button2();
           }
           else{
               button3();
           }
       }
       else if(m2 > 300) {
           Mute();
           d = 1;
       }
        
       else {
            button2();
       }
    }
    else {
        if (d == 0){
            stop();
        }
        else{
            d = 0;
        }
    } 
}

void button() {
    buttonState = digitalRead(buttonPin);
    if (buttonState == 0) {
        m = timeElapsed;
        button2(); 
    }
}

void skip() {
    // Read the current state of inputCLK
    currentStateCLK = digitalRead(inputCLK);

    // If the inputDT state is different than the inputCLK state then 
    // the encoder is rotating counterclockwise
    if (currentStateCLK != previousStateCLK){
        // Encoder is rotating counterclockwise
        if (digitalRead(inputDT) != currentStateCLK) {
            if (counter2 == 2){
                Consumer.write(MEDIA_PREVIOUS);
                Serial.println("MEDIA_PREVIOUS");
                counter2 = 0;
                counter3 = 0;
                delay(350);
            }
            else{
                counter2 += 1;
                counter3 = 0;
            }
            
            //Keyboard.write(KEY_LEFT_ARROW);
            //TrinketHidCombo.pressKey(0, KEYCODE_ARROW_LEFT);
            //TrinketHidCombo.pressKey(0, 0);
            //TrinketHidCombo.poll();
            //Keyboard.releaseAll();
            //encdir ="CCW";
            previousStateCLK = currentStateCLK;
            skip();

        } else {
            // Encoder is rotating clockwise
            if (counter3 == 2){
                Consumer.write(MEDIA_NEXT);
                Serial.println("MEDIA_NEXT");
                counter3 = 0;
                counter2 = 0;
                delay(350);
            }
            else{
                counter3 += 1;
                counter2 = 0;
            }
            //Keyboard.write(KEY_RIGHT_ARROW);
            //TrinketHidCombo.pressKey(0, KEYCODE_ARROW_RIGHT);
            //TrinketHidCombo.pressKey(0, 0);
            //TrinketHidCombo.poll();
            //Keyboard.releaseAll();
            //encdir ="CW";
            previousStateCLK = currentStateCLK;
            skip();
        }
        //Serial.println(encdir); 
    }
}

void re() {
    // Read the current state of inputCLK
    currentStateCLK = digitalRead(inputCLK);

    // If the inputDT state is different than the inputCLK state then 
    // the encoder is rotating counterclockwise
    if (currentStateCLK != previousStateCLK){
        if (digitalRead(inputDT) != currentStateCLK) {
            counter4 += 1;
            if (counter4 == 2){
                counter4 = 0;
                Consumer.write(MEDIA_VOLUME_DOWN);
                Serial.println("MEDIA_VOLUME_DOWN");
                //TrinketHidCombo.pressMultimediaKey(MMKEY_VOL_DOWN);
                //TrinketHidCombo.poll();
                //encdir ="CCW";
                previousStateCLK = currentStateCLK;
            }

        } else {
            counter5 += 1;
            if (counter5 == 2) {
                counter5 = 0;
                // Encoder is rotating clockwise
                Consumer.write(MEDIA_VOLUME_UP);
                Serial.println("MEDIA_VOLUME_UP");
                //TrinketHidCombo.pressMultimediaKey(MMKEY_VOL_UP);
                //TrinketHidCombo.poll();
                //encdir ="CW";
                previousStateCLK = currentStateCLK;
            }
        }
        //Serial.println(encdir); 
    }
}

void srswitch() {
    if (e == 0){
        Serial.println("Volume");
        e = 1;
    }
    else{
        Serial.println("Skip");
        e = 0;
    }
    delay(20);
}

void srcheck() {
    if (e == 0){
        skip();
    }
    else{
        re();
    }
}

void Mute() {
    Serial.println("volume got de/muted");
    Consumer.write(MEDIA_VOLUME_MUTE);
    //TrinketHidCombo.pressMultimediaKey(MMKEY_MUTE);
    //TrinketHidCombo.poll();
    Mute2();
}

void Mute2() {
    buttonState = digitalRead(buttonPin);
    if (buttonState == 1){
        m2 = 0;
        m = 0;
        exit;
    }
    else{
        Mute2();
    }
}

void stop() {
    Serial.println("Media got de/paused");
    Consumer.write(MEDIA_PLAY_PAUSE);
    //TrinketHidCombo.pressMultimediaKey(MMKEY_PLAYPAUSE);
    //TrinketHidCombo.poll();
    delay(150);
}

void loop() {
    Keyboard.releaseAll();
    //TrinketHidCombo.poll();
    button();
    srcheck();
    previousStateCLK = currentStateCLK;
    //Serial.println(timeElapsed);
    if(timeElapsed >= 100000) {
        timeElapsed = 0;
    }
}
