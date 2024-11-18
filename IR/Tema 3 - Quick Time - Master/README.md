# Introducere in Robotica
## Tema de casă 3 - Tema 3 - Quick Time (Master)
### Dima Florin-Alexandru - Grupa 362

## Detalii lucrare

### 1.1. Descriere
Un joc de reflex competitiv adecvat pentru 2 jucatori. Ambii participanti concureaza pentru a obtine cel mai mare punctaj, testandu-si viteza de reactie.

Fiecare jucator are 3 butoane, asociate cu 3 culori (LED rosu, verde, albastru). Scopul fiecarui jucator este sa apese cat mai rapid butonul care corespunde culorii afisate pe LED-ul RGB al echipei sale. Punctajul fiecarui jucator este afisat pe ecranul LCD si se actualizeaza pe parcursul jocului. La finalul jocului, jucatorul cu cel mai mare punctaj este castigatorul.

### 1.2. Componente
- 6x LED-uri (2 grupuri de câte 3 leduri, în cadrul unui grup trebuie să avem culori diferite)
- 2x LED RGB (1 pentru fiecare jucător)
- 6x butoane (3 pentru fiecare jucător)
- 20x Rezistoare (20x 220 ohm)
- 1x LCD
- 1x servomotor
- 1x Potentiometru
- 2x Breadboard
- Fire de legatura
- 2x Arduino Uno

### 1.3. Detalii tehnice
- Butoane: Multiplexate pentru a reduce numarul de pini folositi de pe Arduino. Am folosit pinul analog A0 si citim valoarea analogica pentru a ne da seama care buton este cel apasat din seria de 6 butoane. In urma masuratorilor, am constatat ca o eroare de 10 unitati este adecvata pentru a identifica butoanele.

- SPI: Am folosit biblioteca SPI.h. Intrucat tema implica foarte multe legaturi, un singur Arduino Uno nu ne ofera suficienti pini. Astfel, am impartit functionalitatile necesare intre 2 placi Arduino Uno care comunica prin SPI (Serial Peripherical Interface). Arduino Master este responsabil pentru controlul LCD-ului, servomotorului si tine in memorie starea jocului (punctaj, LED care trebuie aprins etc - este cel care comanda jocul). Arduino Slave controleaza butoanele si LED-urile, primind mesaje prin SPI de la Arduino Master pentru a sti ce comanda trebuie efectuata, dar si pentru a trimite informatii catre Master, cum ar fi care este butonul apasat.

- LED-uri: Fiecare buton are asociat un LED de culoare diferita. Pe parcursul jocului acestea sunt aprinse pentru a vedea carei culoare ii corespunde fiecare buton. LED-ul RGB este aprins doar atunci cand jucatorul este la rand, iar acesta este aprins in una dintre culorile disponibile.

- LCD: Am folosit biblioteca LiquidCrystal. Am setat o luminozitate constanta (100%), iar contrastul este variabil cu ajutorul potentiometrului pentru a spori vizibilitatea textului pe ecran. In ecranul principal, apare un mesaj sugestiv pentru actiunea care trebuie luata pentru a incepe jocul (apasarea oricarui buton). In timpul jocului, pe ecran apar scorurile celor 2 jucatori, iar la sfarsit apare castigatorul si scorul (sau remiza). Dupa cateva secunde de la finalul jocului, se afiseaza ecranul principal din nou.

- Servomotor: Am folosit biblioteca Servo.h. Servomotorul incepe de la pozitia de 0 grade si se deplaseaza astfel incat sa indica scurgerea timpului (timpul ramas din runda).

- Pentru fiecare raspuns corect, jucatorul primeste 1 punct. Pentru greseala, jucatorul nu este depunctat.

### 1.4. Flow
#### Inițializare
Jocul pornește cu afișarea unui mesaj de bun venit pe LCD. Apăsarea oricarui buton declanșează startul jocului.

#### Desfășurarea Rundelor
- Fiecare jucător are trei butoane, fiecare asociat unui LED de o culoare diferită și un al 4-lea LED RGB.
- La fiecare rundă, fiecare jucător este cel activ.
- LED-ul RGB al jucătorului activ se aprinde într-o culoare corespunzătoare unuia dintre butoanele sale.Jucătorul trebuie să apese cât mai rapid butonul care corespunde culorii LED-ului RGB, pentru a obține puncte. Cu cât reacționează mai repede, cu atât primește mai multe puncte.
- La finalul unei runde LCD-ul afișează punctajul actualizat al ambilor jucători.
- Pe tot parcursul jocului display-ul LCD va arata punctajul fiecarui jucator

#### Timpul și Finalizarea Jocului
- Servomotorul se rotește pe parcursul jocului, indicând progresul. O rotație completă a servomotorului marchează sfârșitul jocului.
- La final, LCD-ul afișează numele câștigătorului și scorul final pentru câteva secunde, apoi revine la ecranul de start cu mesajul de bun venit.

## Demo
### 1. Schema electrica si montaj
- https://ibb.co/album/Jn1m5G

### 2. Video
- https://www.youtube.com/watch?v=mb71YiRKb2g
