#include <stdio.h>

void analisar_numeros(int numeros[5])
{
    int pares = 0, impares = 0, positivos = 0, negativos = 0;

    for (int i = 0; i < 5; i++)
    {
        if (numeros[i] % 2 == 0)
        {
            pares++;
        }
        else
        {
            impares++;
        }

        if (numeros[i] >= 0)
        {
            positivos++;
        }
        else if (numeros[i] < 0)
        {
            negativos++;
        }
    }

    printf("Quantidade de numeros pares: %d\n", pares);
    printf("Quantidade de numeros impares: %d\n", impares);
    printf("Quantidade de numeros positivos: %d\n", positivos);
    printf("Quantidade de numeros negativos: %d\n", negativos);
}

int main()
{
    int n;

    scanf("%d", &n);

    for (int i = 0; i < n; i++)
    {
        int numeros[5];

        for (int j = 0; j < 5; j++)
        {
            scanf("%d", &numeros[j]);
        }

        analisar_numeros(numeros);
    }

    return 0;
}