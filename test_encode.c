/*
Name:
Date:
Description:
Sample input:
Sample output:
*/
#include <stdio.h>
#include "encode.h"
#include "types.h"
#include "Encode.c"
#include "Decode.c"
#include "decode.h"
#include<string.h>

int main(int argc,char *argv[])
{
        EncodeInfo encInfo;//structure variable declaration
	DecodeInfo decInfo;
   	int res=check_operation_type(argv);
	if(res==e_success)
	{
		if(argc < 4)
		{
			printf("ERROR:Enter cmd arguments\n");
			return 1;
		}
	}
	else if(res==e_failure)
	{
		if(argc < 3)
		{
			printf("ERROR:Enter cmd argumens\n");
			return 1;
		}
	}
        if(res==e_encode)
        {
                printf("INFO : Selected Encoding\n");
                if(read_and_validate_encode_args(argv,&encInfo)==e_success)
                {
                        printf("read and validate args are success\n");
                        if(do_encoding(&encInfo) == e_success)
                        {
                                printf("Encoding is successfull\n");
                        }
                        else
                        {
                                printf("can't able to return perform encoding\n");
                                return 2;
                        }
                }
                else
                {
                        printf("read and validate args are failure\n");

                }
        }
        else if(res==e_decode)
        {
                printf("INFO :Decoded is selected\n");
		if(read_and_validate_decode_args(argv,&decInfo)==e_success)
                {
                        printf("read and validate args are success\n");
                        if(do_decoding(&decInfo) == e_success)
                        {
                                printf("Decoding is successfull\n");
                        }
                        else
                        {
                                printf("can't able to return perform Decoding\n");
                                return 2;
                        }
                }
                else
                {
                        printf("read and validate args are failure\n");

                }

        }
        else if(res==e_unsupported)
        {
                printf("ERROR : Unsupported option\n");
                printf("For Encoding : ./a.out -e image.bmp secret.txt [stego.bmp]\n");
                printf("For Decoding : ./a.out -d stego.bmp [data.txt]\n");
        }

    return 0;
}
OperationType check_operation_type(char *argv[])
{
        if(strcmp(argv[1],"-e")==0)
        {
                return e_encode;
        }
        else if(strcmp(argv[1],"-d")==0)
        {
                return e_decode;
        }
        else
        {
                return e_unsupported;
        }
}
