/*----------------------------------------------------------------------------*/
/*                                                                            */
/*    Module:       main.cpp                                                  */
/*    Author:       james                                                     */
/*    Created:      Wed Jun 29 2022                                           */
/*    Description:  V5 project                                                */
/*                                                                            */
/*----------------------------------------------------------------------------*/
#include "vex.h"
#include "vex_addrled.h"

// A global instance of vex::brain used for printing to the V5 brain screen
vex::brain       Brain;

// instance of the addressable led on external 3wire extender port A
// we limit to 60 leds max for this strip
addressable_led addrled(vex::triport(PORT20).A, 60 );

// local storage for some pixel data
uint32_t data[addressable_led::MAX_LEDS];

int main() {    
    // set all led black
    // probably need some initial delay, TBD
    this_thread::sleep_for(200);    
    addrled.clear();
    this_thread::sleep_for(1000);    

    while(1) {        
        addrled.clear( color(64,0,0) );
        this_thread::sleep_for(1000);    
        addrled.clear( color(0,64,0) );
        this_thread::sleep_for(1000);    
        addrled.clear( color(0,0,64) );
        this_thread::sleep_for(1000);    
        addrled.clear();
        this_thread::sleep_for(50);

        // set one led at a time
        for( int i=0;i<addrled.max();i++ ) {
          uint32_t c = color(64,0,0);
          addrled.set( &c, i, 1, 0 );
          this_thread::sleep_for(50);
        }

        memset(data, 0, sizeof(data));
        for( int i=0;i<addrled.max();i+=12 ) {
          vex::color c;
          // use low value to keep current down
          for(int j=0;j<12;j++)
            data[i+j] = c.hsv(  j*30, 1, 0.25);    // vex color will be converted to uint32_t
        }

        addrled.set( data, 0, addrled.max(), 0 );
        this_thread::sleep_for(1000);    

        for( int i=0;i<addrled.max()-1;i++ ) {
          addrled.rotate(1);
          addrled.flush();
          this_thread::sleep_for(50);
        }
    }

    // turn all off
    addrled.clear();

    while(1) {
        // Allow other tasks to run
        this_thread::sleep_for(10);
    }
}
