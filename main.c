#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct paciente {
  int id;
  char nome[20];
  int idade;
  char condicao[30];
  struct paciente *esq, *dir;
} TPaciente;

TPaciente *alocarPaciente(int id, char nome[20], int idade, char condicao[30]);
void visualizarRegistro(TPaciente *raiz);
TPaciente *removerPaciente(TPaciente **raiz, int id);
TPaciente *inserirPorId(TPaciente *raiz, int id, char nome[20], int idade, char condicao[30]);
TPaciente *inserirPorNome(TPaciente *raiz, int id, char nome[20], int idade, char condicao[30]);
void buscarId(TPaciente *raiz, int id);
void buscarNome(TPaciente *raiz, char nome[20]);
int verificarExistencia(TPaciente *raiz, int id, char nome[20]);
void liberarMemoria(TPaciente *raiz);
void salvarArquivo(TPaciente *raiz, FILE *file);
void contarPacientes(TPaciente *raiz, int contador);

int main() {
  TPaciente *raiz = NULL;
  int opcao, idade, id = 0, ordem;
  char nome[20], condicao[30];
  int count = 0;

  // o usuário escolhe a forma de ordenação no início do programa
  printf("1 - Ordenar por ID    2 - Ordenar por nome\nEscolha o critério de "
         "ordenação: ");
  scanf("%d", &ordem);

  // carregar arquivo inicial e ordenar conforme a escolha anterior
  FILE *file = fopen("arquivoInicial.txt", "r");
  if (file == NULL) {
    printf("Erro ao abrir arquivo!\n");
  } else {
    while (fscanf(file, "%d %s %d %s", &id, nome, &idade, condicao) == 4) {
      switch (ordem) {
      case 1:
        raiz = inserirPorId(raiz, id, nome, idade, condicao);
        break;

      case 2:
        raiz = inserirPorNome(raiz, id, nome, idade, condicao);
        break;

      default:
        printf("Opcao invalida!");
        break;
      }
    }
  }

  while (opcao != 6) {
    printf("\n===== BEM-VINDO REGISTRO DE PACIENTES =====\n");
    printf("1 - Cadastrar pacientes\n");
    printf("2 - Buscar paciente\n");
    printf("3 - Remover paciente\n");
    printf("4 - Listar todos os pacientes\n");
    printf("5 - Exibir total de pacientes cadastrados\n");
    printf("6 - Sair\n");
    printf("Opcao escolhida: ");
    scanf("%d", &opcao);
    printf("-------------------------------------------\n");

    switch (opcao) {
    case 1:
      printf("Informe ID: ");
      scanf("%d", &id);
      printf("Informe nome: ");
      scanf("%s", nome);
      printf("Informe idade: ");
      scanf("%d", &idade);
      printf("Informe condicao medica: ");
      scanf("%s", condicao);

      // verifica se o ID ou Nome já estão cadastrados
      if (verificarExistencia(raiz, id, nome) == 0) {
        if (ordem == 1) {
          raiz = inserirPorId(raiz, id, nome, idade, condicao); // se o usuario escolher ordenar por ID então a inserção será por ID
        } else if (ordem == 2) {
          raiz =
              inserirPorNome(raiz, id, nome, idade, condicao); // se o usuario escolher ordenar por NOME então a inserção será por nome
        }
      }
      break;

    case 2:
      if (ordem == 1) { // se o criterio escolhido no inicio foi ID então a busca será realizada usando o ID
        printf("Buscar ID: ");
        scanf("%d", &id);
        buscarId(raiz, id);

      } else if (ordem == 2) { // se o criterio escolhido foi NOME então a busca será realizada usando o nome
        printf("Buscar nome: ");
        scanf("%s", nome);
        buscarNome(raiz, nome);
      }
      break;

    case 3:
      printf("Informe o ID do paciente a remover: ");
      scanf("%d", &id);
      raiz = removerPaciente(&raiz, id);
      break;

    case 4:
      visualizarRegistro(raiz);
      break;

    case 5:
      contarPacientes(raiz, count);
      break;

    case 6:
      file = fopen("arquivoFinal.txt", "w");
      if (file != NULL) {
        salvarArquivo(raiz, file);
        fclose(file);
        printf("Dados gravados em arquivoFinal.txt\n");
      } else {
        printf("Erro gravar dados em arquivo!\n");
      }
      printf("Encerrando...");
      liberarMemoria(raiz);
      break;

    default:
      printf("Opcao invalida! Tente novamente.");
      break;
    }
  }
  return 0;
}

