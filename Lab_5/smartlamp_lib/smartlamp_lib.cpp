#include "smartlamp_lib.h"

#include <cstring>


using namespace std;                                   // Permite usar string, ifstream diretamente ao invés de std::string
using namespace android::base;                         // Permite usar GetBoolProperty ao invés de android::base::GetBoolProperty

namespace devtitans::smartlamp {                       // Entra no pacote devtitans::smartlamp

int Smartlamp::connect() {
    char dirPath[] = "/sys/kernel/smartlamp";
    struct stat dirStat;
    if (stat(dirPath, &dirStat) == 0)
        if (S_ISDIR(dirStat.st_mode))
            return 1;                                  // Se o diretório existir, retorna 1

    // Diretório não existe, vamos verificar a propriedade
    bool allowSimulated = GetBoolProperty("devtitans.smartlamp.allow_simulated", true);
    if (!allowSimulated)
        return 0;                                      // Dispositivo não encontrado
    else
        return 2;                                      // Usando valores simulados
}

char* Smartlamp::readFileValue(string file) {
    int connected = this->connect();

    if (connected == 1) {
        string filename = string("/sys/kernel/smartlamp/") + file;
        ifstream fileStream(filename);

        if (fileStream.is_open()) {
            string line;
            getline(fileStream, line);  // Lê a linha do arquivo
            fileStream.close();

            // Alocar memória para uma cópia da string e retornar o ponteiro
            return strdup(line.c_str());
        }
    }

    // Se chegou aqui, não foi possível conectar ou se comunicar com o dispositivo
    return strdup("teste");
}


bool Smartlamp::writeFileValue(string file, int value) {
    int connected = this->connect();

    if (connected == 2) {                                // Usando valores simulados
        if (file == "led") {
            this->simLedValue = value;
            return true;
        }
        else if (file == "threshold") {
            this->simThresholdValue = value;
            return true;
        }
    }

    else if (connected == 1) {                          // Conectado. Vamos solicitar o valor ao dispositivo
        string filename = string("/sys/kernel/smartlamp/") + file;
        ofstream file(filename, ios::trunc);            // Abre o arquivo limpando o seu conteúdo

        if (file.is_open()) {                           // Verifica se o arquivo foi aberto com sucesso
            file << value;                              // Escreve o ledValue no arquivo
            file.close();
            return true;
        }
    }

    // Se chegou aqui, não foi possível conectar ou se comunicar com o dispositivo
    return false;
}

int Smartlamp::getLed() {
    return 0;
}

bool Smartlamp::setLed(int ledValue) {
    return this->writeFileValue("led", ledValue);
}

char* Smartlamp::getLuminosity() {
    char* value = this->readFileValue("ldr");
    return strdup(value);
}

int Smartlamp::getThreshold() {
    return 0;
}

bool Smartlamp::setThreshold(int thresholdValue) {
    return this->writeFileValue("threshold", thresholdValue);
}

} // namespace