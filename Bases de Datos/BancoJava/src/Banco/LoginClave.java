package Banco;

import javax.swing.JFrame;
import java.awt.Window.Type;
import javax.swing.GroupLayout;
import javax.swing.GroupLayout.Alignment;
import javax.swing.JTextField;
import javax.swing.JLabel;
import java.awt.Font;
import javax.swing.JButton;
import javax.swing.LayoutStyle.ComponentPlacement;
import javax.swing.SwingConstants;
import javax.swing.JPasswordField;

public class LoginClave extends JFrame{
	private JPasswordField passwordField;
	public LoginClave() {
		setResizable(false);
		setType(Type.POPUP);
		setTitle("Login Clave");
		
		JLabel lblIngreseNombreDe = new JLabel("Ingrese Contrase\u00F1a");
		lblIngreseNombreDe.setHorizontalAlignment(SwingConstants.CENTER);
		lblIngreseNombreDe.setFont(new Font("Tahoma", Font.BOLD, 12));
		
		JButton btnContinuar = new JButton("Continuar");
		btnContinuar.setFont(new Font("Tahoma", Font.BOLD, 13));
		
		passwordField = new JPasswordField();
		GroupLayout groupLayout = new GroupLayout(getContentPane());
		groupLayout.setHorizontalGroup(
			groupLayout.createParallelGroup(Alignment.LEADING)
				.addGroup(groupLayout.createSequentialGroup()
					.addGroup(groupLayout.createParallelGroup(Alignment.LEADING)
						.addGroup(groupLayout.createSequentialGroup()
							.addGap(162)
							.addComponent(btnContinuar))
						.addGroup(groupLayout.createSequentialGroup()
							.addGap(156)
							.addGroup(groupLayout.createParallelGroup(Alignment.LEADING, false)
								.addComponent(passwordField)
								.addComponent(lblIngreseNombreDe, GroupLayout.DEFAULT_SIZE, GroupLayout.DEFAULT_SIZE, Short.MAX_VALUE))))
					.addContainerGap(133, Short.MAX_VALUE))
		);
		groupLayout.setVerticalGroup(
			groupLayout.createParallelGroup(Alignment.LEADING)
				.addGroup(groupLayout.createSequentialGroup()
					.addGap(65)
					.addComponent(lblIngreseNombreDe)
					.addGap(29)
					.addComponent(passwordField, GroupLayout.PREFERRED_SIZE, GroupLayout.DEFAULT_SIZE, GroupLayout.PREFERRED_SIZE)
					.addPreferredGap(ComponentPlacement.RELATED, 74, Short.MAX_VALUE)
					.addComponent(btnContinuar)
					.addGap(43))
		);
		getContentPane().setLayout(groupLayout);
	}
public String getPass() {
	return passwordField.getPassword().toString();
}
	
}
