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

    def scop(self, informatieNod):
        return informatieNod in self.scopuri

    def succesori(self, nod):
        lSuccesori=[]
        for i in range(len(nod.informatie)):
            if len(nod.informatie[i])==0:
                continue
            copieStive=copy.deepcopy(nod.informatie)
            bloc=copieStive[i].pop()
            for j in range(len(nod.informatie)):
                if i==j:
                    continue
                infoSuccesor=copy.deepcopy(copieStive)
                infoSuccesor[j].append(bloc)
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
        
        

f=open("Lab05-b_input.txt","r")
continut=f.read()
sirStart, sirScopuri= continut.split("=========")

def obtineStive(sirStive):
    rezultatStive=[]
    for sir in sirStive.strip().splitlines():
        if sir!="#":
            rezultatStive.append(sir.strip().split())
        else:
            rezultatStive.append([])
    return rezultatStive
            
start= obtineStive(sirStart)
scopuri=[]
for sirScop in sirScopuri.split("---"):
    scopuri.append(obtineStive(sirScop))
    
print(start)
print(scopuri)  
gr=Graf( start,scopuri)
print("solutii:")
aStar(gr)