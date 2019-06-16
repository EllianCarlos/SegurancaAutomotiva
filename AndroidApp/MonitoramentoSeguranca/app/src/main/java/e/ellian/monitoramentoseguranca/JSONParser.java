package e.ellian.monitoramentoseguranca;

import android.content.Intent;
import android.util.Log;

import org.json.JSONArray;
import org.json.JSONException;
import org.json.JSONObject;


import java.sql.Timestamp;
import java.text.DateFormat;
import java.text.SimpleDateFormat;
import java.util.ArrayList;
import java.util.Date;

public class JSONParser {
    private String toBeParsed;

    public JSONParser(String raw_data){
        toBeParsed = raw_data;
    }

    public JSONParser(){

    }

    public ArrayList<Dados> parseIt(String rawData){
        ArrayList<Dados> lista = new ArrayList<Dados>();
        Dados dados = new Dados();
        DateFormat formatter = new SimpleDateFormat("YYYY-MM-dd hh:mm:ss");
        try {
            JSONArray json = new JSONArray(rawData);
            for (int i =0; i < json.length(); i++){
                JSONObject d = json.getJSONObject(i);
                int id = d.getInt("id");
                Date date = (Date) formatter.parse(d.getString("timeStampSend"));
                Timestamp timeStampSend = new Timestamp((Long) date.getTime());
                date = (Date) formatter.parse(d.getString("timeStampBD"));
                Timestamp timestampBD = new Timestamp((Long) date.getTime());
                int tempoAtivo = d.getInt("tempoAtivo");
                int tempoUmaMaoVolante = d.getInt("tempoUmaMaoVolante");
                int tempoDuasMaoVolante = d.getInt("tempoDuasMaoVolante");
                int velocidade = d.getInt("Velocidade");
                int tempMotor =  d.getInt("tempMotor");
                boolean emerg = (d.getInt("emerg") == 1) ;
                boolean alert = (d.getInt("alerta") == 1);
                boolean airbag = (d.getInt("airbag") == 1);
                dados = new Dados(id,tempMotor,velocidade,tempoAtivo,tempoUmaMaoVolante,tempoDuasMaoVolante,airbag,emerg,alert,null,null);
                lista.add(dados);
            }
        } catch (JSONException ex){
            Log.d(String.valueOf(ex.getCause()),ex.getMessage());
        } catch (Exception e) {
            Log.d(String.valueOf(e.getCause()),e.getMessage());
        }

        return lista;
    }
}
