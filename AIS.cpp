

#include <AIS.h>


AIS::AIS(const char *AISbitstream)
: msgLen(0)
{
	uint8_t* tmp = (uint8_t*)AISbitstream;
	int i = 0;
	while (*tmp != '\0') {
		msg[i] = *tmp;
		i++;
		tmp++;
	}
	msg[i] = *tmp; // Terminate msg

	// Time to decode the AIS data
	decode();
}




/*
 * Decode msg into it self. 4 bytes becomes 3, i.e. it is OK to do it
 * cnt    byte
 *  0   --qqqqqq
 *  1   --yyyyyy
 *  2   --zzzzzz
 *  3   --wwwwww
 *
 * becomes | qq qq qq yy | yy yy zz zz | zz ww ww ww |
 */
void AIS::decode()
{
	int srcIdx=0;
	int dstIdx=0;
	int cnt=0;
	uint8_t src;

	while (msg[srcIdx] != '\0') {
		src = msg[srcIdx] - '0';
		if (src > 40) {
			src -= 8;
		}
		switch (cnt) {
		case 0:
			msg[dstIdx] = (src << 2);
			break;
		case 1:
			msg[dstIdx] |= (src >> 4);
			dstIdx++;
			msg[dstIdx] = (src << 4);
			break;
		case 2:
			msg[dstIdx] |= (src >> 2);
			dstIdx++;
			msg[dstIdx] = (src << 6);
			break;
		case 3:
			msg[dstIdx] |= src;
			dstIdx++;
			break;
		}
		srcIdx++;
		cnt++;
		if (cnt==4) {
			cnt = 0;
		}
	}
	msgLen = srcIdx*6;
}

int AIS::getbit(unsigned int idx)
{
	int byteIdx = idx / 8;
	int bitIdx = 7 - (idx % 8);

	return ((msg[byteIdx] >> bitIdx) & 0x01);
}

/*
 * getdata return in data cnt bits from decoded AIS data, starting from begin
 * If not enough data function returns false, otherwise true
 */
bool AIS::getdata(unsigned int begin, unsigned int cnt, uint8_t *data)
{
	if (begin+cnt > msgLen)
		return false;

	unsigned int srcIdx = begin;
	unsigned int dstIdx = 0;
	unsigned int dstBitIdx = cnt % 8;

	if (dstBitIdx == 0)
		dstBitIdx = 8;

	data[dstIdx] = 0;
	while (srcIdx < begin + cnt) {
		dstBitIdx--;
		data[dstIdx] |= (getbit(srcIdx) << dstBitIdx);
		srcIdx++;
		if (0 == dstBitIdx) {
			dstBitIdx = 8;
			dstIdx++;
			data[dstIdx] = 0;
		}
	}

	return true;
}
