#include "headers.h"

#define VARIABLE_WIDTH_FORMAT_INDEX 2
#define RECORD_OBJ_OFFSET 9
#define X_FLAG_BIT (1 << 15)

int rewriteSICTRecord(RECORD* record, int old_start, int new_start, int m_address, int half_bytes)
{
    // find the correct t-record
    char variableWidthFormat[] = "%0*X";
    variableWidthFormat[VARIABLE_WIDTH_FORMAT_INDEX] = half_bytes + '0';

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

        if (m_address >= currentRecordAddress && m_address <= (currentRecordAddress + recordSize))
            break;

        record = record->next;
    }

    if (record == NULL)
        return 1;

    // process the t-record
    int offset = m_address - currentRecordAddress;

    left = record->data + RECORD_OBJ_OFFSET + (offset * 2);
    unsigned int objAddress = 0;
    if (sscanf(left, variableWidthFormat, &objAddress) == 0)
        return 1;

    unsigned int xBit = objAddress & X_FLAG_BIT;
    objAddress = objAddress & (X_FLAG_BIT - 1);

    unsigned int relocatedAddress = (objAddress - old_start) + new_start;
    relocatedAddress |= xBit;

    char* tempBuffer = nmalloc(half_bytes + 1);
    sprintf(tempBuffer, variableWidthFormat, relocatedAddress); // do this so sprintf doesnt copy over the \0
    strncpy(left, tempBuffer, half_bytes);
    free(tempBuffer);

    return 0;
}