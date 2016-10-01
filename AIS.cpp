

#include <AIS.h>


AIS::AIS(const char *AISmsg)
: next(0), sentences(0),sentenceNmb(0), msgId(0),channel(0), msgLen(0)
{
	if (strncmp(AISmsg, "!AIVDM,", 7)) {
		// This is not an AIVDM message - better bail out
		Serial.println("AIS(): Not an AIVDM message");
		msg[0] = '\0';
		return;
	}

	{ // Copy message into own memory
		int i = 0;
		AISmsg += 7; // Skip first parameter (message type)
		while (*AISmsg != '\0' && *AISmsg != '\n') {
			msg[i] = *(uint8_t*)AISmsg;
			i++;
			AISmsg++;
			if (i >= msg_max) {
				// Bail out - message too long
				msg[0] = '\0';
				Serial.println("AIS(): message too long");
				return;
			}
		}
	}
	{ // Find sentences
		uint8_t* tmp = nextParam();

		if ((next - tmp) != 1) {
			// Expect message never to consist of more than 9 messages
			msg[0] = '\0';
			Serial.println("AIS(): sentence count more than one digit");
			return;
		}
		sentences = *tmp - '0';
		// Can't really handle multi sentence messages for now
		if (sentences != 1) {
			sentences = 0;
			msg[0] = '\0';
			Serial.println("AIS(): Multi sentence message not supported yet");
			return;
		}

	}

	{ // Find sentence number
		uint8_t* tmp = nextParam(); // Don't care for now
	}

	{ // Find msgId
		uint8_t* tmp = nextParam(); // Don't care for now
	}

	{
		// Find channel
		uint8_t* tmp = nextParam(); // Don't care for now
	}

	{ // This is tricky - we basically move the encoded message into the string we are reading
	  // from  - but it is OK.
		uint8_t* tmp = nextParam();
		int i = 0;
		while (*tmp != '\0') {
			msg[i] = *tmp;
			i++;
			tmp++;
		}
		msg[i] = *tmp; // Terminate msg
		Serial.print("msg[] : ");
		Serial.println((char*)msg);

		// Time to decode the AIS data
		decode();
	}
}

/*
 * Return current parameter terminated by '\0'.
 * Prepare next to find next
 */
uint8_t* AIS::nextParam()
{
	uint8_t *first;
	if (next==0) {
		next = msg;
		first = msg;
	} else {
		next++;
		first = next;
	}
	while (*next != ',' && *next != '\n' && *next != '\0') {
		next++;
	}
	*next = '\0';
	Serial.print("Length of param: ");
	Serial.println(next - first);
	return first;
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

	Serial.println("Decoding: ");
	while (msg[srcIdx] != '\0') {
		src = msg[srcIdx] - '0';
		if (src > 40) {
			src -= 8;
		}
		Serial.print(src,16);
		Serial.print(" ");
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
	Serial.println("");
	Serial.print("Decoded string: ");
	for (int i=0; i < msgLen/8; i++) {
		Serial.print(msg[i],16);
		Serial.print(" ");
	}
	Serial.print("\ndecode(): decoded len ");
	Serial.print(msgLen);
	Serial.println(" bits");
}

/*
 * getdata return in data cnt bits from decoded AIS data, starting from begin
 * If not enough data function returns false, otherwise true
 */
bool AIS::getdata(int begin, int cnt, uint8_t *data)
{
	if (begin+cnt > msgLen)
		return false;

	int outIdx = cnt / 8;          // We start from LSB
	int endIdx = (begin+cnt) / 8;
	int endShift = (begin+cnt) % 8;
	int startIdx = begin /8;
	int startBits = begin % 8;

	Serial.print("endIdx:    "); Serial.println(endIdx);
	Serial.print("startIdx:  "); Serial.println(startIdx);
	Serial.print("endShift:  "); Serial.println(endShift);
	Serial.print("startBits: "); Serial.println(startBits);
	Serial.print("outIdx:    "); Serial.println(outIdx);

	if (endIdx == startIdx) {
		if (endShift) {
			data[outIdx] = (msg[endIdx] >> (8 - endShift));
		} else {
			data[outIdx] = msg[endIdx];
		}
	}
	while (endIdx > startIdx) {
		if (endShift) {
			Serial.print("q1) msg[");Serial.print(endIdx);Serial.print("] = ");Serial.println(msg[endIdx],16);
			data[outIdx] = (msg[endIdx] >> (8-endShift));
			Serial.print("a1) data["); Serial.print(outIdx); Serial.print("] = "); Serial.println(data[outIdx],16);
			endIdx--;
			Serial.print("q2) msg[");Serial.print(endIdx);Serial.print("] = ");Serial.println(msg[endIdx],16);
			data[outIdx] |= (msg[endIdx] << endShift);
			Serial.print("a2) data["); Serial.print(outIdx); Serial.print("] = "); Serial.println(data[outIdx],16);
		} else {
			Serial.print("w) msg[");Serial.print(endIdx);Serial.print("] = ");Serial.println(msg[endIdx],16);
			data[outIdx] = msg[endIdx];
			endIdx--;
			Serial.print("b) data["); Serial.print(outIdx); Serial.print("] = "); Serial.println(data[outIdx],16);
		}
		outIdx--;
		cnt -= 8;
	}
	if (startBits) {
		data[outIdx] &= ((1 << (8 - startBits)) - 1);
		cnt += (8-startBits); // Ought now be 0
		Serial.print("c) data["); Serial.print(outIdx); Serial.print("] = "); Serial.println(data[outIdx],16);
	}
	return true;
}
