--------------------------- Tema 1 EGC - AstroKitty ---------------------------
-------------------------- Tudor Paraschivescu, 335CB -------------------------
-------------------------------------------------------------------------------
Structura proiect:
    - Objects
        - Asteroid: clasa tine centrul si raza (folosite la coliziune)
        - Astronaut: headerul cu metoda de calculare a varfului astronautului
        - Color: headerul in care sunt definite culorile geometriei folosite
        - Line: clasa tine o linie de coliziune si proprietatile ei
        - Object: clasa este factory-ul ce genereaza obiectele
    - Transformations
        - Math: headerul care implementeaza algoritmi de calcule matematice
        - Transformation: headerul care creeaza matricele de transformare
    - Homework1: clasa care implementeaza rezolvarea temei 1
-------------------------------------------------------------------------------
Scopul temei:
    - constructia de meshe simple:
        - se face in clasa Object de tip Factory
        - astronautul - triunghi albastru cu varf rosu
        - asteroizii - cercurile verzi discretizate cu un pas constant
        - platformele, care sunt de 3 tipuri:
            - platforme de stationare - culoare gri, astronautul se va opri
                pe acestea la detectarea coliziunii si va ateriza pe baza.
                Plecarea de pe acestea se va face prin click.
            - platformele de reflexie - culoare galbena, astronautul va ricosa
                mai departe cu acelasi unghi cu care a venit pe platforma
            - platforma finala - culoare rosie, la atingerea platformei, se va
                inchide fereastra jocului si se vor afisa statisticile in
                fereastra de cmd a engine-ului.
    - tratarea evenimentelor pentru mouse:
        - astronautul se va misca pe directia in care a fost detectat un click
        - exista clickuri acceptate si refuzate
        - sunt acceptate clickurile de la mouse cand obiectul stationeaza pe o
            platforma (sau la inceputul jocului) si noua directie nu obliga
            astronautul sa se intoarca (evitarea trecerii pe cealalta parte
            a platformei)
    - utilizarea transformarilor 2D: translatie, rotatie, scalare:
        - platformele sunt rendate la inceputul frameului fara a se aplica
            transformari asupra lor
        - asteroizii sunt rendati in update, dupa ce se actualizeaza acestia,
            aplicandu-se astfel transformarile respective fiecaruia
        - astronautul este animat dupa rendarea tuturor formelor din scena.
            Unghiul sub care acesta se misca este schimbat la aparitia unui
            event de click valid. La detectarea unei coliziuni, unghiul de
            deplasare se poate schimba. Astronautul se misca cu viteza
            constanta prin scena datorita descompunerii vectorului de viteza
            pe axe in functie de unghiul de deplasare.
-------------------------------------------------------------------------------
Detectarea coliziunilor:
    - cu platformele: O platforma este un dreptunghi si deci compusa din 4
        segmente de dreapta. Se adauga toate segmentele fiecarei platforme din
        scena intr-un vector. La fiecare frame, se verifica pentru fiecare
        segment daca exista o coliziune intre acesta si aproximarea
        caracterului cu un cerc.
    - cu asteroizii: Toti asteroizii sunt adaugati intr-un vector si sunt
        actualizati dupa fiecare frame, deoarece centrul sau raza lor se
        pot schimba in urma transformarilor din scena. La fiecare frame, se
        verifica pentru fiecare asteroid daca exista o coliziune intre acesta
        si aproximarea caracterului cu un cerc.
- Toate evenimentele importante (coliziuni si clickuri acceptate sau refuzate,
    alaturi de coordonatele unde s-au intamplat) vor fi afisate in fereastra
    engine-ului pentru a urmari usor evolutia jocului.
-------------------------------------------------------------------------------
