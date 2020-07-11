# Pretraga

* Potrebno je razviti rešenje pretrage stabla. 

* Sistem se sastoji iz tri glavne komponente: server, klijent i neodredjen broj čvorova.

* Čvorovi imaju indeks i vrednost, i oni se prijavljuju na server čim postanu aktivni.

* Server za pamćenje registrovanih čvorova koristi binarno stablo. Kada se novi čvor prijavi potrebno je staviti ga na odgovarajuce mesto u stablu, u zavisnosti od indeksa.

* Klijentu treba da bude omogućeno da pretraži stablo, tako što se obrati serveru koji vrši pretragu.

* Pretraga moze da bude po indeksu (binarna pretraga), ili po vrednosti (linearna pretraga).
