/*
	In fisierul data.in va fi introdus un numar N, reprezentand numarul de persoane din lista initiala
	Pe urmatoarele N randuri se vor scrie liste de persoane. Primul numar scris pe linia i + 1 va reprezenta
   numarul de persoane pe care persoana i le cunoaste, denumir NR. Dupa acest numar vor exista NR perechi de numere
   reprezentand numarul persoanei pe care o cunoaste si gradul de apropriere sub forma NR_per grad apropriere. Gradul
   de apropriere este un numar intre (1-8), unde 1 reprezinta cea mai apropriata relatie.
	Fiecare persoana din lista va avea 2 atribute care se pot modifica cu timpul, dar care la inceput vor fi setate cu valori
   implicite:
	I. Gradul de suspiciune, de la 1 la 10, 10 semnificand gradul maxim de suspiciune. Acesta se modifica in cazul in care
	persoana savarseste o infractiune ori cand are o relatie apropriata cu cineva care este infractor. La inceput este 1.
	II. Probabilitatea de infectie, un numar real >= 0. Probabilitatea de infectie creste atunci cand este testat pozitiv
	sau cand este in legatura cu o persoana care a fost infectata. Cu cat persoana infectata este mai indepartata, cu atat
	mai putin creste aceasta probabilitate. Probabilitatea este la inceput 0.
	Exista 9 operatii posibile:
		1) Adaugare persoana infectata. Comanda scrisa de la tastatura va fi I nr, reprezentand ca persoana cu nu-
		marul nr a fost testata pozitiv
		2) Adaugare infractor. Comanda scrisa de la tastatura va fi C nr, reprezentand ca persoana cu numarul
		nr a savarsit o infractiune.
		3) Afisare lista infractori. Comanda scrisa de la tastatura va fi LI. Pe ecran vor fi afisati toti infractorii
		si toti oamenii care au gradul de suspiciuni >= 10
		4) Afisare lista infectati. Comanda scrisa de la tastatura va fi LC. Pe ecran vor fi afisati toate persoanele infectate,
		sau care sunt aproape sigur infectate(probabilitate infectie >= 1)
		5) Modificare grad suspiciune. Comanda scrisa de la tastatura va fi MC NR_PER NR_SUS. Astfel persoana cu numarul
		NR_PER va avea gradul de suspiciune modificat la NR_SUS. Daca NR_SUS este 1 atunci si variabila infractor devine 0
		6) Modificare prob infectie. Comanda scrisa de la tastatura va fi MI NR_PER PROB. Astfel persoana cu numarul
		NR_PER va avea probabilitatea de infectie modificata la PROB. Daca PROB este 0 atunci variabila infectat devine si ea 0.
		7) Adaugare persoana noua. Comanda scrisa de la tastatura va fi A NR_CUN. Dupa NR_CUN vor exista NR_CUN perechi de numere
		reprezentand legaturile pe care le are acesta cu alte persoane sub forma NR-GRAD DE APROPRIERE.
		8) Afisare lista persoane. Comanda scrisa de la tastatura va fi LS. Se va afisa pe ecran lista persoanelor, iar pentru
		fiecare persoana se va afisa gradul de suspiciune si probabilitatea de infectie.
		9) Incheierea programului. Comanda scrisa de la tastatura va fi E. Executia programul se va incheia in acel moment, iar lista de
		persoane va disparea, pana la o noua rulare.
		OBS: daca persoana i o cunoaste pe persoana j, atunci si persoana j o cunoaste pe i, iar gradul de apropriere este egal. Daca exista
		legatura i-j, dar j-i nu exista sau are alta valoare de apropriere, atunci legatura j-i va fi adaugata cu aceeasi valoare de apropriere
		ca si i-j
*/


#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define nmax 100002

typedef struct pers {
	float probabilitate_infectie; //probabilitatea ca acesta sa fie infectat.
	int infectat; //Variabila care este 1 in cazul in care persoana este infectata si 0 in caz contrat
	int grad_suspiciune; //gradul de suspiciune al persoanei
	int infractor; //variabila este 1 daca persoana este infractor, 0 in caz contrar
}persoan; //structura care reprezinta atributele unui om din lista

typedef struct list_pers {
	int numar_persoana; //numarul persoanei cu care este cunostinta
	int grad_apropriere; //gradul de apropriere dintre cele doua persoane
	struct list_pers *next; //urmatoarea persoana din lista;
}Persoanac;//structura pentru lista de persoane

persoan persoana[nmax];//lista persoanelor

