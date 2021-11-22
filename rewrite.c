#include "headers.h"

#define RECORD_ADDR_OFFSET 1
#define RECORD_SIZE_OFFSET 7
#define RECORD_OBJ_OFFSET 9
#define NUM_HALF_BYTES_TO_EDIT 4
#define X_FLAG_BIT (1 << 15)

int rewriteSICTRecord(TRECORD* record, int oldStart, int newStart, int mAddress)
{
	// find the correct t-record
	int currentRecordAddress;
	char* left;

	while (record != NULL)
	{
		left = record->data + RECORD_ADDR_OFFSET;
		sscanf(left, "%06X", &currentRecordAddress);

		left = record->data + RECORD_SIZE_OFFSET;
		int recordSize;
		sscanf(left, "%02X", &recordSize);

		if (mAddress >= currentRecordAddress && mAddress <= (currentRecordAddress + recordSize))
			break;

		record = record->next;
	}

	if (record == NULL)
		return 1;

	// process the t-record
	int offset = mAddress - currentRecordAddress;

	left = record->data + RECORD_OBJ_OFFSET + (offset * 2);
	int objAddress;
	sscanf(left, "%0*X", NUM_HALF_BYTES_TO_EDIT, &objAddress); // assuming the number of half bytes we are editing is 4

	char xBitSet = objAddress & X_FLAG_BIT;
	objAddress = objAddress & (X_FLAG_BIT - 1);

	unsigned int relocatedAddress = (objAddress - oldStart) + newStart;
	relocatedAddress |= xBitSet;

	char tempBuffer[NUM_HALF_BYTES_TO_EDIT + 1];
	sprintf(tempBuffer, "%0*X", NUM_HALF_BYTES_TO_EDIT, relocatedAddress); // do this so sprintf doesnt copy over the \0
	strncpy(left, tempBuffer, NUM_HALF_BYTES_TO_EDIT);

	return 0;
}