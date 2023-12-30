#include <stdio.h>
#include <string.h>
#include "encode.h"
#include "types.h"
#include "common.h"

//read_and_validate_encode_args Function Definition

Status read_and_validate_encode_args(char *argv[], EncodeInfo *encInfo)
{
	//validating whether .bmp format image file is passed or not

	if ( strstr(argv[2],".bmp") != NULL )
	{
		if (  strcmp(strstr(argv[2],".bmp"),".bmp") == 0 )
		{
			encInfo -> src_image_fname = argv[2];				//storing the image file name
		}
	}

	//printing an error if the .bmp file is not passed

	else
	{
		 printf("ERROR: Source Image File Format Should be <.bmp file>\n");
		 printf("ERROR: Usage!\n");
         printf("USAGE: Encoding: %s -e <.bmp file> <.txt file> [output file]\n",argv[0]);	
		 return e_failure;
	}

	//validating whether .txt file is passed or not

	if ( strstr(argv[3],".txt") != NULL )
	{
		if ( strcmp(strstr(argv[3],".txt"),".txt") == 0 )
		{
			encInfo -> secret_fname = argv[3];				//storing the secret file name
		}
	}

	//validating whether .ch file is passed or not
	
	else if ( strstr(argv[3],".sh") != NULL )
	{
		if ( strcmp(strstr(argv[3],".sh"),".sh") == 0 )
		{
			encInfo -> secret_fname = argv[3];				//storing the secret file name
		}
	}

	//validating whether .c file is passed or not
	
	else if ( strstr(argv[3],".c") != NULL )
	{
		if ( strcmp(strstr(argv[3],".c"),".c") == 0 )
		{
			encInfo -> secret_fname = argv[3];				//storing the secret file name
		}
	}

	//printing an error if the .txt file is not passed

	else
	{
	 	 printf("USAGE: Secret File Format Should be <.txt file> or <.sh file> or <.c file>\n");	 
		 printf("ERROR: Usage!\n");
         printf("USAGE: Encoding: %s -e <.bmp file> <.txt file> [output file]\n",argv[0]);
		 return e_failure;
	}

	//validating whether output file name is passed or not 

	if ( argv[4] != NULL )
	encInfo -> stego_image_fname = argv[4];			//storing the output file name if passed

	//creating a default output file name if file name is not passed

	else
	{
		printf("INFO: Output File Not Mentioned. Creating Stego.bmp as Default\n");
		encInfo -> stego_image_fname = "Stego.bmp";		//storing the default file name
	}

	return e_success;
}

//do_encoding Function Definition

