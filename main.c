/* *********************************************************************************************** */
/*                                                                                                 */
/* Akasztófa                                                                                       */
/* Az akasztófás és a szótáras programokhoz használható szólisták: szotar.zip.                     */
/* A program a szavak listájából gondol egy feladványt, amelyet a felhasználónak ki kell találnia. */
/* Betuket tippel, amelyek ha a gondolt szóban szerepelnek, a gép felfedi oket. Pl. ha a gondolt   */
/* szó ökör, a tippek k és r, akkor a felhasználó ezt látja: _k_r. A program legyen képes:         */
/* - A felhasználó által megadott hosszúságú szóra gondolni (ha van olyan az adatbázisában).       */
/* - Szólni, ha már tippelt egy adott betut a felhasználó.                                         */
/* - A szavakat tároló bemeneti fájlt csak egyszer beolvasni.                                      */
/* A szavak tömbben történo tárolása ennél a feladatnál nem elfogadható!                           */
/*                                                                                                 */
/* *********************************************************************************************** */
/*                                                                                                 */
/* Készítette: HA9OOR, Kadocsa Benjámin, v.1.1                                                     */
/*                                                                                                 */
/* *********************************************************************************************** */
/*                                                                                                 */
/* A program kelyes mûködésének 4 feltétele van ékezetes betûk esetén,                             */
/* melybõl az utolsó ponr biztosítása a Felhasználó feladata:                                      */
/* - Át kell állítani a konzol karakterkódolását az 1250 kódlapra: system("CHCP 1250"); sor.       */
/* - A használt "magyar_latin2.txt" fájl kódolása is 1250 kódlapotú legyen.                        */
/* - A forrás "main.c" fájl kódolása is 1250 kódlapotú legyen.                                     */
/* - A konzol képernyo alapértelmezett fontja a 852-es kódlapot támogatja, ezért az elindított     */
/*   konzolalkalmazásban a bal felso sarokban lévo nyomógomb segítségével megnyitjuk a konzol      */
/*   menüjét, majd a "Tulajdonságok" menüpontban átállítjuk a betutípust Lucida Console-ra,        */
/*   amely támogatja az 1250 kódlapot.                                                             */
/*                                                                                                 */
/* *********************************************************************************************** */

#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <time.h>
#include <stdlib.h>
#include <stdbool.h>

#define SZO_HOSSZ_MAX               32
#define BETUK_SZAMA_MAX             256
#define DIN_TOMB_FOGLALASI_MERET    100

#ifndef MAXPATH
#define MAXPATH                     1023
#define MAXDIR                      255
#define MAXFILE                     255
#define MAXEXT                      255
#define MAXDRIVE                    3
#endif

#if defined (__STRICT_ANSI__)
_CRTIMP void __cdecl __MINGW_NOTHROW	_splitpath (const char*, char*, char*, char*, char*);
_CRTIMP char* __cdecl __MINGW_NOTHROW	_fullpath (char*, const char*, size_t);
#endif

typedef struct DinSzoTomb
{
    char **szo;
    int tombMeret;
    int szoDarabszam;
} DinSzoTomb;

int SzoBeolvasas(char *basePath, DinSzoTomb dinSzoTomb[]);
void SzoHosszKiiras(DinSzoTomb dinSzoTomb[]);
char *GondoltSzoValasztas(int szoHossz, DinSzoTomb dinSzoTomb[]);
void SzovegInit(const char *forras, char *cel);
int BetuBeolvasas(char *betu, int *marBeolvasottBetukSzama, char *marBeolvasottBetuk);
bool DinSzoTombFoglalas(DinSzoTomb *dinSzoTomb, int tombMeret);
void DinSzoTombFelszabaditas(DinSzoTomb *dinSzoTomb);
bool DinSzoTombNoveles(DinSzoTomb *dinSzoTomb, int ujtombMeret);

