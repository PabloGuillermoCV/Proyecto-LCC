package Banco;

import java.awt.BorderLayout;
import java.awt.Point;
import java.awt.event.ActionEvent;
import java.awt.event.ActionListener;
import java.awt.event.MouseAdapter;
import java.awt.event.MouseEvent;
import java.beans.PropertyVetoException;
import javax.swing.GroupLayout;
import javax.swing.JButton;
import javax.swing.JComponent;
import javax.swing.JDesktopPane;
import javax.swing.JDialog;
import javax.swing.JLabel;
import javax.swing.JMenu;
import javax.swing.JMenuBar;
import javax.swing.JMenuItem;
import javax.swing.JSeparator;
import javax.swing.LayoutStyle;
import javax.swing.SwingConstants;
import javax.swing.SwingUtilities;
import javax.swing.WindowConstants;
import javax.swing.JPasswordField;
import javax.swing.JTextField;
import java.awt.Button;
import java.awt.Color;
import java.awt.Font;

@SuppressWarnings({ "serial", "unused" })
public class VentanaPrincipal extends javax.swing.JFrame {
	
	private VentanaAdmin ventanaAdmin;
	private VentanaEmpleado ventanaEmpleado;
	private VentanaATM ventanaATM;
	private JButton btnCerrar, loginATM, loginAdmin, loginEmpleado;
	private JPasswordField passwordField;
	private JTextField textField;
	private JMenuItem mniSalir, mniAdmin, mniEmpleado, mniATM;
	private JDesktopPane jDesktopPane1;
	private JMenuBar jMenuBar1;
	private JMenu mnuEjemplos;
	private JSeparator jSeparator1;
	
	/*Por lo que entiendo seria:
	 * Desde aca se accede con uno de los 3 usuarios posibles (admin, empleado y atm),
	 * despues se manda hacia la ventana correspondiente de cada usuario donde pondriamos
	 * las cosas que puede y no puede hacer y sobre que tablas*/
	
	public static void main (String [] args) {
	    SwingUtilities.invokeLater (new Runnable() {
	        public void run () {
	            VentanaPrincipal inst = new VentanaPrincipal ();
	            inst.setLocationRelativeTo (null);
	            inst.setVisible (true);
	        }
	    });
    }
	
	public VentanaPrincipal () {
		super ();
		setTitle("Bienvenido al Banco Patacon");
		initGUI ();
		this.ventanaAdmin = new VentanaAdmin ();
		this.ventanaEmpleado = new VentanaEmpleado ();
		this.ventanaATM = new VentanaATM ();
		getContentPane().setLayout(null);
		
		passwordField = new JPasswordField();
		passwordField.setToolTipText("Contrasena");
		passwordField.setBounds(236, 223, 198, 27);
		getContentPane().add(passwordField);
		
		textField = new JTextField();
		textField.setToolTipText("Nombre de Usuario");
		textField.setBounds(236, 174, 198, 27);
		getContentPane().add(textField);
		textField.setColumns(10);
		
		JLabel lblNombreDeUsuario = new JLabel("Nombre de Usuario");
		lblNombreDeUsuario.setFont(new Font("Tahoma", Font.BOLD, 11));
		lblNombreDeUsuario.setHorizontalAlignment(SwingConstants.CENTER);
		lblNombreDeUsuario.setEnabled(false);
		lblNombreDeUsuario.setBounds(26, 180, 132, 14);
		getContentPane().add(lblNombreDeUsuario);
		
		Button button = new Button("Ingresar como Admin");
		button.setBackground(new Color(135, 206, 250));
		button.setBounds(26, 10, 116, 27);
		getContentPane().add(button);
		
		JLabel lblContrasea = new JLabel("Contrasena");
		lblContrasea.setFont(new Font("Tahoma", Font.BOLD, 11));
		lblContrasea.setEnabled(false);
		lblContrasea.setHorizontalAlignment(SwingConstants.CENTER);
		lblContrasea.setBounds(47, 229, 95, 14);
		getContentPane().add(lblContrasea);
		
		Button button_1 = new Button("Ingresar como Empleado");
		button_1.setBackground(new Color(135, 206, 250));
		button_1.setBounds(26, 61, 145, 22);
		getContentPane().add(button_1);
		
		Button button_2 = new Button("Ingresar como ATM");
		button_2.setBackground(new Color(135, 206, 250));
		button_2.setBounds(26, 110, 121, 22);
		getContentPane().add(button_2);
		this.ventanaAdmin.setVisible(false);
		this.ventanaEmpleado.setVisible(false);
		this.ventanaATM.setVisible(false);
		
	}
	
