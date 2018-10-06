package Banco;

import javax.swing.JFrame;
import javax.swing.JButton;
import javax.swing.JTextField;
import javax.swing.JLabel;
import java.awt.Font;
import java.awt.event.ActionEvent;
import java.awt.event.ActionListener;
import javax.swing.SwingConstants;

@SuppressWarnings("serial")
public class IngresoPrest extends JFrame{
	private JTextField MontoPrest;
	private JTextField MesesPrest;
	private int monIn;
	private int mesIn;
	
	public IngresoPrest() {
		setTitle("Ingreso Prestamo");
		getContentPane().setLayout(null);
		JButton btnContinuar = new JButton("Continuar");
		btnContinuar.setFont(new Font("Times New Roman", Font.BOLD, 11));
		btnContinuar.setBounds(156, 227, 89, 23);
		getContentPane().add(btnContinuar);
		btnContinuar.addActionListener(new ActionListener() {
			public void actionPerformed(ActionEvent e) {
				monIn = Integer.parseInt(MontoPrest.getText().trim());
				mesIn = Integer.parseInt(MesesPrest.getText().trim());
			}
		});
		
		MontoPrest = new JTextField();
		MontoPrest.setBounds(10, 103, 118, 20);
		getContentPane().add(MontoPrest);
		MontoPrest.setColumns(10);
		
		JLabel MontLab = new JLabel("Monto a Prestar");
		MontLab.setFont(new Font("Tahoma", Font.BOLD, 11));
		MontLab.setHorizontalAlignment(SwingConstants.CENTER);
		MontLab.setBounds(10, 67, 118, 14);
		getContentPane().add(MontLab);
		
		MesesPrest = new JTextField();
		MesesPrest.setBounds(246, 103, 153, 20);
		getContentPane().add(MesesPrest);
		MesesPrest.setColumns(10);
		
		JLabel Cantmes = new JLabel("Cantidad de Meses");
		Cantmes.setFont(new Font("Tahoma", Font.BOLD, 11));
		Cantmes.setHorizontalAlignment(SwingConstants.CENTER);
		Cantmes.setBounds(246, 67, 153, 14);
		getContentPane().add(Cantmes);
	}
	
	public int getmon() {
		return monIn;
	}
	public int getper() {
		return mesIn;
	}
	
	
}
