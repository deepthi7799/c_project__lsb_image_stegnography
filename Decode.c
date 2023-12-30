#include<stdio.h>
#include<string.h>
#include "decode.h"
#include "types.h"
#include "common.h"

//read_and_validate_decode_args Function Definition

int name;

Status read_and_validate_decode_args(char *argv[],DecodeInfo *decInfo)
{
	//validating whether .bmp image file is Passed or not

	if ( strstr(argv[2],".bmp") != NULL )
	{
		if ( strcmp(strstr(argv[2],".bmp"),".bmp") == 0 )
		{
			decInfo -> stego_image_fname = argv[2];			//storing the image file name
		}
	}

	//printing an error if the .bmp image file is not passed

	else
	{
		printf("ERROR: Stego Image File Format Should be <.bmp file>\n");
		printf("ERROR: Usage!\n");
		printf("USAGE: Decoding: %s -d <.bmp file> [output file]\n",argv[0]);
		return e_failure;
	}
	
	//creating a default output file name if output file name is not passed

	if ( argv[3] == NULL )
	{
		name = 1;
		decInfo -> decoded_fname = "Decoded.txt";		//storing the output file name
	}
	
	//validating whether .txt file is passed or not
	
	else if ( strstr(argv[3],".txt") != NULL )
	{
		if ( strcmp(strstr(argv[3],".txt"),".txt") == 0 )
		{
			decInfo -> decoded_fname = argv[3];			//storing the output file name
		}
	}

	//validating whether .sh file is passed or not

	else if ( strstr(argv[3],".sh") != NULL )
	{
		if ( strcmp(strstr(argv[3],".sh"),".sh") == 0 )
		{
			decInfo -> decoded_fname = argv[3];			//storing the output file name
		}
	}
	
	//validating whether .c file is passed or not

	else if ( strstr(argv[3],".c") != NULL )
	{
		if ( strcmp(strstr(argv[3],".c"),".c") == 0 )
		{
			decInfo -> decoded_fname = argv[3];			//storing the output file name
		}
	}

	//printing an error if the output file format is not given properly

	else
	{
		printf("ERROR: Output File Format Should be <.txt file> or <.sh file> or <.c file>\n");
		printf("ERROR: Usage!\n");
		printf("USAGE: Decoding: %s -d <.bmp file> [output file]\n",argv[0]);
		return e_failure;
	}

	return e_success;
}

//do_decoding Function Definition

Status do_decoding(DecodeInfo *decInfo)
{
	printf("INFO: ## Decoding Procedure Started ##\n");
	
	int open,check_magic,file_ext_size,file_extn_len,file_ext,file_size,file_data;

	//calling open_files function to open the required files

	open = open_stego_file(decInfo);

	//validating whether opening files operation is success or not

	if ( open == e_success )
	{
		//calling check_magic_string function to validate the magic string

		printf("INFO: Decoding Magic String Signature\n");
		
		fseek(decInfo -> fptr_stego_image,54,SEEK_SET);
		check_magic = decode_magic_string(MAGIC_STRING,decInfo);

		//validating whether decode magic string operation is success or not

		if ( check_magic == e_success )
		{
			printf("INFO: Done\n");
			
			printf("INFO: Decoding Output File Extension Size\n");

			//getting the file extension size

			strcpy(decInfo -> extn_decoded_file,strstr(decInfo -> decoded_fname,"."));

			//getting the length of decoded file extension

			file_extn_len = strlen(decInfo -> extn_decoded_file);
			
			//calling decode_secret_file_extn_size function to decode output file extension size

			file_ext_size = decode_secret_file_extn_size(file_extn_len,decInfo);
			
			//validating whether decode secret file extension size operation is success or not

			if ( file_ext_size == e_success )
			{
				printf("INFO: Done\n");

				//calling decode_secret_file_extn function to decode output file extension

				printf("INFO: Decoding Output File Extension\n");
				file_ext = decode_secret_file_extn(decInfo -> extn_decoded_file,decInfo);

				//validating whether decode secret file extension operation is success or not

				if ( file_ext == e_success )
				{
					printf("INFO: Done\n");

					open_decoded_file(decInfo);

					//calling decode_secret_file_size function to decode secret file size

					printf("INFO: Decoding %s File Size\n",decInfo -> decoded_fname);
					file_size = decode_secret_file_size(decInfo -> size_decoded_file,decInfo);

					//validating whether decode secret file size operation is success or not

					if ( file_size == e_success )
					{
						printf("INFO: Done\n");

						//calling decode_secret_file_data function to decode secret file data
						
						printf("INFO: Decoding %s File Data\n",decInfo -> decoded_fname);
						file_data = decode_secret_file_data(decInfo);

						//validating whether decode secret file data operation is success or not

						if ( file_data == e_success )
						printf("INFO: Done\n");
						
						//printing an error if the decode secret file data operation is failed

						else
						{
							printf("ERROR: Decoding %s File Data is Failure!\n",decInfo -> decoded_fname);
							return e_failure;
						}

					//printing an error if the decode secret file size operation is failed
			}
					else
					{
						printf("ERROR: Decoding %s File Size is Failure!\n",decInfo -> decoded_fname);
						return e_failure;
					}
	
				//printing an error if the decode secret file extension operation is failed
			}
				else
				{
					printf("ERROR: Decoding Output File Extension is Failure!\n");
					return e_failure;
				}
			}

			//printing an error if the decode secret file extension size operation is failed

			else
			{
				printf("ERROR: Decoding Output File Extension Size is Failure!\n");
				return e_failure;
			}
		}
		
		//printing an error if the decode magic string operation is failed

		else
		{
			printf("ERROR: Decoding Magic String Signature is Failure!\n");
			return e_failure;
		}

	}

	//printing an error if the opening files operation is failed
	
	else
	{
		printf("ERROR: Opening Files is Failure!\n");
		return e_failure;
	}

	//calling close_decodedd_files function to close the opened files

	close_decoded_files(decInfo);

	return e_success;

}