int numar_persoane, numar_cunostinte, nr_persoana, grad_apropriere; //variabile sugestive pentru citire.
int grad_sus, numar_pers, proba, numar_infractor, persoana_infectata;
Persoanac *Lista_persoane[nmax];//lista de cunostinte ale fiecarei persoane

char comanda[25]; //comanda primita de la tastatura

void adauga_infectat(int persoana_x)
{//voi parcurge listele persoanelor folosindu-ma de un algoritm asemanator BFS
	persoana[persoana_x].infectat = 1;//persoana_x este infectata
	persoana[persoana_x].probabilitate_infectie = 1;
	int coada[nmax];
	int vizitat[nmax];
	for (int i = 1; i <= numar_persoane; i++)
	{
		vizitat[i] = 0;//initializare vector de vizitati
	}
	vizitat[persoana_x] = 1;
	int contor_start = 1, contor_final = 1;
	coada[1] = persoana_x;
	while (contor_final >= contor_start)
	{
		int persoana_curenta = coada[contor_start++];
		if (Lista_persoane[persoana_curenta]->next != NULL)//persoana curenta are cel putin o cunostinta
		{
			Persoanac *p = Lista_persoane[persoana_curenta]->next;
			while (p != NULL)
			{
				if (vizitat[p->numar_persoana] == 0)
				{
					contor_final++;
					coada[contor_final] = p->numar_persoana;
					vizitat[p->numar_persoana] = 1;
					persoana[p->numar_persoana].probabilitate_infectie +=
						persoana[persoana_curenta].probabilitate_infectie * (float)1 / (p->grad_apropriere);//aceasta persoana
						//are probablitatea de infectie mai ridicata.
				}
				p = p->next;
			}
		}
	}
}

void adauga_infractor(int persoana_x)
{//voi parcurge listele persoanelor folosindu-ma de un algoritm asemanator BFS
	persoana[persoana_x].infractor = 1;//persoana_x este infractor
	persoana[persoana_x].grad_suspiciune = 10;
	int coada[nmax];
	int nivel[nmax];
	for (int i = 1; i <= numar_persoane; i++)
	{
		nivel[i] = 0;//initializare vector de nivel, asemanator cu cel pentru vizitati.
	}
	nivel[persoana_x] = 1;
	int contor_start = 1, contor_final = 1;
	coada[1] = persoana_x;
	while (contor_final >= contor_start)
	{
		int persoana_curenta = coada[contor_start++];
		if (Lista_persoane[persoana_curenta]->next != NULL)//persoana curenta are cel putin o cunostinta
		{
			Persoanac *p = Lista_persoane[persoana_curenta]->next;
			while (p != NULL)
			{
				if (nivel[p->numar_persoana] == 0)
				{
					nivel[p->numar_persoana] = nivel[persoana_curenta] + 1;
					if (nivel[p->numar_persoana] != 3)
					{
						contor_final++;
						coada[contor_final] = p->numar_persoana;
						if (nivel[p->numar_persoana] == 2)//daca este apropriat de infractor
						{
							persoana[p->numar_persoana].grad_suspiciune +=
								(8 - p->grad_apropriere + 1);//numarul cu care aceasta persoana are gradul de suspiciune incrementat
						}
					}
					else//daca este in legatura indirecta cu infractorul(prin intermediul altei persoane)
					{
						int modificare = 1;//pentru aceasta se aduna maximul dintre 1, sau 5 - gradul de apropriere.
						if (5 - p->grad_apropriere > modificare)
							modificare = 6 - grad_apropriere;
						persoana[p->numar_persoana].grad_suspiciune +=
							modificare;
					}
				}
				p = p->next;
			}
		}
	}
}

int cauta(Persoanac *List_persoane1, int persoana)
{
	if (List_persoane1->next == NULL)
	{
		return -1;//Nu am nici o persoana in lista
	}
	else
	{
		Persoanac *p = List_persoane1;//caut in lista de persoane
		while (p != NULL)//parcurg lista
		{
			if (p->numar_persoana == persoana)//daca persoana a fost deja introdusa
				return 1;
			p = p->next;//trec la urmatoarea persoana
		}
	}
	return -1;
}

