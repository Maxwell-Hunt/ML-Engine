import tensorflow as tf
import numpy as np

def generate_data(n):
    # Ensure n is a perfect square by finding the closest lower perfect square
    side_length = int(np.sqrt(float(n)))
    n = side_length ** 2
    
    # Generate the grid points
    X = []
    Y = []
    step = 1 / (side_length - 1) if side_length >= 1 else 1
    for i in range(side_length):
        for j in range(side_length):
            y = i * step
            x = j * step
            X.append([x, y])
            Y.append([float(X[-1][1] > X[-1][0])])
    
    return (np.array(X).astype(np.float32), np.array(Y).astype(np.float32))


learning_rate = 1
data_size = 9
X, Y = generate_data(data_size)

w1 = tf.Variable(np.ones((2, 2)).astype(np.float32) / 2.0, dtype=tf.float32)
w2 = tf.Variable(np.ones((2, 1)).astype(np.float32) / 2.0, dtype=tf.float32)

num_iters = 100
for iters in range(1, num_iters + 1):
    with tf.GradientTape() as tape:
        hidden = tf.matmul(X, w1)
        output = tf.keras.activations.sigmoid(tf.matmul(hidden, w2))

        # accuracy = tf.reduce_mean(tf.cast(tf.equal(tf.round(output), Y), tf.float32))
        accuracy = 0
        for i in range(data_size):
            if(abs(round(output.numpy()[i][0]) - Y[i][0]) < 0.01) : accuracy += 1.0 / data_size
        error = (-1 / data_size) * tf.reduce_sum( output + output)
       # errors = Y * tf.math.log(output) + (1 - Y) * tf.math.log(1 - output)
       # error = -1.0/data_size * tf.reduce_sum(errors)
        
    
    print(f"Iteration {iters}, Error: {error.numpy()}, Accuracy: {accuracy}")
    dw1, dw2, dh, do = tape.gradient(error, [w1, w2, hidden, output])
    # print("DW1:")
    # print(dw1)
    # print("DW2:")
    # print(dw2)
    # print("DHIDDEN: ")
    # print(dh)
    # print("DOUTPUT: ")
    # print(do)
    w1.assign_sub(dw1 * learning_rate)
    w2.assign_sub(dw2 * learning_rate) 