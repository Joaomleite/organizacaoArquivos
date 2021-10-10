#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>

#pragma pack(1)

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

int compara(const void *e1, const void *e2)
{
  return strncmp(((Endereco *)e1)->cep, ((Endereco *)e2)->cep, 8);
}

void ordena(Endereco *e, long tamanho, FILE *arq, FILE *f)
{
  fread(e, sizeof(Endereco), tamanho, f);
  qsort(e, tamanho, sizeof(Endereco), compara);
  fwrite(e, sizeof(Endereco), tamanho, arq);
  fclose(arq);
}

int intercala(int firstIndex, int secondIndex, int lastIndex)
{
  FILE *a, *b, *saida;
  Endereco ea, eb;
  char firstfileName[100];

  sprintf(firstfileName, "cep_%i.dat", firstIndex);

  char secondfileName[100];
  sprintf(secondfileName, "cep_%i.dat", secondIndex);

  char newfileName[100];
  sprintf(newfileName, "cep_%i.dat", lastIndex);

  a = fopen(firstfileName, "r");
  b = fopen(secondfileName, "r");
  saida = fopen(newfileName, "w");

  fread(&ea, sizeof(Endereco), 1, a);
  fread(&eb, sizeof(Endereco), 1, b);

  while (!feof(a) && !feof(b))
  {
    if (compara(&ea, &eb) < 0)
    {
      fwrite(&ea, sizeof(Endereco), 1, saida);
      fread(&ea, sizeof(Endereco), 1, a);
    }
    else
    {
      fwrite(&eb, sizeof(Endereco), 1, saida);
      fread(&eb, sizeof(Endereco), 1, b);
    }
  }

  while (!feof(a))
  {
    fwrite(&ea, sizeof(Endereco), 1, saida);
    fread(&ea, sizeof(Endereco), 1, a);
  }

  while (!feof(b))
  {
    fwrite(&eb, sizeof(Endereco), 1, saida);
    fread(&eb, sizeof(Endereco), 1, b);
  }
  fclose(a);
  fclose(b);
  fclose(saida);
  remove(firstfileName);
  remove(secondfileName);

  return 1;
}

int main(int argc, char **argv)
{
  FILE *f, *saida;
  Endereco *e;

  int N_DIVISAO = 8;
  int countTot = N_DIVISAO;
  int countAux = 0, count = 0;
  long posicao, qtd, particao;

  f = fopen("cep.dat", "r");
  fseek(f, 0, SEEK_END);
  posicao = ftell(f);
  qtd = posicao / sizeof(Endereco);
  particao = qtd / N_DIVISAO;
  char fileName[100];

  e = (Endereco *)malloc(particao * sizeof(Endereco));
  rewind(f);

  for (long i = 0; i < qtd; i += particao + 1)
  {
    sprintf(fileName, "cep_%i.dat", count);
    FILE *arq = fopen(fileName, "w");

    ordena(e, particao, arq, f);
    printf("%i Ordenado\n", count);
    count++;
  }

  for (int j = 0; j < N_DIVISAO;)
  {
    countAux += intercala(j, j + 1, countTot);
    countTot++;
    printf("Lendo arquivos %i e %i em %i \n", j, j + 1, countTot - 1);
    if (j + 2 == N_DIVISAO && countAux != 1)
    {
      N_DIVISAO += countAux;
      countAux = 0;
    }
    j += 2;
  }

  fclose(f);
  int ticks = clock();

  printf("Tempo decorrido: %f segundos", (float)ticks / CLOCKS_PER_SEC);
}