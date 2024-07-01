import numpy as np
import tensorflow as tf

F = np.array([[0.05, 0.07], [0.1, 3.0]])
A = tf.Variable(np.array([[0.0013, 0.002], [0.0018, 0.005]]))
B = tf.Variable(np.array([[0.007], [0.0011]]))
with tf.GradientTape() as tape:
    C = tf.keras.activations.sigmoid(tf.matmul(tf.matmul(F, A), B))
    D = 1 * tf.math.log(1-C)
    result = (1/5) * tf.reduce_sum(D)

dA, dB = tape.gradient(result, [A, B])
print(dA, '\n\n', dB)