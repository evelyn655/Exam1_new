# Exam1_new
```
#include "mbed.h"
#include "uLCD_4DGL.h"


AnalogOut  aout(PA_4);      // D7
AnalogIn Ain(A0); 

InterruptIn but_SEL(D6);
InterruptIn but_UP(D5);
InterruptIn but_DOWN(D3);

uLCD_4DGL uLCD(D1, D0, D2);

void print();
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

// button 按上，counter 計數，並把print在uLCD上的內容放到queue裡執行 
void Up()
{
    if(counter<3) counter++;
    else counter=3; 
    queue.call(print);
}

// 同 void Up()
void Down()
{
    if(counter>0) counter--;
    else counter=0;
    //print(counter);
    queue.call(print); 
    //ThisThread::sleep_for(50ms);
}

// 按下 select 後，把 generating 和 sampling 的 function 分別放入各自的queue中
void Sel()
{
    if (counter==3) t=80;           
    else if (counter==2) t=40;      
    else if (counter==1) t=20;      
    else if (counter==0) t=10;     
    generating_queue.call(generating);
    sampling_queue.call(sampling);
}


int main(void)
{    
    // display on uLCD
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
    

    // 每個 thread 都一直執行
    thread.start(callback(&queue, &EventQueue::dispatch_forever));
    generating_thread.start(callback(&generating_queue, &EventQueue::dispatch_forever));
    sampling_thread.start(callback(&sampling_queue, &EventQueue::dispatch_forever)); 
    but_UP.rise(Up);       
    but_DOWN.rise(Down);
    but_SEL.rise(Sel);
    


    ThisThread::sleep_for(10ms);            
    
}


// generating function
void generating()
{
    while (1) {
        // assuming VCC = 3.3v
        float i=0;
        for (int x=0; x<t;x++) {               // up-slope
            i += (1.0/t);
            aout = i;
            ThisThread::sleep_for(1ms);
        }
        for (int x=0; x<(240-2*t); x++) {     // top
            i = 1.0;
            aout = i;
            ThisThread::sleep_for(1ms);
        }
        for (int x=0; x<t; x++) {            // down-slope
            i -= (1.0/t);
            aout = i;
            ThisThread::sleep_for(1ms);
        }
    }
}

// sampling function
void sampling()
{
    while(1) {
        for (int i=0; i<2000; i++) {          // stored the Ain data into the array ADCdata
            ADCdata[i] = Ain;
            ThisThread::sleep_for(1ms);
        }
        for (int i=500; i<1500; i++) {      // print out the value
            printf("%f\r\n", ADCdata[i]);
        }
        ThisThread::sleep_for(20s);
    }
}

// for display on uLCD
void print() {
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

```