//open_file Function Definition

Status open_stego_file(DecodeInfo *decInfo)
{

		printf("INFO: Opening Required Files\n");

		//opening the stego image

		decInfo -> fptr_stego_image = fopen(decInfo -> stego_image_fname,"r");

		//printing an error if stego image Opening operation is failed

		if ( decInfo -> fptr_stego_image == NULL )
		{
			perror("fopen");
			fprintf(stderr,"ERROR: Unable to Open %s\n",decInfo -> stego_image_fname);
			return e_failure;
		}

	//printing the stego image file name if it is opened successfully
	
	else
		printf("INFO: Opened %s\n",decInfo -> stego_image_fname);

	return e_success;
}		

//decode_magic_string Function Definition

Status decode_magic_string(char *magic_string,DecodeInfo *decInfo)
{
	//getting the length of magic string

	int len = strlen(magic_string);

	//calling decode_data_from_image function to decode magic string

	int ret = decode_data_from_image(magic_string,len,decInfo);

	//validating whether decode data from image operation is success or not

	if ( ret == e_success )
	return e_success;
	else
	return e_failure;

}

//decode_data_from_image Function Definition

Status decode_data_from_image(char *data,int size,DecodeInfo *decInfo)
{
	static int flag = 0;
	char decode = 0;

	//Moving the file offset position to 54

	int ret;

	//using for loop till size times

	for ( int i = 0; i < size; i++ )
	{
		//Reading 8 bytes from stego image

		fread(decInfo -> stego_image_data,sizeof(char),8,decInfo -> fptr_stego_image);

		//calling decode_byte_from_lsb function to decode byte

		ret = decode_byte_from_lsb(decode,decInfo -> stego_image_data,flag);
	}

	//validating whether decode byte from lsb operation is success or not
			
	if ( ret == e_success )
	{
		flag++;
		return e_success;
	}
	else
	return e_failure;

}

//decode_byte_from_lsb Function Definition

Status decode_byte_from_lsb(char data,char *image_buff,int flag)
{
	int i;
    
     //using for loop for 8 times

     for ( i = 0; i < 8; i++ )
     {
         //doing set bit,get bit,clear bit operations and storing the result in data

        data = ( ( image_buff[i] & 0x01 ) << i ) | data ;
	 }

	//validating the magic string if flag is 0

	if ( flag == 0 )
	{
		//getting the length of magic string

		int len = strlen(MAGIC_STRING);
		char magic[len];
		
		//copying the Magic string to magic array

		strcpy(magic,MAGIC_STRING);

		//validating the magic string

		if ( data == magic[len - 1] )
		return e_success;
		else
		return e_failure;
	}

	//validating the secret file extension

	else if ( flag == 1 )
	{
		//validating for extension

		if ( data == 't' || data == 'h' || data == 'c' )
		return e_success;
		else
		return e_failure;
	}
}

//decode_secret_file_extn_size Function Definition

