
/* c206.c **********************************************************}
{* Téma: Dvousměrně vázaný lineární seznam
**
**                   Návrh a referenční implementace: Bohuslav Křena, říjen 2001
**                            Přepracované do jazyka C: Martin Tuček, říjen 2004
**                                            Úpravy: Kamil Jeřábek, září 2018
**
** Implementujte abstraktní datový typ dvousměrně vázaný lineární seznam.
** Užitečným obsahem prvku seznamu je hodnota typu int.
** Seznam bude jako datová abstrakce reprezentován proměnnou
** typu tDLList (DL znamená Double-Linked a slouží pro odlišení
** jmen konstant, typů a funkcí od jmen u jednosměrně vázaného lineárního
** seznamu). Definici konstant a typů naleznete v hlavičkovém souboru c206.h.
**
** Vaším úkolem je implementovat následující operace, které spolu
** s výše uvedenou datovou částí abstrakce tvoří abstraktní datový typ
** obousměrně vázaný lineární seznam:
**
**      DLInitList ...... inicializace seznamu před prvním použitím,
**      DLDisposeList ... zrušení všech prvků seznamu,
**      DLInsertFirst ... vložení prvku na začátek seznamu,
**      DLInsertLast .... vložení prvku na konec seznamu,
**      DLFirst ......... nastavení aktivity na první prvek,
**      DLLast .......... nastavení aktivity na poslední prvek,
**      DLCopyFirst ..... vrací hodnotu prvního prvku,
**      DLCopyLast ...... vrací hodnotu posledního prvku,
**      DLDeleteFirst ... zruší první prvek seznamu,
**      DLDeleteLast .... zruší poslední prvek seznamu,
**      DLPostDelete .... ruší prvek za aktivním prvkem,
**      DLPreDelete ..... ruší prvek před aktivním prvkem,
**      DLPostInsert .... vloží nový prvek za aktivní prvek seznamu,
**      DLPreInsert ..... vloží nový prvek před aktivní prvek seznamu,
**      DLCopy .......... vrací hodnotu aktivního prvku,
**      DLActualize ..... přepíše obsah aktivního prvku novou hodnotou,
**      DLSucc .......... posune aktivitu na další prvek seznamu,
**      DLPred .......... posune aktivitu na předchozí prvek seznamu,
**      DLActive ........ zjišťuje aktivitu seznamu.
**
** Při implementaci jednotlivých funkcí nevolejte žádnou z funkcí
** implementovaných v rámci tohoto příkladu, není-li u funkce
** explicitně uvedeno něco jiného.
**
** Nemusíte ošetřovat situaci, kdy místo legálního ukazatele na seznam
** předá někdo jako parametr hodnotu NULL.
**
** Svou implementaci vhodně komentujte!
**
** Terminologická poznámka: Jazyk C nepoužívá pojem procedura.
** Proto zde používáme pojem funkce i pro operace, které by byly
** v algoritmickém jazyce Pascalovského typu implemenovány jako
** procedury (v jazyce C procedurám odpovídají funkce vracející typ void).
**/

#include "c206.h"

int errflg;
int solved;

void DLError() {
/*
** Vytiskne upozornění na to, že došlo k chybě.
** Tato funkce bude volána z některých dále implementovaných operací.
**/
    printf ("*ERROR* The program has performed an illegal operation.\n");
    errflg = TRUE;             /* globální proměnná -- příznak ošetření chyby */
    return;
}

void DLInitList (tDLList *L) {
/*
** Provede inicializaci seznamu L před jeho prvním použitím (tzn. žádná
** z následujících funkcí nebude volána nad neinicializovaným seznamem).
** Tato inicializace se nikdy nebude provádět nad již inicializovaným
** seznamem, a proto tuto možnost neošetřujte. Vždy předpokládejte,
** že neinicializované proměnné mají nedefinovanou hodnotu.
**/

L->Act=NULL;
L->First=NULL;
L->Last=NULL;
}

