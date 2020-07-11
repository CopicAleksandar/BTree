# 42. Pretraga

## Uvod:
* U ovom projektu resavan je problem pretrage binarnog stabla i analizirane performanse 2 resenja.
* Cilj zadatka je implementacija optimalnog binarnog stabla i njegove pretrage kao i optimalna komunikacija procesa sa serverom, i klijenata sa serverom.

## Dizajn:
* Dizajn implementiranog resenja dat je dijagramom komponenti:
![Dijagram komponenti](https://github.com/sstefan1/Blok1/blob/master/Untitled%20Diagram.png)

* Prilikom dizajniranja resenja, akcenat je stavljen na ponovno iskoriscavanje koda.
- Projekat koristi 3 staticke biblioteke, kao sto je prikazano na dijagramu.
	- **socket.lib**: poseduje funkcije inicijalizacije socket-a(klijentskog ili serverskog), funkcije za
		slanje i primanje podataka, kao i prebacivanje serverskog socket-a u rezim slusanja.
	- **libbtree.lib**: poseduje funkcije za dodavanje elemenata u stablo, brisanje elementa iz stabla
		brisanje celog stabla i ispis stabla.
	- **rbuffer.lib**: poseduje funkcije za inicijalizaciju kruznog bafera zeljene velicine, dodavanje u bafer,
		skidanje sa bafera (pop), brisanje celog bafera i provere popunjenosti bafera.

## Strukture podataka:
* **AVL stablo**: izabrano da bi se izbegli nedostaci obicnog binarnog stabla (dodavanjem elemenata
	ciji su indeksi uzastopni celi brojevi). AVL stablo to resava balansiranjem stabla nakon
	svakog dodavanja.
```c
typedef struct node_t{
	int pid;
	SOCKET socket;
	struct node_t *left, right;
}node_t;
```
* **Ring-buffer**: se koristi kao red prioriteta dodavanja procesa u stablo. Svaki proces kad se prijavi,
	stavlja se u bafer, i ceka da bude upisan.
```c
typedef struct data_t {
	int pid;
	SOCKET socket;
}data_t;

typedef struct ring_buffer_t{
	data_t *buffer;
	data_t *buffer_end;
	data_t *data_start;
	data_t *data_end;
	int count;
	int size;
}ring_buffer_t;
```


## Rezultati testiranja:
* Stress test1:
![Memory Snapshot Binary search](https://github.com/sstefan1/project/blob/dev/MemorySnapshot.PNG)
![Memory Snapshot Traversal](https://github.com/sstefan1/project/blob/dev/LinearSearchSnapshot.PNG)
iz screenshot-a se vidi da nema curenja memorije.
Vec za hiljadu procesa, linearna pretraga traje nekoliko sekundi, za razliku od binarne koja se izvrsava 56ms.

## Zakljucak:
* TODO

## Potencijalna unapredjenja:
* Uvodjenje prosirivog kruznog bafera, koji ce u slucaju opterecenja sistema moci da primi vise procesa na cekanje
