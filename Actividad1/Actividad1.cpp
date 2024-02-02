#include <iostream>
#include <fstream>
#include <chrono>
#include <iomanip>
#include <sstream>
#include <filesystem>
#include <regex> // Necesario para usar expresiones regulares

namespace fs = std::filesystem;
using namespace std;
using namespace chrono;

double openFileAndMeasureTime(const string& filename) {
    auto start = high_resolution_clock::now();

    ifstream file(filename);
    if (!file.is_open()) {
        cerr << "Error al abrir el archivo: " << filename << endl;
        return -1.0;
    }

    // Leer el contenido del archivo HTML
    stringstream buffer;
    buffer << file.rdbuf();
    string content = buffer.str();

    // Eliminar etiquetas HTML usando una expresión regular
    regex htmlTagsRegex("<[^>]*>");
    content = regex_replace(content, htmlTagsRegex, "");

    auto stop = high_resolution_clock::now();
    auto duration = duration_cast<microseconds>(stop - start);

    file.close();

    return duration.count() / 1000000.0;
}

int main() {
    auto programStart = high_resolution_clock::now();

    cout << "Carpeta de trabajo actual: " << fs::current_path() << endl;

    string outputFileName;
    cout << "Ingrese el nombre del archivo de salida: ";
    cin >> outputFileName;

    outputFileName += ".txt"; //Extension de salida del archivo generado.

    ofstream outputFile(outputFileName);

    const string folderPath = "C:\\Users\\Alberto Iwakura\\Documents\\CS13309_Archivos_HTML";

    double totalOpenTime = 0.0;
    double totalRemoveTagsTime = 0.0;

    for (const auto& entry : fs::directory_iterator(folderPath)) {
        if (entry.is_regular_file()) {
            string fileName = entry.path().string();

            // Medir el tiempo de apertura
            double fileOpenTime = openFileAndMeasureTime(fileName);
            if (fileOpenTime >= 0.0) {
                cout << "Tiempo para abrir " << fileName << ": " << fileOpenTime << " segundos." << endl;
                totalOpenTime += fileOpenTime;

                // Medir el tiempo para eliminar las etiquetas
                double removeTagsTime = openFileAndMeasureTime(fileName);
                cout << "Tiempo para eliminar las etiquetas en " << fileName << ": " << removeTagsTime << " segundos." << endl;
                totalRemoveTagsTime += removeTagsTime;

                outputFile << "Archivo: " << fileName << "\nTiempo de apertura: " << fileOpenTime << " segundos."
                    << "\nTiempo de eliminación de etiquetas: " << removeTagsTime << " segundos.\n\n";
            }
            else {
                cerr << "Error al abrir el archivo: " << fileName << endl;
                outputFile << "Error al abrir el archivo: " << fileName << "\n\n";
            }
        }
    }

    outputFile << "Tiempo total en abrir los archivos: " << totalOpenTime << " segundos." << endl;
    outputFile << "Tiempo total en eliminar etiquetas: " << totalRemoveTagsTime << " segundos." << endl;

    auto programStop = high_resolution_clock::now();
    auto programDuration = duration_cast<microseconds>(programStop - programStart);

    outputFile << "Tiempo total de ejecucion: " << programDuration.count() / 1000000.0 << " segundos." << endl;

    outputFile.close();

    return 0;
}
