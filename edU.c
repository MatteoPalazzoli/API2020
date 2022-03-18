/* edU, o ed con Undo multipli
 * Progetto di algoritmi e strutture dati
 * Palazzoli Matteo
 * Politecnico di Milano A.A. 2019/2020 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define LIM 1026
#define LIM_O 100
#define BLOCCO 70
#define P_CHAR sizeof(char*)
#define CHAR sizeof(char)
#define P_INT sizeof(int*)
#define INT sizeof(int)

/* ----- DEFINIZIONI DI TIPO ----- */
typedef enum Flag {FALSE, TRUE} flag;

/* ----- VARIABILI GLOBALI ----- */
int **file, *input;
char **righe;
int tot_stringhe, tot_righe_file, tot_modifiche, tot_input_WO;
flag WriteOnly = TRUE;

/* ----- FUNZIONI AGGIUNTIVE ----- */
void acquisisci(int firstS){
    char buffer[LIM];
    size_t lungStr;
    fgets(buffer, LIM, stdin);
    lungStr = strlen(buffer) + 1;
    righe[firstS] = malloc(lungStr * CHAR);
    memcpy(righe[firstS], buffer, lungStr);
}
void azzeraPrimaMod(int start){
    for(int i = start*BLOCCO; i < tot_righe_file * BLOCCO; i++)
        file[i][0] = 0;
}
void controllaModifiche(int totMod){
    int i, *tempF;
    if(totMod >= tot_modifiche * BLOCCO){
        tot_modifiche++;
        for(i = 0; i < tot_righe_file * BLOCCO; i++){
            tempF = realloc(file[i], tot_modifiche * BLOCCO * INT);
            if(tempF != NULL) file[i] = tempF;
            else{
                puts("Errore: impossibile reallocare nuove modifiche");
                exit(1);
            }
        }
    }
}
void controllaStringhe(int qnt, int firstS){
    int bisogno;
    char **tempS;
    bisogno = qnt + firstS;
    if(tot_stringhe < (bisogno/BLOCCO+1)){
        tot_stringhe = (bisogno/BLOCCO+1);
        tempS = realloc(righe, tot_stringhe * BLOCCO * P_CHAR);
        if (tempS != NULL) righe = tempS;
        else {
            puts("Errore: impossibile reallocare nuove stringhe");
            exit(1);
        }
    }
}
void controllaFile(int c){
    int i, **tempFF, *tempF;
    if(c >= tot_righe_file * BLOCCO){
        tempFF = realloc(file, ((c/BLOCCO)+1) * BLOCCO * P_INT);
        if(tempFF != NULL){
            file = tempFF;
            azzeraPrimaMod(tot_righe_file);
            for(i = tot_righe_file * BLOCCO; i <((c/BLOCCO)+1) * BLOCCO; i++){
                tempF = malloc(tot_modifiche * BLOCCO * INT);
                if(tempF != NULL){
                    file[i] = tempF;
                }
                else{
                    puts("Errore: impossibile reallocare mod. nuove stringhe");
                    exit(1);
                }
            }
            tot_righe_file = (c/BLOCCO)+1;
        }
        else{
            puts("Errore: impossibile reallocare nuove righe di file");
            exit(1);
        }
    }
}
void controllaInput(){
    int *tempI;
    if(input[0] == tot_input_WO){
        tot_input_WO++;
        tempI = realloc(input, tot_input_WO * BLOCCO * INT);
        if(tempI != NULL) input = tempI;
        else{
            puts("Errore. Impossibile reallocare array input");
            exit(1);
        }
    }
}
void interpreta(char *istr, int *a, int *c, char *opcode){
    char *fine;
    *a = (int)strtol(istr, &fine, 10);
    *opcode = *fine;
    if (*opcode == ',') {
        *c = (int)strtol((fine+1), &fine, 10);
        *opcode = *fine;
    } else { *c = -1; }
}
void vediStringhe(int firstS){
    int i;
    for(i=0; i<input[0]+1; i++){
        if(i==0) {printf("[(%d)", input[0]); continue; }
        if(i != input[0] && i != 0){ printf(",%d,", input[i]); continue; }
        if(i == input[0]) { printf("%d]\n", input[i]); continue; }
    }
    printf("tot_stringhe: %d\n firstS: %d\n", tot_stringhe, firstS);
    for(i = 0;i < firstS; i++)
        printf("%d -> %s", i, righe[i]);
}
void vediFile(int numMod, int totMod){
    int i, j, max=0;
    printf("tot_righe_file: %d, tot_modifiche: %d\n", tot_righe_file, tot_modifiche);
    printf("numMod: %d, totMod: %d\n", numMod, totMod);
    for(i=input[0]+1, fputs("0   ", stdout); i<=totMod+1; i++, printf("%d ",i));
    fputs("\n──┬", stdout);
    if(numMod <= input[0]){ numMod = 0; totMod = 0; }
    for(i = 0; i <= totMod; i++) if(file[0][i] > max) max = file[0][i];
    for(i=0; i<=max; i++) {
        if(i==0){ for(j=0; j<=totMod-1; j++){fputs("──", stdout);} puts("");}
        for (j = 0; j <= totMod; j++){
            if (file[i][j] != -1094795586) printf("%d ", file[i][j]);
            else fputs("0 ", stdout);
            if(j == 0) fputs("│ ", stdout);
        } puts("");
        if(i==0){
            fputs("──┼",stdout);
            for(j=0; j<=totMod-1; j++){ fputs("──", stdout); }
            puts("");
        }
    }
}

