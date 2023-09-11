package devtitans.scanwifiapp;

import androidx.appcompat.app.AppCompatActivity;
import android.graphics.Color;
import android.os.Bundle;
import android.util.Log;
import android.view.View;
import android.widget.*;

import android.os.RemoteException;

import java.util.ArrayList;
import java.util.List;
import java.util.Timer;
import java.util.TimerTask;

import devtitans.scanwifimanager.ScanwifiManager;                          // Biblioteca do Manager

public class MainActivity extends AppCompatActivity {
    private static final String TAG = "DevTITANS.ScanwifiApp";

    private TextView textStatus, textWifi;
    private ScanwifiManager manager;                                        // Atributo para o Manager

    private ListView wifiListView;
    private Button btnRefresh;
    private List<String> wifiList;
    private ArrayAdapter<String> adapter;

    private Timer autoUpdateTimer;

    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);

        textStatus = findViewById(R.id.textStatus);                      // Acessa os componentes da tela
        //textWifi = findViewById(R.id.textWifi);

        manager = ScanwifiManager.getInstance();

        wifiListView = findViewById(R.id.wifiListView);
        //btnRefresh = findViewById(R.id.btnRefresh);

        wifiList = new ArrayList<>();
        adapter = new ArrayAdapter<>(this, android.R.layout.simple_list_item_1, wifiList);
        wifiListView.setAdapter(adapter);

        autoUpdateTimer = new Timer();
        autoUpdateTimer.scheduleAtFixedRate(new TimerTask() {
            @Override
            public void run() {
                runOnUiThread(() -> updateAll(null));
            }
        }, 0, 10000); // Atualizar a cada 10 segundos (10000 milissegundos)
    }


    public void updateAll(View view) {
        Log.d(TAG, "Atualizando dados do dispositivo ...");

        textStatus.setText("Atualizando ...");
        textStatus.setTextColor(Color.parseColor("#c47e00"));

        try {
            wifiList.clear(); // Limpa a lista antes de adicionar os novos dados


            String wifi = manager.getWifi();                        // Executa o método getLuminosity via manager

            //implementar

            String[] dados = wifi.split(",");
            int tamanho = dados.length;

            for (int i = 0; i < tamanho/2; i++){
                if (i%2==0){
                    wifiList.add(dados[i]+"   |   "+dados[i+1]);
                }


            }

            //textWifi.setText(String.valueOf(dados[0]));

            int status = manager.connect();                                  // Executa o método connect via manager
            if (status == 0) {
                textStatus.setText("Desconectado");
                textStatus.setTextColor(Color.parseColor("#73312f"));
            }
            else if (status == 1) {
                textStatus.setText("Conectado");
                textStatus.setTextColor(Color.parseColor("#6d790c"));
            }
            else {
                textStatus.setText("Desconectado");
                textStatus.setTextColor(Color.parseColor("#207fb5"));
            }

        } catch (android.os.RemoteException e) {
            Toast.makeText(this, "Erro ao acessar o Binder!", Toast.LENGTH_LONG).show();
            Log.e(TAG, "Erro atualizando dados:", e);
        }

    }
}