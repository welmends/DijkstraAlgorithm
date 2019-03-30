//TODO
// - See a better structure (put all in digraph)
// - optimize the code

////////////////////////////////////////////////////////////////////////////////
//                                  Libraries                                 //
////////////////////////////////////////////////////////////////////////////////
#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>

////////////////////////////////////////////////////////////////////////////////
//                                   Structs                                  //
////////////////////////////////////////////////////////////////////////////////

//Arrow struct
typedef struct DigraphArrow{
  int target;
  int cost;
}Arrow;

//Digraph struct
typedef struct DirectedGraph{
  Arrow** connectedList;
  int* vertices;
  int* arrowsSizes;
  int verticeSize;
  int arrowSize;
}Digraph;

//Dijkstra minimum path struct
typedef struct DijkstraMinimumPath{
  int* d;
  int* previous;
  int size;
  int source;
  int target;
}MinimumPath;

////////////////////////////////////////////////////////////////////////////////
//                                 Prototypes                                 //
////////////////////////////////////////////////////////////////////////////////
MinimumPath* dijkstraAlgorithm(Digraph* digraph, int source, int target);

void initMinimumPath(MinimumPath** digraph, int verticeSize, int source, int target);
void displayMinimumPath(MinimumPath* minimumPath, bool showVectors);
bool getMinimumCostArrowInBorder(Digraph* digraph, MinimumPath* minimumPath, int* Z, int* u, int* v);

void initDigraph(Digraph** digraph);
void inputDigraph(Digraph* digraph, char** argv, int* source, int* target);
void displayDigraph(Digraph* digraph);

int getVertice(Digraph* digraph, int v);
Arrow getArrow(Digraph* digraph, int u, int v);

void addVertice(Digraph* digraph, int newVertice);
void addArrow(Digraph* digraph, int sourceVertice, int targetVertice, int cost);
void delVertice(Digraph* digraph, int delVertice);
void delArrow(Digraph* digraph, int sourceVertice, int targetVertice);

////////////////////////////////////////////////////////////////////////////////
//                                Source Code                                 //
////////////////////////////////////////////////////////////////////////////////

/**
 * @brief: O metodo main chama todos os outros metodos visando realizar todas as
           operacoes necessarias para o calculo do algorito de Dijkstra.
 * @param:  int argc, char **argv
 * @return: int
 */
int main(int argc, char** argv){
  if(argc!=2){
    printf("Error [int main]: arguments number must be 2\n");
    return 1;
  }

  int source, target;
  Digraph *digraph;

  initDigraph(&digraph);
  inputDigraph(digraph, argv, &source, &target);
  displayDigraph(digraph);
  displayMinimumPath(dijkstraAlgorithm(digraph, source, target), false);

  return 0;
}

/**
 * @brief:  O metodo dijkstraAlgorithm realiza todas as operacoes do algoritmo de
            Dijkstra e retorna o caminho minimo do 'source' para o 'target'.
 * @param:  Digraph*, int, int
 * @return: MinimumPath*
 */
MinimumPath* dijkstraAlgorithm(Digraph* digraph, int source, int target){
  int i, u, v;
  int Z[digraph->verticeSize+1];
  MinimumPath* minimumPath;

  //Inicialize a struct DijkstraMinimumPath
  initMinimumPath(&minimumPath, digraph->verticeSize, source, target);//init

  //Percorra zerando previous e setando d para -1 (custo negativo nao conta)
  for(i=0; i<minimumPath->size; i++){
    minimumPath->d[i] = -1;
    minimumPath->previous[i] = 0;
  }

  //Z[0] sera o tamanho do Z atual
  *Z = 1;
  //Coloque source na fronteira
  Z[*Z] = minimumPath->source;

  //Coloque d[0] = 0
  minimumPath->d[0] = 0;

  //Verifique se existe arco na fronteira se existir pegue o de menor custo
  while(getMinimumCostArrowInBorder(digraph, minimumPath, Z, &u, &v)){
    //Coloque v na fronteira
    (*Z)++;
    Z[*Z] = v;

    //Calcule o novo custo de s para v
    minimumPath->d[v-1] = minimumPath->d[u-1]+getArrow(digraph, u, v).cost;
    //Atualize o vetor de anteriores
    minimumPath->previous[v-1] = u;

    //Se chegar no target saia do laco
    if(minimumPath->target==v){
      break;
    }
  }
  return minimumPath;
}

