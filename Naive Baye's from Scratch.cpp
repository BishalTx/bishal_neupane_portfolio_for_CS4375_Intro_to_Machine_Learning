#include <iostream>

using namespace std;

#include <iostream>
#include <conio.h>
#include <string>
#include <vector>
#include <fstream>
#include <algorithm>
#include <cmath>
#include <chrono>

using namespace chrono;

vector<int> pclass;
vector<int> survived;
vector<int> sex;
vector<double> age;

// Function for reading file content and printing details of file
void OpeningFileForReading(vector<int> &pclass, vector<int> &survived, vector<int> &sex, vector<double> &age)
{
    fstream inFs;
    string pclass_in, survived_in, line;
    string sex_in, age_in;
    string row_no;

    cout << "Opening file titanic_project.csv" << endl;
    inFs.open("C:/Users/bisha/OneDrive/Desktop/Fall 2022/Machine Learning/Programming/titanic_project.csv", ios::in);
    if (inFs.is_open())
    {
        cout << "titanic_project.csv opened" << endl;
    }
    if (!inFs.is_open())
    {

        cout << "Could not open file titanic_project.csv" << endl;
    }

    cout << "Reading line 1 " << endl;

    getline(inFs, line);

    cout << "heading: " << line << endl;
    int numObservations = 0;
    while (inFs.good())
    {
        getline(inFs, row_no, ',');
        getline(inFs, pclass_in, ',');
        getline(inFs, survived_in, ',');
        getline(inFs, sex_in, ',');
        getline(inFs, age_in, '\n');
        pclass.push_back(stod(pclass_in));
        survived.push_back(stod(survived_in));
        sex.push_back(stod(sex_in));
        age.push_back(stod(age_in));
        numObservations++;
    }

    cout << "new length: " << pclass.size() << endl;
    cout << "Closing file titanic_project.csv" << endl;
    inFs.close();
    cout << "Number of records: " << numObservations << endl;
}

// Function to calculate zero or 1  in particular column
int zeroOrOneCount(int toCount, vector<int> column)
{
    int nRows = 800;
    int Count = 0;
    for (int i = 0; i < nRows; i++)
    {
        if (column[i] == toCount)
        {
            Count++;
        }
    }
    return Count;
}

// Function to calculate Aprior
vector<double> calculateAprior(vector<int> survived)
{
    vector<double> aprior;
    int nRows = 800;
    int zeroCount = zeroOrOneCount(0, survived);
    int oneCount = zeroOrOneCount(1, survived);

    // cout<< zeroCount << "," << oneCount<< endl;

    aprior.push_back((double)zeroCount / nRows);
    aprior.push_back((double)oneCount / nRows);
    return aprior;
}

//  Function to calculate likelihood for pclass
vector<vector<double>> likHoodPclass(vector<int> survived, vector<int> pclass)
{
    vector<vector<double>> lhPclass(2, vector<double>(3, 0));
    int zeroCount = zeroOrOneCount(0, survived);
    int oneCount = zeroOrOneCount(1, survived);

    for (int i = 0; i < 800; i++)
    {

        if (survived[i] == 0 && pclass[i] == 1)
        {
            lhPclass[0][0]++;

            // cout << sum << endl;
        }
        if (survived[i] == 0 && pclass[i] == 2)
        {
            lhPclass[0][1]++;
        }
        if (survived[i] == 0 && pclass[i] == 3)
        {
            lhPclass[0][2] = lhPclass[0][2] + 1;
        }
        if (survived[i] == 1 && pclass[i] == 1)
        {
            lhPclass[1][0]++;
        }
        if (survived[i] == 1 && pclass[i] == 2)
        {
            lhPclass[1][1]++;
        }
        if (survived[i] == 1 && pclass[i] == 3)
        {
            lhPclass[1][2]++;
        }
    }

    for (int i = 0; i < 2; i++)
    {
        for (int j = 0; j < 3; j++)
        {
            if (i == 0)
            {

                lhPclass[i][j] = lhPclass[i][j] / zeroCount;
            }

            if (i == 1)
            {
                lhPclass[i][j] = lhPclass[i][j] / oneCount;
            }
        }
    }

    return lhPclass;
}

