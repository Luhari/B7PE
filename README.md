# B7PE

Mazes genera una altura i amplada aleatoria i muta a MazeGen 80 vegades creant un laberint aleatori, que el guarda a pipo.txt.

Un cop creat el laberint, Mazes llegeix el laberint de pipo i el guarda en una matriu.

Llavors s'executa l'A* i quan s'acaba guarda el temps que ha tardat a results.txt.

Es fa una pausa d'1 min, per intentar mantenir les condicions inicials de cada prova.

A continuació s'executa el BFS i un cop ha acabat, guarda el temps que ha tardat a results.txt.

Així conseguim dades aparelleades de laberints alteatoris.


Coses a tenir en compte: 

- Utilització de redireccions de canals per la comunicació entre els programes i els archius.

- Utilització de signals per aclariment propi 
