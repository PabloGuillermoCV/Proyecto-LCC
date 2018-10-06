package Banco;

import java.awt.BorderLayout;
import java.awt.Dimension;
import java.awt.event.ActionEvent;
import java.awt.event.ActionListener;
import java.awt.event.ComponentAdapter;
import java.awt.event.ComponentEvent;
import java.awt.event.MouseAdapter;
import java.awt.event.MouseEvent;
import java.sql.Types;
import java.sql.Connection;
import java.sql.ResultSet;
import java.sql.SQLException;
import java.sql.Statement;
import javax.swing.BorderFactory;
import javax.swing.DefaultListModel;
import javax.swing.JButton;
import javax.swing.JOptionPane;
import javax.swing.JPanel;
import javax.swing.JPasswordField;
import javax.swing.JScrollPane;
import javax.swing.JTextArea;
import javax.swing.SwingUtilities;
import javax.swing.WindowConstants;
import javax.swing.border.BevelBorder;
import javax.swing.event.ListSelectionEvent;
import javax.swing.event.ListSelectionListener;

import quick.dbtable.*;
import javax.swing.JList;
import javax.swing.JSplitPane;

@SuppressWarnings("serial")
public class VentanaAdmin extends javax.swing.JInternalFrame {
	
	private JPanel pnlConsulta;
	private JTextArea txtConsulta;
	private JButton botonBorrar;
	private JButton btnEjecutar;
	private DBTable tabla = new DBTable();    
	private JScrollPane scrConsulta;
	protected Connection conexionBD = null;
	private JSplitPane Listas;
	protected int Seleccionado = -1;
	private String clave;
	private JPasswordField pf;
	private JList <String> listaTablas;
	private JList <String> listaAtributosTabla;
	private DefaultListModel <String> modeloListaAtributos;
	
	
	public VentanaAdmin () {
		super ();
		initGUI ();
	}
	
