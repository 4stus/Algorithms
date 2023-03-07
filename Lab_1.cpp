#include <iostream>
#include <fstream>
#include <istream>
#include <string>
#include <random>



/* Обработчик исключений */
enum fileErrors
{
    errorFileOpening,
};

bool CreateFileWithRandomNumbers(const std::string& fileName, const int numbersCount, const int maxNumberValue)
{
    std::ofstream File;
    File.open(fileName);
    if (!File.is_open())
    {
        throw errorFileOpening;
    }
    else
    {
    }

    srand(time(NULL));
    for (int i = 0; i < numbersCount; ++i)
    {
        int random_digit = rand() % maxNumberValue;
        File << random_digit << std::endl;
    }

    File.close();
    return true;
}

bool  isFileContainsSortedArray(const std::string& fileName)
{
    std::ifstream labFile;
    labFile.open(fileName);

    if (!labFile.is_open())
    {
        throw errorFileOpening;
    }

    int previousNumber = -1;
    int currentNumber = -1;

    while (labFile >> currentNumber)
    {
        if (currentNumber < previousNumber)
        {
            return false;
        }

        previousNumber = currentNumber;
    }
    return true;
}

void SplitFile(const std::string& InputFileName, const std::string& OutputFileName1, const std::string& OutputFileName2)
{
    std::ifstream InputFile;
    InputFile.open(InputFileName);
    if (!InputFile.is_open())
    {
        throw errorFileOpening;
    }

    std::ofstream OutputFiles[2];
    OutputFiles[0].open(OutputFileName1);
    OutputFiles[1].open(OutputFileName2);

    int n = 0;
    int item_now;
    int item_prev;
    InputFile >> item_prev;
    while (InputFile >> item_now)
    {
        OutputFiles[n] << item_prev << std::endl;
        if (item_prev > item_now)
            n = 1 - n;
        item_prev = item_now;
    }
    OutputFiles[n] << item_prev << std::endl;;

    InputFile.close();
    OutputFiles[0].close();
    OutputFiles[1].close();
}

bool MergeFiles(const std::string& InputFileName1, const std::string& InputFileName2, const std::string& OutputFileName1, const std::string& OutputFileName2)
{
    std::ifstream InputFiles[2];
    InputFiles[0].open(InputFileName1);
    InputFiles[1].open(InputFileName2);
    std::ofstream OutputFiles[2];
    OutputFiles[0].open(OutputFileName1);
    OutputFiles[1].open(OutputFileName2);



    int m = 0, n = 0, items_now[2], items_prev[2];

    InputFiles[0] >> items_now[0];
    if (!(InputFiles[1] >> items_now[1]))
        return 1;
    items_prev[0] = items_now[0];
    items_prev[1] = items_now[1];

    while (!InputFiles[0].eof() && !InputFiles[1].eof())
    {
        if (items_prev[m] > items_prev[1 - m])
            m = 1 - m;
        OutputFiles[n] << items_prev[m] << std::endl;
        InputFiles[m] >> items_now[m];
        if (InputFiles[m].eof() || items_prev[m] > items_now[m])
        {
            m = 1 - m;
            OutputFiles[n] << items_prev[m] << std::endl;
            InputFiles[m] >> items_now[m];
            while (!InputFiles[m].eof() && items_prev[m] <= items_now[m])
            {
                items_prev[m] = items_now[m];
                OutputFiles[n] << items_prev[m] << std::endl;
                InputFiles[m] >> items_now[m];
            }
            items_prev[1 - m] = items_now[1 - m];
            n = 1 - n;
        }
        items_prev[m] = items_now[m];
    }

    for (int i = 0;i < 2;i++)
    {
        while (!InputFiles[i].eof())
        {
            OutputFiles[n] << items_prev[i] << std::endl;
            InputFiles[i] >> items_now[i];
            if (items_prev[i] > items_now[i])
            {
                n = 1 - n;
            }
            items_prev[i] = items_now[i];
        }
    }

    for (int i = 0; i < 2;i++)
    {
        OutputFiles[i].close();
        InputFiles[i].close();
    }
    return 0;
}

void SortFile(const std::string& InputFileName)
{
    std::string FileNames[4] = { "F1.txt","F2.txt","F3.txt","F4.txt" };

    SplitFile(InputFileName, FileNames[0], FileNames[1]);
    while (!MergeFiles(FileNames[0], FileNames[1], FileNames[2], FileNames[3]))
    {
        std::string tmp;
        for (int i = 0; i < 2; i++)
        {
            tmp = FileNames[i + 2];
            FileNames[i + 2] = FileNames[i];
            FileNames[i] = tmp;
        }
    }
    std::cout << FileNames[0];
}

int main()
{
    setlocale(LC_ALL, "Rus");

    std::string mainFile = "Array.txt";
    const int numbersCount = 100;
    CreateFileWithRandomNumbers(mainFile, numbersCount, 100);

    isFileContainsSortedArray(mainFile);

    SortFile(mainFile);
    system("pause");
}