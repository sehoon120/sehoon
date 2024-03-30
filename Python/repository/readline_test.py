print('='*50)   #start

# readline_test.py
f = open("newfile.txt", 'r')

#print(f.read())

while True:
    line = f.readline()
    if not line: break
    #line = line.strip()

    print(line,end='')


f.close()


print('='*50)   #end