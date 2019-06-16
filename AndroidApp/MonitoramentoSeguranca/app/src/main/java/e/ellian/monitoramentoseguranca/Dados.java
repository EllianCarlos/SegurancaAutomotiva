package e.ellian.monitoramentoseguranca;

import java.sql.Time;
import java.sql.Timestamp;

public class Dados {
    private int id;
    private int tempoAtivo;
    private int tempoUmaMaoVolante;
    private int tempoDuasMaoVolante;
    private Timestamp timeStampSend;
    private Timestamp timestampBD;
    private int velocidade;
    private int tempMotor;
    private boolean emerg;
    private boolean alert;
    private boolean airbag;
    private boolean setted = false;


    public Dados(){
        id = 0;
        tempoAtivo = 0;
        tempoUmaMaoVolante = 0;
        tempoDuasMaoVolante = 0;
        emerg = false;
        alert = false;
    }

    public Dados(int id, int tempMotor, int velocidade, int tempoAtivo, int tempoUmaMaoVolante, int tempoDuasMaoVolante,boolean airbag, boolean emerg, boolean alert, Timestamp timeStampBD, Timestamp timeStampSend) {
        this.id = id;
        this.tempMotor = tempMotor;
        this.velocidade = velocidade;
        this.tempoAtivo = tempoAtivo;
        this.airbag=airbag;
        this.tempoUmaMaoVolante = tempoUmaMaoVolante;
        this.tempoDuasMaoVolante = tempoDuasMaoVolante;
        this.emerg = emerg;
        this.alert = alert;
        this.timestampBD = timeStampBD;
        this.timeStampSend = timeStampSend;
        this.setted = true;
    }

    public int getPorcUmaMao(){
        return (100*this.tempoUmaMaoVolante)/(this.tempoAtivo);
    }

    public int getPorcDuasMao(){
        return (100*this.tempoDuasMaoVolante)/(this.tempoAtivo);
    }

    public int getPorcSemMao(){
        return (100*(this.tempoAtivo - this.tempoUmaMaoVolante - this.tempoDuasMaoVolante))/(this.tempoAtivo);
    }



    public Timestamp getTimeStampSend() {
        return timeStampSend;
    }

    public void setTimeStampSend(Timestamp timeStampSend) {
        this.timeStampSend = timeStampSend;
    }

    public Timestamp getTimestampBD() {
        return timestampBD;
    }

    public void setTimestampBD(Timestamp timestampBD) {
        this.timestampBD = timestampBD;
    }

    public int getTempMotor() {
        return tempMotor;
    }

    public void setTempMotor(int tempMotor) {
        this.tempMotor = tempMotor;
    }

    public boolean isAirbag() {
        return airbag;
    }

    public void setAirbag(boolean airbag) {
        this.airbag = airbag;
    }

    public int getVelocidade() {
        return velocidade;
    }

    public void setVelocidade(int velocidade) {
        this.velocidade = velocidade;
    }

    public void setAll(int tempoAtivo, int tempoUmaMaoVolante, int tempoDuasMaoVolante, boolean emerg, boolean alert){
        setTempoUmaMaoVolante(tempoUmaMaoVolante);
        setTempoDuasMaoVolante(tempoDuasMaoVolante);
        setTempoAtivo(tempoAtivo);
        setAlert(alert);
        setEmerg(emerg);
        this.setted = true;
    }

    public int getId() {
        return id;
    }

    public void setId(int id) {
        this.id = id;
    }

    public int getTempoAtivo() {
        return tempoAtivo;
    }

    private void setTempoAtivo(int tempoAtivo) {
        this.tempoAtivo = tempoAtivo;
    }

    public int getTempoUmaMaoVolante() {
        return tempoUmaMaoVolante;
    }

    private void setTempoUmaMaoVolante(int tempoUmaMaoVolante) {
        this.tempoUmaMaoVolante = tempoUmaMaoVolante;
    }

    public int getTempoDuasMaoVolante() {
        return tempoDuasMaoVolante;
    }

    private void setTempoDuasMaoVolante(int tempoDuasMaoVolante) {
        this.tempoDuasMaoVolante = tempoDuasMaoVolante;
    }

    public boolean isEmerg() {
        return emerg;
    }

    private void setEmerg(boolean emerg) {
        this.emerg = emerg;
    }

    public boolean isAlert() {
        return alert;
    }

    private void setAlert(boolean alert) {
        this.alert = alert;
    }
}
