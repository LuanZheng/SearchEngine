#ifndef _CONSTANT_H_
#define _CONSTANT_H_

#define NULL 0
#define NPOS -1
const unsigned int MSG_HEADER_LEN = 4;
const unsigned int MSG_BUFFER_MAX_SIZE = 4096;
const unsigned int MSG_PLAIN_MAX_SIZE = 4080;

const unsigned int MSG_1001_SEARCH_REQ = 1001;
const unsigned int AESKEYLENGTH = 16;
const unsigned int CFBBLOCKSIZE = 16;

const unsigned int MSG_SPLITER_LEN = 2;

typedef unsigned char byte;
const byte key[AESKEYLENGTH] = { 0x01,0x02,0x03,0x04,0x05,0x06,0x07,0x08,0x01,0x02,0x03,0x04,0x05,0x06,0x07,0x08 };
const char MSG_SPLITER_TAG[MSG_SPLITER_LEN] = {0xFF, 0xFE};

const unsigned int MSG_QUEUE_MAX_LEN = 16384;

#endif
