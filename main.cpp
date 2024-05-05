#include "context.h"
#include <iostream>

Expression MSE(float x1, float y1, float x2, float y2, const Expression& w, const Expression& b) {
    Context& context = w.getContext();
    const Expression& se1 = context.square(w * x1 + b - y1);
    const Expression& se2 = context.square(w * x2 + b - y2);

    return (se1 + se2) / 2;
}

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
        Expression w = context.createVariable(fw);
        Expression b = context.createVariable(fb);

        Expression mse = MSE(x1, y1, x2, y2, w, b);
        
        std::cout << "MSE: " << mse.getValue() << " w: " << w.getValue() << " b: " << b.getValue() << std::endl;

        float dw = mse.getPartial(w);
        float db = mse.getPartial(b);

        fw -= dw * learningRate;
        fb -= db * learningRate;
    }
}