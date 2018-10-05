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
import javax.swing.JFrame;
import javax.swing.JOptionPane;
import javax.swing.JPanel;
import javax.swing.JPasswordField;
import javax.swing.JScrollPane;
import javax.swing.JTextArea;
import javax.swing.JTextField;
import javax.swing.SwingUtilities;
import javax.swing.WindowConstants;
import javax.swing.border.BevelBorder;
import quick.dbtable.*;
import javax.swing.JLabel;
import java.awt.Font;
import javax.swing.SwingConstants;
import java.awt.Rectangle;

@SuppressWarnings({ "serial", "unused" })
public class VentanaEmpleado extends javax.swing.JInternalFrame {
	
	private JPanel pnlConsulta;
	private JTextArea txtConsulta;
	private JButton botonBorrar;
	private JButton btnEjecutar;   
	private JScrollPane scrConsulta;
	private String legajo;
	private String clave;
	private JPasswordField pf;
	private JTextField nf;
	private JTextField Num_doc;
	private JTextField Tipo_Doc;
	private DBTable TablaPagoCuotas;
	private DBTable Listado_Morosos;
	
	public VentanaEmpleado () {
		super ();
		setTitle("Consultas Empleado");
		getContentPane().setLayout(null);
		
		JPanel Pago_Cuotas = new JPanel();
		Pago_Cuotas.setBounds(7, 161, 277, 326);
		getContentPane().add(Pago_Cuotas);
		
		TablaPagoCuotas = new DBTable();
		TablaPagoCuotas.setEditable(false);
		Pago_Cuotas.add(TablaPagoCuotas);
		
		JPanel Morosos = new JPanel();
		Morosos.setBounds(294, 161, 199, 326);
		getContentPane().add(Morosos);
		
		Listado_Morosos = new DBTable();
		Listado_Morosos.setEditable(false);
		Morosos.add(Listado_Morosos);
		
		Num_doc = new JTextField();
		Num_doc.setBounds(10, 83, 129, 20);
		getContentPane().add(Num_doc);
		Num_doc.setColumns(10);
		
		JLabel Label_Doc = new JLabel("Numero de Documento");
		Label_Doc.setHorizontalAlignment(SwingConstants.CENTER);
		Label_Doc.setFont(new Font("Tahoma", Font.BOLD, 11));
		Label_Doc.setBounds(10, 58, 129, 14);
		getContentPane().add(Label_Doc);
		
		JLabel Label_tipo_doc = new JLabel("Tipo de Documento");
		Label_tipo_doc.setHorizontalAlignment(SwingConstants.CENTER);
		Label_tipo_doc.setFont(new Font("Tahoma", Font.BOLD, 11));
		Label_tipo_doc.setBounds(336, 58, 122, 14);
		getContentPane().add(Label_tipo_doc);
		
		Tipo_Doc = new JTextField();
		Tipo_Doc.setBounds(346, 83, 112, 20);
		getContentPane().add(Tipo_Doc);
		Tipo_Doc.setColumns(10);
		
		JButton CrearPrest = new JButton("Crear Prestamo");
		CrearPrest.addActionListener(new ActionListener(){
			public void actionPerformed(ActionEvent arg1) {
				CrearPrest(arg1);
			}
		});
		CrearPrest.setBounds(7, 114, 107, 23);
		getContentPane().add(CrearPrest);
		
		JButton CuotasBtn = new JButton("Ver Cuotas");
		CuotasBtn.addActionListener(new ActionListener() {
			public void actionPerformed(ActionEvent arg0) {
				verCuotas(arg0);
			}
		});
		CuotasBtn.setBounds(179, 114, 107, 23);
		getContentPane().add(CuotasBtn);
		
		JButton btnVerMorosos = new JButton("Ver Morosos");
		btnVerMorosos.addActionListener(new ActionListener() {
			public void actionPerformed(ActionEvent e) {
				verMor(e);
			}
		});
		btnVerMorosos.setBounds(351, 114, 107, 23);
		getContentPane().add(btnVerMorosos);
		
		JLabel lblTablaCuotas = new JLabel("Tabla Cuotas");
		lblTablaCuotas.setHorizontalAlignment(SwingConstants.CENTER);
		lblTablaCuotas.setFont(new Font("Tahoma", Font.BOLD, 11));
		lblTablaCuotas.setBounds(7, 148, 89, 14);
		getContentPane().add(lblTablaCuotas);
		
		JLabel LbTablaMorosos = new JLabel("Tabla Morosos");
		LbTablaMorosos.setFont(new Font("Tahoma", Font.BOLD, 11));
		LbTablaMorosos.setHorizontalAlignment(SwingConstants.CENTER);
		LbTablaMorosos.setBounds(294, 148, 86, 14);
		getContentPane().add(LbTablaMorosos);
		}
	
