import copy

class NodArbore:
    def __init__(self, _informatie, _parinte=None ):
        self.informatie = _informatie
        self.parinte  = _parinte

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
        return f"{self.informatie} ({'->'.join(map(str, self.drumRadacina()))})"


class Graf:
    def __init__(self, _start, _scopuri ):
        self.start=_start
        self.scopuri=_scopuri

    def scop(self, informatieNod):
        return informatieNod in self.scopuri

    def succesori(self, nod):
        lSuccesori=[]
        for i in range(len(nod.informatie)):
            if len(nod.informatie[i]) == 0:
                continue
            copieStive = copy.deepcopy(nod.informatie)
            bloc = copieStive[i].pop()
            for j in range(len(nod.informatie)):
                if i == j:
                    continue
                infoSuccesor=copy.deepcopy(copieStive)
                infoSuccesor[j].append(bloc)

                if not nod.inDrum(infoSuccesor):
                    lSuccesori.append(NodArbore(infoSuccesor, nod))
        return lSuccesori

def breadthFirst(gr, nsol=3):
    coada=[NodArbore(gr.start)]
    while coada:
        nodCurent=coada.pop(0)
        if gr.scop(nodCurent.informatie):
            print(repr(nodCurent))
            nsol-=1
            if not nsol:
                return
        lSuccesori=gr.succesori(nodCurent)
        coada+=lSuccesori

def depthFirstNerecursiv(gr, nsol=3):
    stiva=[NodArbore(gr.start)]
    while stiva:
        nodCurent=stiva.pop()
        if gr.scop(nodCurent.informatie):
            print(repr(nodCurent))
            nsol-=1
            if not nsol:
                return
        lSuccesori=gr.succesori(nodCurent)
        stiva+=lSuccesori[::-1]
        
def depthFirstRecursiv(gr, nsol=3):
    def depthFirstRecursivFn(nodCurent, gr, nsol):
        if nsol <= 0:
            return 0
        if gr.scop(nodCurent.informatie):
            print(repr(nodCurent))
            nsol -= 1
            if nsol == 0:
                return 0

        lSuccesori = gr.succesori(nodCurent)
        for succesor in lSuccesori:
            nsol = depthFirstRecursivFn(succesor, gr, nsol)
            if nsol == 0:
                return 0
        return nsol
    
    nodStart = NodArbore(gr.start)
    depthFirstRecursivFn(nodStart, gr, nsol)

# Tema: Depth first iterative deepening (Cautare iterativa in adancime)
# Tema: Formateaza frumos problema blocurilor

f=open("Lab03_input.txt","r")
continut=f.read()
sirStart,sirScopuri=continut.split("=========")

# def obtineStive(sirStive):  # "a b c" -> ["a", "b", "c"]
#     return [ 
#             sir.strip().split() if sir !="#" else  [] 
#             for sir in sirStive.strip().split("\n") 
#         ] 

def obtineStive(sirStive):
    stiveRezultat = []
    for sir in sirStive.strip().split("\n"):
        if (sir != "#"):
            stiveRezultat.append(sir.strip().split())
        else:
            stiveRezultat.append([])
    return stiveRezultat

start=obtineStive(sirStart)
scopuri = []

for sirScop in sirScopuri.split("---"):
    scopuri.append(obtineStive(sirScop))

gr=Graf(start,scopuri)

breadthFirst(gr,3)