void printWO(int numMod, int a, int c){
    int i, sum = 0;
    for(i = 1; i <= numMod; i++)
        sum += input[i];
    for(i = a; i <= c; i++){
        if(i > sum) break;
        printf("%s", righe[i]);
    }
    for(;i <= c; i++) puts(".");
}

/* ----- PRINT ----- */
void print(int numMod, int a, int c){
    int i;
    if(a == 0){
        puts(".");
        a=1;
    }
    numMod -= input[0];
    for(i=a;i<=c;i++){
        if(i > file[0][numMod]) break;
        printf("%s", righe[file[i][numMod]]);
    }
    for(;i<=c;i++) puts(".");
}

int changeWO(int numMod, int a, int c, int firstS){
    int i;
    char puntino;
    input[0]++;
    controllaInput();
    input[input[0]] = c-a+1;
    for(i=a; i<=c; i++) {
        acquisisci(firstS);
        firstS++;
    }
    puntino = getc_unlocked(stdin); puntino = getc_unlocked(stdin);
    return firstS;
}

int firstChange(int numMod, int a, int c, int firstS){
    /* Inserisce nella tabella l'ultimo file-input valido e fa la change */
    int i, sum = 0;
    char puntino;
    for(i = 1; i < numMod; i++)
        sum += input[i];
    if(sum >= c) file[0][1] = sum;
    else file[0][1] = c;
    for(i = 1; i < a; i++)
        file[i][1] = i;
    for(i = a; i <= c; i++){
        acquisisci(firstS);
        file[i][1] = firstS;
        firstS++;
    }
    for(; i <= sum; i++)
        file[i][1] = i;
    puntino = getc_unlocked(stdin); puntino = getc_unlocked(stdin);
    return firstS;
}

/* ----- CHANGE ----- */
int change(int numMod, int a, int c, int firstS){
    int i;
    char puntino;
    numMod -= input[0];
    for(i = 1; i < a; i++)
        file[i][numMod] = file[i][numMod - 1];
    for(i = a; i <= c; i++) {
        acquisisci(firstS);
        file[i][numMod] = firstS;
        firstS++;
    }
    if(c > file[0][numMod-1]) file[0][numMod] = c;
    else file[0][numMod] = file[0][numMod-1];
    for(i = c+1; i <= file[0][numMod]; i++)
        file[i][numMod] = file[i][numMod - 1];
    puntino = getc_unlocked(stdin); puntino = getc_unlocked(stdin);
    return firstS;
}

void firstDelete(int numMod, int a, int c){
    int i, sum = 0;
    for(i = 1; i < numMod; i++)
        sum += input[i];
    file[0][1] = sum - (c-a+1);
    for(i = 1; i < a; i++)
        file[i][1] = i;
    for(i = a; i <= sum; i++){
        if(i + (c-a+1) > sum) file[i][1] = 0;
        else file[i][1] = i + (c-a+1);
    }
}

