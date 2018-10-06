package Banco;

import javax.swing.JFrame;
import javax.swing.GroupLayout;
import javax.swing.GroupLayout.Alignment;
import javax.swing.JButton;
import java.awt.Font;
import javax.swing.JTextField;
import javax.swing.JLabel;
import javax.swing.SwingConstants;

@SuppressWarnings("serial")
public class LoginNombre extends JFrame{
	private JTextField textField;
	public LoginNombre() {
		setTitle("Login Nombre");
		setType(Type.POPUP);
		
		JButton btnContinuar = new JButton("Continuar");
		btnContinuar.setFont(new Font("Tahoma", Font.BOLD, 13));
		
		textField = new JTextField();
		textField.setColumns(10);
		
		JLabel lblIngreseNombreDe = new JLabel("Ingrese Nombre de Usuario o Numero de Legajo, si se trata de un Empleado");
		lblIngreseNombreDe.setFont(new Font("Tahoma", Font.BOLD, 11));
		lblIngreseNombreDe.setHorizontalAlignment(SwingConstants.CENTER);
		GroupLayout groupLayout = new GroupLayout(getContentPane());
		groupLayout.setHorizontalGroup(
			groupLayout.createParallelGroup(Alignment.LEADING)
				.addGroup(groupLayout.createSequentialGroup()
					.addGroup(groupLayout.createParallelGroup(Alignment.LEADING)
						.addGroup(groupLayout.createSequentialGroup()
							.addGap(121)
							.addComponent(textField, GroupLayout.PREFERRED_SIZE, 173, GroupLayout.PREFERRED_SIZE))
						.addGroup(groupLayout.createSequentialGroup()
							.addGap(159)
							.addComponent(btnContinuar))
						.addGroup(groupLayout.createSequentialGroup()
							.addContainerGap()
							.addComponent(lblIngreseNombreDe)))
					.addContainerGap(GroupLayout.DEFAULT_SIZE, Short.MAX_VALUE))
		);
		groupLayout.setVerticalGroup(
			groupLayout.createParallelGroup(Alignment.LEADING)
				.addGroup(Alignment.TRAILING, groupLayout.createSequentialGroup()
					.addContainerGap(60, Short.MAX_VALUE)
					.addComponent(lblIngreseNombreDe, GroupLayout.PREFERRED_SIZE, 46, GroupLayout.PREFERRED_SIZE)
					.addGap(18)
					.addComponent(textField, GroupLayout.PREFERRED_SIZE, GroupLayout.DEFAULT_SIZE, GroupLayout.PREFERRED_SIZE)
					.addGap(81)
					.addComponent(btnContinuar)
					.addContainerGap())
		);
		getContentPane().setLayout(groupLayout);
	}

public String getNom() {
	return textField.getText();
}
}
