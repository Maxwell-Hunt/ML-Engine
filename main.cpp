#include <iostream>
#include "operations.h"
#include "variable.h"

int main() {
    float alpha = 0.05;
    float sw = 3;
    float sb = 0;

    float x1 = 0;
    float y1 = 1;
    float x2 = 1;
    float y2 = 5;

    for(std::size_t i = 0;i < 100;i++) {
        Variable w(sw);
        Variable b(sb);

        

        auto m = w * x1;
        auto result1 = m + b;

        auto m2 = w * x2;
        auto result2 = m2 + b;

        auto neg1 = result1 * -1;
        auto neg2 = result2 * -1;

        auto error1 = y1 + neg1;
        auto error2 = y2 + neg2;

        auto se1 = error1 * error1;
        auto se2 = error2 * error2;

        auto total = se1 + se2;
        auto mse = total * 0.5;

        float value = mse.getValue();
        float partialW = mse.getPartial(w);
        float partialB = mse.getPartial(b);

        std::cout << value << ' ' << sw << ' ' << sb << std::endl;

        // Update w and b
        sw -= alpha * partialW;
        sb -= alpha * partialB;
    }
    
}