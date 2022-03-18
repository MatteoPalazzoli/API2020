# Progetto di Algoritmi e Strutture Dati 2020
__"EdU"__, un editor di testi a riga di comando che implementa undo e redo.

### I comandi sono i seguenti:
* ind<sub>1</sub>, ind<sub>2</sub>**c**: cambia il testo alle righe comprese tra ind<sub>1</sub> ed ind<sub>2</sub>, seguono ind<sub>2</sub>-ind<sub>1</sub>+1 righe più una riga con il solo punto
* ind<sub>1</sub>, ind<sub>2</sub>**d**: cancella le righe tra ind<sub>1</sub> ed ind<sub>2</sub> (estremi inclusi)
* ind<sub>1</sub>, ind<sub>2</sub>**p**: stampa le righe tra ind<sub>1</sub> ed ind<sub>2</sub>
* n**u**: fa l’undo degli ultimi n comandi (c o d)
* n**r**: fa il redo degli ultimi n comandi annullati con undo
* **q**: termina
