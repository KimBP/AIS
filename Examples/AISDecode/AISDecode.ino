/**********************************************************************
  AISDecode.cpp - Copyright (c) 2016-2020 Kim Bøndergaarg <kim@fam-boendergaard.dk>

  Permission is hereby granted, free of charge, to any person obtaining
  a copy of this software and associated documentation files (the
  ``Software''), to deal in the Software without restriction, including
  without limitation the rights to use, copy, modify, merge, publish,
  distribute, sublicense, and/or sell copies of the Software, and to
  permit persons to whom the Software is furnished to do so, subject to
  the following conditions:

  The above copyright notice and this permission notice shall be
  included in all copies or substantial portions of the Software.

  THE SOFTWARE IS PROVIDED ``AS IS'', WITHOUT WARRANTY OF ANY KIND,
  EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
  MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.
  IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY
  CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT,
  TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE
  SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.

 **********************************************************************/


#include <Arduino.h>
#include <AIS.h>

/* Coming from util.h */
#define htonl(x) ( ((x)<<24 & 0xFF000000UL) | \
                   ((x)<< 8 & 0x00FF0000UL) | \
                   ((x)>> 8 & 0x0000FF00UL) | \
                   ((x)>>24 & 0x000000FFUL) )
#define htons(x) ( ((x)<<8) | (((x)>>8)&0xFF) )

void printDegrees(long min4) 
{
//    Serial.print(min4); Serial.print(" (");Serial.print(min4,16);Serial.println(")");
    long intPart = min4 / 60L;
    long fracPart = intPart % 10000L;
    if (fracPart < 0)
      fracPart = -fracPart;
    char frac[6];
    sprintf(frac, "%04ld", fracPart);
    Serial.print(intPart/10000L);Serial.print(".");Serial.print(frac);
}

void showMMSI(AIS& ais_msg) {
  unsigned long mmsi = ais_msg.get_mmsi();
  Serial.print("Returned MMSI: ");
  Serial.print(mmsi);
  Serial.print(" ("); Serial.print(mmsi, 16); Serial.print(" )");
  Serial.println("");
}

void showSOG(AIS& ais_msg) {
    unsigned int SOG = ais_msg.get_SOG();
    Serial.print("Returned SOG: ");
    Serial.print( (SOG) / 10 ); Serial.print("."); Serial.print( (SOG) % 10 ); Serial.println(" nm");  
}

void showCOG(AIS& ais_msg) {
    unsigned int COG = ais_msg.get_COG();
    Serial.print("Returned COG: ");
    Serial.print( (COG) / 10 ); Serial.print("."); Serial.print( (COG) % 10 ); Serial.println(" degrees");  
}

void showLatitude(AIS& ais_msg) {
    long LAT = ais_msg.get_latitude();
    Serial.print("Returned LAT: "); printDegrees(LAT); Serial.println(" degrees");  
}

void showLongitude(AIS& ais_msg) {
    long LONG = ais_msg.get_longitude();
    Serial.print("Returned LONG: "); printDegrees(LONG); Serial.println(" degrees");  
}

void setup() {
  Serial.begin(115200);

  AIS ais_msg("14eG;o@034o8sd<L9i:a;WF>062D");
  /* Console output (!AIVDM,1,1,,A,14eG;o@034o8sd<L9i:a;WF>062D,0*7D)
   *  
   * Returned MMSI: 316001245 (12D5CBDD )
   * Returned SOG: 19.6 nm
   * Returned COG: 235.0 degrees
   * Returned LAT: 49.2002 degrees
   * Returned LONG: -123.8777 degrees
   */
  //AIS ais_msg("177KQJ5000G?tO`K>RA1wUbN0TKH");
  /*  Console output: (!AIVDM,1,1,,B,177KQJ5000G?tO`K>RA1wUbN0TKH,0*5C)
   *   
   *  Returned MMSI: 477553000 (1C76E168 )
   *  Returned SOG: 0.0 nm
   *  Returned COG: 51.0 degrees
   *  Returned LAT: 47.5828 degrees
   *  Returned LONG: -122.3458 degrees
   */

  showMMSI(ais_msg);
  showSOG(ais_msg);
  showCOG(ais_msg);
  showLatitude(ais_msg);
  showLongitude(ais_msg);
}

void loop() {

}
