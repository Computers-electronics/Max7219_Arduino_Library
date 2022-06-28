#include <Max7219.h>
#define HARDWARE_TYPE Max7219::FC16_HW
#define MAX_DEVICES 4
Max7219 m(HARDWARE_TYPE, 2, 3, 4, MAX_DEVICES); //dataPin = 2,clkPin = 3,csPin = 4
int a = 1;  //to count rows
int b = 0;  //to count devices
int i =  0; //intensity value
void setup(){
  m.begin();  //initializate display
  m.clear();  //clear display
  for(int d = 0;d<MAX_DEVICES;d++){
    m.command(d,INTENSITY,0); //set intensity to 0
  }
}
void loop(){
  if(a==9){
    a = 1;
    b++;
  }
  if(b==MAX_DEVICES){
    b=0;
    i++;
    for(int d = 0;d<MAX_DEVICES;d++){
      m.command(d,INTENSITY,i);
    }
    delay(500);
  }
  if(a%2==0){
    m.command(b,a,0b01010101);
  }
  else{
    m.command(b,a,0b10101010);
  }
  a++;
}