void DLDisposeList (tDLList *L) {
/*
** Zruší všechny prvky seznamu L a uvede seznam do stavu, v jakém
** se nacházel po inicializaci. Rušené prvky seznamu budou korektně
** uvolněny voláním operace free.
**/

  tDLElemPtr tmp;
  while(L->First)
  {
    if(L->First!=NULL)
    {
      tmp=L->First;
      L->First=L->First->rptr;
      free(tmp);
    }
    L->Last=NULL;
    L->Act=NULL;
    L->First=NULL;
  }
}

void DLInsertFirst (tDLList *L, int val) {
/*
** Vloží nový prvek na začátek seznamu L.
** V případě, že není dostatek paměti pro nový prvek při operaci malloc,
** volá funkci DLError().
**/
  tDLElemPtr tmp = malloc(sizeof(struct tDLElem));
  if(tmp==NULL)
  {
    DLError();
    return;
  }
  tmp->data=val; //nahradime hodnotu predanou parametrem
  tmp->rptr=L->First; // pravy nasledovnik noveho prvku bude ukazovat na prvni
  tmp->lptr=NULL; // Levy nasledovnik noveho prvku bude ukazovat do NULL(zacatek)
  if(L->First!=NULL) // pokud seznam neni prazdny
  {
    L->First->lptr=tmp; //levy ukazatel prvniho bude ukazovat na novy
  }
  else // pokud je L->First==NULL, tj seznam je prazdny
  {
    L->Last=tmp;
  }
  L->First=tmp;
}

void DLInsertLast(tDLList *L, int val) {
/*
** Vloží nový prvek na konec seznamu L (symetrická operace k DLInsertFirst).
** V případě, že není dostatek paměti pro nový prvek při operaci malloc,
** volá funkci DLError().
**/
  tDLElemPtr tmp = malloc(sizeof(struct tDLElem));
  if(tmp==NULL)
  {
    DLError();
    return;
  }
  tmp->data=val; //nahradime novou hodnotou
  tmp->lptr=L->Last; // levy nasledovnik noveho prvku bude ukazovat na posledni
  tmp->rptr=NULL; // pravy nasledovnik noveho prvku bude ukazovat na konec
  if(L->Last!=NULL) // pokud seznam neni prazdny
  {
    L->Last->rptr=tmp; // Pravy prvek od posledniho bude ukazovat na novy prvek
  }
  else // pokud seznam je prazdny
  {
    L->First=tmp; // novy prvek bude zaroven i prvni
  }
  L->Last=tmp;
}

void DLFirst (tDLList *L) {
/*
** Nastaví aktivitu na první prvek seznamu L.
** Funkci implementujte jako jediný příkaz (nepočítáme-li return),
** aniž byste testovali, zda je seznam L prázdný.
**/
L->Act=L->First;
}

void DLLast (tDLList *L) {
/*
** Nastaví aktivitu na poslední prvek seznamu L.
** Funkci implementujte jako jediný příkaz (nepočítáme-li return),
** aniž byste testovali, zda je seznam L prázdný.
**/

L->Act=L->Last;
}

void DLCopyFirst (tDLList *L, int *val) {
/*
** Prostřednictvím parametru val vrátí hodnotu prvního prvku seznamu L.
** Pokud je seznam L prázdný, volá funkci DLError().
**/

  if(L->First==NULL)
  {
    DLError();
  }
  else
  {
    *val=L->First->data;
  }
}

void DLCopyLast (tDLList *L, int *val) {
/*
** Prostřednictvím parametru val vrátí hodnotu posledního prvku seznamu L.
** Pokud je seznam L prázdný, volá funkci DLError().
**/
  if(L->First==NULL)
  {
    DLError();
  }
  else
  {
    *val=L->Last->data;
  }
}