/**
 * @brief: O metodo initMinimumPath inicializa a estrutura minimumPath.
 * @param:  MinimumPath**, int, int, int
 * @return: void
 */
void initMinimumPath(MinimumPath** minimumPath, int verticeSize, int source, int target){
  //Aloque o Caminho Minimo
  *minimumPath  = (MinimumPath *)calloc(1, sizeof(Digraph));
  //Inicialize o Caminho Minimo
  **minimumPath = (MinimumPath){NULL, NULL, verticeSize, source, target};
  //Aloque memoria para os vetores
  (*minimumPath)->d = (int *)calloc((*minimumPath)->size, sizeof(int));
  (*minimumPath)->previous = (int *)calloc((*minimumPath)->size, sizeof(int));
  //Verifique a alocacao de memoria
  if(*minimumPath==NULL || (*minimumPath)->d==NULL || (*minimumPath)->previous==NULL){
    printf("Error [void initMinimumPath]: No memory\n");
    exit(-1);
  }
}
/**
 * @brief: O metodo displayMinimumPath mostra como esta a estrutura minimumPath.
 * @param:  MinimumPath*, bool
 * @return: void
 */
void displayMinimumPath(MinimumPath* minimumPath, bool showVectors){
  if(minimumPath->previous[minimumPath->target-1]==0){
    printf("> Minimum Path [%d to %d]: does not exist\n", minimumPath->source, minimumPath->target);
    return;
  }

  bool hasEnded;
  int i, j, z, currVertice;

  hasEnded = false;
  currVertice = minimumPath->target;
  i=0;
  while(!hasEnded){
    if(currVertice==minimumPath->source){
      hasEnded = true;
    }else{
      currVertice = minimumPath->previous[currVertice-1];
    }
    i++;
  }

  int minPath[i];
  hasEnded = false;
  currVertice = minimumPath->target;
  z=j=i;
  while(!hasEnded){
    minPath[--z]=currVertice;
    if(currVertice==minimumPath->source){
      hasEnded = true;
    }else{
      currVertice = minimumPath->previous[currVertice-1];
    }
  }

  printf("> Minimum Path [%d to %d]: ", minimumPath->source, minimumPath->target);
  for(i=0; i<j; i++){
    printf("[%d]",minPath[i]);
    if(i==j-1){
      printf("\n");
    }else{
      printf("->");
    }
  }

  if(showVectors){
    printf("\n  d  -> ");
    for(i=0; i<minimumPath->size; i++){
      printf("[%d]", minimumPath->d[i]);
    }
    printf("\nprev -> ");
    for(i=0; i<minimumPath->size; i++){
      printf("[%d]", minimumPath->previous[i]);
    }
    printf("\nindex-> ");
    for(i=0; i<minimumPath->size; i++){
      printf("[%d]", i+1);
    }
    printf("\n");
  }
}
/**
 * @brief: O metodo getMinimumCostArrowInBorder verifica se existe algum arco na
           fronteira de Z. Se existir algum arco o metodo retorna 'true' e atualiza
           os valores (u,v) que representam o arco minimo na fronteira de Z.
 * @param:  Digraph*, MinimumPath*, int*, int*, int*
 * @return: bool
 */
bool getMinimumCostArrowInBorder(Digraph* digraph, MinimumPath* minimumPath, int* Z, int* u, int* v){
  int i, j, z;
  int min=-1, minAux, uAux, vAux, uIndex, vIndex;
  bool isInBorder;

  for(i=1; i<=*Z; i++){
    uAux = Z[i];
    uIndex = getVertice(digraph, uAux);
    for(j=0; j<digraph->arrowsSizes[uIndex]; j++){
      vAux = digraph->connectedList[uIndex][j].target;
      vIndex = j;
      isInBorder = true;
      for(z=1; z<=*Z; z++){
        if(Z[z]==vAux){
          isInBorder = false;
          break;
        }
      }
      if(isInBorder){
        if(minimumPath->d[uAux-1]==-1){
          minAux = digraph->connectedList[uIndex][vIndex].cost;
        }else{
          minAux = minimumPath->d[uAux-1]+digraph->connectedList[uIndex][vIndex].cost;
        }
        if(min==-1){
          min = minAux;
          *u = uAux;
          *v = vAux;
        }else{
          if(minAux<min){
            min = minAux;
            *u = uAux;
            *v = vAux;
          }
        }
      }
    }
  }
  if(min!=-1){
    return true;
  }
  return false;
}

