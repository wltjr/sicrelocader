#include "headers.h"

int rewriteSICTRecord(TRECORD* record, int oldStart, int newStart, int mAddress)
{
	// TOOD: find the correct t-record

	// process the t-record
	char* left, right;
	left = record->data + 1;
	right = left + 6;
	int currentAddress = strtol(left, &right, 16);
	sprintf(left, "%0*X", 6, (currentAddress - oldStart) + newStart);
}
