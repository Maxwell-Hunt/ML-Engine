import tensorflow as tf
import numpy as np
import pandas as pd
from sklearn.preprocessing import OneHotEncoder

print("Reading Data...")
train_dataset = pd.read_csv("../data/mnist_train.csv").head(10_000)
test_dataset = pd.read_csv("../data/mnist_test.csv").head(1000)

encoder = OneHotEncoder(sparse_output = False)

X_train = train_dataset.drop("label", axis=1).to_numpy() / 255.0
Y_train = encoder.fit_transform(train_dataset["label"].to_numpy().reshape(-1, 1))

X_test = test_dataset.drop("label", axis=1).to_numpy() / 255.0
Y_test = encoder.transform(test_dataset["label"].to_numpy().reshape(-1, 1))
print("Finished Reading Data")

# print("Creating Network")
# nn = tf.keras.models.Sequential([
#     tf.keras.layers.Input(shape=(784,)),
#     tf.keras.layers.Dense(128, activation=tf.keras.activations.relu),
#     tf.keras.layers.Dense(128, activation=tf.keras.activations.relu),
#     tf.keras.layers.Dense(10, activation=tf.keras.activations.softmax)
# ])
# nn.compile(loss=tf.keras.losses.CategoricalCrossentropy(), optimizer=tf.keras.optimizers.Adam(), metrics=tf.keras.metrics.CategoricalAccuracy())
# print("Created Network")

# print("Training Network")
# nn.fit(X_train, Y_train, epochs=10, batch_size=32)
# print("Finished Training")

print("Creating Network")
nn = tf.keras.models.Sequential([
    tf.keras.layers.Input(shape=(784,)),
    tf.keras.layers.Dense(16, activation=tf.keras.activations.relu),
    tf.keras.layers.Dense(16, activation=tf.keras.activations.relu),
    tf.keras.layers.Dense(10, activation=tf.keras.activations.softmax)
])
nn.compile(loss=tf.keras.losses.MeanSquaredError(), optimizer=tf.keras.optimizers.SGD(learning_rate=0.005), metrics=tf.keras.metrics.CategoricalAccuracy())
print("Created Network")

print("Training Network")
nn.fit(X_train, Y_train, epochs=1, batch_size=1)
print("Finished Training")


print("Testing")
nn.evaluate(X_test, Y_test)
print("Finished Testing")