Status decode_secret_file_extn_size(int size,DecodeInfo *decInfo)
{

	//calling decode_size_from_lsb function to decode secret file extension size

	int ret = decode_size_from_lsb(size,decInfo,0);

	//validating whether decode size from lsb operation is success or not

	if ( ret == e_success )
	return e_success;
	else
	return e_failure;
}

//decode_size_from_lsb Function Definition

Status decode_size_from_lsb(int size,DecodeInfo *decInfo,int flag)
{ 
	char read[32];
	int i;
	uint data = 0;

	//reading 32 bytes from stego image

	fread(read,32,1,decInfo -> fptr_stego_image);

	//using for loop for 32 times

	for ( i = 0; i < 32; i++ )
	{

        //doing set bit,get bit,clear bit operations and storing the result in data
		
		data = ( ( read[i] & 0x1 ) << i ) | data;
	}

	//validating whether data is equal to file extension size or not

	if ( flag == 0 )
	{
		if ( data == size )
		return e_success;
		else
		return e_failure;
	}
	
	//storing the size of secret file data if flag is 1

	else if ( flag == 1 )
	{
		decInfo -> size_decoded_file = data;	//storing the decoded file size
	}	
}

//open_decoded_file Function Definition

Status open_decoded_file(DecodeInfo *decInfo)
{
	//validating name value

	if ( name == 1 )
	printf("INFO: Output File Not Mentioned. Creating Decoded.txt as default\n");

	//opening output file

	decInfo -> fptr_decoded = fopen(decInfo -> decoded_fname,"w");

	//validating whether opening output file operation is success or not

	if ( decInfo -> fptr_decoded == NULL )
	{
		perror("fopen");
		fprintf(stderr,"ERROR: Unable to Open %s\n",decInfo -> decoded_fname);
		return e_failure;
	}

	//printing the output file name if it is opened successfully

	else
	{
		printf("INFO: Opened %s\n",decInfo -> decoded_fname);
		printf("INFO: Done. Opened all Required Files\n");
	}	

	return e_success;
}		

//decode_secrte_file_extn Function Definition

Status decode_secret_file_extn(char *file_extn,DecodeInfo *decInfo)
{
	//getting the length of file extension

	int len = strlen(file_extn);

	//calling decode_data_from_image function to decode file extension 

	int ret = decode_data_from_image(file_extn,len,decInfo);

	//validating whether decode data from image operation is success or not

	if ( ret == e_success)
	return e_success;
	else
	return e_failure;
}

//decode_secret_file_size Function Definition

Status decode_secret_file_size(long file_size,DecodeInfo *decInfo)
{
	//calling decode_size_from_lsb function to decode secret file size

	decode_size_from_lsb(file_size,decInfo,1);

	return e_success;
}

//decode_secret_file_data Function Definition

Status decode_secret_file_data(DecodeInfo *decInfo)
{
	char print[decInfo -> size_decoded_file];
	char read = 0;
	long storage;

	//Moving the file offset position to 0

	fseek(decInfo -> fptr_decoded,0,SEEK_SET);

	//using for loop till size_decoded_file

	for ( int i = 0; i < decInfo -> size_decoded_file; i++ )
	{
		//Reading 8 bytes of data from stego image

		fread(print,8,sizeof(char),decInfo -> fptr_stego_image);

		//calling decode_data_to_file function to decode the secret data into output file

		decode_data_to_file(read,print,decInfo);
	}

	//getting the decoded file length

	storage = ftell(decInfo -> fptr_decoded);

	//validating whether decoded file size is equal to storage or not

	if ( storage == decInfo -> size_decoded_file)
	return e_success;
	else
	return e_failure;
}

//status decode_data_to_file Function Definition

Status decode_data_to_file(char decode_data,char *source,DecodeInfo *decInfo)
{
	//using for loop 8 times

	for ( int i = 0; i < 8; i++ )
	{
		//doing get bit,set bit and clear bit operations and storing the result in decode_data

		decode_data = ( ( source[i] & 0x01 ) << i ) | decode_data;
	}

	//writting the decoded data to decoded file 

	fputc(decode_data,decInfo -> fptr_decoded);

	return e_success;
}

//close_decoded_files Function Definition

Status close_decoded_files(DecodeInfo *decInfo)
{
	//closing the stego image file

	fclose(decInfo -> fptr_stego_image);

	//closing the decoded file

	fclose(decInfo -> fptr_decoded);

}

