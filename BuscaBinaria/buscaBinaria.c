#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct _Endereco Endereco;

struct _Endereco
{
  char logradouro[72];
  char bairro[72];
  char cidade[72];
  char uf[72];
  char sigla[2];
  char cep[8];
  char lixo[2];
};

void buscaBinaria(FILE *f, long inicio, long fim, char cp[8])
{
  long meio;
  long offset;
  long searchIndex;
  int tot = 0;
  Endereco e;

  while (inicio <= fim)
  {
    tot++;
    meio = (inicio + fim) / 2;
    offset = meio * sizeof(Endereco);
    fseek(f, offset, SEEK_SET);
    fread(&e, sizeof(Endereco), 1, f);
    searchIndex = strncmp(e.cep, cp, 8);

    if (searchIndex == 0)
    {
      printf("%.72s\n%.72s\n%.72s\n%.72s\n%.2s\n%.8s\n", e.logradouro, e.bairro, e.cidade, e.uf, e.sigla, e.cep);
      break;
    }
    else if (searchIndex < 0)
    {
      inicio = meio - 1;
    }
    else
    {
      fim = meio + 1;
    }
  }

  printf("%i passos realizados!\n", tot);
}

int main(int argc, char **argv)
{

  FILE *f;

  if (argc != 2)
  {
    fprintf(stderr, "USO: %s [CEP]", argv[0]);
    return 1;
  }

  f = fopen("cep_ordenado.dat", "r");
  fseek(f, 1, SEEK_END);

  long arquivoLength = ftell(f);
  rewind(f);

  buscaBinaria(f, 0, arquivoLength / sizeof(Endereco), argv[1]);
}