/**
 * @brief: O metodo initDigraph inicializa a estrutura Digraph.
 * @param:  Digraph**
 * @return: void
 */
void initDigraph(Digraph **digraph){
  //Aloque o Digrafo
  *digraph  = (Digraph *)calloc(1, sizeof(Digraph));
  //Inicialize o Digrafo
  **digraph = (Digraph){NULL, NULL, NULL, 0, 0};
  //Verifique a alocacao de memoria
  if(*digraph==NULL){
    printf("Error [void initDigraph]: No memory\n");
    exit(-1);
  }
}
/**
 * @brief: O metodo inputDigraph abre o arquivo .txt (passado como argumento) e
           preenche o Digrafo.
 * @param:  Digraph**, char**, int*, int*
 * @return: void
 */
void inputDigraph(Digraph *digraph, char **argv, int *source, int *target){
  FILE *file;
  int vSize, aSize;
  int u, v, cost;

  file = fopen(argv[1], "r");
  fscanf(file, "%d %d %d %d", &vSize, &aSize, &*source, &*target);
  if(*source==*target){
    printf("Error [void inputDigraph]: source and target vertices are equal\n");
    exit(-1);
  }
  while(fgetc(file) != EOF){
    fscanf(file, "%d %d %d", &u, &v, &cost);
    addArrow(digraph, u, v, cost);
    if(u==v){
      printf("Error [void inputDigraph]: there is arrow from u to u, this code do not accept not simple graph\n");
      exit(-1);
    }
  }
  fclose(file);

  if(vSize!=digraph->verticeSize || aSize!=digraph->arrowSize){
    printf("Error [void inputDigraph]: the input digraph is wrong, the number of arrow or vertices do not match\n");
    exit(-1);
  }
}
/**
 * @brief: O metodo displayMinimumPath mostra como esta a estrutura Digraph.
 * @param:  Digraph*
 * @return: void
 */
void displayDigraph(Digraph *digraph){
  int i, j;

  //Mostre o digrafo de acordo com o modelo proposto
  printf("> Digraph:\n");
  printf("%d %d\n", digraph->verticeSize, digraph->arrowSize);
  for(i=0; i<digraph->verticeSize; i++){
    for(j=0; j<digraph->arrowsSizes[i]; j++){
      printf("%d %d %d\n",digraph->vertices[i], digraph->connectedList[i][j].target, digraph->connectedList[i][j].cost);
    }
  }
  printf("\n");
}

/**
 * @brief: O metodo getVertice retorna o index do vertice dado o vertice v.
 * @param:  Digraph*
 * @return: int
 */
int getVertice(Digraph *digraph, int v){
  int i;

  //Verifique se ja existe esse vertice
  for(i=0; i<digraph->verticeSize; i++){
    if(digraph->vertices[i]==v){
      return i;//Se sim, retorne seu index
    }
  }
  return -1;//Se nao, retorne -1
}
/**
 * @brief: O metodo getArrow retorna uma instancia da estrutura Arrow dado os
           vertices u e v.
 * @param:  Digraph*
 * @return: Arrow
 */
Arrow getArrow(Digraph *digraph, int u, int v){
  int i;
  Arrow a = {-1, -1};

  //Pegue o vertice
  u = getVertice(digraph, u);

  //Verifique se ja existe esse vertice
  for(i=0; i<digraph->arrowsSizes[u]; i++){
    if(digraph->connectedList[u][i].target==v){
      a = digraph->connectedList[u][i];
      return a;//Se sim, retorne o arrow correto
    }
  }
  return a;//Se nao, retorne a = {-1, -1}
}

/**
 * @brief: O metodo addVertice adiciona um vertice na lista de adjacencias.
 * @param:  Digraph*, int
 * @return: void
 */
