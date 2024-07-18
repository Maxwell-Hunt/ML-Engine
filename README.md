# Autograd Engine Project

Autograd engine written using C++20.

## Current Features

* Interface for numerically computing the derivative of expressions involving simple operations using reverse mode automatic differentiation
* Templated iterable tensors with arbitary rank
* Iterface for creating and training customizable deep neural networks

## What I learned

Over the course of this project I learned about how libraries such as Keras and Pytorch use automatic differentiation to train the deep learning models that are built with their APIs.

I also learned a lot about C++ as well as general object oriented design. After this project I find myself with a better understanding of patterns and principles such as **RAII, Strategy Design Pattern, Iterator Design Pattern and C++ Templating**.

## Example
The following is a simple linear regression example using the engine's autodiff functionality to compute the partials.
```cpp
#include <iostream>
#include "../expression.h"
#include "../context.h"
int main() {
    using namespace Engine;
    const std::size_t DATA_SIZE = 100;
    const auto& [X, Y] = generateData(DATA_SIZE);
    
    Expression w = createVariable(1.f);
    float alpha = 0.01;

    const unsigned int NUM_ITERATIONS = 25;

    for(unsigned int iteration = 1;iteration <= NUM_ITERATIONS;iteration++) {
        Tensor<Expression> result = X * w;
        Tensor<Expression> squaredError = square(result - Y);
        Expression mse = reduceAdd(squaredError) / (float)DATA_SIZE;

         std::cout << "W: " << w.getValue() << " MSE: " << mse.getValue() << std::endl;

        computeGradients(mse);
        float gradient = w.getPartial();
        w = createVariable(w.getValue() - alpha * gradient);
    }
}
```
This example is equivalent to the following python program which uses Tensorflow, and produces exactly the same results.

```py
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
```

More examples, including those involving deep neural networks can be seen in the examples folder.
## Build Instructions

To build this project you must have g++ version 13.2.0 or higher
To build any of the examples in this project clone the repository, navigate to the examples folder and run 
`g++ -std=c++20 -o "executable name" example.cpp ../*.cpp`

## For the Future

This project is very much still in development and I hope to add many features and make improvements to the design of the project in the near future.  Some of the improvements I plan to add are listed below.

* Implementing training with batches
* Implementing different optimizers and using the strategy pattern to decouple the optimization algorithm from the structure of the model
* Improve efficiency by using a multithreaded algorithm for matrix multiplication
* Use Variadic Templates to build the shape of a Tensor into its type so that the validity of performing certain operations can be checked at compile time rather than at runtime

