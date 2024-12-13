#include <stdio.h>

int main() {
	int n;
	FILE *file;
	
	
	scanf("%d", &n);
	
	file = fopen("valores.txt", "w");
	if (file == NULL) {
		printf("Erro ao abrir o arquivo.\n");
		return 1;
	}
	

	for (int i = 0; i < n; i++) {
		double valor;
		scanf("%lf", &valor);
		fwrite(&valor, sizeof(double), 1, file);
	}
	

	fclose(file);
	
	
	file = fopen("valores.txt", "r");
	if (file == NULL) 
	{
		printf("Erro ao abrir o arquivo.\n");
		return 1;
	}
	
	
	for (int i = n - 1; i >= 0; i--) 
	{
		
		fseek(file, i * sizeof(double), SEEK_SET);
		
	
		double valor;
		fread(&valor, sizeof(double), 1, file);
		
		
		if (valor == (long)valor) {
			
			printf("%ld\n", (long)valor);
		}
		else 
		{
		
			printf("%g\n", valor);
		}
	}

	fclose(file);
	
	return 0;
}

