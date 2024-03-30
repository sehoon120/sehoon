print('='*50)

class FourCal:
    #self.n1 = 0
    #self.n2 = 0

    def __init__(self,a,b) -> None:
        self.n1 = a
        self.n2 = b

    def setdata(self,a,b):
        self.n1 = a   
        self.n2 = b
        
    def add(self):
        print (self.n1 + self.n2)
    
    def mul(self):
        print (self.n1*self.n2)
        
    def sub(self):
        print (self.n1-self.n2)
    
    def div(self):
        print ((self.n1/self.n2))

class More(FourCal):
    def pow(self):
        print(self.n1**self.n2)

    def div(self):
        if self.n2 == 0:
            result = 0
        else:
            result = self.n1/self.n2
        print(result)

    c = 'c'

a = FourCal(1,2)

#a.setdata(6,8)
a.add()
a.mul()
a.sub()
a.div()
#print(a.n1)
b = More(5,0)
b.pow()
b.div()
print(b.c)      #c
d = More(5,0)

b.c = 'd'
print(b.c)      #d

print(d.c)      #c
e = More(5,0)
print(e.c)      #c



print('='*50)