#include <stdio.h>
#include <stdbool.h>
#include <string.h>


//palindromo 

bool Palindromo (char s[ ], int i , int n, int res)
{
	
	if (i < n)
	{
	   if
		(!(s[i] >= ' ' && s[i] <= 'z') ) 
	    res = 0;
		
		if (s[i] != s[n]) 
		{	
			res = 0;
		}
		
		res = Palindromo(s, i + 1, n - 1, res); 
	
		
	}
	
	
	return res;
		
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
			
			
			if(Palindromo(s, 0, strlen(s) - 1, 1))
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


