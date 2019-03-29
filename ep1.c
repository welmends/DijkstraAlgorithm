#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>

#define PLUS_INFINITE (1/0.0)
#define DWORD int

typedef struct DigraphArrow{
  DWORD target;
  DWORD cost;
}Arrow;

typedef struct DirectedGraph{
  Arrow **connectedList;
  DWORD *vertices;
  DWORD *arrowsSizes;
  DWORD verticeSize;
  DWORD arrowSize;
}Digraph;

typedef struct DijkstraMinimumPath{
  DWORD *d;
  DWORD *previous;
  DWORD size;
}MinimumPath;

int i, j, z;

MinimumPath* dijkstraAlgorithm(Digraph *digraph, DWORD source, DWORD target);

void initMinimumPath(MinimumPath **digraph, DWORD verticeSize);
void displayMinimumPath(MinimumPath *minimumPath);
bool isInBorder(DWORD *Z, DWORD sizeZ, DWORD vertice);

void initDigraph(Digraph **digraph);
void displayDigraph(Digraph *digraph);
DWORD getVertice(Digraph *digraph, DWORD verticeName);
DWORD addVertice(Digraph *digraph, DWORD newVertice);
DWORD addArrow(Digraph *digraph, DWORD sourceVertice, DWORD targetVertice, DWORD cost);
DWORD delVertice(Digraph *digraph, DWORD currVertice);
DWORD delArrow(Digraph *digraph, DWORD sourceVertice, DWORD targetVertice);

int main(){
  Digraph *digraph;
  initDigraph(&digraph);

  addArrow(digraph, 1, 2, 5);
  addArrow(digraph, 1, 5, 10);
  addArrow(digraph, 2, 3, 4);
  addArrow(digraph, 2, 4, 13);
  addArrow(digraph, 2, 5, 2);
  addArrow(digraph, 3, 4, 7);
  addArrow(digraph, 5, 4, 6);

  displayDigraph(digraph);

  //displayMinimumPath(dijkstraAlgorithm(digraph, 1, 4));
  return 0;
}

MinimumPath* dijkstraAlgorithm(Digraph *digraph, DWORD source, DWORD target){
  //Declare ponteiro da struct DijkstraMinimumPath
  MinimumPath *minimumPath;
  initMinimumPath(&minimumPath, digraph->verticeSize);//init
  //Declare e aloque um vetor Z (fronteira)
  DWORD *Z;
  DWORD sizeZ = 1;
  Z = (DWORD *)calloc(sizeZ, sizeof(DWORD));
  DWORD v;

  //Percorra zerando previous e setando d para PLUS_INFINITE
  for(i=0; i<minimumPath->size; i++){
    minimumPath->d[i] = PLUS_INFINITE;
    minimumPath->previous[i] = 0;
  }
  Z[0] = source;//Coloque source na fronteira
  minimumPath->d[0] = 0;//Coloque d[0] = 0

  //Verifique se existe arco na fronteira
  while(true){
    for(i=0; i<sizeZ; i++){
      for(j=0; j<digraph->arrowsSizes[getVertice(digraph, Z[i])]; j++){
        v = digraph->connectedList[getVertice(digraph, Z[i])][j].target;
        if(isInBorder(Z, sizeZ, v)){
          //minimumPath->d[v]+digraph->connectedList[getVertice(digraph, Z[i])][j].cost;//Continue daqui
        }
      }
    }
  }

  return minimumPath;
}

void initMinimumPath(MinimumPath **minimumPath, DWORD verticeSize){
  //Aloque o Caminho Minimo
  *minimumPath  = (MinimumPath *)calloc(1, sizeof(Digraph));
  //Inicialize o Caminho Minimo
  **minimumPath = (MinimumPath){NULL, NULL, verticeSize};
  //Aloque memoria para os vetores
  (*minimumPath)->d = (DWORD *)calloc((*minimumPath)->size, sizeof(DWORD));
  (*minimumPath)->previous = (DWORD *)calloc((*minimumPath)->size, sizeof(DWORD));
}
void displayMinimumPath(MinimumPath *minimumPath){
  //Implement...
  for(i=0; i<minimumPath->size; i++){
    if(i!=minimumPath->size-1){
      printf("[%d]-", minimumPath->d[i]);
    }else{
      printf("[%d]\n", minimumPath->d[i]);
    }
  }
}
bool isInBorder(DWORD *Z, DWORD sizeZ, DWORD vertice){
  for(z=0; z<sizeZ; z++){
    if(Z[z]==vertice){
      return false;
    }
  }
  return true;
}

