package e.ellian.monitoramentoseguranca;

import android.support.annotation.NonNull;
import android.support.v7.widget.RecyclerView;
import android.util.Log;
import android.view.LayoutInflater;
import android.view.View;
import android.view.ViewGroup;
import android.widget.ArrayAdapter;
import android.widget.TextView;

import java.util.ArrayList;

public class DadosAdapter extends RecyclerView.Adapter<DadosAdapter.ViewHolder> {
    private int listItemLayout;
    private ArrayList<Dados> dadosList;

    public DadosAdapter(int layoutId, ArrayList<Dados> arrayList){
        listItemLayout = layoutId;
        this.dadosList = arrayList;
    }



    @NonNull
    @Override
    public ViewHolder onCreateViewHolder(ViewGroup parent, int viewType) {
        View view = LayoutInflater.from(parent.getContext()).inflate(listItemLayout, parent, false);
        ViewHolder myViewHolder = new ViewHolder(view);
        return myViewHolder;
    }

    @Override
    public void onBindViewHolder(final ViewHolder holder, final int listPosition) {
        //TextView textViewDado = holder;
        //textViewDado.setText(dadosList.get(listPosition).getTempoAtivo());
    }

    @Override
    public int getItemCount() {
        return dadosList == null ? 0 : dadosList.size();
    }

    static class ViewHolder extends RecyclerView.ViewHolder implements View.OnClickListener{
        public TextView dado;
        public ViewHolder(View dadosView){
            super(dadosView);
            dadosView.setOnClickListener(this);
            dado = (TextView) dadosView.findViewById(R.id.textViewDadoMaoVolante);
        }

        @Override
        public void onClick(View v) {
            Log.d("onclick","onClick" + getLayoutPosition() + "" + dado.getText());
        }
    }
}
