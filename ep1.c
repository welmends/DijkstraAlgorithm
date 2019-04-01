/**
 * Copyright by João Wellington, Elias Paulino, Francisco Juarez and Thalys Viana
 *    Federal Institute of Education, Science and Technology of Ceará - IFCE
 *          All rights reserved. This software is public domain.
 *
 * Development context: This software was made in the discipline of Graphs under
 *                      the guidance of Professor Glauber F. Cintra.
 *
 * @file ep1.c
 * @brief Dijkstra Algorithm on C language
 *         Input     : A digraph with n vertices and m arrows, each arrow has a
 *                     non-negative cost, and the source and the target.
 *         Output    : A minimum path from source to target, if exists.
 * @author $Author:$ Wellington; Elias; Juarez; Thalys
 * @version $Final:$
 */

////////////////////////////////////////////////////////////////////////////////
//                                  Libraries                                 //
////////////////////////////////////////////////////////////////////////////////
#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>

////////////////////////////////////////////////////////////////////////////////
//                                   Structs                                  //
////////////////////////////////////////////////////////////////////////////////

//Digraph Arrow Struct
typedef struct DigraphArrow{
  int target;
  int cost;
}Arrow;

//Directed Graph Struct
typedef struct DirectedGraph{
  Arrow** adjacency_list;
  int* vertices_values;
  int* arrows_sizes;
  int vertice_size;
  int arrow_size;
}Digraph;

//Dijkstra Minimum Path Struct
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
MinimumPath* dijkstraAlgorithm(Digraph* digraph, MinimumPath* minimumPath);
bool getMinimumCostArrowInBorder(Digraph* digraph, MinimumPath* minimumPath, int* Z, int* u, int* v);

void initDigraph(Digraph** digraph);
void inputDigraph(Digraph* digraph, char** argv, int* source, int* target);
void displayDigraph(Digraph* digraph);

void initMinimumPath(MinimumPath** digraph, int vertice_size, int source, int target);
void displayMinimumPath(MinimumPath* minimumPath, bool showVectors);

int getVertice(Digraph* digraph, int v);
Arrow getArrow(Digraph* digraph, int u, int v);

void addVertice(Digraph* digraph, int newVertice);
void addArrow(Digraph* digraph, int sourceVertice, int targetVertice, int cost);
void delVertice(Digraph* digraph, int delVertice);
void delArrow(Digraph* digraph, int sourceVertice, int targetVertice);

void freeUp(Digraph* digraph, MinimumPath* minimumPath);

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
    printf("Error [int main]: the number of arguments must be equal to 2\n\n");
    printf("Example: ./ep1.o <path_to_txtFile>/filename.txt\n");
    printf("         ./ep1.o filename.txt\n\n");
    printf("Obs.: The .txt file must have the following pattern:\n");
    printf("             n  m  s  t\n");
    printf("            u1 v1 c1\n");
    printf("            u2 v2 c2\n");
    printf("            ...     \n");
    printf("            um vm cm\n");
    printf("      where,\n");
    printf("            n is the vertices number;\n");
    printf("            m is the arrows number;\n");
    printf("            s is the origin vertice;\n");
    printf("            t is the destination vertice;\n");
    printf("            ui and vi are the origin and destination of the arrow i;\n");
    printf("            ci is the cost of the arrow i.\n");
    return 1;
  }

  int source, target;
  Digraph* digraph;
  MinimumPath* minimumPath;

  initDigraph(&digraph);
  inputDigraph(digraph, argv, &source, &target);
  initMinimumPath(&minimumPath, digraph->vertice_size, source, target);

  dijkstraAlgorithm(digraph, minimumPath);

  displayDigraph(digraph);
  displayMinimumPath(minimumPath, false);

  freeUp(digraph, minimumPath);

  return 0;
}

/**
 * @brief:  O metodo dijkstraAlgorithm realiza todas as operacoes do algoritmo de
            Dijkstra e retorna o caminho minimo do 'source' para o 'target'.
 * @param:  Digraph*, int, int
 * @return: MinimumPath*
 */
