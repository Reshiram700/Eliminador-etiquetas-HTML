#include <iostream>
#include <fstream>
#include <chrono>
#include <iomanip>
#include <sstream>
#include <filesystem>

namespace fs = std::filesystem;  // O std::experimental::filesystem en compiladores más antiguos
using namespace std;
using namespace chrono;

// Función para abrir un archivo HTML y medir el tiempo
double openFileAndMeasureTime(const string& filename) {
    auto start = high_resolution_clock::now();

    ifstream file(filename);
    if (!file.is_open()) {
        cerr << "Error al abrir el archivo: " << filename << endl;
        return -1.0;
    }

    

    auto stop = high_resolution_clock::now();
    auto duration = duration_cast<microseconds>(stop - start);

    file.close();

    return duration.count() / 1000000.0; // Convertir a segundos
}

int main() {
    auto programStart = high_resolution_clock::now();

    cout << "Carpeta de trabajo actual: " << fs::current_path() << endl;

    //Aqui va el nombre que tendra el .txt generado.
    ofstream outputFile("a1_02967752.txt");

    //Esta es la ruta donde estan los HTML para poder tomarlos.
    const string folderPath = "C:\\Users\\Alberto Iwakura\\Documents\\CS13309_Archivos_HTML";

    double totalExecutionTime = 0.0;

    for (const auto& entry : fs::directory_iterator(folderPath)) {
        if (entry.is_regular_file()) {
            string fileName = entry.path().string();
            double fileOpenTime = openFileAndMeasureTime(fileName);

            if (fileOpenTime >= 0.0) {
                cout << "Tiempo para abrir " << fileName << ": " << fileOpenTime << " segundos." << endl;
                totalExecutionTime += fileOpenTime;

                outputFile << fileName << "\n" << fileOpenTime << "\n";
            }
            else {
                cerr << "Error al abrir el archivo: " << fileName << endl;
            }
        }
    }

    outputFile << "tiempo total en abrir los archivos: " << totalExecutionTime << " segundos." << endl;

    auto programStop = high_resolution_clock::now();
    auto programDuration = duration_cast<microseconds>(programStop - programStart);

    outputFile << "tiempo total de ejecucion: " << programDuration.count() / 1000000.0 << " segundos." << endl;

    outputFile.close();

    return 0;
}