void initDigraph(Digraph **digraph){
  //Aloque o Digrafo
  *digraph  = (Digraph *)calloc(1, sizeof(Digraph));
  //Inicialize o Digrafo
  **digraph = (Digraph){NULL, NULL, NULL, 0, 0};
}
void displayDigraph(Digraph *digraph){
  //Mostre o digrafo no modelo proposto
  printf("%d %d\n", digraph->verticeSize, digraph->arrowSize);
  for(i=0; i<digraph->verticeSize; i++){
    for(j=0; j<digraph->arrowsSizes[i]; j++){
      printf("%d %d %d\n",digraph->vertices[i], digraph->connectedList[i][j].target, digraph->connectedList[i][j].cost);
    }
  }
}
DWORD getVertice(Digraph *digraph, DWORD verticeName){
  //Verifique se ja existe esse vertice
  for(i=0; i<digraph->verticeSize; i++){
    if(digraph->vertices[i]==verticeName){
      return i;//Se sim, retorne seu index
    }
  }
  return -1;//Se nao, retorne -1
}
DWORD addVertice(Digraph *digraph, DWORD newVertice){
  if(digraph->verticeSize==0){
    //Se nao houver vertices adicione o primeiro
    //Aloque memoria para a lista conectada
    digraph->connectedList = (Arrow **)calloc(1,sizeof(Arrow *));
    if(digraph->connectedList!=NULL){
      //Aloque memoria para o vetor de vertices (nome do vertice)
      digraph->vertices = (DWORD *)calloc(1,sizeof(DWORD));
      //Aloque memoria para o vetor de tamanhos de arcos (das listas conectadas)
      digraph->arrowsSizes = (DWORD *)calloc(1,sizeof(DWORD));
      if(digraph->vertices!=NULL && digraph->arrowsSizes!=NULL){
        //Adicione o vertice
        digraph->vertices[digraph->verticeSize] = newVertice;
        //Coloque a quantidade de arcos daquele vertice para 0
        digraph->arrowsSizes[digraph->verticeSize] = 0;
        //Incremente o tamanho de vertices do digrafo
        digraph->verticeSize = 1;
      }else{
        return 0;//Saia da funcao sem conseguir adicionar o vertice
      }
      return 1;//Saia da funcao
    }else{
      return 0;//Saia da funcao sem conseguir adicionar o vertice
    }
  }else{
    //Verifique se ja existe esse vertice
    if(getVertice(digraph, newVertice)!=-1){
      return 0;//Se sim, saia da funcao sem conseguir adicionar o vertice
    }

    //Declare variaveis auxiliares para a realocacao de memoria para adicionar um novo vertice
    Arrow ** aux;
    DWORD * aux1, * aux2;
    DWORD iAux;
    bool hasBeenAdded = false;
    //Aloque memoria para a lista conectada e os vertores de vertice e tamanhos de arco
    aux = (Arrow **)calloc(digraph->verticeSize+1, sizeof(Arrow *));
    aux1 = (DWORD *)calloc(digraph->verticeSize+1, sizeof(DWORD));
    aux2 = (DWORD *)calloc(digraph->verticeSize+1, sizeof(DWORD));
    if(aux!=NULL && aux1!=NULL && aux2!=NULL){
      //Passe os vertices
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
          //Lista conectada
          aux[i] = (Arrow *)calloc(digraph->arrowsSizes[iAux], sizeof(Arrow));
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
      digraph->verticeSize++;//Incremente a quantidade de vertices

      free(digraph->connectedList);//Desaloque a lista conectada
      free(digraph->vertices);//Desaloque o vetor de vertices
      free(digraph->arrowsSizes);//Desaloque o vetor de tamanho de arcos

      digraph->connectedList = (Arrow **)calloc(digraph->verticeSize, sizeof(Arrow *));//Aloque memoria novamente para a lista conectada
      digraph->vertices = (DWORD *)calloc(digraph->verticeSize, sizeof(DWORD));//Aloque memoria novamente para o vetor de vertices
      digraph->arrowsSizes = (DWORD *)calloc(digraph->verticeSize, sizeof(DWORD));//Aloque memoria novamente para o vetor de tamanhos de arcos

      if(digraph->connectedList!=NULL && digraph->vertices!=NULL && digraph->arrowsSizes!=NULL){
        //Passe as referencias para as variaveis originais
        digraph->connectedList = aux;
        digraph->vertices = aux1;
        digraph->arrowsSizes = aux2;
      }else{
        return 0;//Saia da funcao sem conseguir deletar o vertice
      }
    }else{
      return 0;//Saia da funcao sem conseguir deletar o vertice
    }
    return 1;//Saia da funcao
  }
}
DWORD addArrow(Digraph *digraph, DWORD sourceVertice, DWORD targetVertice, DWORD cost){
  //Declare uma variavel para indice do source e target na lista conectada
  DWORD source=-1, target=-1;
  addVertice(digraph, sourceVertice);
  addVertice(digraph, targetVertice);

  //Verifique se o arco (s,t) existe no Digrafo
  source = getVertice(digraph, sourceVertice);
  target = getVertice(digraph, targetVertice);

  for(i=0; i<digraph->arrowsSizes[source]; i++){
    if(digraph->connectedList[source][i].target==targetVertice){
      target = i;
      digraph->connectedList[source][target].cost = cost;
      return 1;//Saia da funcao atualizando o custo do arco ja existente
    }
  }

  //Adicione o Arco
  if(digraph->arrowsSizes[source]==0){
    //Se nao houver arcos neste vertice adicione o primeiro
    //Aloque memoria para um vertice da lista conectada
    digraph->connectedList[source] = (Arrow *)calloc(1,sizeof(Arrow));
    if(digraph->connectedList[source]!=NULL){
      //Adicione o arco
      digraph->connectedList[source][0].target = targetVertice;
      digraph->connectedList[source][0].cost = cost;
      //Coloque a quantidade de arcos daquele vertice para 1
      digraph->arrowsSizes[source] = 1;
      //Incremente o tamanho de arcos do digrafo
      digraph->arrowSize++;
    }else{
      return 0;//Saia da funcao sem conseguir adicionar o arco
    }
  }else{
    //Declare variaveis auxiliares para a realocacao de memoria para adicionar um novo arco
    Arrow * aux;

    //Realoque memoria para uma lista conectada
    aux = (Arrow *)realloc(digraph->connectedList[source], (digraph->arrowsSizes[source]+1)*sizeof(Arrow));
    if(aux!=NULL){
      digraph->connectedList[source] = aux;//Passe a referencia para a variavel original
      digraph->connectedList[source][digraph->arrowsSizes[source]].target = targetVertice;//Adicione o target
      digraph->connectedList[source][digraph->arrowsSizes[source]].cost = cost;//Adicione o custo
      digraph->arrowsSizes[source]++;//Incremente a quantidade de arcos do vertice source
      digraph->arrowSize++;//Incremente a quantidade de arcos do Digrafo
    }else{
      return 0;//Saia da funcao sem conseguir adicionar o arco
    }
  }
  return 1;//Saia da funcao
}
DWORD delVertice(Digraph *digraph, DWORD currVertice){
  //Verifique se ja existe esse vertice
  DWORD v = getVertice(digraph, currVertice);
  if(v==-1){
    return 0;//Saia da funcao sem conseguir deletar o vertice
  }else{
    //Declare variaveis auxiliares para a realocacao de memoria para deletar um vertice
    Arrow ** aux;
    DWORD * aux1, * aux2;
    DWORD iAux;

    //Aloque memoria para a lista conectada e os vertores de vertice e tamanhos de arco
    aux = (Arrow **)calloc(digraph->verticeSize-1, sizeof(Arrow *));
    aux1 = (DWORD *)calloc(digraph->verticeSize-1, sizeof(DWORD));
    aux2 = (DWORD *)calloc(digraph->verticeSize-1, sizeof(DWORD));
    if(aux!=NULL && aux1!=NULL && aux2!=NULL){
      //Passe os vertices (menos o que deseja deletar para aux)
      iAux = 0;
      for(i=0; i<digraph->verticeSize; i++){
        if(i!=v){
          //Lista conectada
          aux[iAux] = (Arrow *)calloc(digraph->arrowsSizes[i], sizeof(Arrow));
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
        free(digraph->connectedList[i]);//Desaloque um vertice da lista conectada
      }
      digraph->verticeSize--;//Decremente a quantidade de vertices
      digraph->arrowSize -= digraph->arrowsSizes[v];//Subtraia a quantidade de arcos do digrafo

      free(digraph->connectedList);//Desaloque a lista conectada
      free(digraph->vertices);//Desaloque o vetor de vertices
      free(digraph->arrowsSizes);//Desaloque o vetor de tamanho de arcos

      digraph->connectedList = (Arrow **)calloc(digraph->verticeSize, sizeof(Arrow *));//Aloque memoria novamente para a lista conectada
      digraph->vertices = (DWORD *)calloc(digraph->verticeSize, sizeof(DWORD));//Aloque memoria novamente para a lista conectada
      digraph->arrowsSizes = (DWORD *)calloc(digraph->verticeSize, sizeof(DWORD));//Aloque memoria novamente para a lista conectada

      if(digraph->connectedList!=NULL && digraph->vertices!=NULL && digraph->arrowsSizes!=NULL){
        //Passe as referencias para as variaveis originais
        digraph->connectedList = aux;
        digraph->vertices = aux1;
        digraph->arrowsSizes = aux2;
      }else{
        return 0;//Saia da funcao sem conseguir deletar o vertice
      }
    }else{
      return 0;//Saia da funcao sem conseguir deletar o vertice
    }
  }
  return 1;//Saia da funcao
}
DWORD delArrow(Digraph *digraph, DWORD sourceVertice, DWORD targetVertice){
  //Declare uma variavel para indice do source e target na lista conectada
  DWORD source=-1, target=-1;

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
    DWORD iAux;

    //Aloque memoria para a lista conectada
    aux = (Arrow **)calloc(digraph->verticeSize, sizeof(Arrow *));
    if(aux!=NULL){
      //Passe os vertices (menos o arco que deseja deletar para aux)
      iAux = 0;
      for(i=0; i<digraph->verticeSize; i++){
        //Lista conectada
        if(i==source){
          aux[iAux] = (Arrow *)calloc(digraph->arrowsSizes[i]-1, sizeof(Arrow));
        }else{
          aux[iAux] = (Arrow *)calloc(digraph->arrowsSizes[i], sizeof(Arrow));
        }
        for(j=0; j<digraph->arrowsSizes[i]; j++){
          if(!(i==source && j==target)){
            aux[iAux][j].target = digraph->connectedList[i][j].target;
            aux[iAux][j].cost = digraph->connectedList[i][j].cost;
          }
        }
        //Incremente o iAux
        iAux++;
        free(digraph->connectedList[i]);//Desaloque um vertice da lista conectada
      }
      digraph->arrowSize--;//Decremente a quantidade de arcos do digrafo
      digraph->arrowsSizes[source]--;//Decremente a quantidade de arcos do vertice 'source'

      free(digraph->connectedList);//Desaloque a lista conectada

      digraph->connectedList = (Arrow **)calloc(digraph->verticeSize, sizeof(Arrow *));//Aloque memoria novamente para a lista conectada
      if(digraph->connectedList!=NULL){
        //Passe as referencias para a variavel original
        digraph->connectedList = aux;
      }else{
        return 0;//Saia da funcao sem conseguir deletar o vertice
      }
    }else{
      return 0;//Saia da funcao sem conseguir deletar o vertice
    }
  }else{
    return 0;//Saia da funcao sem conseguir deletar o vertice
  }
  return 1;//Saia da funcao
}
