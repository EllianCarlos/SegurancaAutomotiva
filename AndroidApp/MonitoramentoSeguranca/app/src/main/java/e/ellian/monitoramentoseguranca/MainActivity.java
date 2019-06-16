package e.ellian.monitoramentoseguranca;

import android.content.Intent;
import android.provider.Settings;
import android.support.v7.app.AppCompatActivity;
import android.os.Bundle;
import android.view.View;
import android.widget.EditText;

public class MainActivity extends AppCompatActivity {

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);
        EditText editText = findViewById(R.id.editTextKey);
        editText.setText("");
    }

    public void onClickVai(View view) {
        EditText editText = (EditText) findViewById(R.id.editTextKey);
        Intent intent = new Intent(this, DadosPrincipais.class).putExtra("pass",editText.getText().toString());
        System.out.println("pass sended:" + editText.getText());
        startActivity(intent);
    }

    public void onClickClear(View view){
        EditText editText = findViewById(R.id.editTextKey);
        editText.setText("");
    }
}
