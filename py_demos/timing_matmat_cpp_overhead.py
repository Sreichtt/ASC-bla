import time
import sys
sys.path.append('/mnt/c/Users/jonas/Documents/Studium/Bachelor Technische Mathematik/Einführung in Scientific Computing/ASC-bla/ASC-bla_old/build')
from bla import Matrix
import matplotlib.pyplot as plt

ns = [10, 100, 1000]

total_times = []
cpp_times = []
overheads = []

for n in ns:
    A = Matrix(n, n)
    B = Matrix(n, n)
    for i in range(n):
        for j in range(n):
            A[i, j] = i + j
            B[i, j] = i - j

    t0 = time.perf_counter()
    C, cpp_time = A.matmul_with_time(B)   # C++-Multiplikation mit interner Zeitmessung
    t1 = time.perf_counter()
    total_time = t1 - t0

    # Python-Overhead berechnen
    overhead = total_time - cpp_time

    total_times.append(total_time)
    cpp_times.append(cpp_time)
    overheads.append(overhead)

    print(f"n={n}: total={total_time:.6e}s, cpp={cpp_time:.6e}s, overhead={overhead:.6e}s") #6 Nachkommastellen, Potenzdarstellung

plt.figure(figsize=(8,6))
plt.plot(ns, total_times, "o-", label="Total (Python + C++)")
plt.plot(ns, cpp_times, "s-", label="C++ time")
plt.plot(ns, overheads, "^-", label="Python overhead")

plt.xscale("log")
plt.yscale("log")
plt.xlabel("Matrix size n")
plt.ylabel("Time [s]")
plt.title("Matrix-Matrix Multiplication Timings (like timing_matmat.py)")
plt.grid(True, which="both", linestyle="--", linewidth=0.5)
plt.legend()
plt.tight_layout()
plt.show()
