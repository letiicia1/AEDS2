#include <stdio.h>
#include <stdbool.h>
#include <string.h>


//palindromo 

bool Palindromo (char s[ ])
{
	int n = strlen(s) - 1;
	int i;
	int fim = strlen(s)/2;
	bool res = true;
	
	for(i = 0; i < fim; i++)
	{
		if( !(s[i] >= ' ' && s[i] <= 'z' ) )
		{
	      s[i] = 'a';	
		}
		
		//printf ("%c%c" , s[i] , s[n]);
		if(s[i] != s[n] )
		{
			i = fim;
			res = false;
			
		}
		
		n--;
	}
	
	
	return(res);
}

int str_cmp (char s [ ] , char f [ ])
{
	
	int tamanho = strlen(s);
	int res = 0;
	if(tamanho == strlen(f))
	{
	
		for(int i = 0; i < tamanho; i++)
		{
			if(s[i] != f[i])
			{
				res = s[i] - f[i];
				i = tamanho;
			}
		}
		
	}
	else
	{
		res = -1;
	}
	
	
	return res;
}

int main (void)
{
	char s[500];
	
	
	
	while (str_cmp(s , "FIM") != 0)
	{
		
		scanf("%[^\r\n]" , s);
		getchar( );
	
		
		if (strcmp(s , "FIM") != 0)
		{
		   
			
			if(Palindromo(s))
			{
				printf ("SIM\n");
			}
			else
			{
				printf ("NAO\n");
			}
		}
		
	}
	
	
	
	
	
	return(0);
}


