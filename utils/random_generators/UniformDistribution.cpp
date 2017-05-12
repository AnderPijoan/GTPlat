#include "UniformDistribution.h"

UniformDistribution::UniformDistribution(QObject *parent) : QObject(parent){
    // Initialize uniform distribution generator
    this->uniform_distribution = std::uniform_real_distribution<double> (0.0,1.0);
}

UniformDistribution::~UniformDistribution(){
}

/** Generate value from uniform distribution
 * @brief uniformDistribution
 * @return
 */
double UniformDistribution::uniformDistribution(){
    std::random_device rd;
    std::mt19937 generator(rd());
    return this->uniform_distribution( generator );
}
