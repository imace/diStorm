#include "../../distorm.h"#include <stdio.h>#include <stdlib.h>#include <sys/stat.h>#define INSTSLIMIT 1000int main(){    FILE* fp;    struct stat st;    unsigned char* buf, *buf2;    char* names[] = {"bin16", "bin32", "bin64"};    int modes[] = {Decode16Bits, Decode32Bits, Decode64Bits};    _DecodedInst decodedInstructions[INSTSLIMIT];    unsigned int decodedInstructionsCount = 0, i = 0;    _DecodeResult res;    _OffsetType offset = 0;    unsigned int dver = distorm_version();    int next;    printf("diStorm version: %d.%d.%d\n", (dver >> 16), ((dver) >> 8) & 0xff, dver & 0xff);    for (i = 0; i < 3; i++) {        decodedInstructionsCount = 0;        offset = 0;        printf("%s\n", names[i]);        fp = fopen(names[i], "rb");      	fstat(fileno(fp), &st);        buf2 = buf = (unsigned char*)malloc(st.st_size);        fread(buf, 1, st.st_size, fp);        fclose(fp);        while (1) {    		res = distorm_decode(offset, (const unsigned char*)buf, st.st_size, modes[i], decodedInstructions, INSTSLIMIT, &decodedInstructionsCount);    		if (res == DECRES_SUCCESS) break;    		else if (decodedInstructionsCount == 0) break;    		next = (unsigned int)(decodedInstructions[decodedInstructionsCount-1].addr - offset);    		next += decodedInstructions[decodedInstructionsCount-1].size;    		buf += next;    		st.st_size -= next;    		offset += next;    	}        free(buf2);    }    return 0;}