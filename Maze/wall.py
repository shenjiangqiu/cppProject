#!python3
def pure(a):
    if a[0]>a[1]:
        return (a[1],a[1])
    else:
        return a

def printMaze(height,width,connection):
    print("[W]"+" [W]"*(width*2))#//首先输出第一行


    for i in range(height*2-1) :
        print("[W]",end="")
        for j in range(width*2-1):
            if i%2==0:
                if j%2==0:
                    print(" [R]",end="")
                else:
                    a=pure(((i//2,j//2),(i//2,j//2+1)))
                    if a in connection:
                        print(" [R]",end="")
                    else:
                        print(" [W]",end="")
            else:
                if j%2==0:
                    a=pure(((i//2,j//2),(i//2+1,j//2)))
                    if a in connection:
                        print(" [R]",end="")
                    else:
                        print(" [W]",end="")
                else:
                    print(" [W]",end="")
        print(" [W]")
    print("[W]"+" [W]"*(width*2))
            



cmd="3 3\n0,0 0,1;1,1 1,2"
#你自己解析cmd到height,width,connection里面去
height=3
width=3
connection={pure(((0,0),(0,1))),pure(((1,1),(1,2)))}
printMaze(height,width,connection)

