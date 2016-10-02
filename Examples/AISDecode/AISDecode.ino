
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

void setup() {
  Serial.begin(115200);

  //AIS ais_msg("!AIVDM,1,1,,A,14eG;o@034o8sd<L9i:a;WF>062D,0*7D");
  AIS ais_msg("!AIVDM,1,1,,B,177KQJ5000G?tO`K>RA1wUbN0TKH,0*5C");
  uint8_t data[4];

  // Get MMSI
  if (false == ais_msg.getdata(8, 30, data)) {
    Serial.println("AIS getdata failed");
    return;
  }
  Serial.print("AIS data: ");
  Serial.print(data[0], 16); Serial.print("-");
  Serial.print(data[1], 16); Serial.print("-");
  Serial.print(data[2], 16); Serial.print("-");
  Serial.print(data[3], 16);
  Serial.println();

  unsigned long* mmsi = (unsigned long*)data;
  *mmsi = htonl(*mmsi);
  Serial.print("Returned MMSI: ");
  Serial.print(*mmsi);
  Serial.print(" ("); Serial.print(*mmsi, 16); Serial.print(" )");
  Serial.println("");

  // Get SOG
  if (false == ais_msg.getdata(50, 10, data)) {
    Serial.println("AIS getdata failed");
    return;
  }
  {
    unsigned int *SOG = (unsigned int*)data;
    *SOG = htons(*SOG);
    Serial.print("Returned SOG: ");
    Serial.print( (*SOG) / 10 ); Serial.print("."); Serial.print( (*SOG) % 10 ); Serial.println(" nm");
  }

  // Get COG
  if (false == ais_msg.getdata(116, 12, data)) {
    Serial.println("AIS getdata failed");
    return;
  }
  {
    unsigned int *COG = (unsigned int*)data;
    *COG = htons(*COG);
    Serial.print("Returned COG: ");
    Serial.print( (*COG) / 10 ); Serial.print("."); Serial.print( (*COG) % 10 ); Serial.println(" degrees");
  }

  // Get Latitude
  if (false == ais_msg.getdata(89, 27, data)) {
    Serial.println("AIS getdata failed");
    return;
  }
  {
    long *LAT = (long*)data;
    *LAT = htonl(*LAT);
    *LAT = AIS::fixSign<long>(27, *LAT);    
    Serial.print("Returned LAT: "); printDegrees(*LAT); Serial.println(" degrees");
  }

  // Get Longitude
  if (false == ais_msg.getdata(61, 28, data)) {
    Serial.println("AIS getdata failed");
    return;
  }
  {
    long *LONG = (long*)data;
    *LONG = htonl(*LONG);
    *LONG = AIS::fixSign<long>(28, *LONG);    
    //Serial.print("Raw long: "); Serial.print(*LONG); Serial.print(" ("); Serial.print(*LONG,16); Serial.println(")");
    Serial.print("Returned LONG: "); printDegrees(*LONG); Serial.println(" degrees");
  }

}

void loop() {

}