int main(int argc, char* argv[])
{
    system("CHCP 1250");
    printf("Szó kitalálós program\n");
    printf("Készítette: HA9OOR, Kadocsa Benjámin, BME TTK Fizika (BSc), v.1.0 2019.10.27\n\n");

    char basePath[MAXPATH];
    _fullpath(basePath, argv[0], sizeof(basePath));

    DinSzoTomb dinSzoTomb[SZO_HOSSZ_MAX];
    for (int i=0; i<SZO_HOSSZ_MAX; i++)
    {
        if (DinSzoTombFoglalas(&dinSzoTomb[i], DIN_TOMB_FOGLALASI_MERET) == false)
        {
            printf("Nem sikerult lefoglalni a dinSzoTomb elemeit!");
            return -1;
        }
    }

    if (SzoBeolvasas(basePath, dinSzoTomb) != 0)
        return -1;

    SzoHosszKiiras(dinSzoTomb);

    srand(time(0));

    char megUjabbJatek;
    do
    {
        int szoHossz;
        printf("Milyen hosszu szot szeretnel kitalalni? ");
        scanf("%d", &szoHossz);
        scanf("%*[^\n]%*c");

        if (szoHossz < 0 || szoHossz >= SZO_HOSSZ_MAX || dinSzoTomb[szoHossz].szoDarabszam == 0)
        {
            printf("A megadott hosszusaggal nem szerepel szo a szotarban.\n");
            megUjabbJatek = 'i';
            continue;
        }

        char *gondoltSzo = GondoltSzoValasztas(szoHossz, dinSzoTomb);
        char kitalaltSzo[strlen(gondoltSzo)+1];

        SzovegInit(gondoltSzo, kitalaltSzo);

        printf("Gondoltam egy %d hosszu szot. Talald ki! Talalgass betuket!\n", szoHossz);
        printf("A gondolt szo: %s\n", kitalaltSzo);

        int kerdes = 0;
        int talalat = 0;
        int marBeolvasottBetukSzama = 0;
        char marBeolvasottBetuk[BETUK_SZAMA_MAX];
        do
        {
            char betu;
            if (BetuBeolvasas(&betu, &marBeolvasottBetukSzama, marBeolvasottBetuk) == 0)
            {
                kerdes++;
                for (int i=0; i<strlen(gondoltSzo); i++)
                {
                    if (gondoltSzo[i] == betu)
                    {
                        kitalaltSzo[i] = betu;
                        talalat++;
                    }
                }
                printf("%s\n", kitalaltSzo);
            }
        }
        while (strlen(gondoltSzo) != talalat);

        printf("Gratulalok, %d talalgatasbol kitalaltad a gondolt szot!\n", kerdes);
        printf("Akarsz meg jatszani (i/n)? ");
        scanf(" %c", &megUjabbJatek);
    }
    while (megUjabbJatek == 'i' || megUjabbJatek == 'I');

    for (int i=0; i<SZO_HOSSZ_MAX; i++)
        DinSzoTombFelszabaditas(&dinSzoTomb[i]);

    return 0;
}

int SzoBeolvasas(char *basePath, DinSzoTomb dinSzoTomb[])
{
    printf("program: %s\n", basePath);

    char drive[MAXDRIVE], dir[MAXDIR], file[MAXFILE], ext[MAXEXT];
    _splitpath(basePath, drive, dir, file, ext);

    char filePath[MAXPATH];
    char ekezet;
    printf("Ekezetes szavakat akarsz kitalalni (i/n)? ");
    scanf(" %c", &ekezet);
    if (ekezet == 'i' || ekezet == 'I')
        sprintf(filePath, "%s%s%s%s", drive, dir, "magyar_latin2", ".txt");
    else
        sprintf(filePath, "%s%s%s%s", drive, dir, "magyar_ascii", ".txt");

    printf("szotar:  %s\n\n", filePath);

    FILE *f = fopen(filePath, "rt");
    if (f == NULL)
    {
        if (ekezet == 'i' || ekezet == 'I')
            printf("Nem sikerult megnyitni a \'magyar_latin2.txt\' fajlt!");
        else
            printf("Nem sikerult megnyitni a \'magyar_ascii.txt\' fajlt!");
        return -1;
    }

    char beolvasottSzo[SZO_HOSSZ_MAX + 1];
    while (fgets(beolvasottSzo,SZO_HOSSZ_MAX,f))
    {
        int szoHossz = strlen(beolvasottSzo)-1;
        beolvasottSzo[szoHossz] = '\0';

        if (dinSzoTomb[szoHossz].szoDarabszam == dinSzoTomb[szoHossz].tombMeret)
        {
            if (DinSzoTombNoveles(&dinSzoTomb[szoHossz], dinSzoTomb[szoHossz].tombMeret + DIN_TOMB_FOGLALASI_MERET) == false)
            {
                printf("Nem sikerult megnovelni a dinSzoTomb meretet!");
                fclose(f);
                return -1;
            }
        }

        dinSzoTomb[szoHossz].szo[dinSzoTomb[szoHossz].szoDarabszam] = (char *) malloc((szoHossz+1) * sizeof(char));
        strcpy(dinSzoTomb[szoHossz].szo[dinSzoTomb[szoHossz].szoDarabszam], beolvasottSzo);
        dinSzoTomb[szoHossz].szoDarabszam++;
    }
    fclose(f);

    return 0;
}

