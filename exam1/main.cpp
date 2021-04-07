#include "mbed.h"
#include "uLCD_4DGL.h"


AnalogOut  aout(PA_4);      // D7
AnalogIn Ain(A0); 

InterruptIn but_SEL(D6);
InterruptIn but_UP(D5);
InterruptIn but_DOWN(D3);

uLCD_4DGL uLCD(D1, D0, D2);

void print(int counter);
void sampling();
void generating();
EventQueue sampling_queue(32 * EVENTS_EVENT_SIZE);
EventQueue generating_queue(32 * EVENTS_EVENT_SIZE);
EventQueue queue(32 * EVENTS_EVENT_SIZE);
Thread sampling_thread;
Thread generating_thread;
Thread thread;

float ADCdata[2000];
int counter=3;
int t=80;

void Up();
void Down();
void Sel();

void Up()
{
    if(counter<3) counter++;
    else counter=3; 
    //printf("%d\n", counter);
   //queue.call(print(counter));
    //ThisThread::sleep_for(50ms);
}

void Down()
{
    if(counter>0) counter--;
    else counter=0;
    //queue.call(print(counter)); 
    //ThisThread::sleep_for(50ms);
}

void Sel()
{
    if (counter==3) t=80;           // 100Hz
    else if (counter==2) t=40;      // 50Hz
    else if (counter==1) t=20;      // 25Hz
    else if (counter==0) t=10;     // 10Hz
    generating_queue.call(generating);
    sampling_queue.call(sampling);
}


int main(void)
{    

    uLCD.background_color(WHITE);
    uLCD.cls();
    uLCD.textbackground_color(WHITE);
    uLCD.color(RED);
    uLCD.locate(1, 2);
    uLCD.printf("\n1\n");
    uLCD.color(BLACK);
    uLCD.locate(1, 4);
    uLCD.printf("\n1/2\n");
    uLCD.locate(1, 6);
    uLCD.printf("\n1/4\n");
    uLCD.locate(1, 8);
    uLCD.printf("\n1/8\n");
    //int counter=3;

    //while(1) {
    thread.start(callback(&queue, &EventQueue::dispatch_forever));
    generating_thread.start(callback(&generating_queue, &EventQueue::dispatch_forever));
    sampling_thread.start(callback(&sampling_queue, &EventQueue::dispatch_forever)); 
    but_UP.rise(Up);
    but_DOWN.rise(Down);
    but_SEL.rise(Sel);


    ThisThread::sleep_for(10ms);            
    //}
}




void generating()
{
    while (1) {
        // assuming VCC = 3.3v
        float i=0;
        for (int x=0; x<t;x++) {
            i += (1.0/t);
            aout = i;
            // ADCdata[count]=Ain;             // comment out when not sampling
            // count++;
            ThisThread::sleep_for(1ms);
        }
        for (int x=0; x<(240-2*t); x++) {
            i = 1.0;
            aout = i;
            // ADCdata[count]=Ain;             // comment out when not sampling
            // count++;
            ThisThread::sleep_for(1ms);
        }
        for (int x=0; x<t; x++) {
            i -= (1.0/t);
            aout = i;
            // ADCdata[count]=Ain;             // comment out when not sampling
            // count++;
            ThisThread::sleep_for(1ms);
        }
    }
}

void sampling()
{
    while(1) {
        for (int i=0; i<2000; i++) {
            ADCdata[i] = Ain;
            ThisThread::sleep_for(1ms);
        }
        for (int i=500; i<1500; i++) {
            printf("%f\r\n", ADCdata[i]);
        }
        ThisThread::sleep_for(20s);
    }
}


// int main(void)
// {
//     int t=80;
//     while (1) {
//         // assuming VCC = 3.3v
//         float i=0;
//         for (int x=0; x<t;x++) {
//             i += (1.0/t);
//             aout = i;
//             // ADCdata[count]=Ain;             // comment out when not sampling
//             // count++;
//             ThisThread::sleep_for(1ms);
//         }
//         for (int x=0; x<(240-2*t); x++) {
//             i = 1.0;
//             aout = i;
//             // ADCdata[count]=Ain;             // comment out when not sampling
//             // count++;
//             ThisThread::sleep_for(1ms);
//         }
//         for (int x=0; x<t; x++) {
//             i -= (1.0/t);
//             aout = i;
//             // ADCdata[count]=Ain;             // comment out when not sampling
//             // count++;
//             ThisThread::sleep_for(1ms);
//         }
//     }
// }



void print(int counter) {
    //uLCD.cls();
    uLCD.color(BLACK);
    uLCD.locate(1, 2);
    uLCD.printf("\n1\n");    
    uLCD.locate(1, 4);
    uLCD.printf("\n1/2\n");
    uLCD.locate(1, 6);
    uLCD.printf("\n1/4\n");
    uLCD.locate(1, 8);
    uLCD.printf("\n1/8\n");

    if (counter==3) {
        uLCD.color(RED);
        uLCD.locate(1, 2);
        uLCD.printf("\n1\n");
    } else if (counter==2) {
        uLCD.color(RED);
        uLCD.locate(1, 4);
        uLCD.printf("\n1/2\n");
    } else if (counter==1) {
        uLCD.color(RED);
        uLCD.locate(1, 6);
        uLCD.printf("\n1/4\n");
    } else if (counter==0) {
        uLCD.color(RED);
        uLCD.locate(1, 8);
        uLCD.printf("\n1/8\n");
    }
}



/*
#include "mbed.h"
#include "uLCD_4DGL.h"


AnalogOut  aout(PA_4);
AnalogIn Ain(A0); 

DigitalIn but_SEL(D6);
DigitalIn but_UP(D5);
DigitalIn but_DOWN(D3);

uLCD_4DGL uLCD(D1, D0, D2);

void print(int counter);

float ADCdata[2000];
int main(void)
{
    int t=10;
    while (1) {
        // assuming VCC = 3.3v
        float i=0;
        for (int x=0; x<t;x++) {
            i += (1.0/t);
            aout = i;
            // ADCdata[count]=Ain;             // comment out when not sampling
            // count++;
            ThisThread::sleep_for(1ms);
        }
        for (int x=0; x<(240-2*t); x++) {
            i = 1.0;
            aout = i;
            // ADCdata[count]=Ain;             // comment out when not sampling
            // count++;
            ThisThread::sleep_for(1ms);
        }
        for (int x=0; x<t; x++) {
            i -= (1.0/t);
            aout = i;
            // ADCdata[count]=Ain;             // comment out when not sampling
            // count++;
            ThisThread::sleep_for(1ms);
        }
    }
}
*/