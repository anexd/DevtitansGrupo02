package devtitans.scanwifimanager;

import android.util.Log;
import android.os.ServiceManager;
import android.os.IBinder;
import android.os.RemoteException;

import devtitans.scanwifi.IScanwifi;                      // Criado pelo AIDL

public class ScanwifiManager {
    private static final String TAG = "DevTITANS.ScanwifiManager";
    private IBinder binder;
    private IScanwifi service;

    private static ScanwifiManager instance;

    // Construtor. Configura a "instância da classe" (objeto) recém-criada. 
    // Note o "private" no construtor. Essa classe só pode ser instanciada dentro desse arquivo.
    private ScanwifiManager() {
        Log.d(TAG, "Nova (única) instância do ScanwifiManager ...");

        binder = ServiceManager.getService("devtitans.scanwifi.IScanwifi/default");
        if (binder != null) {
            service = IScanwifi.Stub.asInterface(binder);
            if (service != null)
                Log.d(TAG, "Serviço Scanwifi acessado com sucesso.");
            else
                Log.e(TAG, "Erro ao acessar o serviço Scanwifi!");
        }
        else
            Log.e(TAG, "Erro ao acessar o Binder!");
    }

    // Acessa a (única) instância dessa classe. Se ela não existir ainda, cria.
    // Note o "static" no método. Podemos executá-lo sem precisar instanciar um objeto.
    public static ScanwifiManager getInstance() {
        if (instance == null)
            instance = new ScanwifiManager();

        return instance;
    }

    public int connect() throws RemoteException {
        Log.d(TAG, "Executando método connect() ...");
        return service.connect();
    }

    public String getWifi() throws RemoteException {
        Log.d(TAG, "Executando método getWifi() ...");
        return service.getWifi();
    }
}