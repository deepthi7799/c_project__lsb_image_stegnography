#ifndef DECODE_H
#define DECODE_H

#include "types.h"      //contains user defined data types

#define MAX_DECODED_BUF_SIZE 1
#define MAX_IMAGE_SIZE (MAX_DECODED_BUF_SIZE * 8)
#define MAX_FILE_SUFFIX 4

/*
 * Structure to store information required for
 * decoding stego image and secret file
 * Info about output and intermediate data is
 * also stored
 */

typedef struct _DecodeInfo
{

    /* Stego Image Info */
    char *stego_image_fname;
    FILE *fptr_stego_image;
	char stego_image_data[MAX_IMAGE_SIZE];

    /* Decoded File Info */
    char *decoded_fname;
    FILE *fptr_decoded;
    char extn_decoded_file[MAX_FILE_SUFFIX];
    long size_decoded_file;

} DecodeInfo;

/* Decoding function prototype */

/* Read and validate Decode args from argv */
Status read_and_validate_decode_args(char *argv[],DecodeInfo *decInfo);

/* Perform the decoding */
Status do_decoding(DecodeInfo *decInfo);

/* Get File pointers for i/p files */
Status open_stego_file(DecodeInfo *decInfo);

/* Get File pointers for o/p files */
Status open_decoded_file(DecodeInfo *decInfo);

/* Decode Magic String */
Status decode_magic_string(char *magic_string,DecodeInfo *decInfo);

/* Decode secret file extension size */
Status decode_secret_file_extn_size(int size,DecodeInfo *decInfo);

/* Decode secret file extension */
Status decode_secret_file_extn(char *file_extn,DecodeInfo *decInfo);

/* Decode secret file size */
Status decode_secret_file_size(long file_size,DecodeInfo *decInfo);

/* Decode secret file data */
Status decode_secret_file_data(DecodeInfo *decInfo);

/* Decode Function, which does real decoding */
Status decode_data_from_image(char *data,int size,DecodeInfo *decInfo);

/* Decode a byte from LSB of image data array */
Status decode_byte_from_lsb(char data,char *image_buff,int flag);

/* Decode size from LSB */
Status decode_size_from_lsb(int size,DecodeInfo *decInfo,int flag);

/* Decode secret data to file */
Status decode_data_to_file(char decode_data,char *source,DecodeInfo *decInfo);


/* Close File pointers of i/p files and o/p files */
Status close_decoded_files(DecodeInfo *decInfo);

#endif