// função para alocar memória para um novo cadastro
TPaciente *alocarPaciente(int id, char nome[20], int idade, char condicao[30]) {
  TPaciente *novo = (TPaciente *)malloc(sizeof(TPaciente));
  if (novo) {
    novo->id = id;
    strcpy(novo->nome, nome);
    novo->idade = idade;
    strcpy(novo->condicao, condicao);
    novo->dir = NULL;
    novo->esq = NULL;
  }
  return novo;
}

// inserir na árvore ordenando por ID
TPaciente *inserirPorId(TPaciente *raiz, int id, char nome[20], int idade, char condicao[30]) {
  // se a raiz é nula entao foi encontrado um lugar vazio na arvore
  if (raiz == NULL) {
    return alocarPaciente(id, nome, idade, condicao);
  } else {
    if (id < raiz->id) {
      raiz->esq = inserirPorId(raiz->esq, id, nome, idade, condicao);
    } else {
      raiz->dir = inserirPorId(raiz->dir, id, nome, idade, condicao);
    }
  }
  return raiz;
}

// inserir na árvore ordenando por nome
TPaciente *inserirPorNome(TPaciente *raiz, int id, char nome[20], int idade, char condicao[30]) {
  if (raiz == NULL) {
    return alocarPaciente(id, nome, idade, condicao);
  } else {
    if (strcmp(nome, raiz->nome) < 0) {
      raiz->esq = inserirPorNome(raiz->esq, id, nome, idade, condicao);
    } else {
      raiz->dir = inserirPorNome(raiz->dir, id, nome, idade, condicao);
    }
  }
  return raiz;
}

void visualizarRegistro(TPaciente *raiz) {
  // percorre a arvore de forma recursiva -> travessia em ordem
  if (raiz != NULL) {
    visualizarRegistro(raiz->esq); // subarvore esquerda
    printf("\nNome: %s\n", raiz->nome);
    printf("ID: %d\n", raiz->id);
    printf("Idade: %d\n", raiz->idade);
    printf("Condicao medica: %s\n", raiz->condicao);
    visualizarRegistro(raiz->dir); // subarvore direita
  }
}

TPaciente **ppMenor(TPaciente **raiz) {
  TPaciente **pmenor = raiz;
  while (*pmenor != NULL && (*pmenor)->esq != NULL) {
    pmenor = &(*pmenor)->esq;
  }
  return pmenor;
}

// remove o cadastro conforme ID informado pelo usuario
TPaciente *removerPaciente(TPaciente **raiz, int id){
  if(*raiz == NULL){
    printf("Paciente nao encontrado!\n");
    return NULL;
  }

  TPaciente *aux = *raiz;

  // encontra o nó que deve ser removido, segue a lógica das funções de busca pra percorrer a arvore
  if(id < aux->id){
    aux->esq = removerPaciente(&(aux->esq), id);
  } else if(id > aux->id) {
    aux->dir = removerPaciente(&(aux->dir), id);
  } else{
    // caso 1: o nó não tem filhos 
    if(aux->esq == NULL && aux->dir == NULL){
      free(aux);
      return NULL;
    }
    // caso 2: o nó tem apenas um filho
    else if (aux->esq == NULL){
      TPaciente *temp = aux->dir;
      free(aux);
      return temp;
    } else if (aux->dir == NULL){
      TPaciente *temp = aux->esq;
      free(aux);
      return temp;
    }
    // caso 3: o nó tem dois filhos
    else{
      TPaciente **paux  = ppMenor(&(aux->dir));
      // troca o valor do nó com o sucessor
      aux->id = (*paux )->id;
      strcpy(aux->nome, (*paux )->nome);
      aux->idade = (*paux )->idade;
      strcpy(aux->condicao, (*paux )->condicao);
      // remove o sucessor da árvore
      aux->dir = removerPaciente(&(aux->dir), (*paux )->id);
    }
  }
  return *raiz;
}