MinimumPath* dijkstraAlgorithm(Digraph* digraph, MinimumPath* minimumPath){
  //Declare as variaveis
  int i, u, v;
  int Z[digraph->vertice_size+1];
  //Percorra zerando previous e setando d para -1 (custo negativo nao conta)
  for(i=0; i<minimumPath->size; i++){
    minimumPath->d[i] = -1;
    minimumPath->previous[i] = 0;
  }
  //Z[0] sera o tamanho do Z atual
  *Z = 1;
  //Coloque source na regiao Z
  Z[*Z] = minimumPath->source;
  //Coloque d[0] = 0
  minimumPath->d[0] = 0;
  //Verifique se existe arco na fronteira de Z. Se existir pegue o de menor custo
  while(getMinimumCostArrowInBorder(digraph, minimumPath, Z, &u, &v)){
    //Coloque v na regiao Z
    (*Z)++;
    Z[*Z] = v;
    //OBS.: Os valores sao "u-1" e "v-1", pois sao os valores indexados
    //Calcule o novo custo de s para v (atulizando d[v])
    minimumPath->d[v-1] = minimumPath->d[u-1] + getArrow(digraph, u, v).cost;
    //Atualize o vetor de anteriores (anterior[v])
    minimumPath->previous[v-1] = u;
    //Se chegar no target saia do laco
    if(minimumPath->target==v){
      break;
    }
  }
  return minimumPath;//Retorne a instancia da estrutura MinimumPath
}
/**
 * @brief: O metodo getMinimumCostArrowInBorder verifica se existe algum arco na
           fronteira de Z. Se existir algum arco o metodo retorna 'true' e atualiza
           os valores (u,v) que representam o arco minimo na fronteira de Z.
 * @param:  Digraph*, MinimumPath*, int*, int*, int*
 * @return: bool
 */
