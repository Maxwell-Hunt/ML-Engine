import tensorflow as tf
import numpy as np

def generate_data(size):
    X = np.array([-5.0 + i * 10 / size for i in range(size)]).astype(np.float32)
    Y = 2 * X
    return (X, Y)

if __name__ == '__main__':
    DATA_SIZE = 100
    X, Y = generate_data(DATA_SIZE)
    w = tf.Variable(1.0, dtype=tf.float32)
    alpha = 0.01
    num_iterations = 25

    for iteration in range(1, num_iterations+1):
        with tf.GradientTape() as tape:
            result = X * w
            squared_error = tf.square(result - Y)
            mse = tf.reduce_sum(squared_error) / DATA_SIZE
        print("W:", w.numpy(), "MSE:", mse.numpy())
        dw = tape.gradient(mse, w)
        w.assign_sub(dw * alpha)
