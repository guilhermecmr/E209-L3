#include <stdio.h>

int main()
{
    int numero_de_laranjas;
    float preco_unidade, preco_total;

    scanf("%d", &numero_de_laranjas);

    if (numero_de_laranjas >= 12)
    {
        preco_unidade = 0.65;
    }
    else
    {
        preco_unidade = 0.80;
    }

    preco_total = numero_de_laranjas * preco_unidade;

    printf("Preco da unidade: R$%.2f\n", preco_unidade);
    printf("Preco total: R$%.2f\n", preco_total);

    return 0;
}