	//Métodos donde irian las consultas SQL
	
	/**
	 * llena la Tabla de Clientes Morosos
	 * @param e Action Event del botón, no se usa pa mucho
	 */
	private void verMor(ActionEvent e) {
		
	}
	
	/**
	 * llena la Tabla de Cuotas de un cliente
	 * @param e ActionEvent del botón, no se usa pa mucho, hay que agarrar los textos de los JTextField
	 */
	private void verCuotas(ActionEvent e) {
		
	}
	
	/**
	 * Intenta crear un prestamo para un cliente
	 * @param e ActionEvent del Botón, ne se usa para mucho, hay que agarrar los textos de los JTextField
	 */
	private void CrearPrest(ActionEvent e) {
		
	}
	
	private void thisComponentShown (ComponentEvent evt) {
		login();
		this.conectarBD();
	}
	
	/**
	 * Hace login del Empleado por medio de Pop Ups
	 */
	private void login() {
		final JFrame parent = new JFrame();
        JButton button = new JButton();
        parent.getContentPane().add(button);
        parent.pack();
        parent.setVisible(true);

        button.addActionListener(new java.awt.event.ActionListener() {
            @Override
            public void actionPerformed(java.awt.event.ActionEvent evt) {
                legajo = JOptionPane.showInputDialog(parent,
                        "Ingrese numero de Legajo", null);
            }
        });
		int okCxl = JOptionPane.showConfirmDialog(null, pf, "Enter Password", JOptionPane.OK_CANCEL_OPTION, JOptionPane.PLAIN_MESSAGE);
		if (okCxl == JOptionPane.OK_OPTION) {
			clave = pf.getPassword().toString(); 
		}
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
	        //String legajo = "empleado";
	        //String clave = "empleado";
	        String uriConexion = "jdbc:mysql://" + servidor + "/" + baseDatos+"?serverTimezone=UTC";
	        //Establece una conexion con la  B.D. "banco"  usando directamante una tabla DBTable    
	        Listado_Morosos.connectDatabase (driver, uriConexion, legajo, clave);
	        TablaPagoCuotas.connectDatabase(driver, uriConexion, legajo, clave);   
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
			Listado_Morosos.close();
			TablaPagoCuotas.close();  
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
	    	Listado_Morosos.setSelectSql(this.txtConsulta.getText().trim());
	    	// obtenemos el modelo de la tabla a partir de la consulta para 
	    	// modificar la forma en que se muestran de algunas columnas  
	    	Listado_Morosos.createColumnModelFromQuery();    	    
	    	for (int i = 0; i < Listado_Morosos.getColumnCount(); i++) { 
	    	    // para que muestre correctamente los valores de tipo TIME (hora)  		   		  
	    		if (Listado_Morosos.getColumn(i).getType()==Types.TIME) {    		 
	    		    Listado_Morosos.getColumn(i).setType(Types.CHAR);  
	  	       	}
	    		// cambiar el formato en que se muestran los valores de tipo DATE
	    		if (Listado_Morosos.getColumn(i).getType()==Types.DATE) {
	    		    Listado_Morosos.getColumn(i).setDateFormat("dd/MM/YYYY");
	    		}
	        }  
	    	// seteamos la consulta a partir de la cual se obtendran los datos para llenar la tabla
	    	TablaPagoCuotas.setSelectSql(this.txtConsulta.getText().trim());
	    	// obtenemos el modelo de la tabla a partir de la consulta para 
	    	// modificar la forma en que se muestran de algunas columnas  
	    	TablaPagoCuotas.createColumnModelFromQuery();    	    
	    	for (int i = 0; i < TablaPagoCuotas.getColumnCount(); i++) { 
	    	    // para que muestre correctamente los valores de tipo TIME (hora)  		   		  
	    		if (TablaPagoCuotas.getColumn(i).getType()==Types.TIME) {    		 
	    			TablaPagoCuotas.getColumn(i).setType(Types.CHAR);  
	  	       	}
	    		// cambiar el formato en que se muestran los valores de tipo DATE
	    		if (TablaPagoCuotas.getColumn(i).getType()==Types.DATE) {
	    			TablaPagoCuotas.getColumn(i).setDateFormat("dd/MM/YYYY");
	    		}
	        }  
	    	// actualizamos el contenido de la tabla.   	     	  
	    	Listado_Morosos.refresh();
	    	TablaPagoCuotas.refresh();
	    	// actualizamos el contenido de la tabla.   	     	  
	    	Listado_Morosos.refresh();
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