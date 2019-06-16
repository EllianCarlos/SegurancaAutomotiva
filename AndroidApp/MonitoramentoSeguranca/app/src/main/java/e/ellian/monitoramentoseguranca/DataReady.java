package e.ellian.monitoramentoseguranca;

import com.jjoe64.graphview.series.DataPoint;

import java.util.ArrayList;

import static java.lang.Double.parseDouble;

public class DataReady{
    ArrayList<Dados> lista;

    public DataReady(ArrayList<Dados> lista){
        this.lista = lista;
    }

    public int getPorcUmaMao(){
        int porc = 0;
        int totalTempoAtivo = 0;
        int totalTempoUmaMao = 0;
        for(int i = 0; i < this.lista.size(); i++){
            totalTempoAtivo = totalTempoAtivo + lista.get(i).getTempoAtivo();
            totalTempoUmaMao = totalTempoUmaMao + lista.get(i).getTempoUmaMaoVolante();
        }
        porc = (100*totalTempoUmaMao)/(totalTempoAtivo);
        return porc;
    }

    public int getPorcUmaMao(int n,boolean k){
        int porc = 0;
        int totalTempoAtivo = 0;
        int totalTempoUmaMao = 0;
        if (this.lista.size() >= n) {
            if(k){
                for (int i = 0; i < n; i++) {
                    totalTempoAtivo = totalTempoAtivo + lista.get(i).getTempoAtivo();
                    totalTempoUmaMao = totalTempoUmaMao + lista.get(i).getTempoUmaMaoVolante();
                }
                if(totalTempoAtivo != 0){
                    porc = (int)((100 * totalTempoUmaMao) / (totalTempoAtivo));
                } else {
                    porc = 0;
                }

            } else {
                for (int i = this.lista.size(); i >= n; i--) {
                    totalTempoAtivo = totalTempoAtivo + lista.get(i).getTempoAtivo();
                    totalTempoUmaMao = totalTempoUmaMao + lista.get(i).getTempoUmaMaoVolante();
                }
                if(totalTempoAtivo != 0){
                    porc = (int)((100 * totalTempoUmaMao) / (totalTempoAtivo));
                } else {
                    porc = 0;
                }
            }

        } else {
            System.out.println("ERROR");
        }
        return porc;
    }

    public int getPorcDuasMao(){
        int porc = 0;
        int totalTempoAtivo = 0;
        int totalTempoDuasMao = 0;
        for(int i = 0; i < this.lista.size(); i++){
            totalTempoAtivo = totalTempoAtivo + lista.get(i).getTempoAtivo();
            totalTempoDuasMao = totalTempoDuasMao + lista.get(i).getTempoUmaMaoVolante();
        }
        if(totalTempoAtivo != 0){
            porc = (int)((100 * totalTempoDuasMao) / (totalTempoAtivo));
        } else {
            porc = 0;
        }
        return porc;
    }

    public int getPorcDuasMao(int n, boolean k){
        int porc = 0;
        int totalTempoAtivo = 0;
        int totalTempoDuasMao = 0;
        if (!(n == 0)){
            if (k){
                for(int i = 0; i < n; i++){
                    totalTempoAtivo = totalTempoAtivo + lista.get(i).getTempoAtivo();
                    totalTempoDuasMao = totalTempoDuasMao + lista.get(i).getTempoUmaMaoVolante();
                }
                if(totalTempoAtivo != 0){
                    porc = (int)((100 * totalTempoDuasMao) / (totalTempoAtivo));
                } else {
                    porc = 0;
                }
            }
            else {
                for(int i = this.lista.size(); i >= n ; i--){
                    totalTempoAtivo = totalTempoAtivo + lista.get(i).getTempoAtivo();
                    totalTempoDuasMao = totalTempoDuasMao + lista.get(i).getTempoUmaMaoVolante();
                }
                if(totalTempoAtivo != 0){
                    porc = (int)((100 * totalTempoDuasMao) / (totalTempoAtivo));
                } else {
                    porc = 0;
                }
            }

        }
        return porc;
    }

    public int getPorcSemMao(){
        int porc = 0;
        int totalTempoAtivo = 0;
        int totalTempoSemMao = 0;
        for(int i = 0; i < this.lista.size(); i++){
            totalTempoAtivo = totalTempoAtivo + lista.get(i).getTempoAtivo();
            totalTempoSemMao = totalTempoSemMao + lista.get(i).getTempoUmaMaoVolante();
        }
        porc = (100*totalTempoSemMao)/(totalTempoAtivo);
        return porc;
    }

    public int getPorcSemMao(int n, boolean k){
        int porc = 0;
        int totalTempoAtivo = 0;
        int totalTempoSemMao = 0;
        if(!(n==0)){
            if (k){
                for(int i = 0; i < this.lista.size(); i++){
                    totalTempoAtivo = totalTempoAtivo + lista.get(i).getTempoAtivo();
                    totalTempoSemMao = totalTempoSemMao + lista.get(i).getTempoUmaMaoVolante();
                }
                porc = (100*totalTempoSemMao)/(totalTempoAtivo);
            }
            else {
                for(int i = this.lista.size(); i > n ; i--){
                    totalTempoAtivo = totalTempoAtivo + lista.get(i).getTempoAtivo();
                    totalTempoSemMao = totalTempoSemMao + lista.get(i).getTempoUmaMaoVolante();
                }
                porc = (100*totalTempoSemMao)/(totalTempoAtivo);
            }
        }
        return porc;
    }

    public ArrayList<Integer> getAllVelPoints(){
        ArrayList<Integer> velocidades = new ArrayList<Integer>();
        for(int i = 0; i < this.lista.size();i++){
            velocidades.add(i, lista.get(i).getVelocidade());
        }
        return velocidades;
    }

    public ArrayList<DataPoint> getAllVelDataPoints(){
        ArrayList<DataPoint> points = new ArrayList<DataPoint>();
        for(int i = 0; i < this.lista.size();i++){
            points.add(new DataPoint(i,this.lista.get(i).getVelocidade()));
        }
        return points;
    }

    public DataPoint[] data(){
        int n=this.lista.size();     //to find out the no. of data-points
        DataPoint[] values = new DataPoint[n];     //creating an object of type DataPoint[] of size 'n'
        for(int i=0;i<n;i++){
            DataPoint v = new DataPoint(i,this.lista.get(i).getVelocidade());
            values[i] = v;
        }
        return values;
    }
}