	//Utilizo el initGUI de VentanaConsultas por el momento
    //Tiene elementos de la BD de la clase que hay que cambiar
	private void initGUI () {
        try {
        	 {
 	         	Listas = new JSplitPane();
 	         	getContentPane().add(Listas, BorderLayout.CENTER);
 	         	
 	         }
	        setPreferredSize(new Dimension(800, 600));
	        this.setBounds(0, 0, 800, 600);
	        setVisible(true);
	        BorderLayout thisLayout = new BorderLayout();
	        this.setTitle("Consultas Admin (Utilizando DBTable)");
	        getContentPane().setLayout(thisLayout);
	        this.setClosable(true);
	        this.setDefaultCloseOperation(WindowConstants.HIDE_ON_CLOSE);
	        this.setMaximizable(true);
	        this.addComponentListener(new ComponentAdapter() {
	            public void componentHidden(ComponentEvent evt) {
	                thisComponentHidden(evt);
	            }
	            public void componentShown(ComponentEvent evt) {
	                thisComponentShown(evt);
	            }
	        }
	        ); {
	            pnlConsulta = new JPanel();
	            getContentPane().add(pnlConsulta, BorderLayout.NORTH); {
	                scrConsulta = new JScrollPane();
	                pnlConsulta.add(scrConsulta); {
	                    txtConsulta = new JTextArea();
	                    scrConsulta.setViewportView(txtConsulta);
	                    txtConsulta.setTabSize(3);
	                    txtConsulta.setColumns(80);
	                    txtConsulta.setBorder(BorderFactory.createEtchedBorder(BevelBorder.LOWERED));
	                    txtConsulta.setText("SELECT\r\nFROM \r\nWHERE\r\n");
	                    txtConsulta.setFont(new java.awt.Font("Monospaced",0,12));
	                    txtConsulta.setRows(10);
	                 }
	            }
	            {
	                btnEjecutar = new JButton();
	                pnlConsulta.add(btnEjecutar);
	                btnEjecutar.setText("Ejecutar");
	                btnEjecutar.addActionListener(new ActionListener() {
	                    public void actionPerformed(ActionEvent evt) {
	                        btnEjecutarActionPerformed(evt);
	                    }
	                });
	            }
	            {
	                botonBorrar = new JButton();
	            	pnlConsulta.add(botonBorrar);
	            	botonBorrar.setText("Borrar");            
	            	botonBorrar.addActionListener(new ActionListener() {
	            	    public void actionPerformed(ActionEvent arg0) {
	            		    txtConsulta.setText("");            			
	            		}
	            	});
	            }	
	            {
	            	//Muestra todas las tablas en la base de datos
	            	DefaultListModel <String> dlm = new DefaultListModel <String> ();
	            	listaTablas = new JList <> (dlm);
	            	JScrollPane panelLista = new JScrollPane (listaTablas);
	            	Listas.add (panelLista);
	            	
	            	
	            	Statement stmt = this.conexionBD.createStatement ();
	         		String sql = "SHOW TABLES";
	         		ResultSet rs = stmt.executeQuery(sql);
	         		
	         		int I = 0;
	         		String nombreTabla;
	         		while (rs.next ()) {
	         			nombreTabla = rs.getString(I);
	         			dlm.addElement (nombreTabla);
	         			I++;
	         		}
	         		
	         		rs.close();
	         		stmt.close();
	         		
	         		listaTablas.addListSelectionListener(new ListSelectionListener() {
	         		      public void valueChanged(ListSelectionEvent evt) {
	         		        mostrarAtributos();
	         		      }
	         		});
	            }
	            {
	            	//Muestra todos los atributos de una tabla al seleccionarla
	            	modeloListaAtributos = new DefaultListModel <String> ();
	            	listaAtributosTabla = new JList <> (modeloListaAtributos);
	            	JScrollPane panelLista = new JScrollPane (listaAtributosTabla);
	            	Listas.add (panelLista);
	            	listaAtributosTabla.setEnabled (false);
	            }
	         }
	         {
	              
	        	 // Agrega la tabla al frame (no necesita JScrollPane como Jtable)
	             getContentPane().add(tabla, BorderLayout.WEST);           
	                      
	             // setea la tabla para solo lectura (no se puede editar su contenido)  
	             tabla.setEditable(false);
	         }
	        
	         {
	   
	         }
	    } 
		catch (Exception e) {
	        e.printStackTrace();
	    }
	}
	/**
	 * Método privado para poblar la lista con las tablas de la BD 
	 * (No seria necesario ya que vos ya llenas la lista al inicializar?)
	 */
	/*private void llenarLista(){
		try{
		 String sql = "SHOW TABLES FROM banco";
		}
		catch(SQLException e){
			 JOptionPane.showMessageDialog(this,
                     "Se produjo un error al intentar obtener las Tablas de la BD.\n" + e.getMessage(),
                     "Error",
                     JOptionPane.ERROR_MESSAGE);
				System.out.println("SQLException: " + e.getMessage());
				System.out.println("SQLState: " + e.getSQLState());
				System.out.println("VendorError: " + e.getErrorCode());
		}
	}*/
	
	private void thisComponentShown (ComponentEvent evt) {
		//Para manejar Logins, hago lo siguiente:
		/*
		 * uso JOptionPane.DialogPane para nombres de usuario,/Legajos
		 * para Claves, uso un JOptionPane.showConfirmDialog con un campo password para levantar el password ingresado
		 */
		int okCxl = JOptionPane.showConfirmDialog(null, pf, "Enter Password", JOptionPane.OK_CANCEL_OPTION, JOptionPane.PLAIN_MESSAGE);
		if (okCxl == JOptionPane.OK_OPTION) {
			clave = pf.getPassword().toString(); 
		}
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
			//en clave admin, va lo que ingrese el usuario por pantalla, en los otros casos, se hace directamente
	        String driver ="com.mysql.cj.jdbc.Driver";
	        String servidor = "localhost:3306";
	        String baseDatos = "banco";
	        String usuario = "admin";
	        //String clave = "admin"; se toma de lo que ingreso el usuario
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
	
	private void mostrarAtributos () {
		//Se encarga de ubicar cada atributo en listaAtributosTabla
		try {
			
			Statement stmt = this.conexionBD.createStatement ();
			//DESCRIBE hace lo mismo, pero como son equivalentes lo dejo
			String sql = "SHOW COLUMNS FROM " + listaTablas.getSelectedValue (); 
			ResultSet rs = stmt.executeQuery(sql);
			
			int I = 0;
     		String nombreAtributo;
     		while (rs.next ()) {
     			nombreAtributo = rs.getString(I); 
     			modeloListaAtributos.addElement (nombreAtributo);
     			I++;
     		}
			
			rs.close();
     		stmt.close();
     		
		}
		catch (SQLException e) {
			e.printStackTrace();
		}
 		
 		
	}
}