#include<stdio.h>
#include<string.h>
#include "md5.h"
void help_message(){
	puts("democrypt copyright Hesam Haddad 2015\n\n");
	puts("command form :\n\tdemocrypt -in INFILENAME -out OUTFILENAME -pass PASSWORD [-decrypt]\n\n");
	puts("\t-decrypt \n\t\tenables decrpyting mode, default is encrypt.\n");
	puts("\t-in INFILENAME \n\t\tinput file name. its nessesary to perform operation.\n");
	puts("\t-out OUTFILENAME \n\t\toutput file name. its nessesary to perform operation.\n");
	puts("\t-pass PASSWORD \n\t\tpassword for encrypting or decrypting. its nessesary to perform operation.\n");
}
void file_xor_crypt(int is_encrypt,FILE* in_file,FILE* out_file,char* pass){
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
	int pass_len=strlen(pass);
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
void password_to_md5(char* password_md5,char* pass){
	unsigned int len = strlen (pass);
	MD5_CTX mdContext;
	MD5Init (&mdContext);
	MD5Update (&mdContext, pass, len);
	MD5Final (&mdContext);
	int i;
	for(i = 0; i < 16; i++)
		sprintf(&password_md5[i*2], "%02x", (unsigned int)mdContext.digest[i]);
}
void filename_encrypt(char* str_in_file,char*  str_out_file,char* pass){
	FILE* in_file=fopen(str_in_file,"rb");
	FILE* out_file=fopen(str_out_file,"wb");
	if (in_file==NULL) {fputs ("Input file error",stderr); exit (1);}
	if (out_file==NULL) {fputs ("Output file error",stderr); exit (1);}
	
	char* password_md5[33]={0};
	password_to_md5(password_md5,pass);
	fwrite (password_md5 , sizeof(char), sizeof(char)*32, out_file);
	printf("md5(entered_password) = %s\n",password_md5);
	
	
	file_xor_crypt(1,in_file,out_file,pass);
	
}

void filename_decrypt(char* str_in_file,char*  str_out_file,char* pass){
	FILE* in_file=fopen(str_in_file,"rb");
	FILE* out_file=fopen(str_out_file,"wb");
	if (in_file==NULL) {fputs ("Input file error",stderr); exit (1);}
	if (out_file==NULL) {fputs ("Output file error",stderr); exit (1);}
	
	char* password_md5[33]={0};
	password_to_md5(password_md5,pass);
	printf("md5(entered_password) = %s\n",password_md5);
	
	char* file_password_md5[33]={0};
	size_t result = fread (file_password_md5,sizeof(char), sizeof(char)*32,in_file);
	printf("md5(file_password) = %s\n",file_password_md5);
	if (result != 32) {fputs ("Reading error",stderr); exit (3);}
	
	if(strcmp(password_md5,file_password_md5)){
		puts("The password you have entered is incorrect. md5 doesn't match.");
		exit(1);
	}
	file_xor_crypt(0,in_file,out_file,pass);

}
void check(char* str_in_file,char*  str_out_file,char* pass){
	int err=0;
	if(str_in_file == NULL){
		puts("ERROR: input file name is required to perform operation.");
		err=1;
	}
	if(str_out_file == NULL){
		puts("ERROR: output file name is required to perform operation.");
		err=1;
	}
	if(pass == NULL){
		puts("ERROR: password is required to perform operation.");
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
	int i;
	if(argc == 1){
		help_message();
		exit(1);
	}else{
		for(i=1;i<argc;i++){
			if(strcmp(argv[i],"-decrypt")==0){
				is_encrypt=0;
			}else if(i+1 < argc){
				if(strcmp(argv[i],"-in")==0){
					str_in_file=argv[i+1];
				}else if(strcmp(argv[i],"-out")==0){
					str_out_file=argv[i+1];
				}else if(strcmp(argv[i],"-pass")==0){
					pass=argv[i+1];
				}
			}
		}
	}
	check(str_in_file,str_out_file,pass);
	printf("%s => %s\n",str_in_file,str_out_file);
	
	if(is_encrypt){
		filename_encrypt(str_in_file,str_out_file,pass);
		puts("file encrypted successfully!");
	}else{
		filename_decrypt(str_in_file,str_out_file,pass);
		puts("file decrypted successfully!");
	}
	return 0;
}
