/*----------------------------------------------------------------------------*/
/*                                                                            */
/*    Module:       vex_addrled.h                                             */
/*    Author:       James Pearman                                             */
/*    Created:      1 Sept 2022                                               */
/*    Description:  Class for addressable led on 3wire expander               */
/*                                                                            */
/*----------------------------------------------------------------------------*/
//
#include "vex.h"

extern "C" {
  // private addressable LED API
  int32_t  vexAdiAddrLedSet( uint32_t index, uint32_t port, uint32_t *pData, uint32_t nOffset, uint32_t nLength, uint32_t options );
}
using namespace vex;

// this class extends an special class used for 3wire ports
class addressable_led : private __tridevice {
    public:
      static const int32_t MAX_LEDS = 64;

    private:      
      uint32_t maxled = MAX_LEDS;
      uint32_t ledbuffer[MAX_LEDS];
      uint32_t tmpbuffer[MAX_LEDS];

    public:
      addressable_led( triport::port &port, int32_t max = MAX_LEDS ) : __tridevice( port, triportType::digitalOutput ) {
        maxled = max <= MAX_LEDS ? max : MAX_LEDS;
      }

      ~addressable_led(){}

      void clear( color col = color(0x000000) ) {
        for(int i=0;i<maxled;i++) {
          ledbuffer[i] = col;
        }
        set( ledbuffer, 0, maxled, 0);
      }

      void set( uint32_t *pData, uint32_t nOffset, uint32_t nLength, uint32_t options ) {
        vexAdiAddrLedSet( _index(), _id(), pData, nOffset, nLength, options );

        // make copy
        uint32_t *p = pData;
        for(int i=nOffset;i<nLength && i<maxled;i++)
          ledbuffer[i] = *p++;
      }

      void set( color col = color(0x000000) ) {
        for(int i=0;i<maxled;i++) {
          ledbuffer[i] = col;
        }
      }

      void rotate( int n ) {
          if( abs(n) >= maxled )
            return;

          if( n > 0 ) {
            // make a copy
            memcpy( tmpbuffer, ledbuffer, sizeof(ledbuffer));
            memcpy( &ledbuffer[n], &tmpbuffer[0], (maxled-n)*sizeof(uint32_t) );
            memcpy( &ledbuffer[0], &tmpbuffer[maxled-1-n], (n)*sizeof(uint32_t) );          
          }
          else {
            n = -n;
            memcpy( tmpbuffer, ledbuffer, sizeof(ledbuffer));
            memcpy( &ledbuffer[0], &tmpbuffer[n], (maxled-n)*sizeof(uint32_t) );
            memcpy( &ledbuffer[maxled-n], &tmpbuffer[0], (n)*sizeof(uint32_t) );          
          }
      }

      void flush() {
        set( ledbuffer, 0, maxled, 0);
      }

      int32_t max() {
        return maxled;
      }
};