void adaugare_legatura(int persoana_1, int persoana_2, int grad_apropriere)//se adauga in lista de cunostinta legatura dintre
{//persoana_1 si persoana_2 cu gradul de apropriere grad_apropriere
	if (cauta(Lista_persoane[persoana_1], persoana_2) == -1)//daca nu am adaugat deja legatura aceasta
	{
		Persoanac *p = (Persoanac*)malloc(sizeof(Persoanac));//aloc memorie pentru noua legatura
		p->next = NULL;
		p->grad_apropriere = grad_apropriere;//gradul de apropriere dintre cele doua persoane
		p->numar_persoana = persoana_2;
		if (Lista_persoane[persoana_1]->next == NULL)//lista persoanei persoana_1 este goala
		{
			Lista_persoane[persoana_1]->next = p;
		}
		else
		{
			p->next = Lista_persoane[persoana_1]->next;//pun noua persoana in fata
			Lista_persoane[persoana_1]->next = p;//noul meu inceput de lista este p acum
		}
	}
	if (cauta(Lista_persoane[persoana_2], persoana_1) == -1)//daca nu am adaugat deja aceasta legatura
	{
		Persoanac *p = (Persoanac*)malloc(sizeof(Persoanac));//aloc memorie pentru noua legatura
		p->next = 0;
		p->grad_apropriere = grad_apropriere;//gradul de apropriere dintre cele doua persoane
		p->numar_persoana = persoana_1;
		if (Lista_persoane[persoana_2]->next == NULL)//lista persoanei persoana_1 este goala
		{
			Lista_persoane[persoana_2]->next = p;
		}
		else
		{
			p->next = Lista_persoane[persoana_1]->next;//pun noua persoana in fata
			Lista_persoane[persoana_1]->next = p;//noul meu inceput de lista este p acum
		}
	}
}

void afisare_lista_persoane(int numar_persoane)
{
	printf("Lista persoanelor este:\n\n");
	for (int i = 1; i <= numar_persoane; i++)
	{
		printf("Persoana numarul %d:\n", i);
		printf("   1)Probabilitate infectie:%.2f%\n", persoana[i].probabilitate_infectie);
		printf("   2)Grad suspiciune:%d\n\n", persoana[i].grad_suspiciune);
	}
}

void afisare_lista_infractori(int numar_persoane)
{
	int contor = 0;//numar cate persoane sunt infractori.
	printf("Lista infractorilor este:\n");
	for (int i = 1; i <= numar_persoane; i++)
	{
		if (persoana[i].grad_suspiciune >= 10)
		{
			printf("Persoana numarul %d:", i);
			if (persoana[i].infractor == 1)
			{
				printf("Infractor!");
			}
			else
			{
				printf("Suspect de infractiune!");
			}
			printf("\n");
			contor++;
		}
	}
	if (contor == 0)
	{
		printf("GOALA!\n");
	}
}

void afisare_lista_infectati(int numar_persoane)
{
	int contor = 0;//numar cate persoane sunt infractori.
	printf("Lista celor infectati este:\n");
	for (int i = 1; i <= numar_persoane; i++)
	{
		if (persoana[i].probabilitate_infectie >= 1)
		{
			printf("Persoana numarul %d:", i);
			if (persoana[i].infectat == 1)
			{
				printf("Infectat!");
			}
			else
			{
				printf("Suspect de infectie!");
			}
			printf("\n");
			contor++;
		}
	}
	if (contor == 0)
	{
		printf("GOALA!\n");
	}
}

void modificare_infectat(int numar_persoana, float modificare)
{
	persoana[numar_persoana].probabilitate_infectie = modificare;
	if (modificare == 0)
	{
		persoana[numar_persoana].infectat = 0;
	}
}

