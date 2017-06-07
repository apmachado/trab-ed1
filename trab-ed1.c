#include <stdio.h>
#include <stdlib.h>

typedef struct viz{
	int id_viz;
	int custo;
	struct viz *prox_viz;
}TViz;

typedef struct no{
	int id_no;
	TViz *prim_viz;
	struct no *prox_no;
}TNo;

typedef struct grafo{
	TNo *prim;
}TG;

TG *cria(){
	TG *g = (TG*)malloc(sizeof(TG));
	g->prim = NULL;
	return g;
}

TNo *busca_no(TG *g, int no){
	TNo *p = g->prim;
	while((p) && (p->id_no != no)){
		p = p->prox_no;
	}
	return p;
}

void insere_no(TG *g, int no){
	TNo *p = busca_no(g, no);
	if(p)
		return;
	p = (TNo*)malloc(sizeof(TNo));
	p->id_no = no;
	p->prim_viz = NULL;
	p->prox_no = g->prim;
	g->prim = p;
}

TViz *busca_aresta(TG *g, int no1, int no2){
	TNo *viz1 = busca_no(g, no1);
	if(!viz1)
		return NULL;
	TNo *viz2 = busca_no(g, no2);
	if(!viz2)
		return NULL;
	TViz *p = viz1->prim_viz;
	while((p) && (p->id_viz != no2)){
		p = p->prox_viz;
	}
	return p;
}

void insere_aresta(TG *g, int no1, int no2, int orientacao){
	TNo *p1 = busca_no(g, no1);
	if(!p1)
		return;
	TNo *p2 = busca_no(g, no2);
	if(!p2)
		return;

	TViz *v = busca_aresta(g, no1, no2);
	if(v){
		return;
	}
	v = (TViz*)malloc(sizeof(TViz));;
	v->id_viz = no2;
	v->prox_viz = p1->prim_viz;
	p1->prim_viz = v;
	if(!orientacao){
		TViz *u = (TViz*)malloc(sizeof(TViz));
		u->id_viz = no1;
		u->prox_viz = p2->prim_viz;
		p2->prim_viz = u;
	}
}

void retira_aresta(TG *g, int no1, int no2, int orientacao){
	TNo *p1 = busca_no(g, no1);
	if(!p1)
		return;
	TNo *p2 = busca_no(g, no2);
	if(!p2)
		return;
	TViz *v = p1->prim_viz, *ant = NULL;
	while(v && (v->id_viz != no2)){
		ant = v;
		v = v->prox_viz;
	}
	if(!v)
		return;
	if(!ant){
		p1->prim_viz = v->prox_viz;
		free(v);
	} else {
		ant->prox_viz = v->prox_viz;
		free(v);
	}
	if(!orientacao){
		v = p2->prim_viz;
		while(v && v->id_viz != no1){
			ant = v;
			v = v->prox_viz;
		}
		if(!v)
			return;
		if(!ant){
			p2->prim_viz = v->prox_viz;
			free(v);
		} else {
			ant->prox_viz = v->prox_viz;
			free(v);
		}
	}
}

void retira_no(TG *g, int no){
	TNo *p = g->prim, *ant = NULL;
	while(p && (p->id_no != no)){
		ant = p;
		p = p->prox_no;
	}
	if(!p)
		return;
	TViz *v = p->prim_viz;
	while(v){
		retira_aresta(g, no, v->id_viz, 1);
		v = v->prox_viz;
	}
	if(!ant){
		g->prim = p->prox_no;
		free(p);
	} else {
		ant->prox_no = p->prox_no;
		free(p);
	}
}

void libera(TG *g){
	TNo *p = g->prim;
	while(p){
		TViz *v = p->prim_viz;
		while(v){
			TViz *u = v;
			v = v->prox_viz;
			free(u);
		}
		TNo *q = p;
		p = p->prox_no;
		free(q); 
	}
}

void imprime(TG *g){
	TNo *p = g->prim;
	while(p){
		printf("%d -> ", p->id_no);
		TViz *v = p->prim_viz;
		while(v){
			printf("%d ", v->id_viz);
			v = v->prox_viz;
		}
		printf("\n");
		p = p->prox_no;
	}
}

int main(int argc, char const *argv[])
{
	FILE *fp = fopen(argv[1], "rt");
	if(!fp)
		exit(1);

	TG *g = cria();
	int n_nos;
	fscanf(fp, "%d", &n_nos);
	printf("Quantidade de vertices: %d\n", n_nos);
	while(!feof(fp)){
		int v1, v2;
		fscanf(fp, "%d %d", &v1, &v2);
		insere_no(g, v1);
		insere_no(g, v2);
		insere_aresta(g, v1, v2, 1);
	}
	imprime(g);
	fclose(fp);
	return 0;
}