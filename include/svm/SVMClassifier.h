#ifndef SVMCLASSIFIER_H
#define SVMCLASSIFIER_H

#include <QObject>
#include <QMutex>

#include "include/svm/svm.h"

#include <string>
#include <vector>


class SVMClassifier : public QObject
{
    Q_OBJECT
public:

    SVMClassifier();
    ~SVMClassifier();

    bool load_data( QString file_path );
    bool load_model( QString file_path );
    void train();
    double predict( const svm_node *x );
    void cross_validate(int nfold);
    const svm_model* model() const { return model_; }
    svm_parameter& parameters() { return param_; }
    const svm_problem& problem() const { return problem_; }
    const std::vector<std::vector<double> >& samples() const { return samples_; }

    int nfeatures() const { return samples_.front().size()-1; }

private:
    std::vector<std::vector<double> > samples_;     // to store the raw samples (because the svm_problem
                                                    // uses a sparse format and will discard 0 values)
    svm_problem problem_;
    svm_parameter param_;
    svm_model *model_;

    // because it's easier to manage vectors instead of simple arrays
    std::vector<svm_node> inputs_;
    std::vector<svm_node*> rows_;
    std::vector<double> labels_;

    void get_samples(QString file_path);

    /**
      * Mutex for paralelism
      */
    static QMutex mutex;
};

#endif // SVMCLASSIFIER_H
