package Banco;

import java.awt.BorderLayout;
import java.awt.Dimension;
import java.awt.event.ActionEvent;
import java.awt.event.ActionListener;
import java.awt.event.ComponentAdapter;
import java.awt.event.ComponentEvent;
import java.sql.Types;
import java.util.Arrays;
import java.sql.SQLException;
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
	private JSplitPane Listas;
	protected int Seleccionado = -1;
	private String clave;
	private JPasswordField pf;
	private JList <String> listaTablas;
	private DefaultListModel <String> modeloTablas;
	private JList <String> listaAtributosTabla;
	private DefaultListModel <String> modeloListaAtributos;
	
	
	public VentanaAdmin () {
		super ();
		initGUI ();
	}
	
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
	                    txtConsulta.setText("SELECT \r\nFROM \r\nWHERE \r\n");
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
	            	modeloTablas = new DefaultListModel <String> ();
	            	listaTablas = new JList <> (modeloTablas);
	            	JScrollPane panelLista = new JScrollPane (listaTablas,JScrollPane.VERTICAL_SCROLLBAR_ALWAYS,JScrollPane.HORIZONTAL_SCROLLBAR_ALWAYS);
	            	//panelLista.setBounds(0, 20, 600, 600);
	            	//panelLista.contains(1, 1);
	            	pnlConsulta.add (panelLista);
	         		
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
	            	pnlConsulta.add (panelLista);
	            }
	         }
	         {
	              
	        	 // Agrega la tabla al frame (no necesita JScrollPane como Jtable)
	             getContentPane().add(tabla, BorderLayout.WEST);           
	                      
	             // setea la tabla para solo lectura (no se puede editar su contenido)  
	             tabla.setEditable(false);
	         }
	    } 
		catch (Exception e) {
	        e.printStackTrace();
	    }
	}
	
	private void thisComponentShown (ComponentEvent evt) {
		//Para manejar Logins, hago lo siguiente:
		/*
		 * uso JOptionPane.DialogPane para nombres de usuario,/Legajos
		 * para Claves, uso un JOptionPane.showConfirmDialog con un campo password para levantar el password ingresado
		 */
		char [] pass = {'a','d','m','i','n'};
		pf = new JPasswordField();
		int okCxl = JOptionPane.showConfirmDialog(null, pf, "Enter Password", JOptionPane.OK_CANCEL_OPTION, JOptionPane.PLAIN_MESSAGE);
		if (okCxl == JOptionPane.OK_OPTION) {
			if(Arrays.equals(pf.getPassword(),pass)) {
				clave = "admin";
			}
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
		    String driver = "com.mysql.cj.jdbc.Driver";
		    String usuario = "admin";
		    String urlConexion = "jdbc:mysql://localhost/banco?serverTimezone=UTC&useSSL=false&allowPublicKeyRetrieval=true";
		    //Establece una conexion con la  B.D. "banco"  usando directamante una tabla DBTable
		    tabla.connectDatabase (driver, urlConexion, usuario, clave);
		    mostrarTablas();
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
	
	private void mostrarTablas () {
		//Se encarga de poner el nombre de cada tabla en listaTablas
		try {
			tabla.setSelectSql("SHOW TABLES FROM BANCO");
			
			tabla.createColumnModelFromQuery();
			
			String nombreTabla;
			for (int I = 0; I < tabla.getRowCount(); I++) {
				nombreTabla = (String) tabla.getRowObject(I);
				modeloTablas.addElement (nombreTabla);
			}
		}
		catch (SQLException e) {
			e.printStackTrace();
		}
	}
	
	private void mostrarAtributos () {
		//Se encarga de ubicar cada atributo en listaAtributosTabla
		try {
			tabla.setSelectSql("SHOW COLUMNS FROM " + listaTablas.getSelectedValue ());
			
			tabla.createColumnModelFromQuery();
			
     		String nombreAtributo;
     		for (int I = 0; I < tabla.getRowCount(); I++) {
				nombreAtributo = (String) tabla.getRowObject(I);
				modeloTablas.addElement (nombreAtributo);
			}
		}
		catch (SQLException e) {
			e.printStackTrace();
		}
	}
}