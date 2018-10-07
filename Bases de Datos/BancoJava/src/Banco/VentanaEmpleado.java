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
import java.time.LocalDateTime;
import java.time.format.DateTimeFormatter;
import java.util.GregorianCalendar;
import java.sql.Connection;
import java.sql.Date;
import java.sql.DriverManager;
import java.sql.ResultSet;
import java.sql.SQLException;
import java.sql.Statement;
import javax.swing.BorderFactory;
import javax.swing.JButton;
import javax.swing.JFrame;
import javax.swing.JOptionPane;
import javax.swing.JPanel;
import javax.swing.JPasswordField;
import javax.swing.JScrollPane;
import javax.swing.JTextArea;
import javax.swing.JTextField;
import javax.swing.ListSelectionModel;
import javax.swing.SwingUtilities;
import javax.swing.WindowConstants;
import javax.swing.border.BevelBorder;
import javax.swing.event.ListSelectionEvent;
import javax.swing.event.ListSelectionListener;
import javax.swing.table.DefaultTableModel;
import quick.dbtable.*;
import javax.swing.JLabel;
import java.awt.Font;
import javax.swing.SwingConstants;
import java.awt.Rectangle;
import javax.swing.JList;

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
	private DBTable tabla = new DBTable();
	private String nro;
	private String tipo;
	protected Connection conexionBD = null;
	int seleccionado = -1;
	
	
	public VentanaEmpleado () {
		super ();
		setClosable(true);
		setResizable(true);
		setMaximizable(true);
		setTitle("Consultas Empleado");
		getContentPane().setLayout(null);
		this.addComponentListener(new ComponentAdapter() {
            public void componentHidden(ComponentEvent evt) {
                thisComponentHidden(evt);
            }
            public void componentShown(ComponentEvent evt) {
                thisComponentShown(evt);
            }
        });
		
		JPanel PaneTabla = new JPanel();
		PaneTabla.setBounds(0, 161, 493, 326);
		getContentPane().add(PaneTabla);
			
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
		Label_tipo_doc.setBounds(662, 58, 122, 14);
		getContentPane().add(Label_tipo_doc);
		
		Tipo_Doc = new JTextField();
		Tipo_Doc.setBounds(672, 83, 112, 20);
		getContentPane().add(Tipo_Doc);
		Tipo_Doc.setColumns(10);
		
		tabla = new DBTable();
		PaneTabla.add(tabla);
		//hago que NO pueda ser seleccionable por defecto
		tabla.setEnabled(false);
		tabla.addMouseListener(new MouseAdapter() {
			public void MouseListener(MouseEvent evt) {
				TablaClick(evt);
			}
		});
		
		JButton CrearPrest = new JButton("Crear Prestamo");
		CrearPrest.addActionListener(new ActionListener(){
			public void actionPerformed(ActionEvent arg1) {
				tabla.setEnabled(false);
				CrearPrest(arg1);
			}
		});
		CrearPrest.setBounds(20, 114, 107, 23);
		getContentPane().add(CrearPrest);
		
		JButton CuotasBtn = new JButton("Ver Cuotas");
		CuotasBtn.addActionListener(new ActionListener() {
			public void actionPerformed(ActionEvent arg0) {
				tabla.setEnabled(true);
				verCuotas(arg0);
			}
		});
		CuotasBtn.setBounds(366, 114, 107, 23);
		getContentPane().add(CuotasBtn);
		
		JButton btnVerMorosos = new JButton("Ver Morosos");
		btnVerMorosos.addActionListener(new ActionListener() {
			public void actionPerformed(ActionEvent e) {
				tabla.setEnabled(false);
				verMor(e);
			}
		});
		btnVerMorosos.setBounds(672, 114, 107, 23);
		getContentPane().add(btnVerMorosos);
		
		
		}

	private void TablaClick(MouseEvent evt) {
		if ((this.tabla.getSelectedRow() != -1) && (evt.getClickCount() == 2))
	      {
	         seleccionarFila();
	      }
	}
	
	private void seleccionarFila() {
		this.seleccionado = this.tabla.getSelectedRow();
		//Obtengo los valores en la Tabla por la fila que se selecciono
		String nroCuota = tabla.getValueAt(seleccionado, 1).toString().trim();
		String valor = tabla.getValueAt(seleccionado, 2).toString().trim();
		//hago un pop up para que se confirme, si se confirma, se registra el pago
		int ok = JOptionPane.showConfirmDialog(null, null,"Desea registrar el pago de la cuota numero " + nroCuota + " con valor " + valor + "?", JOptionPane.OK_CANCEL_OPTION,JOptionPane.PLAIN_MESSAGE);
		if(ok == JOptionPane.OK_OPTION) {
			registrarPago(nroCuota,valor);
		}
	}
	
	private void registrarPago(String nroC, String money) {
		try {
			//Le tengo miedo al tema de las fechas, revisar
			 DateTimeFormatter dtf = DateTimeFormatter.ofPattern("dd/MM/yyyy");  
			 LocalDateTime now = LocalDateTime.now(); 
			 String d = now.toString();
			Statement stmt = this.conexionBD.createStatement();
			//Hago un UPDATE Query, revisar si esta bien
			stmt.executeUpdate("UPDATE Pago SET fecha_pago = " +"STR_TO_DATE(" + d + ",\"%d-%m-%Y\")" + 
						"WHERE Pago.nro_pago = " + nroC);
			if(stmt.getUpdateCount() != -1) {
				//El Update tuvo exito
				JOptionPane.showInternalMessageDialog(null, "Se ha registrado el Pago de la Cuota con exito");
			}
		} catch (SQLException e) {
			e.printStackTrace();
		}
	}
	
	//Metodos donde irian las consultas SQL
	/**
	 * llena la Tabla con Clientes Morosos
	 * @param e Action Event del bot�n, no se usa pa mucho
	 */
	private void verMor(ActionEvent e) {
		//Necesito los datos del cliente (Nro Cliente, Tipo y Nro de Doc, Apellido y nombre)
		//Para cada cliente, neceisto los Prestamos en discordia (Nro de Prest, monto, cant_meses y valor cuota
		//para cada Prestamo, necesito saber la cantidad de cuotas atrasadas (tienen que ser al menos 2)
		try {
			Statement stmt = this.conexionBD.createStatement();
			String SQL1 = "SELECT nro_cliente, tipo_doc , nro_doc, apellido, nombre, nro_prestamo, monto, cant_meses,valor_cuota, COUNT(nro_pago)"
					+ "FROM (cliente NATURAL JOIN prestamo) AS x, pago "
					+ "WHERE x.nro_prestamo = pago.nro_prestamo and COUNT(nro_pago) >= 2"; // , verificar, falta la condicion de moroso
			ResultSet rs = stmt.executeQuery(SQL1);
			tabla.refresh(rs);

			//para que esta esto de abajo? lo de arriba ^ deberia hacer todo autom�ticamente
			//Aunque DBTable no tiene SetAutoCreateRowSorter, ver esto
	        int I = 0;
	        while (rs.next()) {
	            tabla.setValueAt(rs.getInt("nro_cliente"), I, 0);
	            tabla.setValueAt(rs.getString("tipo_doc"), I, 1);
	            tabla.setValueAt(rs.getInt("nro_doc"), I, 2);
	            I++;
	        }
			
			rs.close();
	        stmt.close();
		}
		catch (SQLException e1) {
			e1.printStackTrace();
		}
	}
	
	/**
	 * llena la Lista de Cuotas para un cliente
	 * @param e ActionEvent del bot�n, no se usa pa mucho, hay que agarrar los textos de los JTextField
	 */
	private void verCuotas(ActionEvent e) {
		nro = Num_doc.getText();
		tipo = Tipo_Doc.getText();
		Num_doc.setText("");
		Tipo_Doc.setText("");
		try {
			Statement stmt = this.conexionBD.createStatement();
			ResultSet R;
			R = stmt.executeQuery("SELECT nro_pago AS Cuota_Nro,valor_cuota AS Valor,fecha_venc AS Vencimiento"
					+ "FROM prestamo NATURAL JOIN pago NATURAL JOIN Cliente"
					+ "WHERE Cliente.tipo_doc = " + tipo + "and Cliente.nro_doc =" + nro + 
					"and pago.fecha_pago = NULL");
		}
		catch(SQLException f) {
			
		}
	}
	
	/**
	 * Intenta crear un prestamo para un cliente
	 * @param e ActionEvent del Bot�n, ne se usa para mucho, hay que agarrar los textos de los JTextField
	 */
	private void CrearPrest(ActionEvent e) {
		nro = Num_doc.getText();
		tipo = Tipo_Doc.getText();
		noPrestActual(nro, tipo);
	}
	
	private void thisComponentShown (ComponentEvent evt) {
		login();
		this.conectarBD();
	}
	
	private void noPrestActual(String doc, String typeDoc) {
		int mon, mes;
		try {
			Statement stmt = this.conexionBD.createStatement();
			ResultSet R;
			//SQL para determinar los prestamos actuales de un cliente
			R = stmt.executeQuery("SELECT nro_prestamo FROM prestamo WHERE nro_cliente = " + doc) ;
			if(!R.first()) { //si no hay prestamos vigentes (la primer columna del Query es vacia, por ende no hay filas) 
				R = stmt.executeQuery("SELECT MAX(periodo) FROM tasa_prestamo");
				mes = R.getInt(1);
				R = stmt.executeQuery("SELECT MAX(monto_sup) from tasa_prestamo");
				mon = R.getInt(1); //Ojo, es un decimal, no se si anda
				IngresoPrest ven = new IngresoPrest();
				int m = ven.getmon();
				int p = ven.getper();
				if(corroborar(m,p,mes,mon)) { //Los montos ingresados no superan los m�ximos establecidos por las tasas
					EjecutarCreacion(m,p);
				}
			}
			stmt.close();
			R.close();
			
		} catch (SQLException e) {
			e.printStackTrace();
		}
	}
	
	//Corroboro que lo ingresado no supere los m�ximos de las Tasas
	private boolean corroborar(int mon, int mes,int monSup,int perSup) {
		if(mon < monSup && mes < perSup)
			return true;
		else
			return false;
	}
	
	//Con todo corroborado, creo el Prestamo
	//Esto implica crear el prestamo y todas las cuotas asociadas al mismo
	//cantidad de cuotas (Pagos en con fecha_pago = NULL) depende de la cantidad de Meses
	//usar date_add(<Fecha_actual_mientras_se_Itera>, interval 1 month)
	private void EjecutarCreacion(int plata, int periodo) {
		int i = 2;
		try {
			Statement stmt = this.conexionBD.createStatement();
			ResultSet R;
			//cargo el Prestamo
			int tasa_Int;
			int Interes;
			int ValCuota;
			R = stmt.executeQuery("SELECT tasa FROM tasa_prestamo WHERE tasa_prestamo.periodo = " + periodo); //revisar si esta bien
			tasa_Int= R.getInt(1);
			Interes = (plata + tasa_Int + periodo)/1200;
			ValCuota = (plata + tasa_Int)/periodo;
			R = stmt.executeQuery("SELECT nro_cliente FROM Cliente WHERE nro_doc = " + nro + "and tipo_doc = " + tipo);
			int c = R.getInt(1);
			 DateTimeFormatter dtf = DateTimeFormatter.ofPattern("dd/MM/yyyy");  
			 LocalDateTime now = LocalDateTime.now(); 
			 String d = now.toString();
			stmt.executeUpdate("INSERT INTO prestamo (fecha,cant_meses,monto,tasa_interes,interes,"
					+ "valor_cuota,legajo,nro_cliente) VALUES"
					+ "STR_TO_DATE(" + d + ",\"%d-%m-%Y\")," + periodo + "," + plata + "," +
					tasa_Int + "," + Interes + "," + ValCuota + "," + legajo + "," + c); //corroborar que los datos son correctos
			
			//Para cargar las cuotas necesito el nro de prestamo del prestamo reci�n creado
			R = stmt.executeQuery("SELECT nro_prestamo FROM prestamo WHERE nro_cliente =" + c);
			int nro_pre = R.getInt(1);
			cargarCuotas(c,d,nro_pre,periodo);
			
			stmt.close();
			R.close();
		} catch (SQLException e) {
			e.printStackTrace();
		}
		
	}
	
	//Metodo auxiliar para cargar las cuotas
	private void cargarCuotas(int c, String d, int nro_pre, int periodo) {
		Statement stmt;
		try {
			stmt = this.conexionBD.createStatement();
		 		ResultSet R;
			int i = 2;
			//Cargo la primera cuota
			stmt.executeUpdate("INSERT INTO Pago (nro_prestamo,nro_pago,fecha_venc,fecha_pago) VALUES"
					+ nro_pre + "," + 1 + ",STR_TO_DATE(" + d +",%d-%m-%Y)" + ", NULL");
			//Cargo el resto de las cuotas
			while(i <= periodo) {
				R = stmt.executeQuery("SELECT DATE_ADD(" + d + "INTERVAL 1 month)");
				//Aca cargo las cuotas una por una
				stmt.executeUpdate("INSERT INTO Pago (nro_prestamo,nro_pago,fecha_venc,fecha_pago) VALUES"
						+ nro_pre + "," + i + ",STR_TO_DATE(" + d + ",%d-%m-%Y)" + ", NULL)"); 
				//Fecha_Pago = NULL ya que es una cuota que NO se ha pagado todavia
				d = R.getString(1);
			}
		}
		catch (SQLException e) {
			e.printStackTrace();
		}

	}

	/**
	 * Hace login del Empleado por medio de Pop Ups
	 */
	private void login() {
		JTextField Leg = new JTextField();
        int okCx2 = JOptionPane.showConfirmDialog(null,Leg,"Ingrese numero de Legajo", JOptionPane.OK_CANCEL_OPTION, JOptionPane.PLAIN_MESSAGE);
        if(okCx2 == JOptionPane.OK_OPTION) {
        	legajo = Leg.getText().trim();
        }
        pf = new JPasswordField();
		int okCxl = JOptionPane.showConfirmDialog(null, pf, "Enter Password", JOptionPane.OK_CANCEL_OPTION, JOptionPane.PLAIN_MESSAGE);
		if (okCxl == JOptionPane.OK_OPTION) {
			clave = new String (pf.getPassword()); 
		}
	}
	   
	private void thisComponentHidden (ComponentEvent evt) {
	    //Desconecto y  limpio todos los atributos
		this.desconectarBD();
	    nro = "";
	    legajo = "";
	    clave = "";
	    tipo = "";
	    seleccionado = -1;
	}

	private void btnEjecutarActionPerformed (ActionEvent evt) {
	    this.refrescarTabla();      
    }
	
	private void conectarBD () {
		if (conexionBD == null) {
			try {
		        String driver ="com.mysql.cj.jdbc.Driver";
		        String usuario = "empleado";
		        String password = "empleado";
		        String urlConexion = "jdbc:mysql://%25/banco?serverTimezone=UTC";
		        //Establece una conexion con la  B.D. "banco"  usando directamante una tabla DBTable    
		        tabla.connectDatabase (driver, urlConexion, usuario, password);
		        conexionBD = DriverManager.getConnection (urlConexion, usuario, password);
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
	}
	
	private void desconectarBD () {
		if (conexionBD != null) {
			try {
				tabla.close();
				conexionBD.close();
				conexionBD = null;
		    }
		    catch (SQLException ex) {
		        System.out.println("SQLException: " + ex.getMessage());
		        System.out.println("SQLState: " + ex.getSQLState());
		        System.out.println("VendorError: " + ex.getErrorCode());
		    }
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
	    catch (SQLException ex) {
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