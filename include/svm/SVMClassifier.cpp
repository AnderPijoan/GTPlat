#include "SVMClassifier.h"

#include <QDebug>
#include <QFile>

#include <iostream>
#include <vector>
#include <memory>
#include <string>
#include <sstream>
#include <fstream>
#include <cstdlib>
#include <stdexcept>
#include <numeric>
#include <algorithm>
#include <cmath>

using namespace std;

namespace {
struct split
{
    enum empties_t { empties_ok, no_empties };
};
}

namespace util {
vector<string>& split_string(vector<string>&            result,
                             const string&              s,
                             const string&              delimiters,
                             split::empties_t empties = split::empties_ok)
{
    result.clear();
    size_t current;
    size_t next = -1;
    do {
        if (empties == split::no_empties) {
            next = s.find_first_not_of( delimiters, next + 1 );
            if (next == string::npos) break;
            next -= 1;
        }
        current = next + 1;
        next = s.find_first_of( delimiters, current );
        result.push_back( s.substr( current, next - current ) );
    }
    while (next != string::npos);
    return result;
}
}

QMutex SVMClassifier::mutex;

SVMClassifier::SVMClassifier() : model_(0) {
    // set some defaults
    param_.svm_type = C_SVC;
    param_.kernel_type = LINEAR;
    param_.degree = 3;
    param_.gamma = 0;	// 1/num_features
    param_.coef0 = 0;
    param_.nu = 0.5;
    param_.cache_size = 100;
    param_.C = 1;
    param_.eps = 1e-3;
    param_.p = 0.1;
    param_.shrinking = 1;
    param_.probability = 0;
    param_.nr_weight = 0;
    param_.weight_label = 0;
    param_.weight = 0;
}


SVMClassifier::~SVMClassifier() {
    if(model_ != 0) svm_free_and_destroy_model(&model_);
    svm_destroy_param(&param_);
}


void SVMClassifier::get_samples(QString file_path) {

    QByteArray ba = file_path.toLatin1();
    const char *c_str = ba.data();

    ifstream istr(c_str);
    samples_.clear();
    string line;
    while(std::getline(istr, line)) {
        vector<string> fields;
        util::split_string(fields, line, " ");
        vector<double> values;
        for (auto it = begin(fields); it != end(fields); ++it) {
            vector<string> tokens;
            util::split_string(tokens, *it, ":");
            values.push_back(atof(tokens.back().c_str()));
        }
        samples_.push_back(values);
        values.clear();
    }
}


bool SVMClassifier::load_data(QString file_path) {
    QFile file(file_path);
    if( !file.exists() ){
        qWarning() << "[SVMClassifier::load_data] File" << file_path << "not found" << endl;
        return false;
    }

    try {
        get_samples(file_path);
    }  catch (string &err) {
        qWarning() << "[SVMClassifier::load_data] Coult not get samples from file" << file_path << endl;
        return false;
    }

    labels_.clear(); inputs_.clear(); rows_.clear();
    unsigned maxindex = 0;
    for_each(begin(samples_), end(samples_), [&maxindex](vector<double>& v){ if (maxindex < v.size()) maxindex = v.size(); });
    --maxindex; // because the first column is actually the labels

    if (param_.gamma == 0 && maxindex > 0)
        param_.gamma = 1.0 / maxindex;

    // go over samples matrix and convert values into svm sparse-format node matrix (see libSVM README)
    // here we assume the labels are always in the first column
    for (auto s : samples_) {
        labels_.push_back(s[0]);
        for (unsigned int i = 1; i != s.size(); ++i) {
            if (s[i] == 0) continue;
            inputs_.push_back( { i , s[i] } );
        }
        inputs_.push_back( {-1, -0.0 } ); // index=-1 indicates the end of one vector (see libSVM README)
    }

    // update the problem
    problem_.l = labels_.size();
    problem_.y = labels_.data();

    auto it = begin(inputs_);
    while (it->index == -1) ++it; // just paranoid
    rows_.push_back(&(*it));

    if ((end(inputs_)-1)->index != -1) {
        qWarning() << "Warning: possibly corrupted input (last index = " << (end(inputs_)-1)->index << ")" << endl;
    }
    for (; it != end(inputs_)-1; ++it) {
        if (it->index == -1) {
            rows_.push_back(&(*(++it)));
        }
    }
    problem_.x = rows_.data();

    return true;
}


bool SVMClassifier::load_model(QString file_path) {
    if ( model_ != 0 ){
        svm_free_and_destroy_model(&model_);
    }

    QFile file(file_path);
    if( !file.exists() ){
        qWarning() << "[SVMClassifier::load_model] File" << file_path << "not found" << endl;
        return false;
    }

    QByteArray ba = file_path.toLatin1();
    const char *c_str = ba.data();

    try {
        SVMClassifier::mutex.lock();
        model_ = svm_load_model( c_str );
        SVMClassifier::mutex.unlock();
    } catch (...){
        return false;
    }
    return true; // if the model was loaded successfully
}


double SVMClassifier::predict( const svm_node* x ) {
    if (model_ == 0) {
        qWarning() << "[SVMClassifier::predict] Model is null" << endl;
        return log(-1);
    }
    try{
        return svm_predict(model_, x);
    } catch(...){ }
    return log(-1);
}


void SVMClassifier::train() {
    model_ = svm_train(&problem_, &param_);
}


void SVMClassifier::cross_validate(int nfold) {
    int total_correct = 0, length = problem_.l;
    double total_error = 0;
    vector<double> target(length);
    svm_cross_validation(&problem_, &param_, nfold, target.data());

    if (param_.svm_type == EPSILON_SVR || param_.svm_type == NU_SVR) {
        double sumv = 0, sumy = 0, sumvv = 0, sumyy = 0, sumvy = 0;
        for (int i = 0; i != length; ++i) {
            double y = labels_[i];
            double v = target[i];
            total_error += (v-y)*(v-y);
            sumv += v;
            sumy += y;
            sumvv += v*v;
            sumyy += y*y;
            sumvy += v*y;
        }
        double mse = total_error / length;
        double scc = ( (length * sumvy - sumv * sumy) * (length * sumvy - sumv * sumy) ) /
                     ( (length * sumvv - sumv * sumv) * (length * sumyy - sumy * sumy) );
        cout << "Cross Validation Mean squared error = " << mse << endl;
        cout << "Cross Validation Squared correlation coefficient = " << scc << endl;
    } else {
        for (int i = 0; i != length; ++i) {
            if (target[i] == labels_[i]) ++total_correct;
        }
        double acc = 100.0 * total_correct / length;
        cout << "Cross validation accuracy = " << acc << endl;
    }
}
