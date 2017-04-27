
#ifndef _AIS_H_
#define _AIS_H_

/* Information about individual AIS message types currently supported, see:
 *
 * http://catb.org/gpsd/AIVDM.html#_types_1_2_and_3_position_report_class_a
 * http://catb.org/gpsd/AIVDM.html#_type_18_standard_class_b_cs_position_report'
 */

#include <Arduino.h>
class AIS
{

public:
	AIS(const char *AISbitstream, unsigned int fillBits = 0);
	static const int msg_max = 60;
	bool getdata(unsigned int begin, unsigned int cnt, uint8_t *data, bool isigned = false);

	uint32_t get_mmsi();
	uint16_t get_SOG();
	uint16_t get_COG();
	uint16_t get_HDG();
	int32_t get_latitude();
	int32_t get_longitude();
	int8_t get_rot();  // Rate of Turn
	uint8_t get_repeat();
	uint8_t get_navStatus();
	uint8_t get_type();
	uint8_t get_timeStamp();
	uint8_t get_manIndicator();
	uint32_t get_imo();
	const char* get_shipname();
	uint8_t get_shiptype();
	uint16_t get_to_bow();
	uint16_t get_to_stern();
	uint8_t  get_to_port();
	uint8_t  get_to_starboard();
	uint8_t  get_epfd();
	uint8_t get_draught();
	const char* get_destination();
	const char* get_callsign();
	uint8_t get_ais_version();
	uint8_t get_month();
	uint8_t get_day();
	uint8_t get_hour();
	uint8_t get_minute();
	bool get_posAccuracy_flag();
	bool get_raim_flag();
	bool get_dte_flag();
	bool get_cs_flag();
	bool get_display_flag();
	bool get_dsc_flag();
	bool get_band_flag();
	bool get_msg22_flag();
	bool get_assigned_flag();
	uint32_t get_radio();
	uint8_t get_partno();
	const char* get_vendorid();
	uint32_t get_mothership_mmsi();

private:
	void decode(unsigned int fillBits);
	int getbit(unsigned int idx);
	uint16_t get_u16(unsigned start, unsigned len);
	uint32_t get_u32(unsigned start, unsigned len);
	int32_t get_i32(unsigned start, unsigned len);
	int8_t get_i8(unsigned start, unsigned len);
	uint8_t get_u8(unsigned start, unsigned len);
	void get_string(char* str, unsigned start, unsigned cnt);
	bool get_flag(unsigned start);

private:
	static const uint8_t shipname_strlen = 20; // a bits
	static const uint8_t destination_strlen = 20; // a bits
	static const uint8_t callsign_strlen = 7; // a 6 bits
	static const uint8_t vendorid_strlen = 3; // a 6 bits

	uint8_t msg[msg_max];
	unsigned int msgLen;
	uint8_t msgType;
	//TODO: Allocates strings based on msgType
	char shipname[shipname_strlen + 1];
	char destination[destination_strlen+1];
	char callsign[callsign_strlen+1];
	char vendorid[vendorid_strlen+1];
};


#endif // _AIS_H_