Status do_encoding(EncodeInfo *encInfo)
{
	int open,check,copy;
	int magic,file_ext_size,file_ext_len,file_extn,file_size,file_data,copy_data;

	//calling open_files function to open required files to do encoding

	open = open_files(encInfo);

	//validating whether opening files operation is success or not

	if ( open == e_success )
	{
		printf("INFO: ## Encoding Procedure Started ##\n");

		//calling check_capacity function to check the size of files

		check = check_capacity(encInfo);

		//validating whether Capacity is Enough or not

		if ( check == e_success )
		{

			//calling copy_bmp_header function to copy the header of bmp image to stego image
			
			printf("INFO: Copying Image Header\n");
			copy = copy_bmp_header(encInfo -> fptr_src_image,encInfo -> fptr_stego_image);

			//validating whether copying header file operation is success or not

			if ( copy == e_success )
			{
				printf("INFO: Done\n");

				//calling encode_magic_string function to encode the magic string

				printf("INFO: Encoding Magic String Signature\n");
				magic = encode_magic_string(MAGIC_STRING,encInfo);

				//validating whether encoding magic string operation is success or not

				if ( magic == e_success )
				{
					printf("INFO: Done\n");

					//getting the file extension size

					printf("INFO: Encoding %s File Extension Size\n",encInfo -> secret_fname);
					strcpy( encInfo -> extn_secret_file,strstr( encInfo -> secret_fname,"." ) );

					//getting the length of secret file extension

					file_ext_len = strlen(encInfo -> extn_secret_file);

					//calling encode_secret_file_extn_size function to encode file extension size

					file_ext_size = encode_secret_file_extn_size(file_ext_len,encInfo);

					//validating whether encoding secret file extension size operation is success or not

					if ( file_ext_size == e_success )
					{
						printf("INFO: Done\n");

						//calling encode_secret_file_extn function to encode secret file extension

						printf("INFO: Encoding %s File Extension\n",encInfo -> secret_fname);
						file_extn = encode_secret_file_extn(encInfo -> extn_secret_file,encInfo);

						//validating whether encode secret file extension operation is success or not

						if ( file_extn == e_success )
						{
							printf("INFO: Done\n");
							
							//calling encode_secret_file_size function to encode secret file size

							printf("INFO: Encoding %s File Size\n",encInfo -> secret_fname);
							file_size = encode_secret_file_size(encInfo -> size_secret_file,encInfo);
							
							//validating whether encode secret file size operation is success or not

							if ( file_size == e_success )
							{
								printf("INFO: Done\n");

								//calling encode_secret_file_data function to encode secret file data
								
								printf("INFO: Encoding %s File Data\n",encInfo -> secret_fname);
								file_data = encode_secret_file_data(encInfo);
								
								//validating whether encode secret file data operation is success or not

								if ( file_data == e_success )
								{
									printf("INFO: Done\n");

									//calling copy_remaining_img_data function to copy remaining data to stego image

									printf("INFO: Copying Left Over Data\n");
									copy_data = copy_remaining_img_data(encInfo);

									//validating whether copy remaining data operation is success or not

									if ( copy_data == e_success )
									printf("INFO: Done\n");

									//printing an error if the copy remaining data operation is failed

									else
									{
										printf("ERROR: Copying Left Over Data is Failure!\n");
										return e_failure;
									}
								
								}

								//printing an error if the encode secret file data operation is failed

								else
								{
									printf("ERROR: Encoding %s File Data is Failure!\n",encInfo -> secret_fname);
									return e_failure;
								}
							}
							
							//printing an error if the encode secret file size operation is failed

							else
							{
								printf("ERROR: Encoding %s File Size is Failure!\n",encInfo -> secret_fname);
								return e_failure;
							}
						}

						//printing an error if the encode secret file extension operation is failed
						
						else
						{
							printf("ERROR: Encoding %s File Extension is Failure!\n",encInfo -> secret_fname);
							return e_failure;
						}

					}	
					
					//printing an error if the encoding file extension size operation is failed

					else
					{
						printf("ERROR: Encoding %s File Extension Size is Failure!\n",encInfo -> secret_fname);
						return e_failure;
					}
				}	
						

				//printing an error if the encoding magic string operation is failed

				else
				{
					printf("ERROR: Encoding Magic String Signature is Failure!\n");
					return e_failure;
				}
			}

			//printing an error if the Copying header file operation is failed

			else
			{
				printf("ERROR: Copying Image Header is Failure!\n");
				return e_failure;
			}
		}

		//printing an error if the Capacity is not enough

		else
		{
			return e_failure;
		}
	}

	//printing an error if the opening files operation is failed

	else
	{
		printf("ERROR: Opening Files is Failure!\n");
		return e_failure;
	}

	//calling close_files function to close all the opened files

	close_files(encInfo);
	
	return e_success;
}

//open_files Function Definition