void addVertice(Digraph *digraph, int newVertice){
  //Se nao houver vertices adicione o primeiro
  if(digraph->verticeSize==0){
    //Aloque memoria para a lista conectada
    digraph->connectedList = (Arrow **)calloc(1,sizeof(Arrow *));
    //Aloque memoria para o vetor de vertices (nome do vertice)
    digraph->vertices = (int *)calloc(1,sizeof(int));
    //Aloque memoria para o vetor de tamanhos de arcos (das listas conectadas)
    digraph->arrowsSizes = (int *)calloc(1,sizeof(int));
    //Verifique a alocacao de memoria
    if(digraph->connectedList==NULL || digraph->vertices==NULL || digraph->arrowsSizes==NULL){
      printf("Error [void addVertice]: No memory\n");
      exit(-1);
    }

    //Adicione o vertice
    digraph->vertices[digraph->verticeSize] = newVertice;
    //Coloque a quantidade de arcos deste vertice para 0
    digraph->arrowsSizes[digraph->verticeSize] = 0;
    //Incremente o tamanho de vertices do digrafo
    digraph->verticeSize = 1;
  }else{
    //Verifique se ja existe esse vertice
    if(getVertice(digraph, newVertice)!=-1){
      return;//Se sim, saia da funcao sem conseguir adicionar o vertice
    }

    Arrow ** aux;
    int * aux1, * aux2;
    int i, j, iAux;
    bool hasBeenAdded = false;

    //Aloque memoria para a lista conectada e os vertores de vertice e tamanhos de arco
    aux = (Arrow **)calloc(digraph->verticeSize+1, sizeof(Arrow *));
    aux1 = (int *)calloc(digraph->verticeSize+1, sizeof(int));
    aux2 = (int *)calloc(digraph->verticeSize+1, sizeof(int));
    //Verifique a alocacao de memoria
    if(aux==NULL || aux1==NULL || aux2==NULL){
      printf("Error [void addVertice]: No memory\n");
      exit(-1);
    }

    //Transfira para os auxiliares
    iAux = 0;
    for(i=0; i<digraph->verticeSize+1; i++){
      if(!hasBeenAdded && (digraph->vertices[iAux]>newVertice || i==digraph->verticeSize)){
        //Adicione o vertice
        aux1[i] = newVertice;
        //Coloque a quantidade de arcos daquele vertice para 0
        aux2[i] = 0;
        //Coloque hasBeenAdded para verdadeiro
        hasBeenAdded = true;
      }else{
        //Aloque memoria para um vertice da lista conectada
        aux[i] = (Arrow *)calloc(digraph->arrowsSizes[iAux], sizeof(Arrow));
        //Verifique a alocacao de memoria
        if(aux[i]==NULL){
          printf("Error [void addVertice]: No memory\n");
          exit(-1);
        }

        //Lista conectada
        for(j=0; j<digraph->arrowsSizes[iAux]; j++){
          aux[i][j].target = digraph->connectedList[iAux][j].target;
          aux[i][j].cost = digraph->connectedList[iAux][j].cost;
        }
        //Vetor de vertices
        aux1[i] = digraph->vertices[iAux];
        //Vetor de tamanho de arcos
        aux2[i] = digraph->arrowsSizes[iAux];
        //Desaloque um vertice da lista conectada
        free(digraph->connectedList[iAux]);
        //Incremente o iAux
        iAux++;
      }
    }
    //Incremente a quantidade de vertices
    digraph->verticeSize++;

    //Desaloque a lista conectada, o vetor de vertices e o vetor de tamanhos
    free(digraph->connectedList);
    free(digraph->vertices);
    free(digraph->arrowsSizes);

    //Aloque memoria novamente para lista conectada, o vetor de vertices e o vetor de tamanhos
    digraph->connectedList = (Arrow **)calloc(digraph->verticeSize, sizeof(Arrow *));
    digraph->vertices = (int *)calloc(digraph->verticeSize, sizeof(int));
    digraph->arrowsSizes = (int *)calloc(digraph->verticeSize, sizeof(int));
    //Verifique a alocacao de memoria
    if(digraph->connectedList==NULL || digraph->vertices==NULL || digraph->arrowsSizes==NULL){
      printf("Error [void addVertice]: No memory\n");
      exit(-1);
    }

    //Passe as referencias para as variaveis originais
    digraph->connectedList = aux;
    digraph->vertices = aux1;
    digraph->arrowsSizes = aux2;
  }
}
/**
 * @brief: O metodo addArrow adiciona um arco na lista de adjacencias.
 * @param:  Digraph*, int, int, int
 * @return: void
 */
