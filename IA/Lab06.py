import copy

class NodArbore:
    def __init__(self, _informatie, _g=0, _h=0, _parinte=None ):
        self.informatie = _informatie
        self.parinte  = _parinte
        self.g=_g
        self.h=_h
        self.f=_g+_h
        
    def __lt__(self, nod):
        return self.f<nod.f or (self.f==nod.f and self.h<nod.h)
    

    def drumRadacina(self):
        l=[]
        nod=self
        while nod:
            l.append(nod)
            nod=nod.parinte
        return l[::-1]

    def inDrum(self, infonod):
        nod=self
        while nod:
            if nod.informatie==infonod:
                return True
            nod=nod.parinte
        return False

    def __str__(self):
        return str(self.informatie)

    def __repr__(self):
        # "c (a->b->c)"
        return f"{self.informatie} cost:{self.g} ({'->'.join(map(str, self.drumRadacina()))})"


class Graf:
    def __init__(self, _start, _scopuri ):
        self.start=_start
        self.scopuri=_scopuri
    
    def valideaza(self, infoNod):
        lista = infoNod[0] + infoNod[1] + infoNod[2]
        nrInversiuni = 0

        for pozi, i in enumerate(lista[:-1]):
            for pozj, j in enumerate(lista[pozi+1:]):
                if i > j and j != 0:
                    nrInversiuni += 1

        return nrInversiuni % 2 == 0

    def scop(self, informatieNod):
        return informatieNod in self.scopuri

    def succesori(self, nod):
        def gasesteGol(matr):
            for i in range(0, len(matr)):
                for j in range(0, len(matr[i])):
                    if (matr[i][j] == 0):
                        return i, j
            return -1, -1

        lSuccesori=[]
        iGol, jGol = gasesteGol(nod.informatie)
        directii = [[-1, 0], [1, 0], [0, -1], [0, 1]] # sus, jos, stanga, dreapta
        for d in directii:
            iPlacuta = iGol + d[0]
            jPlacuta = jGol + d[1]

            if (0 <= iPlacuta <= 2 and 0 <= jPlacuta <= 2):
                infoSuccesor = copy.deepcopy(nod.informatie)
                infoSuccesor[iGol][jGol], infoSuccesor[iPlacuta][jPlacuta] = infoSuccesor[iPlacuta][jPlacuta], infoSuccesor[iGol][jGol]
                if not nod.inDrum(infoSuccesor):
                    lSuccesori.append(
                        NodArbore(
                            infoSuccesor,
                            nod.g+1,
                            self.estimeaza_h(infoSuccesor),
                            nod))
        return lSuccesori
    
    def estimeaza_h(self, infoNod):
        """_summary_

        Args:
            infoNod (_type_): nodul pentru care calculam estimatia
        """
        minH=float("inf")
        for scop in self.scopuri:
            h=0
            for iStivaScop,stivaScop in enumerate(scop):
                for iElem, elem in enumerate(stivaScop):
                    try:
                        if elem!=infoNod[iStivaScop][iElem]:
                            h+=1
                    except:
                        h+=1
            if h<minH:
                minH=h
        return minH
    # tema optionala pt +1 punct activ lab: modifica estimeaza_h cu functie de estimatie mai buna, folosind distanta Manhattan
    # (ca si cum de la placuta ai culoar liber: numai 0-uri si se face cu abs(i, j)... formula)
    # iei fiecare placuta, for i, for j, gasestePlacuta in loc de gasesteGol ca sa vezi unde trebuie sa ajunga in starea finala
    # distanta Manhattan strict pt valorile de la 1 la 8.. calculezi distanta, le aduni pe toate

    # tema cu canibalii si misionarii e optionala pt +5p activ lab, doar daca ai nevoie de puncte
        

def aStar(gr):
    OPEN=[NodArbore(gr.start)]
    CLOSED=[]
    while OPEN:
        nodCurent=OPEN.pop(0)
        CLOSED.append(nodCurent)
        if gr.scop(nodCurent.informatie):
            print(repr(nodCurent))
            return
        lSuccesori=gr.succesori(nodCurent)
        for s in lSuccesori:
            gasitOPEN=False
            for nod in OPEN:
                if s.informatie==nod.informatie:
                    gasitOPEN=True
                    if s.f<nod.f:
                        OPEN.remove(nod)
                    else:
                        lSuccesori.remove(s)
                    break
            if not gasitOPEN:
                for nod in CLOSED:
                    if s.informatie==nod.informatie:
                        if s.f<nod.f:
                            CLOSED.remove(nod)
                        else:
                            lSuccesori.remove(s)
                        break
        OPEN+=lSuccesori   
        OPEN.sort()
        
        

f=open("Lab06_input.txt","r")
continut=f.read()
start = [list(map(int, linie.strip().split(" "))) for linie in continut.strip().split("\n")] 
scopuri = [[[1, 2, 3], [4, 5, 6], [7, 8, 0]]]

print(start)
print(scopuri)  

gr=Graf( start,scopuri)

if not gr.valideaza(start):
    print("Nu avem solutii")
    exit()

print("solutii:")
aStar(gr)