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

@SuppressWarnings({ "serial", "unused" })
public class VentanaPrincipal extends javax.swing.JFrame {
	
	private VentanaAdmin ventanaAdmin;
	private VentanaEmpleado ventanaEmpleado;
	private VentanaATM ventanaATM;
	private JButton btnCerrar;
	
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
		initGUI ();
		this.ventanaAdmin = new VentanaAdmin ();
		this.ventanaEmpleado = new VentanaEmpleado ();
		this.ventanaATM = new VentanaATM ();
		this.ventanaAdmin.setVisible(false);
		this.ventanaEmpleado.setVisible(false);
		this.ventanaATM.setVisible(false);
		
	}
	
	private void initGUI () {
		
	}
	
	private void mniSalirActionPerformed (ActionEvent evt) {
	    this.dispose();
	}
	   
	private void mniBarcosActionPerformed (ActionEvent evt) {
	    try {
	        this.ventanaAdmin.setMaximum (true);
	    }
	    catch (PropertyVetoException e) {}
	    this.ventanaAdmin.setVisible (true);      
	}
	   
    private void mniConsultasActionPerformed (ActionEvent evt) {
	    try {
	        this.ventanaEmpleado.setMaximum (true);
	    }
	    catch (PropertyVetoException e) {}
	    this.ventanaEmpleado.setVisible (true);      
	}
	   
    private void mniAbmBatallasActionPerformed (ActionEvent evt) {
	    try {
	        this.ventanaATM.setMaximum (true);
	    }
	    catch (PropertyVetoException e) {}
	    this.ventanaATM.setVisible (true);      
    }
}