void addArrow(Digraph *digraph, int sourceVertice, int targetVertice, int cost){
  int i, source=-1, target=-1;

  addVertice(digraph, sourceVertice);
  addVertice(digraph, targetVertice);

  //Verifique se os vertices existem no Digrafo
  source = getVertice(digraph, sourceVertice);
  target = getVertice(digraph, targetVertice);

  //Verifique se o arco existe no Digrafo
  for(i=0; i<digraph->arrowsSizes[source]; i++){
    if(digraph->connectedList[source][i].target==targetVertice){
      target = i;
      digraph->connectedList[source][target].cost = cost;
      return;//Saia da funcao atualizando o custo do arco ja existente
    }
  }

  //Verifique se o grafo eh nao simples
  for(i=0; i<digraph->arrowsSizes[target]; i++){
    if(digraph->connectedList[target][i].target==sourceVertice){
      printf("Error [void addArrow]: the input digraph has an arrow from u to v and from v to u, this code do not accept not simple digraph\n");
      exit(-1);
    }
  }

  //Se nao houver arcos neste vertice adicione o primeiro
  if(digraph->arrowsSizes[source]==0){
    //Aloque memoria para um vertice da lista conectada
    digraph->connectedList[source] = (Arrow *)calloc(1,sizeof(Arrow));
    //Verifique a alocacao de memoria
    if(digraph->connectedList[source]==NULL){
      printf("Error [void addArrow]: No memory\n");
      exit(-1);
    }

    //Adicione o arco
    digraph->connectedList[source][0].target = targetVertice;
    digraph->connectedList[source][0].cost = cost;
    //Coloque a quantidade de arcos daquele vertice para 1
    digraph->arrowsSizes[source] = 1;
    //Incremente o tamanho de arcos do digrafo
    digraph->arrowSize++;
  }else{
    //Declare uma variavel auxiliar para a realocacao de memoria para adicionar um novo arco
    Arrow * aux;

    //Realoque memoria para uma lista conectada
    aux = (Arrow *)realloc(digraph->connectedList[source], (digraph->arrowsSizes[source]+1)*sizeof(Arrow));
    //Verifique a alocacao de memoria
    if(aux==NULL){
      printf("Error [void addArrow]: No memory\n");
      exit(-1);
    }

    //Passe a referencia para a variavel original
    digraph->connectedList[source] = aux;
    //Adicione o arco
    digraph->connectedList[source][digraph->arrowsSizes[source]].target = targetVertice;
    digraph->connectedList[source][digraph->arrowsSizes[source]].cost = cost;
    //Incremente a quantidade de arcos do vertice source
    digraph->arrowsSizes[source]++;
    //Incremente a quantidade de arcos do Digrafo
    digraph->arrowSize++;
  }
}
/**
 * @brief: O metodo delVertice deleta um vertice da lista de adjacencias.
 * @param:  Digraph*, int
 * @return: void
 */
void delVertice(Digraph *digraph, int delVertice){
  //Verifique se ja existe esse vertice
  int v = getVertice(digraph, delVertice);

  if(v==-1){
    return;//Saia da funcao sem conseguir deletar o vertice
  }else{
    Arrow ** aux;
    int * aux1, * aux2;
    int i, j, iAux;

    //Aloque memoria para a lista conectada e os vertores de vertice e tamanhos de arco
    aux = (Arrow **)calloc(digraph->verticeSize-1, sizeof(Arrow *));
    aux1 = (int *)calloc(digraph->verticeSize-1, sizeof(int));
    aux2 = (int *)calloc(digraph->verticeSize-1, sizeof(int));
    //Verifique a alocacao de memoria
    if(aux==NULL || aux1==NULL || aux2==NULL){
      printf("Error [void delVertice]: No memory\n");
      exit(-1);
    }

    //Passe os vertices (menos o que deseja deletar para aux)
    iAux = 0;
    for(i=0; i<digraph->verticeSize; i++){
      if(i!=v){
        //Aloque memoria para um vertice da lista conectada
        aux[iAux] = (Arrow *)calloc(digraph->arrowsSizes[i], sizeof(Arrow));
        //Verifique a alocacao de memoria
        if(aux[i]==NULL){
          printf("Error [void delVertice]: No memory\n");
          exit(-1);
        }

        //Lista conectada
        for(j=0; j<digraph->arrowsSizes[i]; j++){
          aux[iAux][j].target = digraph->connectedList[i][j].target;
          aux[iAux][j].cost = digraph->connectedList[i][j].cost;
        }
        //Vetor de vertices
        aux1[iAux] = digraph->vertices[i];
        //Vetor de tamanho de arcos
        aux2[iAux] = digraph->arrowsSizes[i];
        //Incremente o iAux
        iAux++;
      }
      //Desaloque um vertice da lista conectada
      free(digraph->connectedList[i]);
    }
    //Decremente a quantidade de vertices
    digraph->verticeSize--;
    //Subtraia a quantidade de arcos do digrafo
    digraph->arrowSize -= digraph->arrowsSizes[v];

    //Desaloque a lista conectada, o vetor de vertices e o vetor de tamanhos
    free(digraph->connectedList);
    free(digraph->vertices);
    free(digraph->arrowsSizes);

    //Aloque memoria novamente para a lista conectada, para o vetor de vertices e para o vetor de tamanhos
    digraph->connectedList = (Arrow **)calloc(digraph->verticeSize, sizeof(Arrow *));
    digraph->vertices = (int *)calloc(digraph->verticeSize, sizeof(int));
    digraph->arrowsSizes = (int *)calloc(digraph->verticeSize, sizeof(int));
    //Verifique a alocacao de memoria
    if(digraph->connectedList==NULL || digraph->vertices==NULL || digraph->arrowsSizes==NULL){
      printf("Error [void delVertice]: No memory\n");
      exit(-1);
    }

    //Passe as referencias para as variaveis originais
    digraph->connectedList = aux;
    digraph->vertices = aux1;
    digraph->arrowsSizes = aux2;
  }
}
/**
 * @brief: O metodo delArrow deleta um arco da lista de adjacencias.
 * @param:  Digraph*, int, int
 * @return: void
 */