Status open_files(EncodeInfo *encInfo)
{
	printf("INFO: Opening Required Files\n");

    //Opening the Image File

    encInfo -> fptr_src_image = fopen(encInfo -> src_image_fname,"r");
    
	//printing an error if Image File Opening operation is Failed

    if ( encInfo -> fptr_src_image == NULL )
    {
    	perror("fopen");
    	fprintf(stderr,"ERROR: Unable to Open %s\n",encInfo -> src_image_fname);
    	return e_failure;
    }

	//printing the Image File Name if it is opened successfully

	else
	printf("INFO: Opened %s\n",encInfo -> src_image_fname);

    //Opening the Text File

    encInfo -> fptr_secret = fopen(encInfo -> secret_fname,"r");

	//printing an error if Text File Opening operation is Failed

    if ( encInfo -> fptr_secret == NULL )
    {
    	//perror("fopen");
    	fprintf(stderr,"ERROR: Unable to Open %s\n",encInfo -> secret_fname);
    	return e_failure;
    }
	
	//printing the Text File Name if it is opened successfully
	
	else
	printf("INFO: Opened %s\n",encInfo -> secret_fname); 

    //Opening Output Image File

    encInfo -> fptr_stego_image = fopen(encInfo -> stego_image_fname,"w");
    
    //printing an error if Output Image File Opening operation is Failed

    if ( encInfo -> fptr_stego_image == NULL )
    {
    	perror("fopen");
    	fprintf(stderr,"ERROR: Unable to Open %s\n",encInfo -> stego_image_fname);
    	return e_failure;
    }

	//printing the Output Image File Name if it is opened successfully

	else
	printf("INFO: Opened %s\n",encInfo -> stego_image_fname);
	
	printf("INFO: Done\n");

    return e_success;
}

//check_capacity Function Definition

Status check_capacity(EncodeInfo *encInfo)
{
	uint total_size;

	//calling get_image_size_for_bmp function to get the size of image file

	encInfo -> image_capacity = get_image_size_for_bmp(encInfo -> fptr_src_image);
	
	//calling get_file_size function to get the size of text file

	printf("INFO: Checking for %s size\n",encInfo -> secret_fname);
	encInfo -> size_secret_file = get_file_size(encInfo -> fptr_secret);

	//printing an error if secrete file is empty

	if ( encInfo -> size_secret_file == 0 )
	{
		printf("ERROR: %s is Empty!\n",encInfo -> secret_fname);
		return e_failure;
	}
	
	//printing the secrete file name if it is not empty

	else
	{
		printf("INFO: Done. Not Empty\n");
		total_size = ( encInfo -> size_secret_file + 16 + 32 + 32 + 32 ) * 8;	//calaculating the total size

		printf("INFO: Checking for %s Capacity to Handle %s\n",encInfo -> src_image_fname,encInfo -> secret_fname);

		//validating whether secret file data will fit into image file or not

		if ( encInfo -> image_capacity > total_size )
		{
			printf("INFO: Done. Found OK\n");
			return e_success;
		}

		//printing an error if the secret file data is not fitting into image file
		
		else
		{
		printf("ERROR: %s doesn't have the Capacity to Encode %s\n",encInfo -> src_image_fname,encInfo -> secret_fname);
		return e_failure;
		}
	}
	
}

//get_image_size_for_bmp Function Definition

uint get_image_size_for_bmp(FILE *fptr_image)
{
    uint width,height,image_capacity;

	//Moving File Offset position to 18 to get width and height

    fseek(fptr_image,18,SEEK_SET);

    //Reading width from image file and printing the width

    fread(&width,sizeof(int),1,fptr_image);

    //Reading height from image file and printing the height

    fread(&height,sizeof(int),1,fptr_image);

	image_capacity = width * height * 3;	//getting the image capacity 

    //Returning image_capacity

    return image_capacity;
}

//get_file_size Function Definition

uint get_file_size(FILE *fptr)
{
	uint file_capacity;
	
	//Moving File Offset position to the end

	fseek(fptr,0,SEEK_END);

	//getting the file capacity

	file_capacity = ftell(fptr);

	//Returning file_capacity

	return file_capacity;
}

//copy_bmp_header Function Definition

Status copy_bmp_header(FILE *fptr_src_image,FILE *fptr_dest_image)
{
	char Header[54];

	//Moving the File Offset Position of source image file to 0

	fseek(fptr_src_image,0,SEEK_SET);

	//Reading the Header from source image file

	fread(Header,54,1,fptr_src_image);

	//Copying the Header from source image file to destination image file

	fwrite(Header,54,1,fptr_dest_image);

	return e_success;
}

//encode_magic_string Function Definition

Status encode_magic_string(char *magic_string,EncodeInfo *encInfo)
{
	int size = strlen(magic_string);	//getting the size of magic_string

	//calling encode_data_to_image function to encode the magic string into image 

	encode_data_to_image(magic_string,size,encInfo);

	return e_success;
}

