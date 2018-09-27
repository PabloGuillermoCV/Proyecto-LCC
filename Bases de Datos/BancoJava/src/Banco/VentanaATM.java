package Banco;

import java.awt.BorderLayout;
import java.awt.Dimension;
import java.awt.event.ActionEvent;
import java.awt.event.ActionListener;
import java.awt.event.ComponentAdapter;
import java.awt.event.ComponentEvent;
import java.sql.Types;
import java.sql.SQLException;
import javax.swing.BorderFactory;
import javax.swing.JButton;
import javax.swing.JOptionPane;
import javax.swing.JPanel;
import javax.swing.JScrollPane;
import javax.swing.JTextArea;
import javax.swing.SwingUtilities;
import javax.swing.WindowConstants;
import javax.swing.border.BevelBorder;
import quick.dbtable.*;
import javax.swing.GroupLayout;
import javax.swing.GroupLayout.Alignment;
import javax.swing.JPasswordField;
import javax.swing.JLabel;
import javax.swing.LayoutStyle.ComponentPlacement;
import javax.swing.SwingConstants;
import java.awt.Font;

@SuppressWarnings({ "serial", "unused" })
public class VentanaATM extends javax.swing.JInternalFrame {
	
	private JPanel pnlConsulta;
	private JTextArea txtConsulta;
	private JButton botonBorrar;
	private JButton btnEjecutar;
	private DBTable tabla;    
	private JScrollPane scrConsulta;
	private JPasswordField PIN_Field;
	private JPasswordField Card;
	
	public VentanaATM () {
		super ();
		setTitle("Ingreso ATM");
		
		PIN_Field = new JPasswordField();
		
		Card = new JPasswordField();
		
		JButton btnConsultarCliente = new JButton("Consultar Cliente");
		
		JLabel lblNumeroDeTarjeta = new JLabel("Numero de Tarjeta");
		lblNumeroDeTarjeta.setFont(new Font("Tahoma", Font.BOLD, 11));
		lblNumeroDeTarjeta.setEnabled(false);
		lblNumeroDeTarjeta.setHorizontalAlignment(SwingConstants.CENTER);
		
		JLabel lblPinTarjeta = new JLabel("PIN Tarjeta");
		lblPinTarjeta.setEnabled(false);
		lblPinTarjeta.setFont(new Font("Tahoma", Font.BOLD, 11));
		lblPinTarjeta.setHorizontalAlignment(SwingConstants.CENTER);
		
		JTextArea txtrIngreseUnNmero = new JTextArea();
		txtrIngreseUnNmero.setFont(new Font("Monospaced", Font.PLAIN, 13));
		txtrIngreseUnNmero.setWrapStyleWord(true);
		txtrIngreseUnNmero.setText("Ingrese un N\u00FAmero de Tarjeta y su Correspondiente PIN para visualizar las consultas pertinentes a un Cliente dentro de un ATM");
		txtrIngreseUnNmero.setLineWrap(true);
		txtrIngreseUnNmero.setEditable(false);
		GroupLayout groupLayout = new GroupLayout(getContentPane());
		groupLayout.setHorizontalGroup(
			groupLayout.createParallelGroup(Alignment.LEADING)
				.addGroup(groupLayout.createSequentialGroup()
					.addGroup(groupLayout.createParallelGroup(Alignment.LEADING)
						.addGroup(groupLayout.createSequentialGroup()
							.addGap(157)
							.addGroup(groupLayout.createParallelGroup(Alignment.TRAILING, false)
								.addComponent(Card, Alignment.LEADING)
								.addComponent(PIN_Field, Alignment.LEADING, GroupLayout.DEFAULT_SIZE, 102, Short.MAX_VALUE)
								.addComponent(lblNumeroDeTarjeta, GroupLayout.DEFAULT_SIZE, GroupLayout.DEFAULT_SIZE, Short.MAX_VALUE)
								.addComponent(lblPinTarjeta, Alignment.LEADING, GroupLayout.DEFAULT_SIZE, GroupLayout.DEFAULT_SIZE, Short.MAX_VALUE)))
						.addGroup(groupLayout.createSequentialGroup()
							.addGap(142)
							.addComponent(btnConsultarCliente, GroupLayout.PREFERRED_SIZE, 134, GroupLayout.PREFERRED_SIZE))
						.addGroup(groupLayout.createSequentialGroup()
							.addContainerGap()
							.addComponent(txtrIngreseUnNmero, GroupLayout.DEFAULT_SIZE, 414, Short.MAX_VALUE)))
					.addContainerGap())
		);
		groupLayout.setVerticalGroup(
			groupLayout.createParallelGroup(Alignment.LEADING)
				.addGroup(groupLayout.createSequentialGroup()
					.addContainerGap()
					.addComponent(txtrIngreseUnNmero, GroupLayout.PREFERRED_SIZE, 63, GroupLayout.PREFERRED_SIZE)
					.addPreferredGap(ComponentPlacement.RELATED)
					.addComponent(btnConsultarCliente)
					.addGap(28)
					.addComponent(lblNumeroDeTarjeta)
					.addPreferredGap(ComponentPlacement.UNRELATED)
					.addComponent(Card, GroupLayout.PREFERRED_SIZE, GroupLayout.DEFAULT_SIZE, GroupLayout.PREFERRED_SIZE)
					.addGap(26)
					.addComponent(lblPinTarjeta)
					.addPreferredGap(ComponentPlacement.UNRELATED)
					.addComponent(PIN_Field, GroupLayout.PREFERRED_SIZE, GroupLayout.DEFAULT_SIZE, GroupLayout.PREFERRED_SIZE)
					.addContainerGap(23, Short.MAX_VALUE))
		);
		getContentPane().setLayout(groupLayout);
		initGUI ();
	}
	