void delArrow(Digraph *digraph, int sourceVertice, int targetVertice){
  //Declare uma variavel para indice do source e target na lista conectada
  int i, j, source=-1, target=-1;

  //Verifique se o arco (s,t) existe no Digrafo
  source = getVertice(digraph, sourceVertice);
  if(source!=-1){
    for(i=0; i<digraph->arrowsSizes[source]; i++){
      if(digraph->connectedList[source][i].target==targetVertice){
        target = i;
        break;
      }
    }
  }

  //Se existir o arco (s,t)
  if(source!=-1 && target!=-1){
    //Declare variaveis auxiliares para a realocacao de memoria para deletar um vertice
    Arrow ** aux;
    int iAux;

    //Aloque memoria para a lista conectada
    aux = (Arrow **)calloc(digraph->verticeSize, sizeof(Arrow *));
    //Verifique a alocacao de memoria
    if(aux==NULL){
      printf("Error [void delArrow]: No memory\n");
      exit(-1);
    }

    //Passe os vertices (menos o arco que deseja deletar para aux)
    iAux = 0;
    for(i=0; i<digraph->verticeSize; i++){
      //Aloque memoria para um vertice da lista conectada
      if(i==source){
        aux[iAux] = (Arrow *)calloc(digraph->arrowsSizes[i]-1, sizeof(Arrow));
      }else{
        aux[iAux] = (Arrow *)calloc(digraph->arrowsSizes[i], sizeof(Arrow));
      }
      //Verifique a alocacao de memoria
      if(aux[iAux]==NULL){
        printf("Error [void delArrow]: No memory\n");
        exit(-1);
      }

      //Lista conectada
      for(j=0; j<digraph->arrowsSizes[i]; j++){
        if(!(i==source && j==target)){
          aux[iAux][j].target = digraph->connectedList[i][j].target;
          aux[iAux][j].cost = digraph->connectedList[i][j].cost;
        }
      }
      //Incremente o iAux
      iAux++;
      //Desaloque um vertice da lista conectada
      free(digraph->connectedList[i]);
    }
    //Decremente a quantidade de arcos do digrafo
    digraph->arrowSize--;
    //Decremente a quantidade de arcos do vertice 'source'
    digraph->arrowsSizes[source]--;

    //Desaloque a lista conectada
    free(digraph->connectedList);

    //Aloque memoria novamente para a lista conectada
    digraph->connectedList = (Arrow **)calloc(digraph->verticeSize, sizeof(Arrow *));
    //Verifique a alocacao de memoria
    if(digraph->connectedList==NULL){
      printf("Error [void delArrow]: No memory\n");
      exit(-1);
    }

    //Passe as referencias para a variavel original
    digraph->connectedList = aux;
  }
}
