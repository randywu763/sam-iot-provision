#ifndef _FW_INFO_H_
#define _FW_INFO_H_

#define FIRMWARE_INFO_VECTOR_OFFSET     (4 + 4 + 4)  /*total size + sec offset + sec size (each are 4 byte)*/
#define FLASH_INFO_VECTOR_OFFSET        (4 + 4 + 4 + 4) /*NMIS + total size + sec offset + sec size (each are 4 byte)*/
#define INFO_VECTOR_MAGIC               0xdadbabba

typedef struct {
    unsigned long MagicNumber;
    unsigned long SequnceNumber;
    unsigned long VersionInfo;
    unsigned char Builddate[sizeof(__DATE__)];  //12 byte
    unsigned char Buildtime[sizeof(__TIME__)];  //9 byte
    unsigned char __PAD8__;
    unsigned short SvnNumber;
}tstrInfoVector;


#endif /* _FW_INFO_H_ */
