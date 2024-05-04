#include "context.h"
#include <iostream>

int main() {
    float learningRate = 0.05;

    float x1 = 0;
    float y1 = 1;
    float x2 = 1;
    float y2 = 5;

    float fw = 3;
    float fb = 0;

    for(std::size_t i = 0;i < 30;i++) {
        Context context;
        const Expression& w = context.createVariable(fw);
        const Expression& b = context.createVariable(fb);

        const Expression& error1 = (w * x1 + b - y1);
        const Expression& error2 = (w * x2 + b - y2);

        const Expression& se1 = error1 * error1;
        const Expression& se2 = error2 * error2;

        const Expression& mse = (se1 + se2) / 2;

        std::cout << "MSE: " << mse.getValue() << " w: " << w.getValue() << " b: " << b.getValue() << std::endl;

        float dw = mse.getPartial(w);
        float db = mse.getPartial(b);

        fw -= dw * learningRate;
        fb -= db * learningRate;
    }
}