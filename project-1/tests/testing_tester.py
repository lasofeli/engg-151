import numpy as np
a = [6.992910, -1.375411, 4.276618, -0.400988, -4.893978, -0.915307, 6.201126, -2.670361, -0.440006, 7.291312]
b = [-3.638921, 8.013934, -7.677038, 1.516225, 8.642968, -0.620921, 1.063347]
norm_a = np.linalg.norm(a)
norm_b = np.linalg.norm(b)
a = a / norm_a
b = b / norm_b
c = np.correlate(a, b, mode = 'full')
print(c)
