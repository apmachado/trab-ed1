#include <stdio.h>
#include <stdlib.h>

//Por Alexandre Pedroza Machado, André Luiz Lima Prata Rodrigues e Caio Nogueira Vivas

typedef struct viz{
    int id_viz;
    int marcador;
    struct viz *prox_viz;
}TViz;

typedef struct no{
    int id_no;
    int marcador;
    TViz *prim_viz;
    struct no *prox_no;
}TNo;

typedef struct grafo{
    TNo *prim;
    int conexo;
}TG;

int conta_nos(TG *g){
    if(!g) return 0;
    int n = 0;
    TNo *no = g->prim;

    while(no){
        n++;
        no = no->prox_no;
    }
    return n;
}

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
    p->marcador = 0;
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
    v = (TViz*)malloc(sizeof(TViz));
    v->id_viz = no2;
    v->prox_viz = p1->prim_viz;
    v->marcador = 0;
    p1->prim_viz = v;
    if(!orientacao){ //não sendo orientado, insere no sentido inverso também
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
    if(!busca_aresta(g,no1,no2)) return;
    TViz *v = p1->prim_viz, *ant = NULL;
    while(v && (v->id_viz != no2)){
        ant = v;
        v = v->prox_viz;
    }
    if(!v)
        return;
    if(!ant)
        p1->prim_viz = v->prox_viz;
	else 
        ant->prox_viz = v->prox_viz;
	free(v);
	
    if(!orientacao){ //não sendo orientado, retira no sentido inverso também

        v = p2->prim_viz;
		ant = NULL;
        while(v && (v->id_viz != no1)){
            ant = v;
            v = v->prox_viz;
        }
        if(!v)
            return;
        if(!ant){
            p2->prim_viz = v->prox_viz;
            
        } else {
            ant->prox_viz = v->prox_viz;
            
        }
		free(v);
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
	TNo *base = g->prim;
    while(base){
        if(busca_aresta(g, base->id_no, no)) retira_aresta(g, base->id_no, no, 0);
		base = base->prox_no;
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
    if (!g) return;
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
    free(g);
}



int orientado(TG *g){
    TNo *p = g->prim;
    while(p){
        TViz *v = p->prim_viz;
        while(v){
            TViz *u = busca_aresta(g, v->id_viz, p->id_no);
            if(!u){
                return 1;
            }
            v = v->prox_viz;
        } 
        p = p->prox_no;
    }
    return 0;
}

int existe_caminho(TG* g, int id_a, int id_b){
    if(!g) return 0;
    TNo *no_a = busca_no(g, id_a);
    TNo *no_b = busca_no(g, id_b);
    if(!no_a || !no_b ||!no_a->prim_viz) return 0;
    
    int existe = 0;
    TViz *v = no_a->prim_viz;
    
    while(v && !existe){
        if(v->id_viz == id_b)
            return 1;
        if(!v->marcador){
            v->marcador = 1;
            existe = existe_caminho(g, v->id_viz,id_b);
            v->marcador = 0;
        }
        v=v->prox_viz;
    }
    return existe;
}

void fortemente_conexos(TG *g){
    TNo *p = g->prim;
    int existeComOutro;
    while(p){
        TNo *q = g->prim;
        existeComOutro = 0;
		while(q){
            if(existe_caminho(g, p->id_no, q->id_no) && existe_caminho(g, q->id_no, p->id_no)){
                printf("%d e %d\n", p->id_no, q->id_no);
                existeComOutro = 1;
           	}
            q = q->prox_no;
        }
        if(!existeComOutro) printf("%d (Sozinho)\n", p->id_no);
        p = p->prox_no;
    }
}

TNo *proximoNoNaoMarcado(TG *g){
	if(!g) return NULL;
	
	TNo *p = g->prim;
	
	while(p && p->marcador) p = p->prox_no;
	
	return p;
}

void grupos_conexos_busca(TG *g, TNo *no, int qtd_nos){
	if(!no) return;
	no->marcador = 1;
	printf("%d ", no->id_no);
	if(!no->prim_viz){
		printf("\n");
		grupos_conexos_busca(g, proximoNoNaoMarcado(g), qtd_nos);
	} else {
		TNo *base = g->prim;
		while(base){
			if(base->id_no != no->id_no && !base->marcador && existe_caminho(g, base->id_no, no->id_no)){
				base->marcador = 1;
				printf("%d ", base->id_no);
			}
			base = base->prox_no;
		}
		printf("\n");
		grupos_conexos_busca(g, proximoNoNaoMarcado(g), qtd_nos);
	}
}

void grupos_conexos(TG *g, int qtd_nos){
    TNo *p = g->prim;
    while(p){
        p->marcador = 0;
        p = p->prox_no;
    }
    p = g->prim;

    grupos_conexos_busca(g, p, qtd_nos);
}

int tudoMarcado(TG *g){
    if(!g) return 1;
    TNo *no = g->prim;
    while(no){
        if(!no->marcador) return 0;
        no = no->prox_no;
    }
    return 1;
}

int busca_prof(TG *g, TNo *no, int qtd_nos){
    if(!g || qtd_nos == 1 || !no->prim_viz){
        g->conexo = 0;
        return 0;
    }
	TNo *base = g->prim;
    while(no){
		while(base){
			if(no->id_no != base->id_no) if(!existe_caminho(g, no->id_no, base->id_no)) return 0;
			base = base->prox_no;
		}
		no = no->prox_no;
    }
	return 1;
}

int conexo(TG *g, int qtd_nos){
    TNo *p = g->prim;
    if(!p)
        return 0;
    while(p){
        p->marcador = 0;
        p = p->prox_no;
    }
    p = g->prim;
    while(p){
        p = p->prox_no;
    }
    p = g->prim;
	
    
    return busca_prof(g, p, qtd_nos);
    
}

TG *copia(TG *g){
	
    if(!g)
        return NULL;
    TNo *p = g->prim;
    TG *h = cria();
    while(p){
        insere_no(h, p->id_no);
        p = p->prox_no;
    }
    p = g->prim;
    while(p){
    	TViz *v = p->prim_viz;
        while(v){
            insere_aresta(h, p->id_no, v->id_viz,1);
            v = v->prox_viz;
        }
        p = p->prox_no;
    }
    return h;
}

void p_a(TG *g, int qtd_nos){
    if(!g)
        return;
    TNo *p = g->prim;
	printf("\n------------\nARTICULAÇÕES\n------------\n");
    while(p){
        TG *h = copia(g);
        retira_no(h, p->id_no);
        if(!conexo(h, conta_nos(h))){
            printf("Articulação: %d\n", p-> id_no);
        }

        libera(h);
        p = p->prox_no;
    }
    p = g->prim;
	printf("\n------\nPONTES\n------\n");
    while(p){
        TViz *v = p->prim_viz;
        while(v){
            TG *h = copia(g);

            retira_aresta(h, p->id_no, v->id_viz, 0);
            if(!conexo(h, conta_nos(h))){
                printf("Ponte: %d - %d\n", p->id_no, v->id_viz);
            }
            libera(h);
            v = v->prox_viz;
        }
        p = p->prox_no;
    }
}

void imprime(TG *g, int n_nos){
    TNo *p = g->prim;
	printf("\n\n-----\nGRAFO\n-----\n\n");
    while(p){
        printf("%d -> ", p->id_no);
        TViz *v = p->prim_viz;
        while(v){
            printf("%d ", v->id_viz); // vizinhos
            v = v->prox_viz;
        }
        printf("\n");
        p = p->prox_no;
    }
    
    printf("\n\n\nInformações adicionais:\n\n");
    
    if(orientado(g)){
        printf("---> ORIENTADO <---\n");
        printf("\n------------------------------\nCOMPONENTES FORTEMENTE CONEXAS\n------------------------------\n");
        fortemente_conexos(g);
		printf("\n---------\nAGRUPADAS\n---------\n");
		grupos_conexos(g, n_nos);
    }
    
    else{
        printf("---> NÃO-ORIENTADO E ");
        if(conexo(g, n_nos)){
            printf("CONEXO <---\n");
            p_a(g, n_nos);
        }
        else{
            printf("DESCONEXO <---\n");
            printf("\n-------------------\nCOMPONENTES CONEXAS\n-------------------\n");
            grupos_conexos(g, n_nos);
            
        }
    }
}


void menu(TG *g, int n_nos){
    int dentro = 1, opt, n, m;
    while(dentro){
        printf("1 - Inserir nó\n");
        printf("2 - Remover nó\n");
        printf("3 - Inserir aresta\n");
        printf("4 - Remover aresta\n");
        printf("5 - Imprime\n");
		printf("6 - Sair\n");
        printf("\nOpção: ");
        scanf("%d", &opt);
        
        switch(opt){
            case 1:
                printf("Valor do nó a ser inserido: ");
                scanf("%d", &n);
                TNo *aux = busca_no(g, n);
                if(aux)
                    printf("Nó já existente\n");
                else{
                    insere_no(g, n);
                    n_nos++;
                }
                
                break;
            case 2:
                printf("Valor do nó a ser removido: ");
                scanf("%d", &n);
                TNo *aux2 = busca_no(g, n);
                if(!aux2)
                    printf("Nó não se encontra no grafo\n");
                else{
                    retira_no(g, n);
                    n_nos--;
                }
                break;
            case 3:
                printf("Aresta saindo do nó: ");
                scanf("%d", &n);
                printf("Indo até o nó: ");
                scanf("%d", &m);
                insere_aresta(g, n, m, 1);
                break;
            case 4:
                printf("Aresta saindo do nó: ");
                scanf("%d", &n);
                printf("Indo até o nó: ");
                scanf("%d", &m);
                retira_aresta(g, n, m, 1);
                break;
            case 5:
                imprime(g, n_nos);
                break;
	        case 6:
				printf("\n\n-------------\nVOLTE SEMPRE!\n-------------\n\n");
	            dentro = 0;
	            break;
        }
		printf("\n\n\n---------------------\n\n\n");
    } 
    
}

int main(int argc, char const *argv[]){
    FILE *fp = fopen(argv[1], "rt");
    if(!fp)
        exit(1);
    
    TG *g = cria();
    int n_nos;
    fscanf(fp, "%d", &n_nos);
    printf("\n\n\n\n----------\nBEM-VIND@!\n----------\n\n\n");
    for (int i = 1; i <= n_nos; ++i){
        insere_no(g, i);
    }
    while(!feof(fp)){
        int v1, v2;
        fscanf(fp, "%d %d", &v1, &v2);
        insere_aresta(g, v1, v2, 1);
    }
    menu(g, n_nos);
    
    libera(g);
    fclose(fp);
    return 0;
}