// função para procurar o cadastro do paciente caso o usuario tenha escolhido ordenar pelo ID
void buscarId(TPaciente *raiz, int id) {
  if (raiz == NULL) {
    printf("Paciente nao encontrado!\n");
    return;
  }
  if (id == raiz->id) {
    printf("Paciente encontrado!\n");
    printf("Nome: %s\n", raiz->nome);
    printf("Idade: %d\n", raiz->idade);
    printf("Condicao medica: %s\n", raiz->condicao);
  } else if (id < raiz->id) {
    buscarId(raiz->esq, id);
  } else {
    buscarId(raiz->dir, id);
  }
}

// função para procurar o cadastro do paciente caso o usuario tenha escolhido ordenar pelo nome
void buscarNome(TPaciente *raiz, char nome[20]) {
  if (raiz == NULL) {
    printf("Paciente nao encontrado!\n");
    return;
  }
  if (strcmp(nome, raiz->nome) == 0) {
    printf("Paciente encontrado!\n");
    printf("Nome: %s\n", raiz->nome);
    printf("Idade: %d\n", raiz->idade);
    printf("Condicao medica: %s\n", raiz->condicao);
  } else if (strcmp(nome, raiz->nome) < 0) {
    buscarNome(raiz->esq, nome);
  } else {
    buscarNome(raiz->dir, nome);
  }
}

// essa função serve pra verificar se o id e/ou nome já está cadastrado, a função retorna 1 se existe um cadastro com as mesmas informações e retorna 0 caso não encontre um cadastro igual
int verificarExistencia(TPaciente *raiz, int id, char nome[20]) {
  // se raiz for nulo a àrvore foi foi percorrida até o final sem encontrar
  if (raiz == NULL) {
    // printf("Informações novas\n"); // usado somente nos testes
    return 0;
  }
  // verifica se o id ou o nome já existem
  if (id == raiz->id || strcmp(nome, raiz->nome) == 0) {
    printf("ID e/ou NOME ja cadastrados. Tente novamente!\n");
    return 1;
  } else if (id < raiz->id) {
    return verificarExistencia(raiz->esq, id, nome);
  } else {
    return verificarExistencia(raiz->dir, id, nome);
  }
}

// libera a memória ao final da execução
void liberarMemoria(TPaciente *raiz) {
  if (raiz != NULL) {
    liberarMemoria(raiz->esq);
    liberarMemoria(raiz->dir);
    free(raiz);
  }
}

// função pra salvar os dados em um novo arquivo ao final da execução, travessia em ordem na árvore
void salvarArquivo(TPaciente *raiz, FILE *file) {
  if (raiz != NULL) {
    salvarArquivo(raiz->esq, file);
    fprintf(file, "%d %s %d %s\n", raiz->id, raiz->nome, raiz->idade,
            raiz->condicao);
    salvarArquivo(raiz->dir, file);
  }
}

// função para contar o total de pacientes cadastrados
void contarPacientes(TPaciente *raiz, int contador) {
  int aux = contador;
  if (raiz != NULL) {
    aux++;
    contarPacientes(raiz->dir, aux);
  } else {
    printf("Total de pacientes cadastrados atualmente: %d\n", aux);
  }
}
