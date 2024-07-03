#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <algorithm>
#include <sstream>
#define MAX 100

using namespace std;

// Función para calcular la distancia de edición entre dos cadenas
int editDistance(const string& s1, const string& s2) {
    int m = s1.length();
    int n = s2.length();

    vector<vector<int>> dp(m + 1, vector<int>(n + 1, 0));

    for (int i = 0; i <= m; i++) {
        for (int j = 0; j <= n; j++) {
            if (i == 0) {
                dp[i][j] = j;
            } else if (j == 0) {
                dp[i][j] = i;
            } else if (s1[i - 1] == s2[j - 1]) {
                dp[i][j] = dp[i - 1][j - 1];
            } else {
                dp[i][j] = 1 + min({ dp[i - 1][j], dp[i][j - 1], dp[i - 1][j - 1] });
            }
        }
    }

    return dp[m][n];
}

// Función para cargar datos desde un archivo a un array
int cargarDesdeArchivo(const string& nombreArchivo, string array[MAX]) {
    ifstream archivo(nombreArchivo);
    int count = 0;

    if (archivo.is_open()) {
        while (getline(archivo, array[count]) && count < MAX) {
            transform(array[count].begin(), array[count].end(), array[count].begin(), ::tolower);
            count++;
        }
        archivo.close();
    } else {
        cerr << "No se pudo abrir el archivo " << nombreArchivo << endl;
    }

    return count;
}

int modoPalabras(string arrayEsp[MAX], string arrayIngl[MAX], string Errores[MAX], int count){
    int infinito = 1;
    while (infinito){
        string frase;
        string traducir;
        cout << "\nIngrese una palabra>> ";
        getline(cin, frase);
        if(frase == "p"){
            cout << "\nPalabras agregadas: " << endl;
            for (int i = 0; i < count; i++) {
                cout << arrayEsp[i] << " - " << arrayIngl[i] << endl;
            }
        } else if (frase == "q") {
            infinito = 0;
        } else {
            bool fraseEncontrada = false;
            transform(frase.begin(), frase.end(), frase.begin(), ::tolower);
            for (int i = 0; i < count; i++) {
                string palabraMinusculas = arrayEsp[i];
                transform(palabraMinusculas.begin(), palabraMinusculas.end(), palabraMinusculas.begin(), ::tolower);
                if (frase == palabraMinusculas) {
                    cout << "La traduccion al ingles es>> " << arrayIngl[i] << endl;
                    fraseEncontrada = true;
                    break;
                }  else if (frase == arrayIngl[i]){
                    cout << "La traduccion al espanol es>> " << arrayEsp[i] << endl;
                    fraseEncontrada = true;
                    break;
                }
            }

            if (!fraseEncontrada) {
                cout << "\nLa palabra no se encuentra en el diccionario. Palabras similares>> ";
                for (int i = 0; i < count; i++) {
                    int distancia = editDistance(frase, arrayEsp[i]);
                    if (distancia <= 2) {
                        cout << arrayEsp[i] << " ";
                    }
                }
                cout << endl;
                cout << "Ingrese la traduccion o 'q' si se equivoco>> ";
                getline(cin, traducir);
                if (traducir == "q") {
                    Errores[count] = frase;
                    cout << "\nLa palabra fue descartada" << endl;
                } else {
                    arrayEsp[count] = frase;
                    arrayIngl[count] = traducir;
                    cout << "\nLa palabra se ha agregado correctamente" << endl;
                    count++;
                }
            }
        }
    }

    return count;
}


int modoParrafo(string arrayEsp[MAX], string arrayIngl[MAX], int count){
    int infinito = 1;
    while(infinito == 1){
        string parrafo;
        cout <<"\nIngrese un parrafo: ";
        getline(cin, parrafo);
        if(parrafo == "q"){
            infinito = 0;
        } else {
            string palabra;
            istringstream palabraStream(parrafo);
            while (palabraStream >> palabra){
                transform(palabra.begin(), palabra.end(), palabra.begin(), ::tolower);
                bool palabraEncontrada = false;
                for(int i = 0; i < count; i++){
                    string palabraArrayMinusculas = arrayEsp[i];
                    transform(palabraArrayMinusculas.begin(), palabraArrayMinusculas.end(), palabraArrayMinusculas.begin(), ::tolower);
                    if(palabra == palabraArrayMinusculas){
                        cout << arrayIngl[i] << " ";
                        palabraEncontrada = true;
                        break;
                    } else if (palabra == arrayIngl[i]){
                        cout << arrayEsp[i] << " ";
                        palabraEncontrada = true;
                        break;
                    }
                }

                if(!palabraEncontrada){
                    cout << "La palabra -" << palabra << "- no se encuentra en el diccionario" << endl;
                    cout << "Ingrese la traduccion>> ";
                    string traducir;
                    getline(cin, traducir);
                    arrayEsp[count] = palabra;
                    arrayIngl[count] = traducir;
                    cout << "\nLa palabra se ha agregado correctamente" << endl;
                    count++;
                    break;
                }
            }
            cout << endl;
        }
    }
    return count;
}

void guardarArray(string arrayEsp[MAX], string arrayIngl[MAX], int count){
    ofstream espFile("arrayEsp.txt");
    if (espFile.is_open()) {
        for (int i = 0; i < count; i++) {
            espFile << arrayEsp[i] << endl;
        }
        espFile.close();
        //cout << "Contenido de arrayEsp guardado en arrayEsp.txt" << endl;
    } else {
        cerr << "No se pudo abrir el archivo arrayEsp.txt" << endl;
    }

    ofstream ingFile("arrayIngl.txt");
    if (ingFile.is_open()) {
        for (int i = 0; i < count; i++) {
            ingFile << arrayIngl[i] << endl;
        }
        ingFile.close();
        //cout << "Contenido de arrayIngl guardado en arrayIngl.txt" << endl;
    } else {
        cerr << "No se pudo abrir el archivo arrayIngl.txt" << endl;
    }
}

int main() {
    string arrayEsp[MAX];
    string arrayIngl[MAX];
    string Errores[MAX];
    int countEsp = cargarDesdeArchivo("arrayEsp.txt", arrayEsp);
    int countIngl = cargarDesdeArchivo("arrayIngl.txt", arrayIngl);
    int count = max(countEsp, countIngl);
    int infinito = 1;

    while (infinito) {
        int modo;
        cout << "\nSeleccione el modo:" << endl;
        cout << "\n1. Modo traductor de palabras" << endl;
        cout << "2. Modo traductor de parrafos" << endl;
        cout << "3. Salir" << endl;
        cout << "\n>> ";
        cin >> modo;
        cin.ignore();

        switch (modo) {
            case 1:
                count = modoPalabras(arrayEsp, arrayIngl, Errores, count);
                guardarArray(arrayEsp, arrayIngl, count);
                break;
            case 2:
                count = modoParrafo(arrayEsp, arrayIngl, count);
                guardarArray(arrayEsp, arrayIngl, count);
                break;
            case 3:
                guardarArray(arrayEsp, arrayIngl, count);
                infinito = 0;
                break;
            default:
                cout << "Opcion invalida" << endl;
                break;
        }
    }
    return 0;
}