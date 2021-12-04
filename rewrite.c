#include "headers.h"

#define SIC_HALF_BYTE_DEFAULT 3
#define VARIABLE_WIDTH_FORMAT_INDEX 2
#define RECORD_OBJ_OFFSET 9
#define X_FLAG_BIT (1 << 15)
#define X_FLAG_BIT_XE (1 << 3)
#define N_I_BIT_MASK 3

RECORD* rewriteBaseSICTRecord(RECORD* record, int old_start, int new_start, int m_address, int half_bytes)
{
    char* left;
    char variableWidthFormat[] = "%0*X";
    variableWidthFormat[VARIABLE_WIDTH_FORMAT_INDEX] = half_bytes + '0';

    // process the t-record
    int offset = m_address - record->start;

    left = record->data + RECORD_OBJ_OFFSET + (offset * 2);

    unsigned int objAddress = 0;
    if (sscanf(left, variableWidthFormat, &objAddress) == 0)
        return NULL;

    unsigned int xBit = objAddress & X_FLAG_BIT;
    objAddress = objAddress & (X_FLAG_BIT - 1);

    unsigned int relocatedAddress = (objAddress - old_start) + new_start;
    relocatedAddress |= xBit;

    char* tempBuffer = nmalloc(half_bytes + 1);
    sprintf(tempBuffer, variableWidthFormat, relocatedAddress); // do this so sprintf doesnt copy over the \0
    strncpy(left, tempBuffer, half_bytes);
    free(tempBuffer);

    return record;
}

RECORD* rewriteXeSicTRecord(RECORD* record, int old_start, int new_start, int m_address, int half_bytes)
{
    char* left;
    char variableWidthFormat[] = "%0*X";
    variableWidthFormat[VARIABLE_WIDTH_FORMAT_INDEX] = half_bytes + '0';

    // process the t-record
    int offset = m_address - record->start;

    // get the opcode and check ni bits
    left = record->data + RECORD_OBJ_OFFSET + (offset * 2) - 2;
    unsigned int opcode = 0;
    if (sscanf(left, variableWidthFormat, &opcode) == 0)
        return NULL;

    unsigned int ni_flags = opcode & N_I_BIT_MASK;
    if (!ni_flags) // SIC instruction backwards compatible 
        return rewriteBaseSICTRecord(record, old_start, new_start, m_address, half_bytes);
    // Possible check to see if operand is greater than 7FFF

    left += 3; // jump past xbpe half byte

    unsigned int objAddress = 0;
    if (sscanf(left, variableWidthFormat, &objAddress) == 0)
        return NULL;

    unsigned int relocatedAddress = (objAddress - old_start) + new_start;

    char* tempBuffer = nmalloc(half_bytes + 1);
    sprintf(tempBuffer, variableWidthFormat, relocatedAddress); // do this so sprintf doesnt copy over the \0
    strncpy(left, tempBuffer, half_bytes);
    free(tempBuffer);

    return record;
}

RECORD* rewriteTRecord(RECORD* record, int old_start, int new_start, int m_address, int half_bytes, char XE_flag)
{
    // find the correct t-record
    if (record == NULL) return NULL;

    if (!(m_address >= record->start && m_address <= (record->start + record->len)))
    {
        // we are not at the correct t record
        record = record->next;
        while (record != NULL)
        {

            if (m_address >= record->start && m_address <= (record->start + record->len))
                break;

            record = record->next;
        }
    }

    if (record == NULL)
        return NULL; // TODO: error codes or error message
    
    if (XE_flag)
        return rewriteXeSicTRecord(record, old_start, new_start, m_address, half_bytes);
    else
        return rewriteBaseSICTRecord(record, old_start, new_start, m_address, half_bytes);
}