void SzoHosszKiiras(DinSzoTomb dinSzoTomb[])
{
    printf("A kovetkezo hosszusagu szavak kozul lehet valasztani:\n");
    int sorTores = 0;
    for (int i=0; i<SZO_HOSSZ_MAX; i++)
    {
        if (dinSzoTomb[i].szoDarabszam > 0)
        {
            if (++sorTores == 3)
            {
                printf("%2d hosszu szo: %5d db\n", i, dinSzoTomb[i].szoDarabszam);
                sorTores = 0;
            }
            else
            {
                printf("%2d hosszu szo: %5d db, ", i, dinSzoTomb[i].szoDarabszam);
            }
        }
    }
    printf("\n\n");
}

char *GondoltSzoValasztas(int szoHossz, DinSzoTomb dinSzoTomb[])
{
    int randNumber = rand() % dinSzoTomb[szoHossz].szoDarabszam;
    return dinSzoTomb[szoHossz].szo[randNumber];
}

void SzovegInit(const char *forras, char *cel)
{
    while (*forras)
    {
        *cel = '_';
        cel++;
        forras++;
    }
    *cel = '\0';
}

int BetuBeolvasas(char *betu, int *marBeolvasottBetukSzama, char *marBeolvasottBetuk)
{
    printf("Milyen beture tippelsz?");
    scanf(" %c", betu);

    *betu = tolower(*betu);
    for (int i=0; i<*marBeolvasottBetukSzama; i++)
    {
        if (*betu == marBeolvasottBetuk[i])
        {
            printf("Erre a beture mar tippeltel. Tippelj mas beture!\n");
            return -1;
        }
    }
    marBeolvasottBetuk[*marBeolvasottBetukSzama] = *betu;
    (*marBeolvasottBetukSzama)++;

    return 0;
}

bool DinSzoTombFoglalas(DinSzoTomb *dinSzoTomb, int tombMeret)
{
    dinSzoTomb->tombMeret = tombMeret;
    dinSzoTomb->szo = (char **) malloc(tombMeret * sizeof(char *));
    dinSzoTomb->szoDarabszam = 0;

    return dinSzoTomb->szo != NULL;
}

void DinSzoTombFelszabaditas(DinSzoTomb *dinSzoTomb)
{
    for (int i = 0; i < dinSzoTomb->szoDarabszam; i++)
        free(dinSzoTomb->szo[i]);

    free(dinSzoTomb->szo);
}

bool DinSzoTombNoveles(DinSzoTomb *dinSzoTomb, int ujtombMeret)
{
    char **ujadat = (char **) malloc(ujtombMeret * sizeof(char *));
    if (ujadat == NULL)
        return false;

    for (int i = 0; i < dinSzoTomb->tombMeret; i++)
        ujadat[i] = dinSzoTomb->szo[i];
    free(dinSzoTomb->szo);
    dinSzoTomb->szo = ujadat;
    dinSzoTomb->tombMeret = ujtombMeret;

    return true;
}
