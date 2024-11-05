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
    def __init__(self, _matr, _h, _start, _scopuri ):
        self.matr = _matr
        self.start=_start
        self.scopuri=_scopuri
        self.h = _h

    def scop(self, informatieNod):
        return informatieNod in self.scopuri

    def succesori(self, nod):
        lSuccesori=[]
        for infoSuccesor in range(len(self.matr)):
            if self.matr[nod.informatie][infoSuccesor] > 0 and not nod.inDrum(infoSuccesor):
                lSuccesori.append(
                    NodArbore(
                        infoSuccesor,
                        nod.g + self.matr[nod.informatie][infoSuccesor],
                        self.h[infoSuccesor],
                        nod))
        return lSuccesori
        

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


m = [[0, 3, 5, 10, 0, 0, 100],
    [0, 0, 0, 4, 0, 0, 0],
    [0, 0, 0, 4, 9, 3, 0],
    [0, 3, 0, 0, 2, 0, 0],
    [0, 0, 0, 0, 0, 0, 0],
    [0, 0, 0, 0, 4, 0, 5],
    [0, 0, 3, 0, 0, 0, 0]]
start = 0
scopuri = [4,6]
h=[0,1,6,2,0,3,0]

gr=Graf(m, h, start, scopuri)

print('A*')
aStar(gr)