void modificare_infractor(int numar_persoana, int modificare)
{
	persoana[numar_persoana].grad_suspiciune = modificare;
	if (modificare == 1)
	{
		persoana[numar_persoana].infractor = 0;
	}
}
void unitate_de_comanda(char comanda[])
{
	if (strcmp(comanda, "LI") == 0)
	{
		afisare_lista_infectati(numar_persoane);
		return;
	}
	if (strcmp(comanda, "LC") == 0)
	{
		afisare_lista_infractori(numar_persoane);
		return;
	}
	if (strcmp(comanda, "LS") == 0)
	{
		afisare_lista_persoane(numar_persoane);
		return;
	}
	if (strcmp(comanda, "MC") == 0)
	{
		printf("Va rog dati numarul persoanei si gradul de suspiciune dorit!: ");
		scanf("%d %d", &numar_pers, &grad_sus);
		if (numar_pers > numar_persoane)
		{
			printf("Persoana nu exista!\n");
			return;
		}
		modificare_infractor(numar_pers, grad_sus);
		printf("Modificare realizata cu succes!\n");
		return;
	}
	if (strcmp(comanda, "MI") == 0)
	{
		printf("Va rog dati numarul persoanei si probabilitatea de infectie dorita!: ");
		scanf("%d %d", &numar_pers, &proba);
		if (numar_pers > numar_persoane)
		{
			printf("Persoana nu exista!\n");
			return;
		}
		modificare_infectat(numar_pers, proba);
		printf("Modificare realizata cu succes!\n");
		return;
	}
	if (strcmp(comanda, "A") == 0)
	{
		numar_persoane++;
		printf("Va rog dati numarul de cunostinte pentru persoana cu numarul %d!: ", numar_persoane);
		Lista_persoane[numar_persoane] = (Persoanac *)malloc(sizeof(Persoanac));//aloc memorie pentru lista persoanei i
		Lista_persoane[numar_persoane]->next = NULL;//initializez lista de cunostinte cu valoarea NULL, pentru a putea incepe sa adaug relatii
		scanf("%d", &numar_cunostinte); printf("\n");
		persoana[numar_persoane].grad_suspiciune = 1; // initializare grad_suspect
		persoana[numar_persoane].probabilitate_infectie = 0; //initializare probabilitate
		for (int j = 1; j <= numar_cunostinte; j++)
		{
			scanf("%d %d", &nr_persoana, &grad_apropriere);//se citeste persoana pe care o cunoaste de gradul de apropriere
			if (nr_persoana >= numar_persoane)
			{
				printf("Persoana nu exista!\n");
				continue;
			}
			adaugare_legatura(nr_persoana, numar_persoane, grad_apropriere);//creez legaturile dintre acestia
		}
		printf("Persoana adaugata cu succes!\n");
		return;
	}
	if (strcmp(comanda, "I") == 0)
	{
		printf("Va rog dati numarul persoanei infectate!: ");
		scanf("%d", &persoana_infectata);
		if (persoana_infectata > numar_persoane)
		{
			printf("Persoana nu exista!\n");
			return;
		}
		adauga_infectat(persoana_infectata);
		printf("Modificare realizata cu succes!");
		return;
	}
	if (strcmp(comanda, "C") == 0)
	{
		printf("Va rog dati numarul persoanei care a comis o infractiune!: ");
		scanf("%d", &numar_infractor);
		if (numar_infractor > numar_persoane)
		{
			printf("Persoana nu exista!\n");
			return;
		}
		adauga_infractor(numar_infractor);
		printf("Modificare realizata cu succes!");
		return;
	}
	printf("Comanda nerecunoscuta!\n");
}



int main()
{
	FILE *f;
	f = fopen("data.in", "r");
	fscanf(f, "%d", &numar_persoane);//citesc numarul de persoane din lista
	for (int i = 1; i <= numar_persoane; i++)
	{
		Lista_persoane[i] = (Persoanac *)malloc(sizeof(Persoanac));//aloc memorie pentru lista persoanei i
		Lista_persoane[i]->next = NULL;//initializez lista de cunostinte cu valoarea NULL, pentru a putea incepe sa adaug relatii.
	}
	for (int i = 1; i <= numar_persoane; i++)
	{
		persoana[i].grad_suspiciune = 1; // initializare grad_suspect
		persoana[i].probabilitate_infectie = 0; //initializare probabilitate
		fscanf(f, "%d", &numar_cunostinte);
		for (int j = 1; j <= numar_cunostinte; j++)
		{
			fscanf(f, "%d %d", &nr_persoana, &grad_apropriere);//se citeste persoana pe care o cunoaste de gradul de apropriere
			adaugare_legatura(i, nr_persoana, grad_apropriere);//creez legaturile dintre acestia
		}
	}
	printf("Buna! Lista de persoane a fost actualizata!\n");
	printf("Alegeti o operatiune din cele descrise mai jos!\n\n");
	printf("I - adauga infectat!\n");
	printf("C - adauga infractor!\n");
	printf("LI - afiseaza lista infectati!\n");
	printf("LC - afiseaza lista infractori!\n");
	printf("MI - modifica infectat!\n");
	printf("MC - modifica infractor!\n");
	printf("A - adauga om nou!\n");
	printf("LS - afiseaza oamenii!\n");
	printf("E - iesire din program!\n\n");
	scanf("%s", &comanda);
	while (strcmp(comanda, "E") != 0)
	{
		unitate_de_comanda(comanda);//subprogram care proceseaza comanda si o executa.
		printf("\nIntroduceti alta comanda!\n\n");
		scanf("%s", &comanda);
	}
	printf("Program incheiat cu succes!");
	return 0;
}
