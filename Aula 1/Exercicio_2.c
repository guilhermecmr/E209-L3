#include <stdio.h>

int main()
{
    int numero;

    scanf("%d", &numero);

    if (numero < 1 || numero > 50)
    {
        return 1;
    }

    for (int i = numero - 1; i > 0; i--)
    {
        printf("Resto da divisao de %d por %d: %d\n", numero, i, numero % i);
    }

    return 0;
}