	private void initGUI () {
		
	}
	
	private void thisComponentShown (ComponentEvent evt) {
		this.conectarBD();
	}
	   
	private void thisComponentHidden (ComponentEvent evt) {
	    this.desconectarBD();
	}

	private void btnEjecutarActionPerformed (ActionEvent evt) {
	    this.refrescarTabla();      
    }
	
	private void conectarBD () {
		try {
	        String driver ="com.mysql.cj.jdbc.Driver";
	        String servidor = "%";
	        String baseDatos = "banco";
	        String usuario = "atm";
	        String clave = "atm";
	        String uriConexion = "jdbc:mysql://" + servidor + "/" + baseDatos+"?serverTimezone=UTC";
	        //Establece una conexion con la  B.D. "banco"  usando directamante una tabla DBTable    
	        tabla.connectDatabase (driver, uriConexion, usuario, clave);
	           
	    }
	    catch (SQLException ex) {
	        JOptionPane.showMessageDialog (this,
	        			"Se produjo un error al intentar conectarse a la base de datos.\n" + ex.getMessage(),
	                    "Error",
	                    JOptionPane.ERROR_MESSAGE);
	        System.out.println("SQLException: " + ex.getMessage());
	        System.out.println("SQLState: " + ex.getSQLState());
	        System.out.println("VendorError: " + ex.getErrorCode());
	    }
	    catch (ClassNotFoundException e) {
	        e.printStackTrace();
	    }
	}
	
	private void desconectarBD () {
		try {
		    tabla.close();            
	    }
	    catch (SQLException ex) {
	        System.out.println("SQLException: " + ex.getMessage());
	        System.out.println("SQLState: " + ex.getSQLState());
	        System.out.println("VendorError: " + ex.getErrorCode());
	    }
	}
	
	private void refrescarTabla () {
	    try {    
	        // seteamos la consulta a partir de la cual se obtendran los datos para llenar la tabla
	    	tabla.setSelectSql(this.txtConsulta.getText().trim());
	    	//aca iria las consultas sql generadas a partir de lo que eligio el usuario, lo ponemos nosotros y el dbtable se encarga de pasarlo a sql
	    	// obtenemos el modelo de la tabla a partir de la consulta para 
	    	// modificar la forma en que se muestran de algunas columnas  
	    	tabla.createColumnModelFromQuery();    	    
	    	for (int i = 0; i < tabla.getColumnCount(); i++) { 
	    	    // para que muestre correctamente los valores de tipo TIME (hora)  		   		  
	    		if (tabla.getColumn(i).getType()==Types.TIME) {    		 
	    		    tabla.getColumn(i).setType(Types.CHAR);  
	  	       	}
	    		// cambiar el formato en que se muestran los valores de tipo DATE
	    		if (tabla.getColumn(i).getType()==Types.DATE) {
	    		    tabla.getColumn(i).setDateFormat("dd/MM/YYYY");
	    		}
	    		//al entrar, pido el pin de tarjeta y compruebo que exista ese pin en la tabla
	        }  
	    	// actualizamos el contenido de la tabla.   	     	  
	    	tabla.refresh();
	    	// No es necesario establecer  una conexion, crear una sentencia y recuperar el 
	    	// resultado en un resultSet, esto lo hace automaticamente la tabla (DBTable) a 
	    	// patir de la conexion y la consulta seteadas con connectDatabase() y 
	        // setSelectSql() respectivamente.
	    }
	    catch (SQLException ex)
	    {
	        // en caso de error, se muestra la causa en la consola
	        System.out.println("SQLException: " + ex.getMessage());
	        System.out.println("SQLState: " + ex.getSQLState());
	        System.out.println("VendorError: " + ex.getErrorCode());
	        JOptionPane.showMessageDialog(SwingUtilities.getWindowAncestor(this),
	                            ex.getMessage() + "\n", 
	                            "Error al ejecutar la consulta.",
	                            JOptionPane.ERROR_MESSAGE);
	         
	    }
    }
}