//encode_data_to_image Function Definition

Status encode_data_to_image(char *data,int size,EncodeInfo *encInfo)
{
	int i;

	//using for loop till size times

	for ( i = 0; i < size; i++ )
	{
		//reading 8 bytes from source image 

		fread(encInfo -> image_data,8,1,encInfo -> fptr_src_image);

		//calling encode_byte_to_lsb function to encode the bytes to lsb

		encode_byte_to_lsb(data[i],encInfo -> image_data);

		//writting 8 bytes from source image to stego image

		fwrite(encInfo -> image_data,8,1,encInfo -> fptr_stego_image);
	}
}

//encode_byte_to_lsb Function Definition

Status encode_byte_to_lsb(char data,char *image_buffer)
{
	int i;
	
	//using for loop for 8 times

	for ( i = 0; i < 8; i++ )
	{
		//doing set bit,get bit,clear bit operations and storing the result in image_buffer[i]

		image_buffer[i] = ( image_buffer[i] & 0xFE ) | ( ( data >> i ) & 1 );
	}

}

//encode_secret_file_extn_size Function Definition

Status encode_secret_file_extn_size(int size,EncodeInfo *encInfo)
{
	//calling encode_size_to_lsb function to encode file extension size

	encode_size_to_lsb(size,encInfo);
	return e_success;
}

//encode_size_to_lsb Function Definition

Status encode_size_to_lsb(int size,EncodeInfo *encInfo)
{
	char read[32];
	int i;

	//reading 32 bytes from source image

	fread(read,32,1,encInfo -> fptr_src_image);

	//using for loop for 32 times

	for ( i = 0; i < 32; i++ )
	{
		//doing set bit,get bit,clear bit operations and storing the result in read[i]

		read[i] = ( read[i] & 0XFE ) | ( ( size >> i ) & 1 );
	}

	//writting 32 bytes from source image to stego image

	fwrite(read,32,1,encInfo -> fptr_stego_image);
}

//encode_secret_file_extn Function Definition

Status encode_secret_file_extn(char *file_extn,EncodeInfo *encInfo)
{
	//getting the size of file extension

	int len = strlen(file_extn);

	//calling encode_data_to_image function to encode extension to image

	encode_data_to_image(file_extn,len,encInfo);

	return e_success;
}

//encode_secret_file_size Function Definition

Status encode_secret_file_size(int file_size,EncodeInfo *encInfo)
{
	//calling encode_size_to_lsb function to encode file size

	encode_size_to_lsb(file_size,encInfo);
	return e_success;
}

//encode_secret_file_data Function Definition

Status encode_secret_file_data(EncodeInfo *encInfo)
{
	char read[encInfo -> size_secret_file];

	//Moving the file offset position to 0

	fseek(encInfo -> fptr_secret,0,SEEK_SET);
	
	//Reading the data of secret file

	fread(read,encInfo -> size_secret_file,1,encInfo -> fptr_secret);
	
	//calling encode_data_to_image function to encode secret file data

	encode_data_to_image(read,encInfo -> size_secret_file,encInfo);

	return e_success;
}

//copy_remaining_img_data Function Definition

Status copy_remaining_img_data(EncodeInfo *encInfo)
{
	int len,size_of_src_image,size_of_stego_image;

	//getting the sizes of source and stego images

	size_of_src_image = encInfo -> image_capacity + 54;
	size_of_stego_image = ftell(encInfo -> fptr_stego_image);

	//getting the length

	len = size_of_src_image - size_of_stego_image;

	char copy[len];

	//Reading the remaining data from source image

	fread(copy,len,1,encInfo -> fptr_src_image);

	//Writting the remaining data from source image to stego image

	fwrite(copy,len,1,encInfo -> fptr_stego_image);

	return e_success;
}

//close_files Function Definition

Status close_files(EncodeInfo *encInfo)
{
	//closing source image file

	fclose(encInfo -> fptr_src_image);

	//closing secret file

	fclose(encInfo -> fptr_secret);

	//closing stego image file

	fclose(encInfo -> fptr_stego_image);

}

