
file = open("abcfile",'w')
file.write('askdfl')
file.close
var = ('r')
file = open("abcfile",var)
print(file.read())

try:
    8/0
    print('5/0')
except ZeroDivisionError as e:
    print('error')
    print(e)
