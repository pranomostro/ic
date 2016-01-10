#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <regex.h>

#define RESIZEFACTOR 2

char* nalread(char* in, size_t* len, int handle);
void* resize(void* data, size_t old, size_t new);

int main(int argc, char** argv)
{
	if(argc!=2)
	{
		fprintf(stderr, "ic REGEX\n");
		exit(255);
	}

	int in=fileno(stdin);
	size_t datalen=BUFSIZ;
	char* data=calloc(datalen, sizeof(char));
	regex_t needle;

	if(regcomp(&needle, argv[1], REG_EXTENDED|REG_NOSUB)!=0)
	{
		fprintf(stderr, "ic: first argument is not a valid ERE, exiting.\n");
		exit(254);
	}

	while((data=nalread(data, &datalen, in))!=NULL)
		if(regexec(&needle, data, datalen, NULL, NULL)==0)
			exit(0);
	free(data);
	exit(1);
}

/*read a line, no arbitrary limits*/

char* nalread(char* in, size_t* len, int handle)
{
	char c=0;
	size_t count=0;

	read(handle, &c, sizeof(char));

	while(c!=0&&c!=-1)
	{
		if(count>=*(len)-3)
		{
			in=(char*)resize(in, sizeof(char)*(*len), sizeof(char)*(*len*RESIZEFACTOR));
			if(in==NULL)
				return in;
			(*len)*=2;
		}
		in[count++]=c;
		if(c=='\n')
			break;
		read(handle, &c, sizeof(char));
	}

	in[count]='\0';

	return (c!=0&&c!=-1)?in:NULL;
}

/*resize an array from a certain size to another*/

void* resize(void* data, size_t old, size_t new)
{
	void* na=realloc(data, new);
	if(na==NULL)
	{
		na=malloc(new);
		if(na==NULL)
		{
			fprintf(stderr, "error: no memory left, exiting.");
			return NULL;
		}
		memcpy(na, data, old);
		free(data);
	}
	return na;
}
