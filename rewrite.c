#include "headers.h"

#define SIC_HALF_BYTE_DEFAULT 3
#define VARIABLE_WIDTH_FORMAT_INDEX 2
#define RECORD_OBJ_OFFSET 9
#define X_FLAG_BIT (1 << 15)

int rewriteTRecord(RECORD* record, int old_start, int new_start, int m_address, int half_bytes, char XE_flag)
{
    // find the correct t-record
    char* left;
    char variableWidthFormat[] = "%0*X";
    variableWidthFormat[VARIABLE_WIDTH_FORMAT_INDEX] = half_bytes + '0';

    while (record != NULL)
    {

        if (m_address >= record->start && m_address <= (record->start + record->len))
            break;

        record = record->next;
    }

    if (record == NULL)
        return 1; // TODO: error codes or error message

    // process the t-record
    int offset = m_address - record->start;

    left = record->data + RECORD_OBJ_OFFSET + (offset * 2);
    if (XE_flag && half_bytes > SIC_HALF_BYTE_DEFAULT)
        left++;

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