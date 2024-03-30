print('='*50)   #start


f = open("newfile.txt", 'w')

for i in range(1, 11):
    data = "%d번째 줄입니다.%d\n" % (i, i)
    f.write(data)
    
f.write("Hello World!")
f.close()

print('='*50)   #end