void DLDeleteFirst (tDLList *L) {
/*
** Zruší první prvek seznamu L. Pokud byl první prvek aktivní, aktivita
** se ztrácí. Pokud byl seznam L prázdný, nic se neděje.
**/
  tDLElemPtr tmp;
  if(L->First!=NULL) // seznam neni przdny
  {
    tmp=L->First; // do pomocneho si nahrajeme prvni
    if(L->First==L->Act) // pokud je prvni zaroven aktivni
    {
      L->Act=NULL; //zrusime aktivitu
    }
    if(L->First==L->Last) // pokud je v seznamu pouze jeden prvek
    {
      L->First=NULL;
      L->Last=NULL;
    }
    else // pokud je v seznamu vice jak 1 prvek
    {
      L->First=L->First->rptr; // do prvniho prvku ulozime jeho nasledovnika
      L->First->lptr=NULL ; // predchudce prvniho prvku posleme do NULL
    }
    free(tmp);
  }
  if(L->First==NULL)
  {

  }
}

void DLDeleteLast (tDLList *L) {
/*
** Zruší poslední prvek seznamu L. Pokud byl poslední prvek aktivní,
** aktivita seznamu se ztrácí. Pokud byl seznam L prázdný, nic se neděje.
**/
  tDLElemPtr tmp;
  if(L->First!=NULL) // pokud seznam neni prazdny
  {
    tmp=L->Last; // do pomocneho si nahrajeme posledni
    if(L->Last==L->Act) // pokud je posledni prvek zaroven aktivni
    {
      L->Act=NULL; // zrusime aktivitu
    }
    if(L->Last==L->First) // pokud je v seznamu pouze 1 prvek
    {
      L->First=NULL;
      L->Last=NULL;
      //L->Act=NULL;
    }
    else // pokud ma seznam vice prvku
    {
      L->Last=L->Last->lptr;
      L->Last->rptr=NULL;
    }
    free(tmp); //uvolnime pamet
  }
  if(L->First==NULL)
  {

  }
}

void DLPostDelete (tDLList *L) {
/*
** Zruší prvek seznamu L za aktivním prvkem.
** Pokud je seznam L neaktivní nebo pokud je aktivní prvek
** posledním prvkem seznamu, nic se neděje.
**/
  if(L->Act!=NULL) // pokud je aktivni
  {
    if(L->Act->rptr!=NULL) //
    {
      tDLElemPtr tmp;
      tmp =L->Act->rptr; //v tmp mame ruseny prvek
      L->Act->rptr=tmp->rptr; // do nasledovnika aktualniho ulozime nasledovnika ruseneho
      if(tmp==L->Last) //pokud je ruseny posledni
      {
        L->Last=L->Act; //posledni prvek bude aktivni
      }
      else
      {
        tmp->rptr->lptr=L->Act; //nasledovnik predchudce ruseneho prvku bude aktivni
      }
      free(tmp); // uvolnime pamet
    }
  }
  if(L->Act==NULL && L->Act==L->Last)
  {

  }
}

void DLPreDelete (tDLList *L) {
/*
** Zruší prvek před aktivním prvkem seznamu L .
** Pokud je seznam L neaktivní nebo pokud je aktivní prvek
** prvním prvkem seznamu, nic se neděje.
**/
  if(L->Act!=NULL)
  {
      if(L->Act->lptr!=NULL) // pokud predchudce aktivniho neni NULL
      {
        tDLElemPtr tmp;
        tmp=L->Act->lptr; // v tmp mame ruseny prvek, tzn prvek nalevo od aktivniho
        L->Act->lptr=tmp->lptr; //do predchudce aktivniho ulozime mazany prvek
        if(tmp==L->First) // pokud je mazany prvek prvni
        {
          L->First=L->Act; // prvni prvek bude aktivni
        }
        else //
        {
          tmp->lptr->rptr=L->Act; // predchudce nasledovnika bude aktivni
        }
        free(tmp);
      }
  }
  if(L->Act==NULL && L->Act==L->First)
  {

  }
}

