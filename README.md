# Proiect-2
Detecţie facială - Gheorghita Ana, Mihalache Cristiana Andreea

Descriere algoritm 
– paşi iniţiali–

Pentru implementarea algoritmului de detecție facială cu ajutorul Viola-Jones, ne vom ocupa de următoarele: 
 1. 	 Caracteristicile Haar – selecția lor 
 2. 	 Imaginea Integrală – crearea ei 
 3. 	 Antrenarea Adaboost 
 4. 	 Cascade de clasificatori 
                 
 Întrucât unele regiuni ale feței sunt mai închise decât altele (după ce convertim imaginea din RGB în Grayscale), 
 vom avea nevoie de caracteristicile Haar, pe care le vom aplica peste imagine, porțiune cu porțiune, 
 modificând de fiecare data scara la care lucrăm. 
 
 Scopul nostru este acela de a căuta toate caracteristicile pentru toate dimensiunile.Totuși, vrem să reducem numărul
 de caracteristici pentru care vom folosi algoritmul Adaboost (deci să mărim viteza de calcul), iar pentru a face asta 
 avem nevoie să lucrăm cu “imaginea integrala”. Unele caracteristici vor fi irelevante, deci vom scăpa de ele: 
 ->Adaboost ne va ajuta să le alegem doar pe cele relevante cu ajutorul clasificatorilor puternici, care sunt o combinație liniară 
 a clasificatorilor slabi (referindu-ne la o fereastra de detectie de 24x24 pixeli, avem nevoie de aprox 2500 clasificatori slabi 
 pentru a construi un clasificator puternic), bazându-ne pe o clasificare binară. 
 ->Astfel, imaginea integrală este folosită pentru crearea clasificatorilor slabi. 
        
Așadar, în proiectul nostru, vom avea nevoie în principal de clase pentru caracteristicile Haar, clasificatorii slabi, 
clasificatorii puternici și procesul de “cascadare” a clasificatorilor. 

În fișierul features_vector am încercat să construim câteva metode care ne vor ajuta să lucrăm cu caracteristicile Haar. 
O caracteristică este reprezentată prin două, trei sau patru blocuri dreptunghiulare adiacente, cu tentă întunecată, 
respectiv deschisă. Fiecărei caracteristici îi este atribuită câte o valoare calculată ca diferența dintre suma intensităților pixelilor 
din fiecare regiune dreptunghiulară:

ValoareDreptunghi = Σ (pixelii din zona întunecată) - Σ (pixelii din zona deschisă).

Imaginii integrale S asociate cu imaginea dată I, i se pot calcula valorile intensității pixelilor după cum urmează: 

S(x, y) = Σ I(x’, y’), unde 1≤x’≤x, resp. 1≤y’≤y., iar I(x,y) este valoarea intensității pixelului (x, y) din imaginea originală 
și S(x, y) este valoarea intensității pixelului corespunzător din imaginea integrală. 
Astfel, imaginea integrală din locația (x, y) va conține suma tuturor intensităților pixelilor ai imaginii originale, 
de deasupra și din stânga lui (x, y). -> Odată ce imaginea integrală este construită, suma valorile pixelilor din orice 
zonă dreptunghiulară a imaginii originale poate fi calculată bazată doar pe patru vectori de pixeli.
 
Deci, suma valorilor intensităților pixelilor din dreptunghiul D (schema este cea din documentul de pe mail) va fi calculată ca:
S(x4,y4)-S(x2,y2)-S(x3,y3)+S(x1,y1).
Acest lucru urmează să îl implementăm cu ajutorul metodelor rectangleValue și getValue ale clasei Feature. 

În restul implementării am ținut cont de cele menționate mai sus, dar și de faptul că fiecare fereastră dreptunghiulară de detecție 
este parametrizată prin locație și mărime.
De asemenea, vom folosi fiecare valoare a unei caracteristici pentru a construi un clasificator slab 
(care ulterior ne va spune dacă fereastra respectivă conține o față).

 
 
pentru Adaboost (idei)

-clasificator slab1 + clasificator slab2+ clasificator slab3 + clasificator slab4 + ... + clasificator slab n = clasificator puternic1
(nu e adunare efectiva)

-sa presupunem ca avem un vector de dim n care sa contina
clasificatorii slabi 

-clasificatorii slabi putem spune ca sunt nuantele de gri 
de la 0 la 255 (n=256)
//rectific, cred ca acesti clasificatori n poti fi mult
mai multi
//in principiu avem nevoie doar de 2 clasificatori
puternici?

-trebuie sa "trainuim" clasificatorii slabi astfel incat
nuantele de gri intre 0 si 128 sa faca parte din 
clasificatorul puternic alb si intr 129 si 255 sa faca
parte din clasificatorul puternic negru

De aici am luat unele idei pe care le-am putea implementa (mai simplu):
https://www.cs.cmu.edu/~efros/courses/LBMV07/Papers/viola-cvpr-01.pdf -> Articolul stiintific oficial al lui Viola si Jones din 2001
http://mccormickml.com/2013/12/13/adaboost-tutorial/
https://en.wikipedia.org/wiki/Viola%E2%80%93Jones_object_detection_framework
https://en.wikipedia.org/wiki/Summed-area_table -> despre imaginea integrala
https://books.google.ro/books?id=ZPt2xrZFtzkC&printsec=frontcover&hl=ro -> "Introduction to Biometrics" -> in special pentru extragerea trasaturilor si potrivirea lor

1. Avem "scheletul" algoritmului, dar cum putem sa implementam toate acestea si sa le combinam?
2. Cum putem sa abordam antrenarea clasificatorilor slabi?
3. Cum facem algoritmul sa se descurce cu eventualele variatii ale dimensiunilor fetelor? 