// Likelihood for sex
vector<vector<double>> likHoodSex(vector<int> survived, vector<int> sex)
{
    vector<vector<double>> lhSex(2, vector<double>(2, 0));
    int zeroCount = zeroOrOneCount(0, survived);
    int oneCount = zeroOrOneCount(1, survived);

    for (int i = 0; i < 800; i++)
    {

        if (survived[i] == 0 && sex[i] == 0)
        {
            lhSex[0][0]++;

            // cout << sum << endl;
        }
        if (survived[i] == 0 && sex[i] == 1)
        {
            lhSex[0][1]++;
        }

        if (survived[i] == 1 && sex[i] == 0)
        {
            lhSex[1][0]++;
        }
        if (survived[i] == 1 && sex[i] == 1)
        {
            lhSex[1][1]++;
        }
    }

    for (int i = 0; i < 2; i++)
    {
        for (int j = 0; j < 2; j++)
        {
            if (i == 0)
            {

                lhSex[i][j] = lhSex[i][j] / zeroCount;
            }

            if (i == 1)
            {
                lhSex[i][j] = lhSex[i][j] / oneCount;
            }
        }
    }

    return lhSex;
}

// Mean for age
vector<double> calculateMeanAge(vector<double> age, vector<int> survived)
{
    vector<double> mean;
    int zeroCount = zeroOrOneCount(0, survived);
    int oneCount = zeroOrOneCount(1, survived);
    double age0 = 0;
    double age1 = 0;
    for (int i = 0; i < 800; i++)
    {
        if (survived[i] == 0)
        {
            age0 = age[i] + age0;
        }
        if (survived[i] == 1)
        {
            age1 = age[i] + age1;
        }
    }
    mean.push_back(age0 / zeroCount);
    mean.push_back(age1 / oneCount);
    return mean;
}

// calculating variance for age
vector<double> calculateVarianceAge(vector<double> age, vector<int> survived)
{
    vector<double> var;
    int zeroCount = zeroOrOneCount(0, survived);
    int oneCount = zeroOrOneCount(1, survived);
    double var0 = 0;
    double var1 = 0;
    double mean0 = calculateMeanAge(age, survived)[0];
    double mean1 = calculateMeanAge(age, survived)[1];
    for (int i = 0; i < 800; i++)
    {
        if (survived[i] == 0)
        {
            var0 = var0 + (mean0 - age[i]) * (mean0 - age[i]);
        }
        if (survived[i] == 1)
        {
            var1 = var1 + (mean1 - age[i]) * (mean1 - age[i]);
        }
    }
    var.push_back(var0 / zeroCount);
    var.push_back(var1 / oneCount);
    return var;
}

// calculating age likelihood
double calculateAgeLh(double age, double mean, double var)
{
    double pow = ((age - mean) * (age - mean)) / (2 * var);
    return 1 / sqrt(2 * 22 / 7 * var) * exp(-pow);
}

// calculating raw probability
vector<double> calculateRawProb(int pclass1, int sex1, double age1)
{
    vector<vector<double>> lhPclass = likHoodPclass(survived, pclass);
    vector<vector<double>> lhsex = likHoodSex(survived, sex);
    vector<double> apriori = calculateAprior(survived);
    vector<double> result(2, 0);
    double num_s = 0.0;
    double num_p = 0.0;
    double denominator = 0.0;
    vector<double> mean = calculateMeanAge(age, survived);
    vector<double> var = calculateVarianceAge(age, survived);

    num_s = lhPclass[1][pclass1] * lhsex[1][sex1] * apriori[1] * calculateAgeLh(age1, mean[1], var[1]);
    num_p = lhPclass[0][pclass1] * lhsex[0][sex1] * apriori[0] * calculateAgeLh(age1, mean[0], var[0]);
    denominator = num_s + num_p;
    result[0] = (num_p / denominator);
    result[1] = (num_s / denominator);

    return result;
}

