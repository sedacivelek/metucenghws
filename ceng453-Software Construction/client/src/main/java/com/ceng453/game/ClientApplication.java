package com.ceng453.game;

import javafx.application.Application;
import org.springframework.boot.autoconfigure.SpringBootApplication;

@SpringBootApplication
public class ClientApplication {

	public static void main(String[] args) {
		Application.launch(FxApplication.class, args);
	}

}
