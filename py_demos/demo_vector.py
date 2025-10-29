# search for libraray like bla.cpython-312-darwin.so in the build directory:
import sys
# sys.path.append('/Users/joachim/texjs/lva/ws2324/ScientificComputing/ASC-bla/build')
# sys.path.append('../build')
# from bla import Vector

# import from the installed ASCsoft package:
import sys
sys.path.append('/mnt/c/Users/jonas/Documents/Studium/Bachelor Technische Mathematik/Einführung in Scientific Computing/ASC-bla/ASC-bla_old/build')
sys.path.append("D:\\Users\\screic\\Documents\\Studium\\scicomp\\ASC-bla\\ASC-bla\\build\\Release")
from bla import Vector

x = Vector(3)
y = Vector(3)

for i in range(len(x)):
    x[i] = i
y[:] = 2    

print ("x =", x)
print ("y =", y)
print ("x+3*y =", x+3*y)


x = Vector(10)
x[0:] = 1
print (x)

x[3:7] = 2
print (x)

x[0:10:2] = 3
print (x)



