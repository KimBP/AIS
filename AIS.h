
#ifndef _AIS_H_
#define _AIS_H_

#include <Arduino.h>
class AIS
{

public:
	AIS(const char *AISmsg);
	static const int msg_max = 100;
	bool getdata(int begin, int cnt, uint8_t *data);

private:
	uint8_t* nextParam();
	void decode();

private:
	uint8_t* next;
	int sentences;
	int sentenceNmb;
	char msgId;
	char channel;
	uint8_t msg[msg_max];
	int msgLen;
};


#endif // _AIS_H_