bool getMinimumCostArrowInBorder(Digraph* digraph, MinimumPath* minimumPath, int* Z, int* u, int* v){
  //Declare as variaveis
  int i, j, z;
  int min = -1, minAux;
  int uAux, vAux;
  int uIndex, vIndex;
  bool isInBorder;
  //Percorra os vertices contidos na regiao Z
  for(i=1; i<=*Z; i++){
    uAux = Z[i];
    uIndex = getVertice(digraph, uAux);
    //Obtenha os arcos de cada um desses vertices
    for(j=0; j<digraph->arrows_sizes[uIndex]; j++){
      vAux = digraph->adjacency_list[uIndex][j].target;
      vIndex = j;
      //Verifique se esses arcos estao na fronteira de Z
      isInBorder = true;
      for(z=1; z<=*Z; z++){
        if(Z[z]==vAux){
          isInBorder = false;
          break;
        }
      }
      //Se sim
      if(isInBorder){
        //Obtenha d[u] + c_uv para cada um deles
        if(minimumPath->d[uAux-1]==-1){
          minAux = digraph->adjacency_list[uIndex][vIndex].cost;
        }else{
          minAux = minimumPath->d[uAux-1]+digraph->adjacency_list[uIndex][vIndex].cost;
        }
        //Alem disso, obtenha o d[u] + c_uv minimo
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
    return true;//Se existir um arco na fronteira, retorne true
  }
  return false;//Se nao, retorne false
}

/**
 * @brief: O metodo initDigraph inicializa a estrutura Digraph.
 * @param:  Digraph**
 * @return: void
 */
void initDigraph(Digraph **digraph){
  //Aloque o Digrafo
  *digraph  = (Digraph *)calloc(1, sizeof(Digraph));
  //Verifique a alocacao de memoria
  if(*digraph==NULL){
    printf("Error [void initDigraph]: No memory\n");
    exit(-1);
  }
  //Inicialize o Digrafo
  **digraph = (Digraph){NULL, NULL, NULL, 0, 0};
}
/**
 * @brief: O metodo inputDigraph abre o arquivo .txt (passado como argumento) e
           preenche o Digrafo.
 * @param:  Digraph**, char**, int*, int*
 * @return: void
 */
void inputDigraph(Digraph *digraph, char **argv, int *source, int *target){
  //Declare as variaveis
  FILE *file;
  int vSize, aSize;
  int u, v, cost;
  //Efetue a leitura do arquivo passado como entrado
  file = fopen(argv[1], "r");
  if(file == NULL){
    printf("Error [void inputDigraph]: '%s' could not be opened\n", argv[1]);
    exit(-1);
  }
  fscanf(file, "%d %d %d %d", &vSize, &aSize, &*source, &*target);
  //Verifique se source e target sao iguais
  if(*source==*target){
    printf("Error [void inputDigraph]: source and target vertice are equal\n");
    exit(-1);
  }
  while(fgetc(file) != EOF){
    fscanf(file, "%d %d %d", &u, &v, &cost);
    addArrow(digraph, u, v, cost);
    //Verifique se o grafo passado como entrada eh nao simples
    if(u==v){
      printf("Error [void inputDigraph]: there is arrow from u to u, this code do not accept not simple graph\n");
      exit(-1);
    }
  }
  fclose(file);
  //Verifique se a quantidade de vertices e arestas coincidem
  if(vSize!=digraph->vertice_size || aSize!=digraph->arrow_size){
    printf("Error [void inputDigraph]: the input digraph is wrong, the number of arrow or vertice do not match\n");
    exit(-1);
  }
}
/**
 * @brief: O metodo displayMinimumPath mostra como esta a estrutura Digraph.
 * @param:  Digraph*
 * @return: void
 */
void displayDigraph(Digraph *digraph){
  //Declare as variaveis
  int i, j;
  //Mostre o digrafo de acordo com o modelo proposto
  printf("> Digraph:\n");
  printf("%d %d\n", digraph->vertice_size, digraph->arrow_size);
  for(i=0; i<digraph->vertice_size; i++){
    for(j=0; j<digraph->arrows_sizes[i]; j++){
      printf("%d %d %d\n",digraph->vertices_values[i], digraph->adjacency_list[i][j].target, digraph->adjacency_list[i][j].cost);
    }
  }
  printf("\n");
}

/**
 * @brief: O metodo initMinimumPath inicializa a estrutura minimumPath.
 * @param:  MinimumPath**, int, int, int
 * @return: void
 */
void initMinimumPath(MinimumPath** minimumPath, int vertice_size, int source, int target){
  //Aloque o Caminho Minimo
  *minimumPath  = (MinimumPath *)calloc(1, sizeof(Digraph));
  //Verifique a alocacao de memoria
  if(*minimumPath==NULL){
    printf("Error [void initMinimumPath]: No memory\n");
    exit(-1);
  }
  //Inicialize o Caminho Minimo
  **minimumPath = (MinimumPath){NULL, NULL, vertice_size, source, target};
  //Aloque memoria para os vetores
  (*minimumPath)->d = (int *)calloc((*minimumPath)->size, sizeof(int));
  (*minimumPath)->previous = (int *)calloc((*minimumPath)->size, sizeof(int));
  //Verifique a alocacao de memoria
  if((*minimumPath)->d==NULL || (*minimumPath)->previous==NULL){
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
  //Verifique se existe um caminho minimo entre source e target
  if(minimumPath->previous[minimumPath->target-1]==0){
    printf("> Minimum Path [%d to %d]: does not exist\n", minimumPath->source, minimumPath->target);
    return;
  }
  //Declare as variaveis
  bool hasEnded;
  int i, currVertice, amountVertice;
  //Obtenha o tamanho do caminho minimo
  hasEnded = false;
  currVertice = minimumPath->target;
  amountVertice=0;
  while(!hasEnded){
    if(currVertice==minimumPath->source){
      hasEnded = true;
    }else{
      currVertice = minimumPath->previous[currVertice-1];
    }
    amountVertice++;
  }
  //Armazene o caminho minimo em minPath
  int minPath[amountVertice];
  hasEnded = false;
  currVertice = minimumPath->target;
  i=amountVertice;
  while(!hasEnded){
    minPath[--i]=currVertice;
    if(currVertice==minimumPath->source){
      hasEnded = true;
    }else{
      currVertice = minimumPath->previous[currVertice-1];
    }
  }
  //Mostre o caminho minimo
  printf("> Minimum Path [%d to %d]: ", minimumPath->source, minimumPath->target);
  for(i=0; i<amountVertice; i++){
    printf("[%d]",minPath[i]);
    if(i==amountVertice-1){
      printf("\n");
    }else{
      printf("->");
    }
  }
  //Mostre o custo do caminho minimo
  printf("> Minimum Path Cost: %d\n", minimumPath->d[minimumPath->target-1]);
  //Se showVectors==true, mostre os vetores da estrutura minimumPath
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
 * @brief: O metodo getVertice retorna o indice do vertice dado o vertice v.
 * @param:  Digraph*
 * @return: int
 */
int getVertice(Digraph *digraph, int v){
  //Declare a variavel
  int i;
  //Verifique se ja existe esse vertice
  for(i=0; i<digraph->vertice_size; i++){
    if(digraph->vertices_values[i]==v){
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
  //Declare as variaveis
  int i;
  Arrow a = {-1, -1};
  //Pegue o vertice indice do u
  u = getVertice(digraph, u);
  //Verifique se ja existe esse vertice
  for(i=0; i<digraph->arrows_sizes[u]; i++){
    if(digraph->adjacency_list[u][i].target==v){
      a = digraph->adjacency_list[u][i];
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
  if(digraph->vertice_size==0){
    //Aloque memoria para a lista de adjacencias
    digraph->adjacency_list = (Arrow **)calloc(1,sizeof(Arrow *));
    //Aloque memoria para o vetor de vertices (nome do vertice)
    digraph->vertices_values = (int *)calloc(1,sizeof(int));
    //Aloque memoria para o vetor de tamanhos de arcos (das listas conectadas)
    digraph->arrows_sizes = (int *)calloc(1,sizeof(int));
    //Verifique a alocacao de memoria
    if(digraph->adjacency_list==NULL || digraph->vertices_values==NULL || digraph->arrows_sizes==NULL){
      printf("Error [void addVertice]: No memory\n");
      exit(-1);
    }
    //Adicione o vertice
    digraph->vertices_values[digraph->vertice_size] = newVertice;
    //Coloque a quantidade de arcos deste vertice para 0
    digraph->arrows_sizes[digraph->vertice_size] = 0;
    //Incremente o tamanho de vertices do digrafo
    digraph->vertice_size = 1;
  }else{
    //Verifique se ja existe esse vertice
    if(getVertice(digraph, newVertice)!=-1){
      return;//Se sim, saia da funcao sem conseguir adicionar o vertice
    }
    //Declare variaveis auxiliares e de controle
    Arrow ** aux;
    int * aux1, * aux2;
    int i, j, iAux;
    bool hasBeenAdded = false;
    //Aloque memoria para a lista de adjacencias e os vertores de vertice e tamanhos de arco
    aux = (Arrow **)calloc(digraph->vertice_size+1, sizeof(Arrow *));
    aux1 = (int *)calloc(digraph->vertice_size+1, sizeof(int));
    aux2 = (int *)calloc(digraph->vertice_size+1, sizeof(int));
    //Verifique a alocacao de memoria
    if(aux==NULL || aux1==NULL || aux2==NULL){
      printf("Error [void addVertice]: No memory\n");
      exit(-1);
    }
    //Transfira para os auxiliares
    iAux = 0;
    for(i=0; i<digraph->vertice_size+1; i++){
      if(!hasBeenAdded && (digraph->vertices_values[iAux]>newVertice || i==digraph->vertice_size)){
        //Adicione o vertice
        aux1[i] = newVertice;
        //Coloque a quantidade de arcos daquele vertice para 0
        aux2[i] = 0;
        //Coloque hasBeenAdded para verdadeiro
        hasBeenAdded = true;
      }else{
        //Aloque memoria para um vertice da lista de adjacencias
        aux[i] = (Arrow *)calloc(digraph->arrows_sizes[iAux], sizeof(Arrow));
        //Verifique a alocacao de memoria
        if(aux[i]==NULL){
          printf("Error [void addVertice]: No memory\n");
          exit(-1);
        }
        //lista de adjacencias
        for(j=0; j<digraph->arrows_sizes[iAux]; j++){
          aux[i][j].target = digraph->adjacency_list[iAux][j].target;
          aux[i][j].cost = digraph->adjacency_list[iAux][j].cost;
        }
        //Vetor de vertices
        aux1[i] = digraph->vertices_values[iAux];
        //Vetor de tamanho de arcos
        aux2[i] = digraph->arrows_sizes[iAux];
        //Desaloque um vertice da lista de adjacencias
        free(digraph->adjacency_list[iAux]);
        //Incremente o iAux
        iAux++;
      }
    }
    //Incremente a quantidade de vertices
    digraph->vertice_size++;
    //Desaloque a lista de adjacencias, o vetor de vertices e o vetor de tamanhos
    free(digraph->adjacency_list);
    free(digraph->vertices_values);
    free(digraph->arrows_sizes);
    //Aloque memoria novamente para lista de adjacencias, o vetor de vertices e o vetor de tamanhos
    digraph->adjacency_list = (Arrow **)calloc(digraph->vertice_size, sizeof(Arrow *));
    digraph->vertices_values = (int *)calloc(digraph->vertice_size, sizeof(int));
    digraph->arrows_sizes = (int *)calloc(digraph->vertice_size, sizeof(int));
    //Verifique a alocacao de memoria
    if(digraph->adjacency_list==NULL || digraph->vertices_values==NULL || digraph->arrows_sizes==NULL){
      printf("Error [void addVertice]: No memory\n");
      exit(-1);
    }
    //Passe as referencias para as variaveis originais
    digraph->adjacency_list = aux;
    digraph->vertices_values = aux1;
    digraph->arrows_sizes = aux2;
  }
}
/**
 * @brief: O metodo addArrow adiciona um arco na lista de adjacencias.
 * @param:  Digraph*, int, int, int
 * @return: void
 */
void addArrow(Digraph *digraph, int sourceVertice, int targetVertice, int cost){
  //Declare as variaveis
  int i, source=-1, target=-1;
  //Adicione os vertices se eles nao existirem
  addVertice(digraph, sourceVertice);
  addVertice(digraph, targetVertice);
  //Verifique se os vertices existem no Digrafo
  source = getVertice(digraph, sourceVertice);
  target = getVertice(digraph, targetVertice);
  //Verifique se o arco existe no Digrafo
  for(i=0; i<digraph->arrows_sizes[source]; i++){
    if(digraph->adjacency_list[source][i].target==targetVertice){
      target = i;
      digraph->adjacency_list[source][target].cost = cost;
      return;//Saia da funcao atualizando o custo do arco ja existente
    }
  }
  //Verifique se o grafo eh nao simples
  for(i=0; i<digraph->arrows_sizes[target]; i++){
    if(digraph->adjacency_list[target][i].target==sourceVertice){
      printf("Error [void addArrow]: the input digraph has an arrow from u to v and from v to u, this code do not accept not simple digraph\n");
      exit(-1);
    }
  }
  //Se nao houver arcos neste vertice adicione o primeiro
  if(digraph->arrows_sizes[source]==0){
    //Aloque memoria para um vertice da lista de adjacencias
    digraph->adjacency_list[source] = (Arrow *)calloc(1,sizeof(Arrow));
    //Verifique a alocacao de memoria
    if(digraph->adjacency_list[source]==NULL){
      printf("Error [void addArrow]: No memory\n");
      exit(-1);
    }
    //Adicione o arco
    digraph->adjacency_list[source][0].target = targetVertice;
    digraph->adjacency_list[source][0].cost = cost;
    //Coloque a quantidade de arcos daquele vertice para 1
    digraph->arrows_sizes[source] = 1;
    //Incremente o tamanho de arcos do digrafo
    digraph->arrow_size++;
  }else{
    //Declare uma variavel auxiliar
    Arrow * aux;
    //Realoque memoria para uma lista de adjacencias
    aux = (Arrow *)realloc(digraph->adjacency_list[source], (digraph->arrows_sizes[source]+1)*sizeof(Arrow));
    //Verifique a alocacao de memoria
    if(aux==NULL){
      printf("Error [void addArrow]: No memory\n");
      exit(-1);
    }
    //Passe a referencia para a variavel original
    digraph->adjacency_list[source] = aux;
    //Adicione o arco
    digraph->adjacency_list[source][digraph->arrows_sizes[source]].target = targetVertice;
    digraph->adjacency_list[source][digraph->arrows_sizes[source]].cost = cost;
    //Incremente a quantidade de arcos do vertice source
    digraph->arrows_sizes[source]++;
    //Incremente a quantidade de arcos do Digrafo
    digraph->arrow_size++;
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
  //Se sim
  if(v==-1){
    return;//Saia da funcao sem conseguir deletar o vertice
  }else{
    //Declare variaveis auxiliares e de controle
    Arrow ** aux;
    int * aux1, * aux2;
    int i, j, iAux;
    //Aloque memoria para a lista de adjacencias e os vertores de vertice e tamanhos de arco
    aux = (Arrow **)calloc(digraph->vertice_size-1, sizeof(Arrow *));
    aux1 = (int *)calloc(digraph->vertice_size-1, sizeof(int));
    aux2 = (int *)calloc(digraph->vertice_size-1, sizeof(int));
    //Verifique a alocacao de memoria
    if(aux==NULL || aux1==NULL || aux2==NULL){
      printf("Error [void delVertice]: No memory\n");
      exit(-1);
    }
    //Passe os vertices (menos o que deseja deletar para aux)
    iAux = 0;
    for(i=0; i<digraph->vertice_size; i++){
      if(i!=v){
        //Aloque memoria para um vertice da lista de adjacencias
        aux[iAux] = (Arrow *)calloc(digraph->arrows_sizes[i], sizeof(Arrow));
        //Verifique a alocacao de memoria
        if(aux[i]==NULL){
          printf("Error [void delVertice]: No memory\n");
          exit(-1);
        }
        //lista de adjacencias
        for(j=0; j<digraph->arrows_sizes[i]; j++){
          aux[iAux][j].target = digraph->adjacency_list[i][j].target;
          aux[iAux][j].cost = digraph->adjacency_list[i][j].cost;
        }
        //Vetor de vertices
        aux1[iAux] = digraph->vertices_values[i];
        //Vetor de tamanho de arcos
        aux2[iAux] = digraph->arrows_sizes[i];
        //Incremente o iAux
        iAux++;
      }
      //Desaloque um vertice da lista de adjacencias
      free(digraph->adjacency_list[i]);
    }
    //Decremente a quantidade de vertices
    digraph->vertice_size--;
    //Subtraia a quantidade de arcos do digrafo
    digraph->arrow_size -= digraph->arrows_sizes[v];
    //Desaloque a lista de adjacencias, o vetor de vertices e o vetor de tamanhos
    free(digraph->adjacency_list);
    free(digraph->vertices_values);
    free(digraph->arrows_sizes);
    //Aloque memoria novamente para a lista de adjacencias, para o vetor de vertices e para o vetor de tamanhos
    digraph->adjacency_list = (Arrow **)calloc(digraph->vertice_size, sizeof(Arrow *));
    digraph->vertices_values = (int *)calloc(digraph->vertice_size, sizeof(int));
    digraph->arrows_sizes = (int *)calloc(digraph->vertice_size, sizeof(int));
    //Verifique a alocacao de memoria
    if(digraph->adjacency_list==NULL || digraph->vertices_values==NULL || digraph->arrows_sizes==NULL){
      printf("Error [void delVertice]: No memory\n");
      exit(-1);
    }
    //Passe as referencias para as variaveis originais
    digraph->adjacency_list = aux;
    digraph->vertices_values = aux1;
    digraph->arrows_sizes = aux2;
  }
}
/**
 * @brief: O metodo delArrow deleta um arco da lista de adjacencias.
 * @param:  Digraph*, int, int
 * @return: void
 */
void delArrow(Digraph *digraph, int sourceVertice, int targetVertice){
  //Declare as variaveis
  int i, j, source=-1, target=-1;
  //Verifique se o arco (s,t) existe no Digrafo
  source = getVertice(digraph, sourceVertice);
  if(source!=-1){
    for(i=0; i<digraph->arrows_sizes[source]; i++){
      if(digraph->adjacency_list[source][i].target==targetVertice){
        target = i;
        break;
      }
    }
  }
  //Se existir o arco (s,t)
  if(source!=-1 && target!=-1){
    //Declare variaveis auxiliares
    Arrow ** aux;
    int iAux;
    //Aloque memoria para a lista de adjacencias
    aux = (Arrow **)calloc(digraph->vertice_size, sizeof(Arrow *));
    //Verifique a alocacao de memoria
    if(aux==NULL){
      printf("Error [void delArrow]: No memory\n");
      exit(-1);
    }
    //Passe os vertices (menos o arco que deseja deletar para aux)
    iAux = 0;
    for(i=0; i<digraph->vertice_size; i++){
      //Aloque memoria para um vertice da lista de adjacencias
      if(i==source){
        aux[iAux] = (Arrow *)calloc(digraph->arrows_sizes[i]-1, sizeof(Arrow));
      }else{
        aux[iAux] = (Arrow *)calloc(digraph->arrows_sizes[i], sizeof(Arrow));
      }
      //Verifique a alocacao de memoria
      if(aux[iAux]==NULL){
        printf("Error [void delArrow]: No memory\n");
        exit(-1);
      }
      //lista de adjacencias
      for(j=0; j<digraph->arrows_sizes[i]; j++){
        if(!(i==source && j==target)){
          aux[iAux][j].target = digraph->adjacency_list[i][j].target;
          aux[iAux][j].cost = digraph->adjacency_list[i][j].cost;
        }
      }
      //Incremente o iAux
      iAux++;
      //Desaloque um vertice da lista de adjacencias
      free(digraph->adjacency_list[i]);
    }
    //Decremente a quantidade de arcos do digrafo
    digraph->arrow_size--;
    //Decremente a quantidade de arcos do vertice 'source'
    digraph->arrows_sizes[source]--;
    //Desaloque a lista de adjacencias
    free(digraph->adjacency_list);
    //Aloque memoria novamente para a lista de adjacencias
    digraph->adjacency_list = (Arrow **)calloc(digraph->vertice_size, sizeof(Arrow *));
    //Verifique a alocacao de memoria
    if(digraph->adjacency_list==NULL){
      printf("Error [void delArrow]: No memory\n");
      exit(-1);
    }
    //Passe as referencias para a variavel original
    digraph->adjacency_list = aux;
  }
}

/**
 * @brief: O metodo freeUp, desaloca as structs passadas como entrada.
 * @param:  Digraph*, MinimumPath*
 * @return: void
 */
void freeUp(Digraph* digraph, MinimumPath* minimumPath){
  //Declare as variaveis
  int i;
  //Digraph
  for(i=0; i<digraph->vertice_size; i++){
    free(digraph->adjacency_list[i]);
  }
  free(digraph->adjacency_list);
  free(digraph->vertices_values);
  free(digraph->arrows_sizes);
  free(digraph);
  //MinimumPath
  free(minimumPath->d);
  free(minimumPath->previous);
  free(minimumPath);
}