	private void initGUI () {
		try 
	      {
	         javax.swing.UIManager.setLookAndFeel(javax.swing.UIManager.getSystemLookAndFeelClassName());
	      } 
	      catch(Exception e) 
	      {
	         e.printStackTrace();
	      }
		try {
	         {
	            this.setTitle("Java y MySQL");
	            this.setDefaultCloseOperation(WindowConstants.EXIT_ON_CLOSE);
	         }
	         {
	            jDesktopPane1 = new JDesktopPane();
	            getContentPane().add(jDesktopPane1, BorderLayout.CENTER);
	            jDesktopPane1.setPreferredSize(new java.awt.Dimension(800, 600));
	         }
	         {
	            jMenuBar1 = new JMenuBar();
	            setJMenuBar(jMenuBar1);
	            {
	               mnuEjemplos = new JMenu();
	               jMenuBar1.add(mnuEjemplos);
	               mnuEjemplos.setText("Opciones");
	               {
	                  mniEmpleado = new JMenuItem();
	                  mnuEjemplos.add(mniEmpleado);
	                  mniEmpleado.setText("Busqueda para Empleados (Utilizando JTable)");
	                  mniEmpleado.addActionListener(new ActionListener() {
	                     public void actionPerformed(ActionEvent evt) {
	                        mniEmpleadoActionPerformed(evt);
	                     }
	                  });
	               }
	               {
	                  mniATM = new JMenuItem();
	                  mnuEjemplos.add(mniATM);
	                  mniATM.setText("Busqueda para ATM (Utilizando DBTable)");
	                  mniATM.addActionListener(new ActionListener() {
	                     public void actionPerformed(ActionEvent evt) {
	                        mniATMActionPerformed(evt);
	                     }
	                  });
	               }
	               {
	                  mniAdmin = new JMenuItem();
	                  mnuEjemplos.add(mniAdmin);
	                  mniAdmin.setText("Busqueda para Admin (Utilizando DBTable)");
	                  mniAdmin.addActionListener(new ActionListener() {
	                     public void actionPerformed(ActionEvent evt) {
	                        mniAdminActionPerformed(evt);
	                     }
	                  });
	               }
	               {
	                  jSeparator1 = new JSeparator();
	                  mnuEjemplos.add(jSeparator1);
	               }
	               {
	                  mniSalir = new JMenuItem();
	                  mnuEjemplos.add(mniSalir);
	                  mniSalir.setText("Salir");
	                  mniSalir.addActionListener(new ActionListener() {
	                     public void actionPerformed(ActionEvent evt) {
	                        mniSalirActionPerformed(evt);
	                     }
	                  });
	               }
	            }
	         }
	         this.setSize(800, 600);
	         pack();
	      } catch (Exception e) {
	         e.printStackTrace();
	      }
	}
	
	private void mniSalirActionPerformed (ActionEvent evt) {
	    this.dispose();
	}
	   
	private void mniAdminActionPerformed (ActionEvent evt) {
	    try {
	        this.ventanaAdmin.setMaximum (true);
	        //al abrir, pido el password de admin para poder hacer el script de coneccion
	    }
	    catch (PropertyVetoException e) {}
	    this.ventanaAdmin.setVisible (true);      
	}
	   
    private void mniEmpleadoActionPerformed (ActionEvent evt) {
	    try {
	        this.ventanaEmpleado.setMaximum (true);
	    }
	    catch (PropertyVetoException e) {}
	    this.ventanaEmpleado.setVisible (true);      
	}
	   
    private void mniATMActionPerformed (ActionEvent evt) {
	    try {
	        this.ventanaATM.setMaximum (true);
	    }
	    catch (PropertyVetoException e) {}
	    this.ventanaATM.setVisible (true);      
    }
}