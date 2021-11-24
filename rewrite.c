#include "headers.h"

#define RECORD_OBJ_OFFSET 9
#define NUM_HALF_BYTES_TO_EDIT 4
#define X_FLAG_BIT (1 << 15)

int rewriteSICTRecord(TRECORD* record, int oldStart, int newStart, int mAddress)
{
    // find the correct t-record
    int currentRecordAddress = 0;
    char* left;

    while (record != NULL)
    {
        left = record->data + RECORD_ADDR_OFFSET;
        if (sscanf(left, "%06X", &currentRecordAddress) == 0)
            return 1;

        left = record->data + RECORD_SIZE_OFFSET;
        int recordSize = 0;
        if (sscanf(left, "%02X", &recordSize) == 0)
            return 1;

        if (mAddress >= currentRecordAddress && mAddress <= (currentRecordAddress + recordSize))
            break;

        record = record->next;
    }

    if (record == NULL)
        return 1;

    // process the t-record
    int offset = mAddress - currentRecordAddress;

    left = record->data + RECORD_OBJ_OFFSET + (offset * 2);
    unsigned int objAddress = 0;
    if (sscanf(left, "%04X", &objAddress) == 0) // assuming the number of half bytes we are editing is 4
        return 1;

    unsigned int xBit = objAddress & X_FLAG_BIT;
    objAddress = objAddress & (X_FLAG_BIT - 1);

    unsigned int relocatedAddress = (objAddress - oldStart) + newStart;
    relocatedAddress |= xBit;

    char tempBuffer[NUM_HALF_BYTES_TO_EDIT + 1] = { 0 };
    sprintf(tempBuffer, "%04X", relocatedAddress); // do this so sprintf doesnt copy over the \0
    strncpy(left, tempBuffer, NUM_HALF_BYTES_TO_EDIT);

    return 0;
}