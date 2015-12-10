#include<stdio.h>
#include<string.h>
#include "md5.h"

void help_message(){
	puts("democrypt copyright Hesam Haddad 2015\n\n");
	puts("command form for encrypting using password :\n\tdemocrypt -in INFILENAME -out OUTFILENAME -pass PASSWORD [-decrypt]\n\n");
	puts("command form for encrypting using key file :\n\tdemocrypt -in INFILENAME -out OUTFILENAME -keyfile KEYFILENAME [-decrypt]\n\n");
	puts("\t-decrypt \n\t\tenables decrpyting mode, default is encrypt.\n");
	puts("\t-in INFILENAME \n\t\tinput file name. its nessesary to perform operation.\n");
	puts("\t-out OUTFILENAME \n\t\toutput file name. its nessesary to perform operation.\n");
	puts("\t-pass PASSWORD \n\t\tpassword for encrypting or decrypting. its nessesary to perform operation.\n");
	puts("\t-keyfile KEYFILENAME \n\t\tpassword for encrypting or decrypting. its nessesary to perform operation.\n");
}

void file_xor_crypt(int is_encrypt,FILE* in_file,FILE* out_file,char* pass,int pass_len){
  long long file_size;
  char * buffer;
  size_t result;
	/*
	* Read Phase
	*/
  if(is_encrypt){
	  fseek (in_file , 0 , SEEK_END);
	  file_size = ftell (in_file);
	  rewind (in_file);

  }else{
	  fseek (in_file , 0 , SEEK_END);
	  file_size = ftell (in_file) - 32;
	  rewind (in_file);
	  char trash[33]={0};
	  fread (trash,sizeof(char), sizeof(char)*32,in_file);
  }
  /*int x=file_size;
  printf("file size = %d\n",x);*/
  buffer = (char*) malloc (sizeof(char)*file_size);
  if (buffer == NULL) {fputs ("Memory error",stderr); exit (2);}
  result = fread (buffer,1,file_size,in_file);
  if (result != file_size) {fputs ("Reading error",stderr); exit (3);}
  	/*
	* Encrypt or Decrpt Phase
	*/
  int i;

  for(i=0;i<file_size;i++){
	  buffer[i]=buffer[i]^pass[i%pass_len];
  }
  	/*
	* Write Phase
	*/

  fwrite (buffer , sizeof(char), sizeof(char)*file_size, out_file);

  fclose (out_file);
  fclose (in_file);
  free (buffer);
}
void xormake(char* data,int xol,char* pass,int pass_len){
    int i;
    for(i=0;i<xol;i++){
        data[i]=data[i]^pass[i%pass_len];
    }
}
char* md5_byte_to_readable(char* inp){
    char* buffer = (char*) malloc (sizeof(char)*33);
    buffer[32]='\0';
    int i;
	for(i = 0; i < 16; i++)
		sprintf(&buffer[i*2], "%02x", (unsigned int)((unsigned char)inp[i]));
    return buffer;
}
void password_to_md5_byte(char* password_md5,char* pass,int pass_len){
	MD5_CTX mdContext;
	MD5Init (&mdContext);
	MD5Update (&mdContext, pass, pass_len);
	MD5Final (&mdContext);
	int i;
	for(i = 0; i < 16; i++){
		password_md5[i]=mdContext.digest[i];
    }
}
void password_to_md5_readable(char* password_md5,char* pass,int pass_len){
	MD5_CTX mdContext;
	MD5Init (&mdContext);
	MD5Update (&mdContext, pass, pass_len);
	MD5Final (&mdContext);
	int i;
	for(i = 0; i < 16; i++)
		sprintf(&password_md5[i*2], "%02x", (unsigned int)mdContext.digest[i]);
}
void filename_encrypt(char* str_in_file,char*  str_out_file,char* pass,int pass_len){
	FILE* in_file=fopen(str_in_file,"rb");
	FILE* out_file=fopen(str_out_file,"wb");
	if (in_file==NULL) {fputs ("Input file error",stderr); exit (1);}
	if (out_file==NULL) {fputs ("Output file error",stderr); exit (1);}

	char* header[33]={0};
	password_to_md5_byte(header,pass,pass_len);

	printf("md5(entered_password) = %s\n",md5_byte_to_readable(header));
    xormake(header,16,pass,pass_len);
    fwrite (header , sizeof(char), sizeof(char)*32, out_file);

	file_xor_crypt(1,in_file,out_file,pass,pass_len);

}
void filename_decrypt(char* str_in_file,char*  str_out_file,char* pass,int pass_len){
	FILE* in_file=fopen(str_in_file,"rb");
	FILE* out_file=fopen(str_out_file,"wb");
	if (in_file==NULL) {fputs ("Input file error",stderr); exit (1);}
	if (out_file==NULL) {fputs ("Output file error",stderr); exit (1);}

	char* header[33]={0};
	password_to_md5_byte(header,pass,pass_len);
	printf("md5(entered) = %s\n",md5_byte_to_readable(header));

	char* file_header[33]={0};
	size_t result = fread (file_header,sizeof(char), sizeof(char)*32,in_file);
	xormake(file_header,16,pass,pass_len);
	printf("md5(encrypt) = %s\n",md5_byte_to_readable(file_header));
	if (result != 32) {fputs ("Reading error",stderr); exit (3);}

	if(strcmp(header,file_header)){
		puts("The password or key file you have entered is incorrect. md5 doesn't match.");
		exit(1);
	}
	file_xor_crypt(0,in_file,out_file,pass,pass_len);

}
void filename_encrypt_keyfile(char* str_in_file,char*  str_out_file,char* str_key_file){
    FILE* key_file=fopen(str_key_file,"rb");
    if (key_file==NULL) {fputs ("Key file error",stderr); exit (1);}
    fseek (key_file , 0 , SEEK_END);
    long long int file_size = ftell (key_file);
    rewind (key_file);
    char* buffer = (char*) malloc (sizeof(char)*file_size);
    if (buffer == NULL) {fputs ("Memory error",stderr); exit (2);}
    int result = fread (buffer,1,file_size,key_file);
    if (result != file_size) {fputs ("Reading error",stderr); exit (3);}
    fclose(key_file);
    filename_encrypt(str_in_file,str_out_file,buffer,file_size);
}
void filename_decrypt_keyfile(char* str_in_file,char*  str_out_file,char* str_key_file){
    FILE* key_file=fopen(str_key_file,"rb");
    if (key_file==NULL) {fputs ("Key file error",stderr); exit (1);}
    fseek (key_file , 0 , SEEK_END);
    long long int file_size = ftell (key_file);
    rewind (key_file);
    char* buffer = (char*) malloc (sizeof(char)*file_size);
    if (buffer == NULL) {fputs ("Memory error",stderr); exit (2);}
    int result = fread (buffer,1,file_size,key_file);
    if (result != file_size) {fputs ("Reading error",stderr); exit (3);}
    fclose(key_file);
    filename_decrypt(str_in_file,str_out_file,buffer,file_size);
}
void check(char* str_in_file,char*  str_out_file,char* pass,char* key_file){
	int err=0;
	if(str_in_file == NULL){
		puts("ERROR: input file name is required to perform operation.");
		err=1;
	}
	if(str_out_file == NULL){
		puts("ERROR: output file name is required to perform operation.");
		err=1;
	}
	if(pass == NULL && key_file == NULL){
		puts("ERROR: password or key file is required to perform operation.");
		err=1;
	}
	if(pass != NULL && key_file != NULL){
		puts("ERROR: can't use both password and key file to encrypt.");
		err=1;
	}
	if(err){
		puts("there is some error in your command type.\nto figure out correct form, call democrypt without any argument to see help.");
		exit(1);
	}
}
int main(int argc,char* argv[]){
	int is_encrypt=1;
	char * str_in_file=NULL;
	char * str_out_file=NULL;
	char * pass=NULL;
	char * key_file=NULL;
	int i,found;
	if(argc == 1){
		help_message();
		exit(1);
	}else{
		for(i=1;i<argc;i++){
            found=0;
			if(strcmp(argv[i],"-decrypt")==0){
				is_encrypt=0;
				found=1;
			}else if(i+1 < argc){
				if(strcmp(argv[i],"-in")==0){
					str_in_file=argv[i+1];
					found=1;
				}else if(strcmp(argv[i],"-out")==0){
					str_out_file=argv[i+1];
					found=1;
				}else if(strcmp(argv[i],"-pass")==0){
					pass=argv[i+1];
					found=1;
				}else if(strcmp(argv[i],"-keyfile")==0){
					key_file=argv[i+1];
					found=1;
				}
				i++; // skiping next argument
			}
			if(found == 0){
                printf("ERROR: invalid argument %s.\n",argv[i]);
                exit(1);
            }
		}
	}
	check(str_in_file,str_out_file,pass,key_file);
	printf("%s => %s\n",str_in_file,str_out_file);

	if(is_encrypt){
        if(key_file == NULL){
            filename_encrypt(str_in_file,str_out_file,pass,strlen(pass));
		}else{
            filename_encrypt_keyfile(str_in_file,str_out_file,key_file);
		}
		puts("file encrypted successfully!");
	}else{
        if(key_file == NULL){
            filename_decrypt(str_in_file,str_out_file,pass,strlen(pass));
		}else{
            filename_decrypt_keyfile(str_in_file,str_out_file,key_file);
		}
		puts("file decrypted successfully!");
	}
	return 0;
}
