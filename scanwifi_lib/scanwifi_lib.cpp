#include "scanwifi_lib.h"

#include <cstring>


using namespace std;                                   // Permite usar string, ifstream diretamente ao invés de std::string
using namespace android::base;                         // Permite usar GetBoolProperty ao invés de android::base::GetBoolProperty

namespace devtitans::scanwifi {                       // Entra no pacote devtitans::smartlamp

int Scanwifi::connect() {
    char dirPath[] = "/sys/kernel/scanwifi";
    struct stat dirStat;
    if (stat(dirPath, &dirStat) == 0)
        if (S_ISDIR(dirStat.st_mode))
            return 1;                                  // Se o diretório existir, retorna 1

    // Diretório não existe, vamos verificar a propriedade
    bool allowSimulated = GetBoolProperty("devtitans.scanwifi.allow_simulated", true);
    if (!allowSimulated)
        return 0;                                      // Dispositivo não encontrado
    else
        return 2;                                      // Usando valores simulados
}

char* Scanwifi::readFileValue(string file) {
    int connected = this->connect();

    if (connected == 1) {
        string filename = string("/sys/kernel/scanwifi/") + file;
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


char* Scanwifi::getWifi() {
    char* value = this->readFileValue("wifi");
    return strdup(value);
}



} // namespace