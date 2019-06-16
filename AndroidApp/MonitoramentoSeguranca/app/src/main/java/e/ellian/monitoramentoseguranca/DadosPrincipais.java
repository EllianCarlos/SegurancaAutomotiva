package e.ellian.monitoramentoseguranca;

import android.content.Context;
import android.content.Intent;
import android.net.ConnectivityManager;
import android.net.NetworkInfo;
import android.provider.ContactsContract;
import android.support.v4.app.FragmentActivity;
import android.os.Bundle;
import android.view.View;
import android.widget.ScrollView;
import android.widget.TextView;

import com.jjoe64.graphview.GraphView;
import com.jjoe64.graphview.series.DataPoint;
import com.jjoe64.graphview.series.LineGraphSeries;
import com.jjoe64.graphview.series.PointsGraphSeries;
import com.jjoe64.graphview.series.Series;

import java.util.ArrayList;

public class DadosPrincipais extends FragmentActivity implements DownloadCallback {

    private NetworkFragment mNetworkFragment;
    private boolean mDownloading = false;
    private TextView mDataText;
    private boolean setted = false;
    private String password;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_dados_principais);
        password = getIntent().getStringExtra("pass");
        System.out.println("pass got:" + password);
        mDataText = findViewById(R.id.textView2);
        mDataText.setVisibility(View.INVISIBLE);
        mNetworkFragment = NetworkFragment.getInstance(getSupportFragmentManager(),
                "https://gsm-arduino.000webhostapp.com/receive.php",password);
        TextView textViewTempo = (TextView) findViewById(R.id.textViewDadosTempo);
        textViewTempo.setVisibility(View.INVISIBLE);
        TextView textViewChamaTempo = (TextView) findViewById(R.id.textViewChamadaTempo);
        textViewChamaTempo.setVisibility(View.INVISIBLE);
        TextView textViewErro = (TextView) findViewById(R.id.textViewError);
        textViewChamaTempo.setVisibility(View.INVISIBLE);
    }

    @Override
    protected void onStart() {
        super.onStart();
        TextView textViewErro = (TextView) findViewById(R.id.textViewError);
        textViewErro.setVisibility(View.VISIBLE);
        textViewErro.setText("Página Recarregando....");
        refresh((View)findViewById(R.layout.activity_dados_principais));
        ScrollView scrollView = (ScrollView) findViewById(R.id.ScrollView);
        scrollView.setVisibility(View.INVISIBLE);
    }

    protected void errorHandler(String error){
        ScrollView scrollView = (ScrollView) findViewById(R.id.ScrollView);
        scrollView.setVisibility(View.INVISIBLE);
        TextView textViewError = (TextView) findViewById(R.id.textViewError);
        textViewError.setText("Erro: " + error + "\n   tente recarregar.");
        textViewError.setTextColor(getColor(R.color.dangerRed));
        textViewError.setVisibility(View.VISIBLE);
    }

    protected void setAll(int velocidade,int tempMotor, boolean botao, int umaMaoVolante, int semMaoVolante) {
        ScrollView scrollView = (ScrollView) findViewById(R.id.ScrollView);
        scrollView.setVisibility(View.VISIBLE);
        TextView textViewError = (TextView) findViewById(R.id.textViewError);
        textViewError.setVisibility(View.INVISIBLE);

        TextView textViewSemMaoVolante = (TextView) findViewById(R.id.textViewDadoSemMaoVolante);
        textViewSemMaoVolante.setText(String.valueOf(semMaoVolante).concat("%"));

        TextView textViewDuasMaoVolante = (TextView) findViewById(R.id.textViewDadoMaoVolante);
        textViewDuasMaoVolante.setText(String.valueOf(umaMaoVolante).concat("%"));

        TextView textViewVelocidade = (TextView) findViewById(R.id.textViewDadoVelocidade);
        textViewVelocidade.setText(String.valueOf(velocidade).concat(" km/h"));

        TextView textViewEmerg = (TextView) findViewById(R.id.textViewDadoEmerg);
        if (botao) {
            textViewEmerg.setText(R.string.botaoAtivado);
            textViewEmerg.setTextColor(getColor(R.color.dangerRed));
        } else {
            textViewEmerg.setText(R.string.botaoDesativado);
            textViewEmerg.setTextColor(getColor(R.color.calmBlue));
        }

        TextView textViewTempMotor = (TextView) findViewById(R.id.textViewDadosTempMotor);
        textViewTempMotor.setText(String.valueOf(tempMotor).concat(" ºC"));
    }

    protected void setGraph(DataReady dataReady){
        GraphView graph = (GraphView) findViewById(R.id.graphVel);
        LineGraphSeries<DataPoint> series = new LineGraphSeries<>(dataReady.data());
        graph.addSeries(series);
    }


    public void refresh(View view) {
        startDownload();
    }
    public void done(View view){
        finishDownloading();
        Intent intent = new Intent(this, MainActivity.class);
        startActivity(intent);
    }

    private void startDownload() {
        if (!mDownloading && mNetworkFragment != null) {
            // Execute the async download.
            mNetworkFragment.startDownload();
            mDownloading = true;
        }
    }

    @Override
    public void updateFromDownload(String result) {
        if (result != null && !setted) {
            mDataText.setText(result);
            JSONParser jsonParser = new JSONParser();
            ArrayList<Dados> lista = jsonParser.parseIt((String)mDataText.getText());
            DataReady dataReady = new DataReady(lista);
            if(lista.size()>=1){
                setAll(lista.get(lista.size()-1).getVelocidade(),lista.get(lista.size()-1).getTempMotor(),lista.get(lista.size()-1).isAlert(),dataReady.getPorcUmaMao(lista.size()-1,true),dataReady.getPorcSemMao(lista.size()-1,true));
                setGraph(dataReady);
                //setAll();
            } else {
                errorHandler("ERRO ADIQUIRINDO A MENSAGEM");
            }
            setted = true;
        } else {
           mDataText.setText("AAAAAAA");
        }
    }

    @Override
    public NetworkInfo getActiveNetworkInfo() {
        ConnectivityManager connectivityManager =
                (ConnectivityManager) getSystemService(Context.CONNECTIVITY_SERVICE);
        NetworkInfo networkInfo = connectivityManager.getActiveNetworkInfo();
        return networkInfo;
    }

    @Override
    public void finishDownloading() {
        mDownloading = false;
        setted = false;
        if (mNetworkFragment != null) {
            mNetworkFragment.cancelDownload();
        }
    }

    @Override
    public void onProgressUpdate(int progressCode, int percentComplete) {
        switch (progressCode) {
            // You can add UI behavior for progress updates here.
            case Progress.ERROR:
                System.out.println("ERROR");
                break;
            case Progress.CONNECT_SUCCESS:
                System.out.println("CONNECT SUCCESS");
                break;
            case Progress.GET_INPUT_STREAM_SUCCESS:
                System.out.println("GET INPUT STREAM SUCCESS");
                break;
            case Progress.PROCESS_INPUT_STREAM_IN_PROGRESS:
                System.out.println("PROCESS INPUT STREAM IN PROGRESS");
                break;
            case Progress.PROCESS_INPUT_STREAM_SUCCESS:
                System.out.println("PROCESS INPUT STREAM SUCCESS");
                break;
        }
    }
}