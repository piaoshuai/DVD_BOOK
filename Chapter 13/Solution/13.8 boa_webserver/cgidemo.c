#include<stdio.h>

int main()
{
	printf("Content-Type:text/html \r\n\r\n");

	printf("<html><head>\n");
	printf("<title> cgi </title> \n");
	printf("</head> \n");
	printf("<body> \n");
	printf("<h1> hello! </h1> \n");
	printf("<p>this is a demo of cgi's program in the boa server</p> \n");
	printf("</body> </html> \n");
	return 0;
} 
