package edu.temple.formactivity;

import androidx.appcompat.app.AppCompatActivity;

import android.os.Bundle;
import android.view.View;
import android.widget.Button;
import android.widget.TextView;
import android.widget.Toast;

import java.util.regex.Pattern;

public class MainActivity extends AppCompatActivity {

    TextView textInputName;
    TextView textInputEmail;
    TextView textInputPassword;
    TextView textInputPwCnfrm;

    Button button;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);

        textInputName = findViewById(R.id.inputName);
        textInputEmail = findViewById(R.id.inputEmail);
        textInputPassword = findViewById(R.id.inputPassword);
        textInputPwCnfrm = findViewById(R.id.inputPwCnfrm);

        findViewById(R.id.registerButton).setOnClickListener(new View.OnClickListener(){
            @Override
            public void onClick(View v){
                if(textInputName.getEditableText().toString().isEmpty() ||
                        textInputEmail.getEditableText().toString().isEmpty() ||
                        textInputPassword.getEditableText().toString().isEmpty() ||
                        textInputPwCnfrm.getEditableText().toString().isEmpty()) {
                    Toast.makeText(MainActivity.this, "Please enter all information", Toast.LENGTH_SHORT).show();
                    return;
                }
                if((textInputPassword.getEditableText().toString().compareTo(textInputPwCnfrm.getEditableText().toString())) != 0){
                    Toast.makeText(MainActivity.this, "Password confirmation failed", Toast.LENGTH_SHORT).show();
                    return;
                }
                Toast.makeText(MainActivity.this, "Welcome, "+textInputName.getEditableText().toString()+" to this app", Toast.LENGTH_SHORT).show();
            }
        });
    }
}

