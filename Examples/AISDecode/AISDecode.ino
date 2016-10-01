
#include <Arduino.h>
#include <AIS.h>

/* Coming from util.h */
#define htonl(x) ( ((x)<<24 & 0xFF000000UL) | \
                   ((x)<< 8 & 0x00FF0000UL) | \
                   ((x)>> 8 & 0x0000FF00UL) | \
                   ((x)>>24 & 0x000000FFUL) )

void setup() {
	Serial.begin(115200);

	//char *msg = "!AIVDM,1,1,,A,14eG;o@034o8sd<L9i:a;WF>062D,0*7D";

	AIS ais_msg("!AIVDM,1,1,,A,14eG;o@034o8sd<L9i:a;WF>062D,0*7D");
	uint8_t data[30];

	if (false == ais_msg.getdata(8,30,data)) {
		Serial.println("AIS getdata failed");
		return;
	}
	Serial.print("AIS data: ");
	Serial.print(data[0],16); Serial.print("-");
	Serial.print(data[1],16); Serial.print("-");
	Serial.print(data[2],16); Serial.print("-");
	Serial.print(data[3],16);
	Serial.println();

	unsigned long* mmsi = (unsigned long*)data;
  *mmsi =htonl(*mmsi);
	Serial.print("Returned MMSI: ");
	Serial.print(*mmsi);
  Serial.print(" ("); Serial.print(*mmsi,16); Serial.print(" )");
  Serial.println("");
  
	
}

void loop() {

}