void DLPostInsert (tDLList *L, int val) {
/*
** Vloží prvek za aktivní prvek seznamu L.
** Pokud nebyl seznam L aktivní, nic se neděje.
** V případě, že není dostatek paměti pro nový prvek při operaci malloc,
** volá funkci DLError().
**/
  if(L->Act!=NULL) // je aktivni
  {
    tDLElemPtr tmp= malloc(sizeof(struct tDLElem));
    if(tmp==NULL)
    {
      DLError();
      return;
    }
    tmp->data=val; // nahrajeme prvek
    tmp->rptr=L->Act->rptr; // prepojeni sousednich prvku
    tmp->lptr=L->Act;
    L->Act->rptr=tmp;
    if(L->Act==L->Last) // pokud je aktivni prvek posledniho
    {
      L->Last=tmp;
    }
    else
    {
      tmp->rptr->lptr=tmp;
    }
  }
  if(L->Act==NULL)
  {

  }
}

void DLPreInsert (tDLList *L, int val) {
/*
** Vloží prvek před aktivní prvek seznamu L.
** Pokud nebyl seznam L aktivní, nic se neděje.
** V případě, že není dostatek paměti pro nový prvek při operaci malloc,
** volá funkci DLError().
**/
  if(L->Act!=NULL) // pokud je seznam aktivni
  {
    tDLElemPtr tmp = malloc(sizeof(struct tDLElem));
    if(tmp==NULL) //malloc se nepovedl
    {
      DLError();
      return;
    }
    tmp->data=val; // nahrajeme hodnotou
    tmp->lptr=L->Act->lptr; // provedeme propojeni sousednich prvku
    tmp->rptr=L->Act;
    L->Act->lptr=tmp;
    if(L->Act==L->First)//pokud je aktivni prvek zaroven prvnim; tj vkladame pred prvni
    {
      L->First=tmp;
    }
    else
    {
      tmp->lptr->rptr=tmp;
    }
  }
  if(L->Act==NULL)
  {

  }
}

void DLCopy (tDLList *L, int *val) {
/*
** Prostřednictvím parametru val vrátí hodnotu aktivního prvku seznamu L.
** Pokud seznam L není aktivní, volá funkci DLError ().
**/
  if(L->Act==NULL) // pokud seznam neni aktivni
  {
    DLError();
  }
  else
  {
    *val=L->Act->data;
  }
}
void DLActualize (tDLList *L, int val) {
/*
** Přepíše obsah aktivního prvku seznamu L.
** Pokud seznam L není aktivní, nedělá nic.
**/
  if(L->Act!=NULL) // pokud je seznam aktivni
  {
    L->Act->data=val;
  }
  if(L->Act==NULL)
  {

  }
}
void DLSucc (tDLList *L) {
/*
** Posune aktivitu na následující prvek seznamu L.
** Není-li seznam aktivní, nedělá nic.
** Všimněte si, že při aktivitě na posledním prvku se seznam stane neaktivním.
**/
  if(L->Act!=NULL) // pokud je seznam aktivni
  {
    if(L->Act==L->Last) // pokud je posledni prvek aktivni
    {
      L->Act=NULL; // zrusime aktivitu
    }
    else //pokud neni aktivni posledni prvek
    {
      L->Act=L->Act->rptr; // jako aktivni udelame prvek za aktivnim
    }
  }
  if(L->Act == NULL)
  {

  }
}
void DLPred (tDLList *L) {
/*
** Posune aktivitu na předchozí prvek seznamu L.
** Není-li seznam aktivní, nedělá nic.
** Všimněte si, že při aktivitě na prvním prvku se seznam stane neaktivním.
**/
  if(L->Act!=NULL) //seznam je aktivni
  {
    if(L->Act==L->First) // pokud je prvni prvek aktivni
    {
      L->Act=NULL;
    }
    else // pokud neni aktivni prvni prvek
    {
      L->Act=L->Act->lptr; // jako aktivni prvek je nyni predchudce aktivniho
    }
  }
  if(L->Act==NULL)
  {

  }
}
int DLActive (tDLList *L) {
/*
** Je-li seznam L aktivní, vrací nenulovou hodnotu, jinak vrací 0.
** Funkci je vhodné implementovat jedním příkazem return.
**/
 return(L->Act!=NULL);
}

/* Konec c206.c*/