/* ----- DELETE ----- */
void delete(int numMod, int a, int c){
    int i;
    numMod -= input[0];
    if((a == 0 && c == 0) || a > file[0][numMod-1]){
        file[0][numMod] = file[0][numMod-1];
        for(i = 0; i <= file[0][numMod]; i++)
            file[i][numMod] = file[i][numMod-1];
    }else{
        if(c > file[0][numMod-1]) c = file[0][numMod-1];
        for(i = 1; i < a; i++)
            file[i][numMod] = file[i][numMod - 1];
        for(i = a; i <= file[0][numMod-1]; i++){
            if(i+c-a+1 > file[0][numMod-1]) file[i][numMod] = 0;
            else file[i][numMod] = file[i + (c - a + 1)][numMod - 1];
        }
        file[0][numMod] = file[0][numMod-1] - (c-a+1);
    }
}

int main(){
    int i, a, c, firstS=1, numMod=0, totMod=0;
    char opcode, istr[LIM_O];
    flag NonLeggere = FALSE, postUR = FALSE;
    tot_righe_file = 1;
    tot_stringhe = 1;
    tot_modifiche = 1;
    tot_input_WO = 1;
    input = malloc(tot_input_WO * BLOCCO * INT); input[0] = 0;
    righe = malloc(tot_stringhe * BLOCCO * P_CHAR);
    righe[0] = malloc(3 * CHAR); righe[0][0] = '.'; righe[0][1] = '\n'; righe[0][2] = '\0';
    file = malloc(tot_righe_file * BLOCCO * P_INT);
    for(i = 0; i < BLOCCO; i++) file[i] = malloc(tot_modifiche * BLOCCO * INT);
    azzeraPrimaMod(0);
    do{
        if(NonLeggere == FALSE){
            fgets(istr, LIM_O, stdin);
            interpreta(istr, &a, &c, &opcode);
            //printf("a = %d; c = %d; opcode = %c\n",a,c,opcode);
        }else{ NonLeggere = FALSE; }
        switch (opcode) {
            case 'p':
                if(WriteOnly || numMod <= input[0]) printWO(numMod, a, c);
                else print(numMod, a, c);
                break;
            case 'S': vediStringhe(firstS); break;
            case 'F': vediFile(numMod, totMod); break;
            case 'c':
            case 'd':
                if(postUR == TRUE){
                    postUR = FALSE;
                    totMod = numMod;
                    if(numMod < input[0]) input[0] = numMod;
                }
                numMod++;
                totMod++;
                if(opcode == 'c'){
                    controllaStringhe(c-a+1, firstS);
                    if(WriteOnly == TRUE && a == firstS)
                        firstS = changeWO(numMod, a, c, firstS);
                    else{
                        controllaModifiche(totMod);
                        if(WriteOnly || numMod <= input[0]){
                            if(WriteOnly){
                                WriteOnly = FALSE;
                                input = realloc(input, (input[0]+1)*INT);
                            }
                            if(c >= firstS) controllaFile(c);
                            else controllaFile(firstS);
                            firstS = firstChange(numMod, a, c, firstS);
                        }else{
                            controllaFile(c);
                            firstS = change(numMod, a, c, firstS);
                        }
                    }
                }else{
                    if(WriteOnly || numMod <= input[0]){
                        if(WriteOnly){
                            WriteOnly = FALSE;
                            input = realloc(input, (input[0]+1)*INT);
                        }
                        firstDelete(numMod, a, c);
                    }else{
                        controllaModifiche(totMod);
                        delete(numMod, a, c);
                    }
                }
                break;
            case 'u':
            case 'r':       /* Accorpamento U-R */
                do{
                    if(opcode == 'u'){
                        numMod = numMod - a;
                        if(numMod < 0) numMod = 0;
                    }else{
                        numMod = numMod + a;
                        if(numMod > totMod) numMod = totMod;
                    }
                    fgets(istr, LIM_O, stdin);
                    interpreta(istr, &a, &c, &opcode);
                }while(istr[0] != 'q' && (opcode == 'u' || opcode == 'r'));
                postUR = TRUE;
                NonLeggere = TRUE;
                break;
            default: if(istr[0] != 'q') puts("Errore: comando non riconosciuto"); break;
        } /* switch */
    }while(istr[0] != 'q'); /* do-while */
    return 0;
}