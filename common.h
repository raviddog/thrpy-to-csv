/*
 * common.h
 *
 *  Created on: 2010-5-9
 *      Author: Argon
 */

#ifndef COMMON_H_
#define COMMON_H_

unsigned int th06_decrypt(unsigned char *buf, char key, unsigned int length);
unsigned int get_bit(unsigned char* buffer, unsigned int &pointer, unsigned char &filter, unsigned char length);
//  lzss
unsigned int decompress(unsigned char * buffer, unsigned char * decode, unsigned int length);
void decode(unsigned char * buffer, int length, int block_size, unsigned char base, unsigned char add);


#endif /* COMMON_H_ */