// Calculating accuracy
int calculateAccuracy(vector<int> prediction)
{
    int trueResult = 0;
    for (int i = 800; i < 1046; i++)
    {
        if (survived[i] == prediction[i - 800])
        {

            trueResult++;
        }
    }
    return trueResult;
}

// calculating sensitivity
double calculateSensitivity(vector<int> prediction, vector<int> survived)
{
    double truePositive = 0;
    double falseNegative = 0;
    for (int i = 0; i < prediction.size(); i++)
    {
        if (survived[800 + i] == 1 && prediction[i] == 1)
        {
            truePositive++;
        }
        else if (survived[800 + i] == 1 && prediction[i] == 0)
        {
            falseNegative++;
        }
    }

    double sensitivity = truePositive / (truePositive + falseNegative);

    return sensitivity;
}

// calculating specificity
double calculateSpecificity(vector<int> prediction, vector<int> survived)
{
    double trueNegative = 0;
    double falsePositive = 0;
    for (int i = 0; i < 246; i++)
    {
        if (survived[800 + i] == 0 && prediction[i] == 0)
        {
            trueNegative++;
        }
        else if (survived[800 + i] == 0 && prediction[i] == 1)
        {
            falsePositive++;
        }
    }

    return (trueNegative / (trueNegative + falsePositive));
}

int main()
{

    OpeningFileForReading(pclass, survived, sex, age);
    auto startTime = steady_clock::now();
    vector<double> aprior = calculateAprior(survived);
    cout << "-------------------\n";
    cout << "\nAPrior \n";
    cout << "-------------------\n";
    cout << aprior[0] << ", " << aprior[1] << endl;

    vector<vector<double>> lhpclass = likHoodPclass(survived, pclass);
    cout << "----------------------\n";
    cout << "Likelihood for pclass \n";
    cout << "----------------------\n";
    cout << "Likelihood values for p(pclass|survived):\n";
    for (int i = 0; i < 2; i++)
    {
        for (int j = 0; j < 3; j++)
        {
            cout << lhpclass[i][j] << " ";
        }
        cout << endl;
    }

    vector<vector<double>> lhSex = likHoodSex(survived, sex);
    cout << "----------------------\n";
    cout << "Likelihood for sex \n";
    cout << "----------------------\n";
    cout << "Likelihood values for p(psex|survived):\n";
    for (int i = 0; i < 2; i++)
    {
        for (int j = 0; j < 2; j++)
        {
            cout << lhSex[i][j] << " ";
        }
        cout << endl;
    }
    auto endTime = steady_clock::now();
    // Printing test result
    double average0 = 0;
    double average1 = 0;
    vector<int> prediction;
    cout << "\n\nPrinting test result";
    cout << "\n------------------------\n\n";
    for (int i = 800; i < 1046; i++)
    {

        vector<double> rawResult = calculateRawProb(pclass[i], sex[i], age[i]);
        cout << rawResult[0] << " " << rawResult[1] << endl;
        if (rawResult[0] > rawResult[1])
        {
            prediction.push_back(0);
        }
        else
        {

            prediction.push_back(1);
        }
    }
    cout << "prediction size = " << prediction.size() << endl;
    // for(int i=0; i < prediction.size(); i++){
    //     cout << "pred = " << prediction[i] << "survival = " << survived[800-i] << endl;
    // }

    cout << "\n---------------------------\n";
    cout << "Accuracy = " << (double)calculateAccuracy(prediction) / 246 << endl;
    double sensitivity = calculateSensitivity(prediction, survived);
    cout << "Sensitivity = " << sensitivity << endl;
    double specificity = calculateSpecificity(prediction, survived);
    cout << "Specificity = " << specificity << endl;
    cout << "-------------------------------------\n";
    duration<double> elapsed_time = endTime - startTime;
    cout << "Time taken for training: " << elapsed_time.count() << "s\n\n";
    cout << "---------------------------------------\n\n";
}
