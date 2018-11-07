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
	private ConsultasATM ConsultasATM;
	private JButton btnCerrar, loginATM, loginAdmin, loginEmpleado;
	private JPasswordField passwordField;
	private JTextField textField;
	private JMenuItem mniSalir, mniAdmin, mniEmpleado, mniATM;
	private JDesktopPane jDesktopPane1;
	private JMenuBar jMenuBar1;
	private JMenu mnuEjemplos;
	private JSeparator jSeparator1;
	
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
		this.ConsultasATM = new ConsultasATM ();
		getContentPane().setLayout(null);
		
		Button button = new Button("Ingresar como Admin");
		button.setBackground(new Color(135, 206, 250));
		button.setBounds(26, 10, 116, 27);
		getContentPane().add(button);
		
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
		this.ConsultasATM.setVisible(false);
		
		this.jDesktopPane1.add(this.ventanaAdmin);
		this.jDesktopPane1.add(this.ventanaEmpleado);
		this.jDesktopPane1.add(this.ConsultasATM);
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
	            this.setTitle("Proyecto 2 Ejercicio 2- Ceballos Vitale, Gomez");
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
	                  mniEmpleado.setText("Busqueda para Empleados");
	                  mniEmpleado.addActionListener(new ActionListener() {
	                     public void actionPerformed(ActionEvent evt) {
	                        mniEmpleadoActionPerformed(evt);
	                     }
	                  });
	               }
	               {
	                  mniATM = new JMenuItem();
	                  mnuEjemplos.add(mniATM);
	                  mniATM.setText("Busqueda para ATM");
	                  mniATM.addActionListener(new ActionListener() {
	                     public void actionPerformed(ActionEvent evt) {
	                        mniATMActionPerformed(evt);
	                     }
	                  });
	               }
	               {
	                  mniAdmin = new JMenuItem();
	                  mnuEjemplos.add(mniAdmin);
	                  mniAdmin.setText("Busqueda para Admin");
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
	         this.setSize(1000, 1000);
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
	        this.ConsultasATM.setMaximum (true);
	    }
	    catch (PropertyVetoException e) {}
	    this.ConsultasATM.setVisible (true);      
    }
}