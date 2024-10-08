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
    def __init__(self, _matr, _start, _scopuri ):
        self.matr=_matr
        self.start=_start
        self.scopuri=_scopuri

    def scop(self, informatieNod):
        return informatieNod in self.scopuri

    def succesori(self, nod):
        lSuccesori=[]
        for infoSuccesor in range(len(self.matr)):
            if self.matr[nod.informatie][infoSuccesor]==1 and not nod.inDrum(infoSuccesor):
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



m = [
    [0, 1, 0, 1, 1, 0, 0, 0, 0, 0],
    [1, 0, 1, 0, 0, 1, 0, 0, 0, 0],
    [0, 1, 0, 0, 0, 1, 0, 1, 0, 0],
    [1, 0, 0, 0, 0, 0, 1, 0, 0, 0],
    [1, 0, 0, 0, 0, 0, 0, 1, 0, 0],
    [0, 1, 1, 0, 0, 0, 0, 0, 0, 0],
    [0, 0, 0, 1, 0, 0, 0, 0, 0, 0],
    [0, 0, 1, 0, 1, 0, 0, 0, 1, 1],
    [0, 0, 0, 0, 0, 0, 0, 1, 0, 0],
    [0, 0, 0, 0, 0, 0, 0, 1, 0, 0]
]

# tema depth first . ex 3, 4 - putem sa facem, dar prezentam si poate modificam putin (ne intreaba)
# coada -> stiva , apoi coada -> stiva cu functie recursiva
# tema e optionala , dar ne pune puncte bonus (rezerva de puncte)

start = 0
scopuri = [5, 9]
gr=Graf(m, start,scopuri)
breadthFirst(gr,3)