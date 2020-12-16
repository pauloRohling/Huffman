#include <Clock.h>
#include <HuffmanAlgorithm.h>

//SEQUENCIAL
const int NUMBERS_OF_TESTS = 40;
const string TESTS_FILEPATH = "tests.txt";

double CalculateAverageTime() {
    LinkedList<double>* list = FileManager::GetTimes(TESTS_FILEPATH);
    int listSize = list->GetSize();
    double avg = 0.0;

    for(int i = 0; i < listSize; i++)
        avg += list->GetValueAt(i);

    avg = avg / listSize;
    FileManager::RegisterLine("Average: " + to_string(avg), TESTS_FILEPATH);
    return avg;
}

int main() {
    FileManager::EraseFile(TESTS_FILEPATH);
    Clock* clock = new Clock();
    HuffmanAlgorithm* root = new HuffmanAlgorithm();

    for(int i = 0; i < NUMBERS_OF_TESTS; i++) {
        clock->Tick();

        cout << "#" << i+1 << endl;
        cout << "######################## ENCODING ########################" << endl;
        root->Encode("prologue.txt", "prologueText.txt", "prologueKey.txt");
        root->Reset();

        cout << "######################## DECODING ########################" << endl;
        root->Decode("prologue2.txt", "prologueText.txt", "prologueKey.txt");
//        cout << "TARGET TEXT: " << endl << root->GetTargetText() << endl << endl;
//        cout << "BIT TEXT: " << endl << root->GetBitText() << endl << endl;
//        cout << "HUFFMAN TEXT: " << endl << root->GetHuffmanText() << endl << endl << endl << endl;

        double timeSpent = clock->Tock();
        cout << "Time spent: " << timeSpent << " seconds." << endl << endl;
        FileManager::RegisterLine(to_string(timeSpent), TESTS_FILEPATH);

        root->Reset();
    }

    cout << endl << "Average time: " << CalculateAverageTime() << endl;

    return 0;
}

